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
            this.LobbyRoomList = new System.Windows.Forms.ListBox();
            this.RoomEnterBtn = new System.Windows.Forms.Button();
            this.UserList = new System.Windows.Forms.ListBox();
            this.ChatSend = new System.Windows.Forms.Button();
            this.ChatBox = new System.Windows.Forms.TextBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.RoomExit = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.listBoxRoomChatMsg = new System.Windows.Forms.ListBox();
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
            this.IPText.Location = new System.Drawing.Point(121, 48);
            this.IPText.Name = "IPText";
            this.IPText.Size = new System.Drawing.Size(150, 31);
            this.IPText.TabIndex = 0;
            this.IPText.Text = "127.0.0.1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(21, 52);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(84, 25);
            this.label2.TabIndex = 2;
            this.label2.Text = "서버주소";
            // 
            // PortText
            // 
            this.PortText.Location = new System.Drawing.Point(403, 48);
            this.PortText.Name = "PortText";
            this.PortText.Size = new System.Drawing.Size(150, 31);
            this.PortText.TabIndex = 3;
            this.PortText.Text = "5000";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(294, 53);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(84, 25);
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
            this.groupBox1.Location = new System.Drawing.Point(17, 57);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox1.Size = new System.Drawing.Size(790, 130);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "클라이언트 설정";
            // 
            // ConnectBtn
            // 
            this.ConnectBtn.Location = new System.Drawing.Point(603, 37);
            this.ConnectBtn.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.ConnectBtn.Name = "ConnectBtn";
            this.ConnectBtn.Size = new System.Drawing.Size(159, 58);
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
            this.groupBox2.Location = new System.Drawing.Point(17, 220);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox2.Size = new System.Drawing.Size(790, 130);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "로그인 설정";
            // 
            // LoginBtn
            // 
            this.LoginBtn.Location = new System.Drawing.Point(603, 37);
            this.LoginBtn.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.LoginBtn.Name = "LoginBtn";
            this.LoginBtn.Size = new System.Drawing.Size(159, 58);
            this.LoginBtn.TabIndex = 6;
            this.LoginBtn.Text = "로그인";
            this.LoginBtn.UseVisualStyleBackColor = true;
            this.LoginBtn.Click += new System.EventHandler(this.LoginBtn_Click);
            // 
            // PWText
            // 
            this.PWText.Location = new System.Drawing.Point(403, 48);
            this.PWText.Name = "PWText";
            this.PWText.Size = new System.Drawing.Size(150, 31);
            this.PWText.TabIndex = 3;
            this.PWText.Text = "test001";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(294, 53);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 25);
            this.label1.TabIndex = 4;
            this.label1.Text = "비밀번호";
            // 
            // IDText
            // 
            this.IDText.Location = new System.Drawing.Point(121, 48);
            this.IDText.Name = "IDText";
            this.IDText.Size = new System.Drawing.Size(150, 31);
            this.IDText.TabIndex = 0;
            this.IDText.Text = "test001";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(21, 52);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(66, 25);
            this.label3.TabIndex = 2;
            this.label3.Text = "아이디";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.RoomListRefreshBtn);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.LobbyRoomList);
            this.groupBox3.Controls.Add(this.RoomEnterBtn);
            this.groupBox3.Location = new System.Drawing.Point(17, 383);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox3.Size = new System.Drawing.Size(790, 400);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "로비";
            // 
            // RoomListRefreshBtn
            // 
            this.RoomListRefreshBtn.Location = new System.Drawing.Point(579, 87);
            this.RoomListRefreshBtn.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.RoomListRefreshBtn.Name = "RoomListRefreshBtn";
            this.RoomListRefreshBtn.Size = new System.Drawing.Size(159, 58);
            this.RoomListRefreshBtn.TabIndex = 10;
            this.RoomListRefreshBtn.Text = "새로고침";
            this.RoomListRefreshBtn.UseVisualStyleBackColor = true;
            this.RoomListRefreshBtn.Click += new System.EventHandler(this.RoomListRefreshBtn_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(21, 57);
            this.label7.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(66, 25);
            this.label7.TabIndex = 9;
            this.label7.Text = "방목록";
            // 
            // LobbyRoomList
            // 
            this.LobbyRoomList.FormattingEnabled = true;
            this.LobbyRoomList.ItemHeight = 25;
            this.LobbyRoomList.Location = new System.Drawing.Point(21, 87);
            this.LobbyRoomList.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.LobbyRoomList.Name = "LobbyRoomList";
            this.LobbyRoomList.Size = new System.Drawing.Size(507, 279);
            this.LobbyRoomList.TabIndex = 7;
            // 
            // RoomEnterBtn
            // 
            this.RoomEnterBtn.DialogResult = System.Windows.Forms.DialogResult.Abort;
            this.RoomEnterBtn.Location = new System.Drawing.Point(579, 178);
            this.RoomEnterBtn.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.RoomEnterBtn.Name = "RoomEnterBtn";
            this.RoomEnterBtn.Size = new System.Drawing.Size(159, 58);
            this.RoomEnterBtn.TabIndex = 6;
            this.RoomEnterBtn.Text = "접속";
            this.RoomEnterBtn.UseVisualStyleBackColor = true;
            this.RoomEnterBtn.Click += new System.EventHandler(this.RoomEnterBtn_Click);
            // 
            // UserList
            // 
            this.UserList.FormattingEnabled = true;
            this.UserList.ItemHeight = 25;
            this.UserList.Location = new System.Drawing.Point(21, 65);
            this.UserList.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.UserList.Name = "UserList";
            this.UserList.Size = new System.Drawing.Size(190, 229);
            this.UserList.TabIndex = 7;
            // 
            // ChatSend
            // 
            this.ChatSend.Location = new System.Drawing.Point(646, 307);
            this.ChatSend.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.ChatSend.Name = "ChatSend";
            this.ChatSend.Size = new System.Drawing.Size(116, 58);
            this.ChatSend.TabIndex = 8;
            this.ChatSend.Text = "보내기";
            this.ChatSend.UseVisualStyleBackColor = true;
            this.ChatSend.Click += new System.EventHandler(this.ChatSend_Click);
            // 
            // ChatBox
            // 
            this.ChatBox.Location = new System.Drawing.Point(257, 318);
            this.ChatBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.ChatBox.Name = "ChatBox";
            this.ChatBox.Size = new System.Drawing.Size(378, 31);
            this.ChatBox.TabIndex = 9;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.RoomExit);
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Controls.Add(this.listBoxRoomChatMsg);
            this.groupBox4.Controls.Add(this.ChatBox);
            this.groupBox4.Controls.Add(this.ChatSend);
            this.groupBox4.Controls.Add(this.UserList);
            this.groupBox4.Location = new System.Drawing.Point(17, 810);
            this.groupBox4.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox4.Size = new System.Drawing.Size(790, 412);
            this.groupBox4.TabIndex = 8;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "채팅방";
            // 
            // RoomExit
            // 
            this.RoomExit.Location = new System.Drawing.Point(21, 313);
            this.RoomExit.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.RoomExit.Name = "RoomExit";
            this.RoomExit.Size = new System.Drawing.Size(190, 47);
            this.RoomExit.TabIndex = 12;
            this.RoomExit.Text = "퇴장";
            this.RoomExit.UseVisualStyleBackColor = true;
            this.RoomExit.Click += new System.EventHandler(this.RoomExit_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(21, 30);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(84, 25);
            this.label5.TabIndex = 11;
            this.label5.Text = "유저목록";
            // 
            // listBoxRoomChatMsg
            // 
            this.listBoxRoomChatMsg.FormattingEnabled = true;
            this.listBoxRoomChatMsg.ItemHeight = 25;
            this.listBoxRoomChatMsg.Location = new System.Drawing.Point(257, 65);
            this.listBoxRoomChatMsg.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.listBoxRoomChatMsg.Name = "listBoxRoomChatMsg";
            this.listBoxRoomChatMsg.Size = new System.Drawing.Size(503, 229);
            this.listBoxRoomChatMsg.TabIndex = 10;
            // 
            // logListBox
            // 
            this.logListBox.FormattingEnabled = true;
            this.logListBox.ItemHeight = 25;
            this.logListBox.Location = new System.Drawing.Point(863, 90);
            this.logListBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.logListBox.Name = "logListBox";
            this.logListBox.Size = new System.Drawing.Size(791, 1129);
            this.logListBox.TabIndex = 9;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(863, 57);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(48, 25);
            this.label6.TabIndex = 10;
            this.label6.Text = "로그";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1691, 1262);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.logListBox);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
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
        private ListBox LobbyRoomList;
        private Button RoomEnterBtn;
        private ListBox UserList;
        private Button ChatSend;
        private TextBox ChatBox;
        private GroupBox groupBox4;
        private Label label5;
        private ListBox listBoxRoomChatMsg;
        private ListBox logListBox;
        private Label label6;
        private Button RoomListRefreshBtn;
        private Label label7;
        private Button RoomExit;
    }
}