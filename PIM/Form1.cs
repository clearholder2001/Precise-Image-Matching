/// -----------------------------------------------
/// 作者：陳俋臣、Achmad Junaidi
/// 修改日期：2017/11/20
/// 內容：主程式介面
/// -----------------------------------------------


using System;
using System.IO;
using System.Linq;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel;

using PIMCore;


namespace PIM
{
    public partial class Form1 : Form
    {
        private PIMProject project;
        private String logName = @".\PIMCore\log.txt";
        private String resultPath = @".\Matching Result\";

        //-----------------------------------------------

        public Form1()
        {
            InitializeComponent();

            imageList1.ImageSize = new Size(PIMImage.thumbSize, PIMImage.thumbSize);
            imageList2.ImageSize = new Size(115, 115);
            imageList3.ImageSize = new Size(PIMImage.thumbSize, PIMImage.thumbSize);
            imageList4.ImageSize = new Size(PIMImage.thumbSize, PIMImage.thumbSize);
            imageList5.ImageSize = new Size(100, 100);
            project = new PIMProject();

            if (File.Exists(logName))
                File.Delete(logName);

            checkBox1.Checked = false;
            button3.Enabled = false;
            toolStripStatusLabel1.Text = "Initialize MATLAB Runtime......";
            backgroundWorker1.RunWorkerAsync();
        }

        private void resetControl(bool resetInput)
        {
            if (resetInput)
            {
                listView1.Items.Clear();
                imageList1.Images.Clear();
            }
            
            listView2.Items.Clear();
            listView3.Items.Clear();
            listView4.Items.Clear();
            
            imageList2.Images.Clear();
            imageList3.Images.Clear();
            imageList4.Images.Clear();
            imageBox1.Image = null;
            imageBox2.Image = null;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Multiselect = true;
            ofd.Filter = "Image Files(*.TIF;*.JPG;*.BMP;*.PNG;*.GIF)|*.TIF;*.JPG;*.BMP;*.PNG;*.GIF| All files (*.*)|*.*";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                resetControl(true);

                project.loadImageSeq(ofd.FileNames);

                foreach (String fileName in ofd.FileNames)
                {
                    listView1.Items.Add(Path.GetFileName(fileName), fileName);
                    imageList1.Images.Add(fileName, project.imgDict[fileName].bmpThumbBgr);
                }

                toolStripStatusLabel1.Text = String.Format("Total: {0}", ofd.FileNames.Count());
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            resetControl(false);

            int selectedCount = listView1.SelectedIndices.Count;

            if (project.imgDict.Count == 0)
            {
                MessageBox.Show("Please load images first!");
                return;
            }

            if (selectedCount == 0 || selectedCount == 1)
            {
                foreach (ListViewItem item in listView1.Items)
                    item.Selected = true;

                selectedCount = listView1.SelectedIndices.Count;
            }

            if (selectedCount >= 2)
            {
                for (int i = 0; i < project.imgSelected.Count; i++)
                    project.imgSelected[i] = false;

                for (int i = 0; i < selectedCount; i++)
                    project.imgSelected[listView1.SelectedIndices[i]] = true;

                if (project.matchingFunc() && project.loadMatchingResult(resultPath))
                {
                    //show image sequence of matching result
                    foreach (PIMImage img in project.descriptorImgList)
                    {
                        listView2.Items.Add(Path.GetFileNameWithoutExtension(img.GetFileName()), img.GetFileName());
                        imageList2.Images.Add(img.GetFileName(), img.bmpThumbBgr);
                    }

                    foreach (PIMImage img in project.homographyImgList)
                    {
                        listView3.Items.Add(Path.GetFileNameWithoutExtension(img.GetFileName()), img.GetFileName());
                        imageList3.Images.Add(img.GetFileName(), img.bmpThumbBgr);
                    }

                    foreach (PIMImage img in project.mapperImgList)
                    {
                        listView4.Items.Add(Path.GetFileNameWithoutExtension(img.GetFileName()), img.GetFileName());
                        imageList4.Images.Add(img.GetFileName(), img.bmpThumbBgr);
                    }

                    listView5.Items.Add(Path.GetFileNameWithoutExtension(project.jointImgList.Last().GetFileName()), project.jointImgList.Last().GetFileName());
                    imageList5.Images.Add(project.jointImgList.Last().GetFileName(), project.jointImgList.Last().bmpThumbBgr);

                    imageBox1.Image = project.jointImgList.Last().GetBgrImage();
                    imageBox3.Image = project.jointImgList.Last().GetBgrImage();
                    toolStripStatusLabel1.Text = String.Format("Image matching is done. Total time: {0:0.##}s", project.processTime);
                }
                else
                {
                    toolStripStatusLabel1.Text = string.Empty;
                    MessageBox.Show("Matching failed!");
                }

                richTextBox1.Lines = project.log;
            }
            else
            {
                MessageBox.Show("Please select at least 2 images!");
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (project.mcrInitStatus)
            {
                if (imageBox3.Image != null)
                {
                    if (project.srFunc(imageBox3.Image.Bitmap))
                    {
                        imageBox4.Image = project.HRImg.GetBgrImage();
                        toolStripStatusLabel1.Text = String.Format("Super resolution generation is done. Total time: {0:0.##}s", project.processTime);
                    }
                    else
                    {
                        MessageBox.Show("Generation failed!");
                    }
                }
                else
                {
                    toolStripStatusLabel1.Text = string.Empty;
                    MessageBox.Show("No source image!");
                }
            }
            else
            {
                //警告：沒有偵測到MCR
                if (MessageBox.Show("Matlab Runtime may not be detected.\nPlease install Matlab Runtime R2017b (9.3).\nDownload now?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Asterisk) == DialogResult.Yes)
                {
                    System.Diagnostics.Process.Start("http://www.mathworks.com/products/compiler/mcr");
                }
            }
        }

        private void logToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (File.Exists(logName))
                System.Diagnostics.Process.Start(logName);
            else
                MessageBox.Show("No log file!");
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void settingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form2 form2 = new Form2(project.getParam());
            if (form2.ShowDialog() == DialogResult.OK)
                project.setParam(form2.param);
        }

        private void listView2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listView2.SelectedIndices.Count == 0)
                return;

            imageBox1.Image = project.descriptorImgList[listView2.SelectedIndices[0]].GetBgrImage();
        }

        private void listView3_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listView3.SelectedIndices.Count == 0)
                return;

            imageBox2.Image = project.homographyImgList[listView3.SelectedIndices[0]].GetBgrImage();
        }

        private void listView4_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listView4.SelectedIndices.Count == 0)
                return;

            imageBox2.Image = project.mapperImgList[listView4.SelectedIndices[0]].GetBgrImage();
        }

        private void listView5_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listView5.SelectedIndices.Count == 0)
                return;

            imageBox3.Image = project.jointImgList[listView5.SelectedIndices[0]].GetBgrImage();
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            project.initMCR();
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            button3.Enabled = true;

            if (project.mcrInitStatus)
            {
                checkBox1.Checked = true;
                toolStripStatusLabel1.Text = "Initialization Success!";
            }
            else
            {
                checkBox1.Checked = false;
                toolStripStatusLabel1.Text = "Initialization Fail!";
            }
        }
    }
}
