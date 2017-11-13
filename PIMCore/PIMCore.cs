using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;

using ImageTool;

using Emgu.CV;
using Emgu.CV.Util;
using Emgu.CV.CvEnum;
using Emgu.CV.Structure;

using MathWorks.MATLAB.NET.Arrays;
using PIMMatlab;


namespace PIMCore
{
    public class PIMProject
    {
        private PIMParam param;

        public int imgCount;
        public List<String> imgPathList;
        public List<bool> imgSelected;
        public Dictionary<String, PIMImage> imgDict;
        public List<PIMImage> matchedImgList;
        public PIMImage HRImg;
        public String[] log;

        public PIMProject()
        {
            imgPathList = new List<String>();
            imgSelected = new List<bool>();
            imgDict = new Dictionary<String, PIMImage>();
            matchedImgList = new List<PIMImage>();
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

            bool success = PIMCppImport.matchFunc(swName, param.minHessian, param.octave, param.octavelayer, param.extended, param.upRight, param.rmsThres, resultOA);

            for (int i = 0; i < imgCount; i++)
                imgSelected[i] = false;

            log = File.ReadAllLines(@".\PIMCore\log.txt", Encoding.Default);

            if (success && resultOA != OutputArray.GetEmpty())
            {
                matchedImgList.Add(new PIMImage(resultOA.GetMat()));
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool SRFunc()
        {
            if (matchedImgList.Count == 0)
                return false;

            Bitmap LRBmp = matchedImgList.Last().GetBgrBitmap();

            try
            {
                //Matlab function
                //function [HRIm] = SRFunc(LRIm, Factor, GuassianSigma, GuassianKernelSize, Lamda)

                SRClass srClass = new SRClass();

                MWNumericArray LRIm = null;
                LRIm = new MWNumericArray(MWArrayComplexity.Real, MWNumericType.Int8, 3, LRBmp.Height, LRBmp.Width);
                LRIm = BitmapTool.Bitmap2Array(LRBmp);

                MWArray Factor = 2;
                MWArray GuassianSigma = 1.3;
                MWArray GuassianKernelSize = 13;
                MWArray Lamda = 0.2;

                MWArray res = srClass.SRFunc(LRIm, Factor, GuassianSigma, GuassianKernelSize, Lamda);

                byte[,,] resByteArray = (byte[,,])((MWNumericArray)res).ToArray(MWArrayComponent.Real);

                using (Bitmap resBmp = BitmapTool.Array2Bitmap(resByteArray, PixelFormat.Format24bppRgb))
                {
                    HRImg = new PIMImage(new Image<Bgr, byte>(resBmp).Mat);
                }

            }
            catch
            {
                return false;
            }

            return true;
        }

        public Mat GetMatByInd(int index)
        {
            if (index < imgPathList.Count && index >= 0 && imgDict.ContainsKey(imgPathList[index]))
            {
                return imgDict[imgPathList[index]].GetMat();
            }

            return null;
        }

        public void setParam(PIMParam param)
        {
            this.param = param;
        }

        private void resetParam()
        {
            param.minHessian = 300;
            param.octave = 5;
            param.octavelayer = 5;
            param.extended = true;
            param.upRight = false;
            param.rmsThres = 3.0;
        }

        private void reNew()
        {
            imgCount = 0;
            imgPathList.Clear();
            imgSelected.Clear();
            imgDict.Clear();
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
            mat = CvInvoke.Imread(fileName, ImreadModes.AnyColor);
            bmpThumbBgr = utilClass.ResizeBitmap2Square(mat.ToImage<Bgr, Byte>().Resize(thumbSize, thumbSize, Inter.Linear, true).ToBitmap());
            bmpThumbGray = utilClass.ResizeBitmap2Square(mat.ToImage<Gray, Byte>().Resize(thumbSize, thumbSize, Inter.Linear, true).ToBitmap());
            width = mat.Width;
            height = mat.Height;
        }

        public PIMImage(Mat mat)
        {
            this.fileName = String.Empty;
            this.mat = mat;
            bmpThumbBgr = utilClass.ResizeBitmap2Square(mat.ToImage<Bgr, Byte>().Resize(thumbSize, thumbSize, Inter.Linear, true).ToBitmap());
            bmpThumbGray = utilClass.ResizeBitmap2Square(mat.ToImage<Gray, Byte>().Resize(thumbSize, thumbSize, Inter.Linear, true).ToBitmap());
            width = mat.Width;
            height = mat.Height;
        }

        public Mat GetMat() { return mat; }
        public Bitmap GetBgrBitmap() { return mat.ToImage<Bgr, Byte>().Bitmap; }
        public Bitmap GetGrayBitmap() { return mat.ToImage<Gray, Byte>().Bitmap; }

    }

    public struct PIMParam
    {
        public int minHessian;
        public int octave;
        public int octavelayer;
        public bool extended;
        public bool upRight;
        public double rmsThres;
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
