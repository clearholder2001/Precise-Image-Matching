namespace PIM
{
    partial class Form2
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.txtUpright = new System.Windows.Forms.TextBox();
            this.txtOctaveLayer = new System.Windows.Forms.TextBox();
            this.txtOctave = new System.Windows.Forms.TextBox();
            this.txtExtend = new System.Windows.Forms.TextBox();
            this.txtMinHessian = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.cmdSaveSetting = new System.Windows.Forms.Button();
            this.cmdExit = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.txtUpright);
            this.groupBox1.Controls.Add(this.txtOctaveLayer);
            this.groupBox1.Controls.Add(this.txtOctave);
            this.groupBox1.Controls.Add(this.txtExtend);
            this.groupBox1.Controls.Add(this.txtMinHessian);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft JhengHei UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(321, 236);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "SURF";
            // 
            // txtUpright
            // 
            this.txtUpright.Location = new System.Drawing.Point(136, 162);
            this.txtUpright.Name = "txtUpright";
            this.txtUpright.Size = new System.Drawing.Size(116, 24);
            this.txtUpright.TabIndex = 9;
            // 
            // txtOctaveLayer
            // 
            this.txtOctaveLayer.Location = new System.Drawing.Point(136, 131);
            this.txtOctaveLayer.Name = "txtOctaveLayer";
            this.txtOctaveLayer.Size = new System.Drawing.Size(116, 24);
            this.txtOctaveLayer.TabIndex = 8;
            // 
            // txtOctave
            // 
            this.txtOctave.Location = new System.Drawing.Point(136, 100);
            this.txtOctave.Name = "txtOctave";
            this.txtOctave.Size = new System.Drawing.Size(116, 24);
            this.txtOctave.TabIndex = 7;
            // 
            // txtExtend
            // 
            this.txtExtend.Location = new System.Drawing.Point(136, 66);
            this.txtExtend.Name = "txtExtend";
            this.txtExtend.Size = new System.Drawing.Size(116, 24);
            this.txtExtend.TabIndex = 6;
            // 
            // txtMinHessian
            // 
            this.txtMinHessian.Location = new System.Drawing.Point(136, 35);
            this.txtMinHessian.Name = "txtMinHessian";
            this.txtMinHessian.Size = new System.Drawing.Size(116, 24);
            this.txtMinHessian.TabIndex = 5;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(7, 168);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(55, 17);
            this.label5.TabIndex = 4;
            this.label5.Text = "Upright";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 135);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(87, 17);
            this.label4.TabIndex = 3;
            this.label4.Text = "Octave Layer";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 100);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(51, 17);
            this.label3.TabIndex = 2;
            this.label3.Text = "Octave";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 66);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(100, 17);
            this.label2.TabIndex = 1;
            this.label2.Text = "Extend(128,64)";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 35);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(85, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "Min. Hessian";
            // 
            // groupBox2
            // 
            this.groupBox2.Font = new System.Drawing.Font("Microsoft JhengHei UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.Location = new System.Drawing.Point(349, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(325, 236);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "RANSAC";
            // 
            // cmdSaveSetting
            // 
            this.cmdSaveSetting.Location = new System.Drawing.Point(258, 270);
            this.cmdSaveSetting.Name = "cmdSaveSetting";
            this.cmdSaveSetting.Size = new System.Drawing.Size(75, 23);
            this.cmdSaveSetting.TabIndex = 2;
            this.cmdSaveSetting.Text = "Save";
            this.cmdSaveSetting.UseVisualStyleBackColor = true;
            this.cmdSaveSetting.Click += new System.EventHandler(this.cmdSaveSetting_Click);
            // 
            // cmdExit
            // 
            this.cmdExit.Location = new System.Drawing.Point(353, 270);
            this.cmdExit.Name = "cmdExit";
            this.cmdExit.Size = new System.Drawing.Size(75, 23);
            this.cmdExit.TabIndex = 3;
            this.cmdExit.Text = "Exit";
            this.cmdExit.UseVisualStyleBackColor = true;
            this.cmdExit.Click += new System.EventHandler(this.cmdExit_Click);
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(680, 301);
            this.Controls.Add(this.cmdExit);
            this.Controls.Add(this.cmdSaveSetting);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form2";
            this.Text = "Setting SURF and RANSAC";
            this.Load += new System.EventHandler(this.Form2_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button cmdSaveSetting;
        private System.Windows.Forms.Button cmdExit;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtUpright;
        private System.Windows.Forms.TextBox txtOctaveLayer;
        private System.Windows.Forms.TextBox txtOctave;
        private System.Windows.Forms.TextBox txtExtend;
        private System.Windows.Forms.TextBox txtMinHessian;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
    }
}