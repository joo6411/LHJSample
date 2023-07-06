namespace LHJSampleClientCS
{
    partial class Form1
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
            this.IPText = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.PortText = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.ConnectBtn = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.LoginBtn = new System.Windows.Forms.Button();
            this.PWText = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.IDText = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.RoomListRefreshBtn = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.RoomCreateBtn = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.RoomEnterBtn = new System.Windows.Forms.Button();
            this.listBox2 = new System.Windows.Forms.ListBox();
            this.button5 = new System.Windows.Forms.Button();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.listBox3 = new System.Windows.Forms.ListBox();
            this.logListBox = new System.Windows.Forms.ListBox();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // IPText
            // 
            this.IPText.Location = new System.Drawing.Point(85, 29);
            this.IPText.Margin = new System.Windows.Forms.Padding(2);
            this.IPText.Name = "IPText";
            this.IPText.Size = new System.Drawing.Size(106, 23);
            this.IPText.TabIndex = 0;
            this.IPText.Text = "127.0.0.1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 31);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(55, 15);
            this.label2.TabIndex = 2;
            this.label2.Text = "서버주소";
            // 
            // PortText
            // 
            this.PortText.Location = new System.Drawing.Point(282, 29);
            this.PortText.Margin = new System.Windows.Forms.Padding(2);
            this.PortText.Name = "PortText";
            this.PortText.Size = new System.Drawing.Size(106, 23);
            this.PortText.TabIndex = 3;
            this.PortText.Text = "5000";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(206, 32);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(55, 15);
            this.label4.TabIndex = 4;
            this.label4.Text = "포트번호";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.ConnectBtn);
            this.groupBox1.Controls.Add(this.PortText);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.IPText);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(12, 34);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(553, 78);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "클라이언트 설정";
            // 
            // ConnectBtn
            // 
            this.ConnectBtn.Location = new System.Drawing.Point(422, 22);
            this.ConnectBtn.Name = "ConnectBtn";
            this.ConnectBtn.Size = new System.Drawing.Size(111, 35);
            this.ConnectBtn.TabIndex = 6;
            this.ConnectBtn.Text = "서버 접속";
            this.ConnectBtn.UseVisualStyleBackColor = true;
            this.ConnectBtn.Click += new System.EventHandler(this.button1_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.LoginBtn);
            this.groupBox2.Controls.Add(this.PWText);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.IDText);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Location = new System.Drawing.Point(12, 132);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(553, 78);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "로그인 설정";
            // 
            // LoginBtn
            // 
            this.LoginBtn.Location = new System.Drawing.Point(422, 22);
            this.LoginBtn.Name = "LoginBtn";
            this.LoginBtn.Size = new System.Drawing.Size(111, 35);
            this.LoginBtn.TabIndex = 6;
            this.LoginBtn.Text = "로그인";
            this.LoginBtn.UseVisualStyleBackColor = true;
            this.LoginBtn.Click += new System.EventHandler(this.LoginBtn_Click);
            // 
            // PWText
            // 
            this.PWText.Location = new System.Drawing.Point(282, 29);
            this.PWText.Margin = new System.Windows.Forms.Padding(2);
            this.PWText.Name = "PWText";
            this.PWText.Size = new System.Drawing.Size(106, 23);
            this.PWText.TabIndex = 3;
            this.PWText.Text = "test001";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(206, 32);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(55, 15);
            this.label1.TabIndex = 4;
            this.label1.Text = "비밀번호";
            // 
            // IDText
            // 
            this.IDText.Location = new System.Drawing.Point(85, 29);
            this.IDText.Margin = new System.Windows.Forms.Padding(2);
            this.IDText.Name = "IDText";
            this.IDText.Size = new System.Drawing.Size(106, 23);
            this.IDText.TabIndex = 0;
            this.IDText.Text = "test001";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 31);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(43, 15);
            this.label3.TabIndex = 2;
            this.label3.Text = "아이디";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.RoomListRefreshBtn);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.RoomCreateBtn);
            this.groupBox3.Controls.Add(this.listBox1);
            this.groupBox3.Controls.Add(this.RoomEnterBtn);
            this.groupBox3.Location = new System.Drawing.Point(12, 230);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(553, 240);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "로비";
            // 
            // RoomListRefreshBtn
            // 
            this.RoomListRefreshBtn.Location = new System.Drawing.Point(405, 52);
            this.RoomListRefreshBtn.Name = "RoomListRefreshBtn";
            this.RoomListRefreshBtn.Size = new System.Drawing.Size(111, 35);
            this.RoomListRefreshBtn.TabIndex = 10;
            this.RoomListRefreshBtn.Text = "새로고침";
            this.RoomListRefreshBtn.UseVisualStyleBackColor = true;
            this.RoomListRefreshBtn.Click += new System.EventHandler(this.RoomListRefreshBtn_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(15, 34);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(43, 15);
            this.label7.TabIndex = 9;
            this.label7.Text = "방목록";
            // 
            // RoomCreateBtn
            // 
            this.RoomCreateBtn.Location = new System.Drawing.Point(405, 176);
            this.RoomCreateBtn.Name = "RoomCreateBtn";
            this.RoomCreateBtn.Size = new System.Drawing.Size(111, 35);
            this.RoomCreateBtn.TabIndex = 8;
            this.RoomCreateBtn.Text = "방 생성";
            this.RoomCreateBtn.UseVisualStyleBackColor = true;
            this.RoomCreateBtn.Click += new System.EventHandler(this.RoomCreateBtn_Click);
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 15;
            this.listBox1.Location = new System.Drawing.Point(15, 52);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(356, 169);
            this.listBox1.TabIndex = 7;
            // 
            // RoomEnterBtn
            // 
            this.RoomEnterBtn.DialogResult = System.Windows.Forms.DialogResult.Abort;
            this.RoomEnterBtn.Location = new System.Drawing.Point(405, 114);
            this.RoomEnterBtn.Name = "RoomEnterBtn";
            this.RoomEnterBtn.Size = new System.Drawing.Size(111, 35);
            this.RoomEnterBtn.TabIndex = 6;
            this.RoomEnterBtn.Text = "접속";
            this.RoomEnterBtn.UseVisualStyleBackColor = true;
            this.RoomEnterBtn.Click += new System.EventHandler(this.RoomEnterBtn_Click);
            // 
            // listBox2
            // 
            this.listBox2.FormattingEnabled = true;
            this.listBox2.ItemHeight = 15;
            this.listBox2.Location = new System.Drawing.Point(15, 60);
            this.listBox2.Name = "listBox2";
            this.listBox2.Size = new System.Drawing.Size(134, 154);
            this.listBox2.TabIndex = 7;
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(452, 184);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(81, 35);
            this.button5.TabIndex = 8;
            this.button5.Text = "보내기";
            this.button5.UseVisualStyleBackColor = true;
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(180, 191);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(266, 23);
            this.textBox5.TabIndex = 9;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Controls.Add(this.listBox3);
            this.groupBox4.Controls.Add(this.textBox5);
            this.groupBox4.Controls.Add(this.button5);
            this.groupBox4.Controls.Add(this.listBox2);
            this.groupBox4.Location = new System.Drawing.Point(12, 486);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(553, 247);
            this.groupBox4.TabIndex = 8;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "채팅방";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(15, 39);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(55, 15);
            this.label5.TabIndex = 11;
            this.label5.Text = "유저목록";
            // 
            // listBox3
            // 
            this.listBox3.FormattingEnabled = true;
            this.listBox3.ItemHeight = 15;
            this.listBox3.Location = new System.Drawing.Point(180, 39);
            this.listBox3.Name = "listBox3";
            this.listBox3.Size = new System.Drawing.Size(353, 139);
            this.listBox3.TabIndex = 10;
            // 
            // logListBox
            // 
            this.logListBox.FormattingEnabled = true;
            this.logListBox.ItemHeight = 15;
            this.logListBox.Location = new System.Drawing.Point(604, 54);
            this.logListBox.Name = "logListBox";
            this.logListBox.Size = new System.Drawing.Size(555, 679);
            this.logListBox.TabIndex = 9;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(604, 34);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(31, 15);
            this.label6.TabIndex = 10;
            this.label6.Text = "로그";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1184, 757);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.logListBox);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private TextBox IPText;
        private Label label2;
        private TextBox PortText;
        private Label label4;
        private GroupBox groupBox1;
        private Button ConnectBtn;
        private GroupBox groupBox2;
        private Button LoginBtn;
        private TextBox PWText;
        private Label label1;
        private TextBox IDText;
        private Label label3;
        private GroupBox groupBox3;
        private Button RoomCreateBtn;
        private ListBox listBox1;
        private Button RoomEnterBtn;
        private ListBox listBox2;
        private Button button5;
        private TextBox textBox5;
        private GroupBox groupBox4;
        private Label label5;
        private ListBox listBox3;
        private ListBox logListBox;
        private Label label6;
        private Button RoomListRefreshBtn;
        private Label label7;
    }
}