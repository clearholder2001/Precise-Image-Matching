using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using PIMCore;

namespace PIM
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        private void Form2_Load(object sender, EventArgs e)
        {

        }

        private void cmdExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        private void cmdSaveSetting_Click(object sender, EventArgs e)
        {
            PIMParam param = new PIMParam();
            param.minHessian = int.Parse(txtExtend.Text);
            param.extended = Convert.ToBoolean(int.Parse(txtOctave.Text));
            param.octave = int.Parse(txtOctaveLayer.Text);
            param.octavelayer = int.Parse(txtUpright.Text);
            param.upRight = Convert.ToBoolean(int.Parse(txtUpright.Text));
            param.rmsThres = 2.0;
            //project.setParam(param);
        }

    }
}
