namespace PJ1000_Update_Server
{
    partial class MainForm
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.stopServerBtn = new System.Windows.Forms.Button();
            this.startServerBtn = new System.Windows.Forms.Button();
            this.portTxtBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.binPathTxtBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.browseBinBtn = new System.Windows.Forms.Button();
            this.dockContainerItem1 = new DevComponents.DotNetBar.DockContainerItem();
            this.clientGridView = new DevComponents.DotNetBar.Controls.DataGridViewX();
            this.allowUpdateCol = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.clientIMEICol = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.deviceNameCol = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.clientIPCol = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.clientPortCol = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.clientVersionCol = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.newFirmwareCol = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.elapsedTimeCol = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.downloadProgressCol = new DevComponents.DotNetBar.Controls.DataGridViewProgressBarXColumn();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.logTxtBox = new System.Windows.Forms.TextBox();
            this.selectAllBtn = new System.Windows.Forms.Button();
            this.selectNoneBtn = new System.Windows.Forms.Button();
            this.downloadFirmwareBtn = new System.Windows.Forms.Button();
            this.openBinDialog = new System.Windows.Forms.OpenFileDialog();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.firmwareVersionSearchTxt = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.ipSearchTxt = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.deviceNameSearchTxt = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.imeiSearchTxt = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.searchBtn = new System.Windows.Forms.Button();
            this.firmwareVersionFilterTxt = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.setFilterBtn = new System.Windows.Forms.Button();
            this.ipFilterTxt = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.deviceNameFilterTxt = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.imeiFilterTxt = new System.Windows.Forms.TextBox();
            this.registerBtn = new System.Windows.Forms.Button();
            this.nConnectedDevicesLbl = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.clientGridView)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // stopServerBtn
            // 
            this.stopServerBtn.Enabled = false;
            this.stopServerBtn.Location = new System.Drawing.Point(254, 12);
            this.stopServerBtn.Name = "stopServerBtn";
            this.stopServerBtn.Size = new System.Drawing.Size(92, 23);
            this.stopServerBtn.TabIndex = 18;
            this.stopServerBtn.Text = "Stop Server";
            this.stopServerBtn.UseVisualStyleBackColor = true;
            this.stopServerBtn.Click += new System.EventHandler(this.stopServerBtn_Click);
            // 
            // startServerBtn
            // 
            this.startServerBtn.Location = new System.Drawing.Point(156, 12);
            this.startServerBtn.Name = "startServerBtn";
            this.startServerBtn.Size = new System.Drawing.Size(92, 23);
            this.startServerBtn.TabIndex = 17;
            this.startServerBtn.Text = "Start Server";
            this.startServerBtn.UseVisualStyleBackColor = true;
            this.startServerBtn.Click += new System.EventHandler(this.startServerBtn_Click);
            // 
            // portTxtBox
            // 
            this.portTxtBox.Location = new System.Drawing.Point(73, 14);
            this.portTxtBox.Name = "portTxtBox";
            this.portTxtBox.Size = new System.Drawing.Size(77, 20);
            this.portTxtBox.TabIndex = 16;
            this.portTxtBox.Text = "50000";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(26, 13);
            this.label1.TabIndex = 15;
            this.label1.Text = "Port";
            // 
            // binPathTxtBox
            // 
            this.binPathTxtBox.Location = new System.Drawing.Point(73, 40);
            this.binPathTxtBox.Name = "binPathTxtBox";
            this.binPathTxtBox.Size = new System.Drawing.Size(405, 20);
            this.binPathTxtBox.TabIndex = 20;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(50, 13);
            this.label2.TabIndex = 19;
            this.label2.Text = "BIN Path";
            // 
            // browseBinBtn
            // 
            this.browseBinBtn.Location = new System.Drawing.Point(484, 38);
            this.browseBinBtn.Name = "browseBinBtn";
            this.browseBinBtn.Size = new System.Drawing.Size(92, 23);
            this.browseBinBtn.TabIndex = 21;
            this.browseBinBtn.Text = "Browse...";
            this.browseBinBtn.UseVisualStyleBackColor = true;
            this.browseBinBtn.Click += new System.EventHandler(this.browseBinBtn_Click);
            // 
            // dockContainerItem1
            // 
            this.dockContainerItem1.Name = "dockContainerItem1";
            this.dockContainerItem1.Text = "dockContainerItem1";
            // 
            // clientGridView
            // 
            this.clientGridView.AllowUserToAddRows = false;
            this.clientGridView.AllowUserToDeleteRows = false;
            this.clientGridView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.clientGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.clientGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.allowUpdateCol,
            this.clientIMEICol,
            this.deviceNameCol,
            this.clientIPCol,
            this.clientPortCol,
            this.clientVersionCol,
            this.newFirmwareCol,
            this.elapsedTimeCol,
            this.downloadProgressCol});
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.clientGridView.DefaultCellStyle = dataGridViewCellStyle1;
            this.clientGridView.GridColor = System.Drawing.Color.FromArgb(((int)(((byte)(208)))), ((int)(((byte)(215)))), ((int)(((byte)(229)))));
            this.clientGridView.Location = new System.Drawing.Point(12, 67);
            this.clientGridView.Name = "clientGridView";
            this.clientGridView.RowHeadersVisible = false;
            this.clientGridView.Size = new System.Drawing.Size(1155, 251);
            this.clientGridView.TabIndex = 23;
            this.clientGridView.RowsAdded += new System.Windows.Forms.DataGridViewRowsAddedEventHandler(this.clientGridView_RowsAdded);
            this.clientGridView.RowsRemoved += new System.Windows.Forms.DataGridViewRowsRemovedEventHandler(this.clientGridView_RowsRemoved);
            // 
            // allowUpdateCol
            // 
            this.allowUpdateCol.HeaderText = "Accept";
            this.allowUpdateCol.MinimumWidth = 50;
            this.allowUpdateCol.Name = "allowUpdateCol";
            this.allowUpdateCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.allowUpdateCol.Width = 50;
            // 
            // clientIMEICol
            // 
            this.clientIMEICol.HeaderText = "IMEI";
            this.clientIMEICol.MinimumWidth = 150;
            this.clientIMEICol.Name = "clientIMEICol";
            this.clientIMEICol.ReadOnly = true;
            this.clientIMEICol.Width = 150;
            // 
            // deviceNameCol
            // 
            this.deviceNameCol.HeaderText = "Device Name";
            this.deviceNameCol.MinimumWidth = 120;
            this.deviceNameCol.Name = "deviceNameCol";
            this.deviceNameCol.ReadOnly = true;
            this.deviceNameCol.Width = 120;
            // 
            // clientIPCol
            // 
            this.clientIPCol.HeaderText = "IP";
            this.clientIPCol.MinimumWidth = 150;
            this.clientIPCol.Name = "clientIPCol";
            this.clientIPCol.ReadOnly = true;
            this.clientIPCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.clientIPCol.Width = 150;
            // 
            // clientPortCol
            // 
            this.clientPortCol.HeaderText = "Port";
            this.clientPortCol.MinimumWidth = 75;
            this.clientPortCol.Name = "clientPortCol";
            this.clientPortCol.ReadOnly = true;
            this.clientPortCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            // 
            // clientVersionCol
            // 
            this.clientVersionCol.HeaderText = "Current Version";
            this.clientVersionCol.MinimumWidth = 100;
            this.clientVersionCol.Name = "clientVersionCol";
            this.clientVersionCol.ReadOnly = true;
            // 
            // newFirmwareCol
            // 
            this.newFirmwareCol.HeaderText = "New Firmware";
            this.newFirmwareCol.MinimumWidth = 200;
            this.newFirmwareCol.Name = "newFirmwareCol";
            this.newFirmwareCol.Width = 200;
            // 
            // elapsedTimeCol
            // 
            this.elapsedTimeCol.HeaderText = "Elapsed Time";
            this.elapsedTimeCol.MinimumWidth = 100;
            this.elapsedTimeCol.Name = "elapsedTimeCol";
            this.elapsedTimeCol.ReadOnly = true;
            // 
            // downloadProgressCol
            // 
            this.downloadProgressCol.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.downloadProgressCol.HeaderText = "Progress";
            this.downloadProgressCol.MinimumWidth = 50;
            this.downloadProgressCol.Name = "downloadProgressCol";
            this.downloadProgressCol.ReadOnly = true;
            this.downloadProgressCol.Text = null;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.logTxtBox);
            this.groupBox1.Location = new System.Drawing.Point(12, 413);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(1155, 137);
            this.groupBox1.TabIndex = 24;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Log";
            // 
            // logTxtBox
            // 
            this.logTxtBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.logTxtBox.Location = new System.Drawing.Point(3, 16);
            this.logTxtBox.Multiline = true;
            this.logTxtBox.Name = "logTxtBox";
            this.logTxtBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.logTxtBox.Size = new System.Drawing.Size(1149, 118);
            this.logTxtBox.TabIndex = 0;
            this.logTxtBox.WordWrap = false;
            // 
            // selectAllBtn
            // 
            this.selectAllBtn.Location = new System.Drawing.Point(6, 20);
            this.selectAllBtn.Name = "selectAllBtn";
            this.selectAllBtn.Size = new System.Drawing.Size(92, 23);
            this.selectAllBtn.TabIndex = 25;
            this.selectAllBtn.Text = "Select All";
            this.selectAllBtn.UseVisualStyleBackColor = true;
            this.selectAllBtn.Click += new System.EventHandler(this.selectAllBtn_Click);
            // 
            // selectNoneBtn
            // 
            this.selectNoneBtn.Location = new System.Drawing.Point(104, 20);
            this.selectNoneBtn.Name = "selectNoneBtn";
            this.selectNoneBtn.Size = new System.Drawing.Size(92, 23);
            this.selectNoneBtn.TabIndex = 26;
            this.selectNoneBtn.Text = "Select None";
            this.selectNoneBtn.UseVisualStyleBackColor = true;
            this.selectNoneBtn.Click += new System.EventHandler(this.selectNoneBtn_Click);
            // 
            // downloadFirmwareBtn
            // 
            this.downloadFirmwareBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.downloadFirmwareBtn.Location = new System.Drawing.Point(6, 49);
            this.downloadFirmwareBtn.Name = "downloadFirmwareBtn";
            this.downloadFirmwareBtn.Size = new System.Drawing.Size(190, 23);
            this.downloadFirmwareBtn.TabIndex = 27;
            this.downloadFirmwareBtn.Text = "Download Firmware";
            this.downloadFirmwareBtn.UseVisualStyleBackColor = true;
            this.downloadFirmwareBtn.Click += new System.EventHandler(this.downloadFirmwareBtn_Click);
            // 
            // openBinDialog
            // 
            this.openBinDialog.Filter = "BIN File (*.BIN)|*.BIN";
            this.openBinDialog.Title = "Select BIN File";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(221, 24);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 13);
            this.label3.TabIndex = 28;
            this.label3.Text = "IMEI";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.firmwareVersionSearchTxt);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.ipSearchTxt);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.deviceNameSearchTxt);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Controls.Add(this.imeiSearchTxt);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.searchBtn);
            this.groupBox2.Controls.Add(this.firmwareVersionFilterTxt);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.setFilterBtn);
            this.groupBox2.Controls.Add(this.ipFilterTxt);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.deviceNameFilterTxt);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.imeiFilterTxt);
            this.groupBox2.Controls.Add(this.selectAllBtn);
            this.groupBox2.Controls.Add(this.downloadFirmwareBtn);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.selectNoneBtn);
            this.groupBox2.Location = new System.Drawing.Point(12, 324);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(1155, 83);
            this.groupBox2.TabIndex = 29;
            this.groupBox2.TabStop = false;
            // 
            // firmwareVersionSearchTxt
            // 
            this.firmwareVersionSearchTxt.Location = new System.Drawing.Point(856, 51);
            this.firmwareVersionSearchTxt.Name = "firmwareVersionSearchTxt";
            this.firmwareVersionSearchTxt.Size = new System.Drawing.Size(130, 20);
            this.firmwareVersionSearchTxt.TabIndex = 45;
            this.firmwareVersionSearchTxt.TextChanged += new System.EventHandler(this.searchDetails_Changed);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(763, 54);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(87, 13);
            this.label8.TabIndex = 44;
            this.label8.Text = "Firmware Version";
            // 
            // ipSearchTxt
            // 
            this.ipSearchTxt.Location = new System.Drawing.Point(627, 51);
            this.ipSearchTxt.Name = "ipSearchTxt";
            this.ipSearchTxt.Size = new System.Drawing.Size(130, 20);
            this.ipSearchTxt.TabIndex = 43;
            this.ipSearchTxt.TextChanged += new System.EventHandler(this.searchDetails_Changed);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(604, 54);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(17, 13);
            this.label9.TabIndex = 42;
            this.label9.Text = "IP";
            // 
            // deviceNameSearchTxt
            // 
            this.deviceNameSearchTxt.Location = new System.Drawing.Point(468, 51);
            this.deviceNameSearchTxt.Name = "deviceNameSearchTxt";
            this.deviceNameSearchTxt.Size = new System.Drawing.Size(130, 20);
            this.deviceNameSearchTxt.TabIndex = 41;
            this.deviceNameSearchTxt.TextChanged += new System.EventHandler(this.searchDetails_Changed);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(390, 54);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(72, 13);
            this.label10.TabIndex = 40;
            this.label10.Text = "Device Name";
            // 
            // imeiSearchTxt
            // 
            this.imeiSearchTxt.Location = new System.Drawing.Point(254, 51);
            this.imeiSearchTxt.Name = "imeiSearchTxt";
            this.imeiSearchTxt.Size = new System.Drawing.Size(130, 20);
            this.imeiSearchTxt.TabIndex = 39;
            this.imeiSearchTxt.TextChanged += new System.EventHandler(this.searchDetails_Changed);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(221, 54);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(29, 13);
            this.label7.TabIndex = 38;
            this.label7.Text = "IMEI";
            // 
            // searchBtn
            // 
            this.searchBtn.Location = new System.Drawing.Point(1009, 49);
            this.searchBtn.Name = "searchBtn";
            this.searchBtn.Size = new System.Drawing.Size(140, 23);
            this.searchBtn.TabIndex = 37;
            this.searchBtn.Text = "Search";
            this.searchBtn.UseVisualStyleBackColor = true;
            this.searchBtn.Click += new System.EventHandler(this.searchBtn_Click);
            // 
            // firmwareVersionFilterTxt
            // 
            this.firmwareVersionFilterTxt.Location = new System.Drawing.Point(856, 21);
            this.firmwareVersionFilterTxt.Name = "firmwareVersionFilterTxt";
            this.firmwareVersionFilterTxt.Size = new System.Drawing.Size(130, 20);
            this.firmwareVersionFilterTxt.TabIndex = 36;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(763, 24);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(87, 13);
            this.label6.TabIndex = 35;
            this.label6.Text = "Firmware Version";
            // 
            // setFilterBtn
            // 
            this.setFilterBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.setFilterBtn.Location = new System.Drawing.Point(1009, 19);
            this.setFilterBtn.Name = "setFilterBtn";
            this.setFilterBtn.Size = new System.Drawing.Size(140, 23);
            this.setFilterBtn.TabIndex = 34;
            this.setFilterBtn.Text = "Set Filter";
            this.setFilterBtn.UseVisualStyleBackColor = true;
            this.setFilterBtn.Click += new System.EventHandler(this.setFilterBtn_Click);
            // 
            // ipFilterTxt
            // 
            this.ipFilterTxt.Location = new System.Drawing.Point(627, 21);
            this.ipFilterTxt.Name = "ipFilterTxt";
            this.ipFilterTxt.Size = new System.Drawing.Size(130, 20);
            this.ipFilterTxt.TabIndex = 33;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(604, 24);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(17, 13);
            this.label5.TabIndex = 32;
            this.label5.Text = "IP";
            // 
            // deviceNameFilterTxt
            // 
            this.deviceNameFilterTxt.Location = new System.Drawing.Point(468, 21);
            this.deviceNameFilterTxt.Name = "deviceNameFilterTxt";
            this.deviceNameFilterTxt.Size = new System.Drawing.Size(130, 20);
            this.deviceNameFilterTxt.TabIndex = 31;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(390, 24);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(72, 13);
            this.label4.TabIndex = 30;
            this.label4.Text = "Device Name";
            // 
            // imeiFilterTxt
            // 
            this.imeiFilterTxt.Location = new System.Drawing.Point(254, 21);
            this.imeiFilterTxt.Name = "imeiFilterTxt";
            this.imeiFilterTxt.Size = new System.Drawing.Size(130, 20);
            this.imeiFilterTxt.TabIndex = 29;
            // 
            // registerBtn
            // 
            this.registerBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.registerBtn.Location = new System.Drawing.Point(982, 12);
            this.registerBtn.Name = "registerBtn";
            this.registerBtn.Size = new System.Drawing.Size(185, 44);
            this.registerBtn.TabIndex = 30;
            this.registerBtn.Text = "Register Your Product";
            this.registerBtn.UseVisualStyleBackColor = true;
            this.registerBtn.Click += new System.EventHandler(this.registerBtn_Click);
            // 
            // nConnectedDevicesLbl
            // 
            this.nConnectedDevicesLbl.AutoSize = true;
            this.nConnectedDevicesLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.nConnectedDevicesLbl.Location = new System.Drawing.Point(615, 20);
            this.nConnectedDevicesLbl.Name = "nConnectedDevicesLbl";
            this.nConnectedDevicesLbl.Size = new System.Drawing.Size(215, 24);
            this.nConnectedDevicesLbl.TabIndex = 31;
            this.nConnectedDevicesLbl.Text = "Connected Devices: 0";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1179, 562);
            this.Controls.Add(this.nConnectedDevicesLbl);
            this.Controls.Add(this.registerBtn);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.clientGridView);
            this.Controls.Add(this.stopServerBtn);
            this.Controls.Add(this.browseBinBtn);
            this.Controls.Add(this.binPathTxtBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.startServerBtn);
            this.Controls.Add(this.portTxtBox);
            this.Controls.Add(this.label1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(1000, 600);
            this.Name = "MainForm";
            this.Text = "PJ1000 OTA firmware update";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.clientGridView)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button stopServerBtn;
        private System.Windows.Forms.Button startServerBtn;
        private System.Windows.Forms.TextBox portTxtBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox binPathTxtBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button browseBinBtn;
        private DevComponents.DotNetBar.DockContainerItem dockContainerItem1;
        private DevComponents.DotNetBar.Controls.DataGridViewX clientGridView;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button selectAllBtn;
        private System.Windows.Forms.Button selectNoneBtn;
        private System.Windows.Forms.Button downloadFirmwareBtn;
        private System.Windows.Forms.OpenFileDialog openBinDialog;
        public System.Windows.Forms.TextBox logTxtBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button setFilterBtn;
        private System.Windows.Forms.TextBox ipFilterTxt;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox deviceNameFilterTxt;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox imeiFilterTxt;
        private System.Windows.Forms.TextBox firmwareVersionFilterTxt;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.DataGridViewCheckBoxColumn allowUpdateCol;
        private System.Windows.Forms.DataGridViewTextBoxColumn clientIMEICol;
        private System.Windows.Forms.DataGridViewTextBoxColumn deviceNameCol;
        private System.Windows.Forms.DataGridViewTextBoxColumn clientIPCol;
        private System.Windows.Forms.DataGridViewTextBoxColumn clientPortCol;
        private System.Windows.Forms.DataGridViewTextBoxColumn clientVersionCol;
        private System.Windows.Forms.DataGridViewTextBoxColumn newFirmwareCol;
        private System.Windows.Forms.DataGridViewTextBoxColumn elapsedTimeCol;
        private DevComponents.DotNetBar.Controls.DataGridViewProgressBarXColumn downloadProgressCol;
        private System.Windows.Forms.Button registerBtn;
        private System.Windows.Forms.TextBox firmwareVersionSearchTxt;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox ipSearchTxt;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox deviceNameSearchTxt;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox imeiSearchTxt;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button searchBtn;
        private System.Windows.Forms.Label nConnectedDevicesLbl;
    }
}

