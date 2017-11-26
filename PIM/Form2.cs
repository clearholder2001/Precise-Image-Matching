/// -----------------------------------------------
/// 作者：陳俋臣、Achmad Junaidi
/// 修改日期：2017/11/20
/// 內容：參數編輯介面
/// -----------------------------------------------


using System;
using System.Windows.Forms;

using PIMCore;


namespace PIM
{
    public partial class Form2 : Form
    {
        public PIMParam param;

        //-----------------------------------------------

        public Form2(PIMParam param)
        {
            InitializeComponent();

            this.param = param;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            param.minHessian = int.Parse(textBox1.Text);
            param.extended = Convert.ToBoolean(int.Parse(textBox2.Text));
            param.octave = int.Parse(textBox3.Text);
            param.octavelayer = int.Parse(textBox4.Text);
            param.upRight = Convert.ToBoolean(int.Parse(textBox5.Text));
            param.rmsThres = double.Parse(textBox6.Text);
            param.enableRANSAC = Convert.ToBoolean(int.Parse(textBox7.Text));

            param.Factor = double.Parse(textBox8.Text);
            param.GuassianSigma = double.Parse(textBox9.Text);
            param.GuassianKernelSize = int.Parse(textBox10.Text);
            param.Lamda = double.Parse(textBox11.Text);
        }

        private void button2_Click(object sender, EventArgs e)
        {

        }

        private void Form2_Load(object sender, EventArgs e)
        {
            textBox1.Text = param.minHessian.ToString();
            textBox2.Text = Convert.ToInt32(param.extended).ToString();
            textBox3.Text = param.octave.ToString();
            textBox4.Text = param.octavelayer.ToString();
            textBox5.Text = Convert.ToInt32(param.upRight).ToString();
            textBox6.Text = param.rmsThres.ToString();
            textBox7.Text = Convert.ToInt32(param.enableRANSAC).ToString();

            textBox8.Text = param.Factor.ToString();
            textBox9.Text = param.GuassianSigma.ToString();
            textBox10.Text = param.GuassianKernelSize.ToString();
            textBox11.Text = param.Lamda.ToString();
        }
    }
}
