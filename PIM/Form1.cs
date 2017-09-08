using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows.Forms;

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

        private void imagesToolStripMenuItem_Click(object sender, EventArgs e)
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
#if DEBUG
                
                //tabControl1.SelectedIndex = 1;
                //button1.PerformClick();
#endif
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int selectedCount = listView1.SelectedIndices.Count;

            if (selectedCount >= 2)
            {
                PIMParam param = new PIMParam();
                param.minHessian = int.Parse(textBox1.Text);
                param.extended = Convert.ToBoolean(int.Parse(textBox2.Text));
                param.octave = int.Parse(textBox3.Text);
                param.octavelayer = int.Parse(textBox4.Text);
                param.upRight = Convert.ToBoolean(int.Parse(textBox5.Text));
                param.rmsThres = 2.0;
                project.setParam(param);

                for (int i = 0; i < selectedCount; i++)
                    project.imgSelected[listView1.SelectedIndices[i]] = true;

                if (project.matchingFunc())
                {
                    imageBox1.Image = project.matchedImgList.Last().GetMat();
                    toolStripStatusLabel1.Text = "Image matching is done.";
                }
                else
                {
                    MessageBox.Show("Matching failed!");
                }

                richTextBox1.Lines = project.log;
                tabControl1.SelectedIndex = 2;
            }
            else
            {
                MessageBox.Show("Please select at least 2 images!");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (project.matchedImgList.Count != 0)
            {
                if (false)
                {
                    toolStripStatusLabel1.Text = "Super resolution generation is done.";
                }
                else
                    MessageBox.Show("Generation failed!");
            }
            else
            {
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
    }
}
