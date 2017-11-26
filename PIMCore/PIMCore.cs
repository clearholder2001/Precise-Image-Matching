/// -----------------------------------------------
/// 作者：陳俋臣
/// 修改日期：2017/11/20
/// 內容：專案類別宣告、主程式核心、引用Matlab DLL
/// -----------------------------------------------


using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;

using ImageTool;

using Emgu.CV;
using Emgu.CV.CvEnum;
using Emgu.CV.Structure;

using MathWorks.MATLAB.NET.Arrays;
using PIMMatlab;


namespace PIMCore
{
    public class PIMProject
    {
        private PIMParam param;
        private Stopwatch sw;
        private PIMClass srClass;

        public bool mcrInitStatus;
        public double processTime;
        public int imgCount;
        public List<String> imgPathList;
        public List<bool> imgSelected;
        public Dictionary<String, PIMImage> imgDict;
        public List<PIMImage> descriptorImgList;
        public List<PIMImage> homographyImgList;
        public List<PIMImage> mapperImgList;
        public List<PIMImage> jointImgList;
        public PIMImage HRImg;
        public String[] log;

        public PIMProject()
        {
            imgPathList = new List<String>();
            imgSelected = new List<bool>();
            imgDict = new Dictionary<String, PIMImage>();
            descriptorImgList = new List<PIMImage>();
            homographyImgList = new List<PIMImage>();
            mapperImgList = new List<PIMImage>();
            jointImgList = new List<PIMImage>();
            resetParam();
        }

        public bool loadImageSeq(String[] fileNames)
        {
            reNew();

            imgCount = fileNames.Count();
            imgPathList.Capacity = imgCount;
            imgSelected.Capacity = imgCount;

            try
            {
                foreach (String fileName in fileNames)
                {
                    imgPathList.Add(fileName);
                    imgSelected.Add(false);
                    imgDict.Add(fileName, new PIMImage(fileName));
                }
            }
            catch (Exception)
            {
                return false;
            }

            return true;
        }

        public bool matchingFunc()
        {
            String swName = "imgPathList" + ".txt";

            List<String> list = new List<string>();
            for (int i = 0; i < imgCount; i++)
            {
                if (imgSelected[i])
                    list.Add(imgPathList[i]);
            }

            using (StreamWriter sw = new StreamWriter(@".\PIMCore\" + swName))
            {
                foreach (string line in list.ToArray())
                    sw.WriteLine(line);
            }


            Mat result = new Mat();
            OutputArray resultOA = result.GetOutputArray();

            processTime = 0.0;
            sw = Stopwatch.StartNew();

            bool success = PIMCppImport.matchFunc(swName, param.minHessian, param.octave, param.octavelayer, param.extended, param.upRight, param.rmsThres, param.enableRANSAC, resultOA);

            sw.Stop();
            processTime = (double)sw.ElapsedMilliseconds / 1000;

            for (int i = 0; i < imgCount; i++)
                imgSelected[i] = false;

            log = File.ReadAllLines(@".\PIMCore\log.txt", Encoding.Default);

            if (success && resultOA != OutputArray.GetEmpty())
            {
                jointImgList.Add(new PIMImage(resultOA.GetMat(), String.Format("{0:00}", jointImgList.Count + 1)));
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool loadMatchingResult(String path)
        {
            try
            {
                if (Directory.Exists(path + "\\1. Input Image")
                 && Directory.Exists(path + "\\2. Detected Feature")
                 && Directory.Exists(path + "\\3. Homography")
                 && Directory.Exists(path + "\\4. Mapper")
                 && Directory.Exists(path + "\\5. Joint Image"))
                {
                    descriptorImgList.Clear();
                    homographyImgList.Clear();
                    mapperImgList.Clear();

                    foreach (string fileName in Directory.GetFiles(path + "\\2. Detected Feature"))
                        descriptorImgList.Add(new PIMImage(fileName));

                    foreach (string fileName in Directory.GetFiles(path + "\\3. Homography"))
                        homographyImgList.Add(new PIMImage(fileName));

                    foreach (string fileName in Directory.GetFiles(path + "\\4. Mapper"))
                        mapperImgList.Add(new PIMImage(fileName));
                }
                else
                    return false;
            }
            catch (Exception)
            {
                return false;
            }

            return true;
        }

        public bool srFunc(Bitmap LRBmp)
        {
            try
            {
                //Matlab function
                //function [HRIm] = SRFunc(LRIm, Factor, GuassianSigma, GuassianKernelSize, Lamda)

                if (mcrInitStatus == false)
                {
                    if (!initMCR())
                    {
                        return false;
                    }
                }

                MWNumericArray LRIm = null;
                LRIm = new MWNumericArray(MWArrayComplexity.Real, MWNumericType.UInt8, 3, LRBmp.Width, LRBmp.Height);
                LRIm = BitmapTool.Bitmap2Array(LRBmp);

                MWArray Factor = param.Factor;
                MWArray GuassianSigma = param.GuassianSigma;
                MWArray GuassianKernelSize = param.GuassianKernelSize;
                MWArray Lamda = param.Lamda;


                processTime = 0.0;
                sw = Stopwatch.StartNew();

                MWArray res = srClass.SRFunc(LRIm, Factor, GuassianSigma, GuassianKernelSize, Lamda);

                sw.Stop();
                processTime = (double)sw.ElapsedMilliseconds / 1000;

                byte[,,] resByteArray = (byte[,,])((MWNumericArray)res).ToArray(MWArrayComponent.Real);

                using (Bitmap resBmp = BitmapTool.Array2Bitmap(resByteArray, PixelFormat.Format24bppRgb))
                {
                    HRImg = new PIMImage(new Image<Bgr, byte>(resBmp).Mat, "SRResult");
                }
            }
            catch
            {
                return false;
            }

            return true;
        }

        public void setParam(PIMParam param)
        {
            this.param = param;
        }

        public PIMParam getParam()
        {
            return param;
        }

        public bool initMCR()
        {
            try
            {
                srClass = new PIMClass();
            }
            catch (Exception)
            {
                mcrInitStatus = false;
                return false;
            }

            mcrInitStatus = true;
            return true;
        }

        private void resetParam()
        {
            param.minHessian = 300;
            param.octave = 5;
            param.octavelayer = 5;
            param.extended = true;
            param.upRight = false;
            param.rmsThres = 3.0;
            param.enableRANSAC = true;

            param.Factor = 2;
            param.GuassianSigma = 1.3;
            param.GuassianKernelSize = 13;
            param.Lamda = 0.05;
        }

        private void reNew()
        {
            imgCount = 0;
            imgPathList.Clear();
            imgSelected.Clear();
            imgDict.Clear();

            descriptorImgList.Clear();
            homographyImgList.Clear();
            mapperImgList.Clear();
        }
    }

    public class PIMImage
    {
        private String fileName;
        private Mat mat;

        public int width, height;
        public Bitmap bmpThumbBgr, bmpThumbGray;
        public const int thumbSize = 150;

        public PIMImage(String fileName)
        {
            this.fileName = fileName;
            mat = new Mat(fileName, ImreadModes.AnyColor);
            width = mat.Width;
            height = mat.Height;

            using (Image<Bgr, Byte> img = mat.ToImage<Bgr, Byte>())
                bmpThumbBgr = utilClass.ResizeBitmap2Square(img.Resize(thumbSize, thumbSize, Inter.Linear, true).ToBitmap());
            using (Image<Gray, Byte> img = mat.ToImage<Gray, Byte>())
                bmpThumbGray = utilClass.ResizeBitmap2Square(img.Resize(thumbSize, thumbSize, Inter.Linear, true).ToBitmap());

        }

        public PIMImage(Mat mat, string stringIndex)
        {
            this.fileName = String.Empty;
            this.mat = mat;
            width = mat.Width;
            height = mat.Height;

            using (Image<Bgr, Byte> img = mat.ToImage<Bgr, Byte>())
                bmpThumbBgr = utilClass.ResizeBitmap2Square(img.Resize(thumbSize, thumbSize, Inter.Linear, true).ToBitmap());
            using (Image<Gray, Byte> img = mat.ToImage<Gray, Byte>())
                bmpThumbGray = utilClass.ResizeBitmap2Square(img.Resize(thumbSize, thumbSize, Inter.Linear, true).ToBitmap());

            fileName = stringIndex;
        }

        public Mat GetMat() { return mat.Clone(); }

        public String GetFileName() { return fileName; }

        public Image<Bgr, Byte> GetBgrImage() { return new Image<Bgr, Byte>(mat.Bitmap); }

        public Image<Gray, Byte> GetGrayImage() { return new Image<Gray, Byte>(mat.Bitmap); }
    }

    public struct PIMParam
    {
        public int minHessian;
        public int octave;
        public int octavelayer;
        public bool extended;
        public bool upRight;
        public double rmsThres;
        public bool enableRANSAC;

        public double Factor;
        public double GuassianSigma;
        public int GuassianKernelSize;
        public double Lamda;
    }

    public class utilClass
    {
        static public Bitmap ResizeBitmap2Square(Bitmap originalImage)
        {
            int width = originalImage.Width;
            int height = originalImage.Height;
            int newWidth, newHeight, xCorner, yCorner;

            if (width >= height)
            {
                newWidth = newHeight = width;
                xCorner = 0;
                yCorner = (newWidth - height) / 2;
            }
            else
            {
                newWidth = newHeight = height;
                xCorner = (newHeight - width) / 2;
                yCorner = 0;
            }

            Bitmap squareImage = new Bitmap(newWidth, newHeight);
            using (Graphics g = Graphics.FromImage(squareImage))
            {
                g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                g.Clear(Color.White);
                g.DrawImage(originalImage, xCorner, yCorner);

                return squareImage;
            }
        }
    }
}
