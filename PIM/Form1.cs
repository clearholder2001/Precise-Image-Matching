using System;
using System.IO;
using System.Linq;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel;
using System.Threading;

using PIMCore;

namespace PIM
{
    public partial class Form1 : Form
    {
        private PIMProject project;
        private String logName = @".\PIMCore\log.txt";


        public Form1()
        {
            InitializeComponent();
            imageList1.ImageSize = new Size(PIMImage.thumbSize, PIMImage.thumbSize);
            project = new PIMProject();

            if (File.Exists(logName))
                File.Delete(logName);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Multiselect = true;
            ofd.Filter = "Image Files(*.TIF;*.JPG;*.BMP;*.PNG;*.GIF)|*.TIF;*.JPG;*.BMP;*.PNG;*.GIF| All files (*.*)|*.*";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                listView1.Items.Clear();
                imageList1.Images.Clear();

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
            int selectedCount = listView1.SelectedIndices.Count;

            if (selectedCount >= 2)
            {
                PIMParam param = new PIMParam();
                //param.minHessian = int.Parse(textBox1.Text);
                //param.extended = Convert.ToBoolean(int.Parse(textBox2.Text));
                //param.octave = int.Parse(textBox3.Text);
                //param.octavelayer = int.Parse(textBox4.Text);
                //param.upRight = Convert.ToBoolean(int.Parse(textBox5.Text));
                //param.rmsThres = 2.0;
                //project.setParam(param);

                for (int i = 0; i < selectedCount; i++)
                    project.imgSelected[listView1.SelectedIndices[i]] = true;

                if (project.matchingFunc())
                {
                    imageBox1.Image = project.matchedImgList.Last().GetMat();
                    toolStripStatusLabel1.Text = "Image matching is done.";
                }
                else
                {
                    toolStripStatusLabel1.Text = string.Empty;
                    MessageBox.Show("Matching failed!");
                }

                richTextMethod.Lines = project.log;
                //tabControl1.SelectedIndex = 2;
            }
            else
            {
                MessageBox.Show("Please select at least 2 images!");
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (project.matchedImgList.Count != 0)
            {
                if (project.SRFunc()) 
                {
                    //★加一個警告：沒有偵測到MCR★
                    //if (MessageBox.Show("Matlab Runtime is not detected.\nPlease install Matlab Runtime R2017b.\nDownload now?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Asterisk) == DialogResult.Yes)
                    //{
                    //    System.Diagnostics.Process.Start("https://www.mathworks.com/products/compiler/matlab-runtime.html");
                    //}

                    imageBox3.Image = project.matchedImgList.Last().GetMat();
                    imageBox4.Image = project.HRImg.GetMat();
                    toolStripStatusLabel1.Text = "Super resolution generation is done.";
                }
                else
                    MessageBox.Show("Generation failed!");
            }
            else
            {
                toolStripStatusLabel1.Text = string.Empty;
                MessageBox.Show("No matched image!");
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

        private void settingOfImageMatchingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form2 settingForm = new Form2();
            settingForm.Show();
        }

    }
}
