namespace PIM
{
    partial class Form1
    {
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        /// <param name="disposing">如果應該處置 Managed 資源則為 true，否則為 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 設計工具產生的程式碼

        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
        /// 這個方法的內容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabSurfRansac = new System.Windows.Forms.TabPage();
            this.progressBarSuran = new System.Windows.Forms.ProgressBar();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.listView1 = new System.Windows.Forms.ListView();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.groupMethod = new System.Windows.Forms.GroupBox();
            this.lvMethod = new System.Windows.Forms.ListView();
            this.panelSuran = new System.Windows.Forms.Panel();
            this.optSurf = new System.Windows.Forms.RadioButton();
            this.optSRansac = new System.Windows.Forms.RadioButton();
            this.richTextMethod = new System.Windows.Forms.RichTextBox();
            this.cmdRefresh = new System.Windows.Forms.Button();
            this.imageBox1 = new Emgu.CV.UI.ImageBox();
            this.tabABMatching = new System.Windows.Forms.TabPage();
            this.panelAbm = new System.Windows.Forms.Panel();
            this.imgboxAbm = new Emgu.CV.UI.ImageBox();
            this.cmdAbm = new System.Windows.Forms.Button();
            this.lblProgresAbm = new System.Windows.Forms.Label();
            this.groupAbm = new System.Windows.Forms.GroupBox();
            this.lvAbm = new System.Windows.Forms.ListView();
            this.groupSR = new System.Windows.Forms.GroupBox();
            this.lvSuran = new System.Windows.Forms.ListView();
            this.progressBarAbm = new System.Windows.Forms.ProgressBar();
            this.tabSR = new System.Windows.Forms.TabPage();
            this.panelSR = new System.Windows.Forms.Panel();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.imageBox4 = new Emgu.CV.UI.ImageBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.imageBox3 = new Emgu.CV.UI.ImageBox();
            this.button3 = new System.Windows.Forms.Button();
            this.lvSR = new System.Windows.Forms.ListView();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.logToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingOfImageMatchingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.tabControl1.SuspendLayout();
            this.tabSurfRansac.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupMethod.SuspendLayout();
            this.panelSuran.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageBox1)).BeginInit();
            this.tabABMatching.SuspendLayout();
            this.panelAbm.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imgboxAbm)).BeginInit();
            this.groupAbm.SuspendLayout();
            this.groupSR.SuspendLayout();
            this.tabSR.SuspendLayout();
            this.panelSR.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageBox4)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageBox3)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabSurfRansac);
            this.tabControl1.Controls.Add(this.tabABMatching);
            this.tabControl1.Controls.Add(this.tabSR);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Font = new System.Drawing.Font("Microsoft JhengHei UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.tabControl1.Location = new System.Drawing.Point(0, 25);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1008, 682);
            this.tabControl1.TabIndex = 0;
            // 
            // tabSurfRansac
            // 
            this.tabSurfRansac.Controls.Add(this.progressBarSuran);
            this.tabSurfRansac.Controls.Add(this.groupBox3);
            this.tabSurfRansac.Controls.Add(this.button1);
            this.tabSurfRansac.Controls.Add(this.button2);
            this.tabSurfRansac.Controls.Add(this.groupMethod);
            this.tabSurfRansac.Font = new System.Drawing.Font("Microsoft JhengHei UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.tabSurfRansac.Location = new System.Drawing.Point(4, 26);
            this.tabSurfRansac.Name = "tabSurfRansac";
            this.tabSurfRansac.Size = new System.Drawing.Size(1000, 652);
            this.tabSurfRansac.TabIndex = 2;
            this.tabSurfRansac.Text = "     SURF + RANSAC  ";
            this.tabSurfRansac.UseVisualStyleBackColor = true;
            // 
            // progressBarSuran
            // 
            this.progressBarSuran.Location = new System.Drawing.Point(16, 49);
            this.progressBarSuran.Name = "progressBarSuran";
            this.progressBarSuran.Size = new System.Drawing.Size(205, 17);
            this.progressBarSuran.TabIndex = 8;
            this.progressBarSuran.Visible = false;
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox3.Controls.Add(this.listView1);
            this.groupBox3.Location = new System.Drawing.Point(8, 71);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(222, 580);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Image Source";
            // 
            // listView1
            // 
            this.listView1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.listView1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.listView1.LargeImageList = this.imageList1;
            this.listView1.Location = new System.Drawing.Point(6, 21);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(210, 554);
            this.listView1.TabIndex = 0;
            this.listView1.UseCompatibleStateImageBehavior = false;
            // 
            // imageList1
            // 
            this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth24Bit;
            this.imageList1.ImageSize = new System.Drawing.Size(8, 8);
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(16, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(105, 31);
            this.button1.TabIndex = 7;
            this.button1.Text = "Choose Image";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(134, 12);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(87, 31);
            this.button2.TabIndex = 6;
            this.button2.Text = "Start";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // groupMethod
            // 
            this.groupMethod.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupMethod.Controls.Add(this.lvMethod);
            this.groupMethod.Controls.Add(this.panelSuran);
            this.groupMethod.Controls.Add(this.imageBox1);
            this.groupMethod.Location = new System.Drawing.Point(236, 3);
            this.groupMethod.Name = "groupMethod";
            this.groupMethod.Size = new System.Drawing.Size(764, 649);
            this.groupMethod.TabIndex = 4;
            this.groupMethod.TabStop = false;
            this.groupMethod.Text = "Method";
            // 
            // lvMethod
            // 
            this.lvMethod.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lvMethod.Location = new System.Drawing.Point(7, 21);
            this.lvMethod.Name = "lvMethod";
            this.lvMethod.Size = new System.Drawing.Size(514, 163);
            this.lvMethod.TabIndex = 7;
            this.lvMethod.UseCompatibleStateImageBehavior = false;
            // 
            // panelSuran
            // 
            this.panelSuran.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelSuran.Controls.Add(this.optSurf);
            this.panelSuran.Controls.Add(this.optSRansac);
            this.panelSuran.Controls.Add(this.richTextMethod);
            this.panelSuran.Controls.Add(this.cmdRefresh);
            this.panelSuran.Location = new System.Drawing.Point(527, 21);
            this.panelSuran.Name = "panelSuran";
            this.panelSuran.Size = new System.Drawing.Size(229, 627);
            this.panelSuran.TabIndex = 6;
            // 
            // optSurf
            // 
            this.optSurf.AutoSize = true;
            this.optSurf.Location = new System.Drawing.Point(7, 3);
            this.optSurf.Name = "optSurf";
            this.optSurf.Size = new System.Drawing.Size(60, 21);
            this.optSurf.TabIndex = 1;
            this.optSurf.TabStop = true;
            this.optSurf.Text = "SURF";
            this.optSurf.UseVisualStyleBackColor = true;
            // 
            // optSRansac
            // 
            this.optSRansac.AutoSize = true;
            this.optSRansac.Location = new System.Drawing.Point(8, 28);
            this.optSRansac.Name = "optSRansac";
            this.optSRansac.Size = new System.Drawing.Size(125, 21);
            this.optSRansac.TabIndex = 2;
            this.optSRansac.TabStop = true;
            this.optSRansac.Text = "SURF+RANSAC";
            this.optSRansac.UseVisualStyleBackColor = true;
            // 
            // richTextMethod
            // 
            this.richTextMethod.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)));
            this.richTextMethod.Location = new System.Drawing.Point(3, 53);
            this.richTextMethod.Name = "richTextMethod";
            this.richTextMethod.ReadOnly = true;
            this.richTextMethod.Size = new System.Drawing.Size(223, 570);
            this.richTextMethod.TabIndex = 3;
            this.richTextMethod.Text = "";
            // 
            // cmdRefresh
            // 
            this.cmdRefresh.Location = new System.Drawing.Point(142, 19);
            this.cmdRefresh.Name = "cmdRefresh";
            this.cmdRefresh.Size = new System.Drawing.Size(75, 29);
            this.cmdRefresh.TabIndex = 4;
            this.cmdRefresh.Text = "Refresh";
            this.cmdRefresh.UseVisualStyleBackColor = true;
            // 
            // imageBox1
            // 
            this.imageBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.imageBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.imageBox1.Location = new System.Drawing.Point(6, 189);
            this.imageBox1.Name = "imageBox1";
            this.imageBox1.Size = new System.Drawing.Size(515, 454);
            this.imageBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.imageBox1.TabIndex = 5;
            this.imageBox1.TabStop = false;
            // 
            // tabABMatching
            // 
            this.tabABMatching.Controls.Add(this.panelAbm);
            this.tabABMatching.Font = new System.Drawing.Font("Microsoft JhengHei UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.tabABMatching.Location = new System.Drawing.Point(4, 26);
            this.tabABMatching.Name = "tabABMatching";
            this.tabABMatching.Size = new System.Drawing.Size(1000, 652);
            this.tabABMatching.TabIndex = 3;
            this.tabABMatching.Text = "  Area-Based Matching  ";
            this.tabABMatching.UseVisualStyleBackColor = true;
            // 
            // panelAbm
            // 
            this.panelAbm.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelAbm.AutoSize = true;
            this.panelAbm.BackColor = System.Drawing.Color.Transparent;
            this.panelAbm.Controls.Add(this.imgboxAbm);
            this.panelAbm.Controls.Add(this.cmdAbm);
            this.panelAbm.Controls.Add(this.lblProgresAbm);
            this.panelAbm.Controls.Add(this.groupAbm);
            this.panelAbm.Controls.Add(this.groupSR);
            this.panelAbm.Controls.Add(this.progressBarAbm);
            this.panelAbm.Location = new System.Drawing.Point(0, 3);
            this.panelAbm.Name = "panelAbm";
            this.panelAbm.Size = new System.Drawing.Size(1000, 662);
            this.panelAbm.TabIndex = 0;
            // 
            // imgboxAbm
            // 
            this.imgboxAbm.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.imgboxAbm.BackColor = System.Drawing.Color.LightGray;
            this.imgboxAbm.Location = new System.Drawing.Point(412, 49);
            this.imgboxAbm.Name = "imgboxAbm";
            this.imgboxAbm.Size = new System.Drawing.Size(580, 604);
            this.imgboxAbm.TabIndex = 2;
            this.imgboxAbm.TabStop = false;
            // 
            // cmdAbm
            // 
            this.cmdAbm.Location = new System.Drawing.Point(412, 12);
            this.cmdAbm.Name = "cmdAbm";
            this.cmdAbm.Size = new System.Drawing.Size(96, 31);
            this.cmdAbm.TabIndex = 3;
            this.cmdAbm.Text = "Start";
            this.cmdAbm.UseVisualStyleBackColor = true;
            // 
            // lblProgresAbm
            // 
            this.lblProgresAbm.AutoSize = true;
            this.lblProgresAbm.Location = new System.Drawing.Point(541, 20);
            this.lblProgresAbm.Name = "lblProgresAbm";
            this.lblProgresAbm.Size = new System.Drawing.Size(277, 17);
            this.lblProgresAbm.TabIndex = 2;
            this.lblProgresAbm.Text = "Click Start to proceed Area-Based Matching";
            // 
            // groupAbm
            // 
            this.groupAbm.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupAbm.Controls.Add(this.lvAbm);
            this.groupAbm.Location = new System.Drawing.Point(197, 3);
            this.groupAbm.Name = "groupAbm";
            this.groupAbm.Size = new System.Drawing.Size(209, 650);
            this.groupAbm.TabIndex = 1;
            this.groupAbm.TabStop = false;
            this.groupAbm.Text = "Area-Based Matching";
            // 
            // lvAbm
            // 
            this.lvAbm.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.lvAbm.Location = new System.Drawing.Point(6, 21);
            this.lvAbm.Name = "lvAbm";
            this.lvAbm.Size = new System.Drawing.Size(197, 623);
            this.lvAbm.TabIndex = 0;
            this.lvAbm.UseCompatibleStateImageBehavior = false;
            // 
            // groupSR
            // 
            this.groupSR.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupSR.Controls.Add(this.lvSuran);
            this.groupSR.Location = new System.Drawing.Point(0, 3);
            this.groupSR.Name = "groupSR";
            this.groupSR.Size = new System.Drawing.Size(191, 650);
            this.groupSR.TabIndex = 0;
            this.groupSR.TabStop = false;
            this.groupSR.Text = "SURF + RANSAC Result";
            // 
            // lvSuran
            // 
            this.lvSuran.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.lvSuran.Location = new System.Drawing.Point(3, 21);
            this.lvSuran.Name = "lvSuran";
            this.lvSuran.Size = new System.Drawing.Size(182, 623);
            this.lvSuran.TabIndex = 0;
            this.lvSuran.UseCompatibleStateImageBehavior = false;
            // 
            // progressBarAbm
            // 
            this.progressBarAbm.Location = new System.Drawing.Point(531, 18);
            this.progressBarAbm.Name = "progressBarAbm";
            this.progressBarAbm.Size = new System.Drawing.Size(295, 21);
            this.progressBarAbm.TabIndex = 4;
            this.progressBarAbm.Visible = false;
            // 
            // tabSR
            // 
            this.tabSR.Controls.Add(this.panelSR);
            this.tabSR.Font = new System.Drawing.Font("Microsoft JhengHei UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.tabSR.Location = new System.Drawing.Point(4, 26);
            this.tabSR.Name = "tabSR";
            this.tabSR.Size = new System.Drawing.Size(1000, 652);
            this.tabSR.TabIndex = 4;
            this.tabSR.Text = "  Super Resolution  ";
            this.tabSR.UseVisualStyleBackColor = true;
            // 
            // panelSR
            // 
            this.panelSR.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelSR.Controls.Add(this.groupBox2);
            this.panelSR.Controls.Add(this.groupBox1);
            this.panelSR.Controls.Add(this.button3);
            this.panelSR.Controls.Add(this.lvSR);
            this.panelSR.Location = new System.Drawing.Point(1, 2);
            this.panelSR.Name = "panelSR";
            this.panelSR.Size = new System.Drawing.Size(999, 656);
            this.panelSR.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.imageBox4);
            this.groupBox2.Location = new System.Drawing.Point(509, 173);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(487, 481);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Super Resolution";
            // 
            // imageBox4
            // 
            this.imageBox4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.imageBox4.Location = new System.Drawing.Point(6, 21);
            this.imageBox4.Name = "imageBox4";
            this.imageBox4.Size = new System.Drawing.Size(475, 454);
            this.imageBox4.TabIndex = 2;
            this.imageBox4.TabStop = false;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox1.Controls.Add(this.imageBox3);
            this.groupBox1.Location = new System.Drawing.Point(8, 173);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(495, 481);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Area-Based Matching";
            // 
            // imageBox3
            // 
            this.imageBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.imageBox3.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Bicubic;
            this.imageBox3.Location = new System.Drawing.Point(6, 21);
            this.imageBox3.Name = "imageBox3";
            this.imageBox3.Size = new System.Drawing.Size(483, 454);
            this.imageBox3.TabIndex = 2;
            this.imageBox3.TabStop = false;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(8, 6);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 24);
            this.button3.TabIndex = 1;
            this.button3.Text = "Start";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // lvSR
            // 
            this.lvSR.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lvSR.Location = new System.Drawing.Point(6, 34);
            this.lvSR.Name = "lvSR";
            this.lvSR.Size = new System.Drawing.Size(993, 133);
            this.lvSR.TabIndex = 0;
            this.lvSR.UseCompatibleStateImageBehavior = false;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Font = new System.Drawing.Font("Microsoft JhengHei UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.settingToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1008, 25);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.toolStripSeparator1,
            this.closeToolStripMenuItem});
            this.fileToolStripMenuItem.Font = new System.Drawing.Font("Microsoft JhengHei UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(40, 21);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.logToolStripMenuItem});
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(110, 22);
            this.openToolStripMenuItem.Text = "Open";
            // 
            // logToolStripMenuItem
            // 
            this.logToolStripMenuItem.Name = "logToolStripMenuItem";
            this.logToolStripMenuItem.Size = new System.Drawing.Size(99, 22);
            this.logToolStripMenuItem.Text = "Log";
            this.logToolStripMenuItem.Click += new System.EventHandler(this.logToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(107, 6);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(110, 22);
            this.closeToolStripMenuItem.Text = "Exit";
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // settingToolStripMenuItem
            // 
            this.settingToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.settingOfImageMatchingToolStripMenuItem});
            this.settingToolStripMenuItem.Font = new System.Drawing.Font("Microsoft JhengHei UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.settingToolStripMenuItem.Name = "settingToolStripMenuItem";
            this.settingToolStripMenuItem.Size = new System.Drawing.Size(63, 21);
            this.settingToolStripMenuItem.Text = "Setting";
            // 
            // settingOfImageMatchingToolStripMenuItem
            // 
            this.settingOfImageMatchingToolStripMenuItem.Name = "settingOfImageMatchingToolStripMenuItem";
            this.settingOfImageMatchingToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.settingOfImageMatchingToolStripMenuItem.Text = "Image Matching";
            this.settingOfImageMatchingToolStripMenuItem.Click += new System.EventHandler(this.settingOfImageMatchingToolStripMenuItem_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 707);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1008, 22);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1008, 729);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.statusStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Precise Image Matching v3 by CYC";
            this.tabControl1.ResumeLayout(false);
            this.tabSurfRansac.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupMethod.ResumeLayout(false);
            this.panelSuran.ResumeLayout(false);
            this.panelSuran.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageBox1)).EndInit();
            this.tabABMatching.ResumeLayout(false);
            this.tabABMatching.PerformLayout();
            this.panelAbm.ResumeLayout(false);
            this.panelAbm.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imgboxAbm)).EndInit();
            this.groupAbm.ResumeLayout(false);
            this.groupSR.ResumeLayout(false);
            this.tabSR.ResumeLayout(false);
            this.panelSR.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.imageBox4)).EndInit();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.imageBox3)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logToolStripMenuItem;
        private System.Windows.Forms.TabPage tabSurfRansac;
        private System.Windows.Forms.TabPage tabABMatching;
        private System.Windows.Forms.TabPage tabSR;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupMethod;
        private System.Windows.Forms.RadioButton optSurf;
        private System.Windows.Forms.RichTextBox richTextMethod;
        private System.Windows.Forms.RadioButton optSRansac;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.Button cmdRefresh;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private Emgu.CV.UI.ImageBox imageBox1;
        private System.Windows.Forms.Panel panelAbm;
        private System.Windows.Forms.ProgressBar progressBarAbm;
        private System.Windows.Forms.Button cmdAbm;
        private System.Windows.Forms.Label lblProgresAbm;
        private System.Windows.Forms.GroupBox groupAbm;
        private System.Windows.Forms.GroupBox groupSR;
        private System.Windows.Forms.ToolStripMenuItem settingToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem settingOfImageMatchingToolStripMenuItem;
        private System.Windows.Forms.Panel panelSuran;
        private System.Windows.Forms.ProgressBar progressBarSuran;
        private System.Windows.Forms.ListView lvMethod;
        private Emgu.CV.UI.ImageBox imgboxAbm;
        private System.Windows.Forms.ListView lvAbm;
        private System.Windows.Forms.ListView lvSuran;
        private System.Windows.Forms.Panel panelSR;
        private System.Windows.Forms.GroupBox groupBox2;
        private Emgu.CV.UI.ImageBox imageBox4;
        private System.Windows.Forms.GroupBox groupBox1;
        private Emgu.CV.UI.ImageBox imageBox3;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.ListView lvSR;
    }
}

