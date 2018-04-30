using System;
using System.Threading;
using System.Windows.Forms;
using System.IO;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Net;
using System.Collections.Generic;
using System.Drawing;
using System.ComponentModel;
using System.Linq;
using System.Diagnostics;
using System.Text;
using System.IO.Ports;

namespace PJ1000TrackerConfiguration
{
    public partial class MainForm : Form
    {
        readonly int GPS_CONFIG_DATA_SIZE = Marshal.SizeOf(typeof(DEVICE_CONFIG_DATA));

        const int MAX_TEXT_LENGTH = 1000000;

        const int DEFAULT_COMM_TIMEOUT = 500;
        const byte COMM_START_CODE = 0xCA;
        const int MAX_COMM_RETRY = 1;
        const int MAX_READ_REPORT_RETRY = 10;

        const byte DISABLE_LOG_OPCODE = 0x01;
        const byte ENABLE_LOG_OPCODE = 0x02;
        const byte READ_LOG_OPCODE = 0x03;
        const byte ENABLE_GPS_LOG_OPCODE = 0x04;
        const byte ENABLE_GSM_LOG_OPCODE = 0x05;
        const byte ENABLE_MCU_LOG_OPCODE = 0x06;
        const byte READ_CONFIG_OPCODE = 0x0C;
        const byte WRITE_CONFIG_OPCODE = 0x0D;
        const byte BOSS_PHONE_MAX = 10;
        const byte READ_FIRMWARE_VERSION_OPCODE = 0x11;
        const byte DOWNLOAD_FIRMWARE_OPCODE = 0x12;
        const byte SET_CAMERA_ID_OPCODE = 0x13;
        const byte SEND_SMS_OPCODE = 0x14;

        const byte BOOT_MODE_OPCODE = 0x88;
        const byte JUMP_TO_APP_OPCODE = 0xA5;
        const byte UPLOAD_REPORT_OPCODE = 0x0F;
        const byte RESET_FACTORY_OPCODE = 0xFD;
        const byte RESET_DEVICE_OPCODE = 0xFE;
        const byte PRINT_REPORT_OPCODE = 0x91;
        const byte FORMAT_SDCARD_OPCODE = 0xFB;
		const byte SERIAL_COMM_NTF_OPCODE = 0x88;

        const uint DEFAULT_PACKET_NO = 0xA5A5A5A5;
        const uint ERROR_PACKET_NO = 0xFFFFFFFF;
        const uint FINISH_PACKET_NO = 0x5A5A5A5A;
        const int START_CODE_OFFSET = 0;
        const int LENGTH_OFFSET = 1;
        const int OPCODE_OFFSET = 3;
        const int DATA_OFFSET = 4;
        const int PACKET_NO_OFFSET = 4;
        const int DATA_WITH_PACKET_NO_OFFSET = 8;
        const int SIMPLE_CRC_WITH_PACKET_NO_OFFSET = 8;
        const int LENGTH_SIZE = 2;
        const int PACKET_NO_SIZE = 4;
        const int HEADER_SIZE = OPCODE_OFFSET + 1;
        const int CRC_SIZE = 1;
        const int SIMPLE_PACKET_SIZE = HEADER_SIZE + PACKET_NO_SIZE + CRC_SIZE;
        const int MAX_DATA_SIZE = 507;
        const int SMS_SIZE = 32;

        const int READ_FROM_DEVICE = 0;
        const int READ_FROM_HARDDISK = 1;

        const int USB_COMMUNICATION = 0;
        const int COM_COMMUNICATION = 1;

        const int OTA_SERVER_SELECTED = 0;
        const int MQTT_SERVER_SELECTED = 1;
        const int CONFIG_SERVER_SELECTED = 2;
        const int FIRMWARE_SERVER_SELECTED = 3;
        //const int INFO_SERVER_SELECTED = 2;

        const int SERVER_USE_IP = 1;

        const int LOG_LENGTH_LOW_THRESHOLD = 10;
        const int LOG_LENGTH_HIGH_THRESHOLD = 50;
        const int LOG_TIME_THRESHOLD = 200;

        const uint MAX_DEVICE_BUFFER_SIZE = 5242880; // 5MB

        bool isInTrialPeriod = false;
        IntPtr notificationHandle = IntPtr.Zero;
        HID.Device hidDevice = new HID.Device();
        byte[] hidSendBuffer = new byte[65];
        byte[] hidReceivedBuffer = new byte[65];
        byte[] hidTempBuffer = new byte[4096];
        byte[] hidOutBuffer = new byte[4096];
        byte[] hidInBuffer = new byte[4096];
        byte[] recordBuff = new byte[4096];
		byte[] comInBuffer = new byte[4096];
		byte[] comOutBuffer = new byte[4096];
		byte[] comTempBuffer = new byte[4096];
        short vid = 0x2251;
        short pid = 0x2201;
        byte[] cfgData = new byte[4096];
        int cfgDataOffset;
        uint cfgDataSize;
        uint cfgDeviceBuffSize;
        uint cfgNumByteToSend;
        uint fDeviceBuffSize;
        DEVICE_CONFIG_DATA cfg;
        DEVICE_CONFIG_DATA tempCfg;
        string firmwareVersion;
        BinaryReader firmwareReader;
        bool downloadingFirmware;
        int hidBufIndex = 0;
		int comBufIndex = 0;
        bool configUpdateDisabled;
        int currentOperation;
        DateTime prevLogReceivedTime;
        bool bootMode;
        System.Timers.Timer operationTimer;
		int communicationType;

        private void MainForm_Load(object sender, EventArgs e)
        {
			Logger.addTextBoxAppender(logTxtBox);
            registerToolStripMenuItem.Visible = isInTrialPeriod;
            configUpdateDisabled = true;
			communicationType = COM_COMMUNICATION;

            cfg = new DEVICE_CONFIG_DATA();
            tempCfg = new DEVICE_CONFIG_DATA();

            selectLogTypeCmbBox.SelectedIndex = 0;

            firmwareVersion = "";
            firmwareVersionLbl.Text = "";

            prevLogReceivedTime = DateTime.Now;

            bootMode = false;

            operationTimer = new System.Timers.Timer();
            operationTimer.Interval = 1000;
            operationTimer.AutoReset = false;
            operationTimer.Elapsed += new System.Timers.ElapsedEventHandler(operationTimer_Elapsed);

            HID.RegisterDeviceNotification(this.Handle, ref notificationHandle);
            OpenHIDDevice();
        }

        private void OpenHIDDevice()
        {
            string[] devicePathName = new string[0];

            if (HID.FindDevices(vid, pid, ref devicePathName) > 0)
            {
                hidDevice.pathName = devicePathName[0];

                if (HID.OpenDevice(ref hidDevice))
                {
                    hidDevice.readStream.BeginRead(hidReceivedBuffer, 0, hidReceivedBuffer.Length, new AsyncCallback(hidDevice_DataReceived), null);

                    ReadConfig();
                }
            }

            UpdateUI();
        }

        private void CloseHIDDevice()
        {
            HID.CloseDevice(ref hidDevice);
            UpdateUI();
        }

        private void ReadConfig()
        {
			if (communicationType == USB_COMMUNICATION)
			{
				if (!hidDevice.connected)
				{
					return;
				}

				CreateHIDBuffer(PACKET_NO_SIZE, READ_CONFIG_OPCODE, DEFAULT_PACKET_NO);
				hidOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(hidOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				cfgDataOffset = 0;
				cfgDeviceBuffSize = 0;
				cfgDataSize = 0;
				SendHIDData(hidOutBuffer, 0, SIMPLE_PACKET_SIZE, 2000);
			}
			else
			{
				if (!serialPort.IsOpen)
				{
					return;
				}

				CreateCOMBuffer(PACKET_NO_SIZE, READ_CONFIG_OPCODE, DEFAULT_PACKET_NO);
				comOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(comOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				cfgDataOffset = 0;
				cfgDeviceBuffSize = 0;
				cfgDataSize = 0;
				SendCOMData(comOutBuffer, 0, SIMPLE_PACKET_SIZE, 1000);
			}
        }

        private void CreateHIDBuffer(int length, byte opcode, uint packetNo)
        {
            Array.Clear(hidOutBuffer, 0, hidOutBuffer.Length);
            hidOutBuffer[START_CODE_OFFSET] = COMM_START_CODE;
            Array.Copy(BitConverter.GetBytes(length), 0, hidOutBuffer, LENGTH_OFFSET, LENGTH_SIZE);
            hidOutBuffer[OPCODE_OFFSET] = opcode;
            Array.Copy(BitConverter.GetBytes(packetNo), 0, hidOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
            currentOperation = opcode;
        }

		private void CreateCOMBuffer(int length, byte opcode, UInt32 packetNo)
		{
			Array.Clear(comOutBuffer, 0, comOutBuffer.Length);
			comOutBuffer[START_CODE_OFFSET] = COMM_START_CODE;
			Array.Copy(BitConverter.GetBytes(length), 0, comOutBuffer, LENGTH_OFFSET, LENGTH_SIZE);
			comOutBuffer[OPCODE_OFFSET] = opcode;
			Array.Copy(BitConverter.GetBytes(packetNo), 0, comOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
			currentOperation = opcode;
		}

		#region data received handler
		private void hidDevice_DataReceived(IAsyncResult ar)
        {
            int length;
			int copyIndex;

            try
            {
                length = hidDevice.readStream.EndRead(ar);
                if (!ar.IsCompleted)
                {
                    // read is not complete
                    return;
                }
				if (hidBufIndex == 0)
				{
					copyIndex = -1;
					for (int i = 1; i < length; i++)
					{
						if (hidReceivedBuffer[i] == COMM_START_CODE)
						{
							copyIndex = i;
							break;
						}
					}
					if (copyIndex >= 0)
					{
						Array.Clear(hidTempBuffer, 0, hidTempBuffer.Length);
						Array.Copy(hidReceivedBuffer, copyIndex, hidTempBuffer, 0, length - copyIndex);
						hidBufIndex = length - copyIndex;
					}

				}
				else
				{
					Array.Copy(hidReceivedBuffer, 1, hidTempBuffer, hidBufIndex, length - 1);
					hidBufIndex += length - 1;
				}
                if (hidBufIndex >= (BitConverter.ToInt16(hidTempBuffer, LENGTH_OFFSET) + HEADER_SIZE + CRC_SIZE))
                {
                    hidBufIndex = 0;
                    Array.Copy(hidTempBuffer, hidInBuffer, hidInBuffer.Length);
                    ProcessReceivedHIDData();
                }

				hidDevice.readStream.BeginRead(hidReceivedBuffer, 0, hidReceivedBuffer.Length, new AsyncCallback(hidDevice_DataReceived), null);
            }
            catch
            {
            }
        }

		private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			byte[] b;
			int length;
			int copyIndex;

			length = serialPort.BytesToRead;
			b = new byte[length];
			serialPort.Read(b, 0, length);
			if (comBufIndex == 0)
			{
				copyIndex = -1;
				for (int i = 0; i < b.Length; i++)
				{
					if (b[i] == COMM_START_CODE)
					{
						copyIndex = i;
						break;
					}
				}
				if (copyIndex < 0)
				{
					return;
				}
				Array.Clear(comTempBuffer, 0, comTempBuffer.Length);
				Array.Copy(b, copyIndex, comTempBuffer, 0, b.Length - copyIndex);
				comBufIndex = b.Length - copyIndex;
			}
			else
			{
				Array.Copy(b, 0, comTempBuffer, comBufIndex, b.Length);
				comBufIndex += b.Length;
			}
			if (comBufIndex >= (BitConverter.ToInt16(comTempBuffer, LENGTH_OFFSET) + HEADER_SIZE + CRC_SIZE))
			{
				comBufIndex = 0;
				Array.Copy(comTempBuffer, comInBuffer, comInBuffer.Length);
				ProcessReceivedCOMData();
			}
		}
		#endregion

		private void UpdateTextBox()
        {
            int length;
            string str;

            length = BitConverter.ToInt16(hidInBuffer, LENGTH_OFFSET);
            str = System.Text.Encoding.ASCII.GetString(hidInBuffer, DATA_OFFSET, length);

            lock (logTxtBox)
            {
                if (logTxtBox.TextLength > MAX_TEXT_LENGTH)
                {
                    logTxtBox.Clear();
                }
                logTxtBox.Text += str;
                logTxtBox.SelectionStart = logTxtBox.Text.Length;
                logTxtBox.ScrollToCaret();
            }
        }

        private void UpdateConfigUI()
        {
            configUpdateDisabled = true;

            tempCfg = cfg;            

            // GPRS config
            gprsApnTxtBox.Text = cfg.gprsApn;
            gprsUserTxtBox.Text = cfg.gprsUser;
            gprsPwdTxtBox.Text = cfg.gprsPass;
            imeiLbl.Text = "IMEI: " + cfg.imei;
            imeiTxtBox.Text = cfg.imei;
            ccidTxtBox.Text = cfg.ccid;
            cimiTxtBox.Text = cfg.cimi;
            // server parameter

            serverSelection_Changed(null, null);

           
            //thienhaiblue
            // others
            idTxt.Text = cfg.id;
            ownerNumberCbBox.SelectedIndex = 0;
            ownerNumber_SelectionChanged(null, null);
           // resetCntTxt.Text = cfg.resetCnt.ToString();

            ccidTxtBox.Text = cfg.ccid;
            mqttUserPassTxtBox.Text = cfg.mqttPassword;
            mqttUserTxtBox.Text = cfg.mqttUsername;
            mqttUserPassTxtBox.Text = tempCfg.mqttPassword;
            idTxt.Text = tempCfg.id;
            thingNameTxtBox.Text = tempCfg.thingName;
            thingOrgTxtBox.Text = tempCfg.thingOrgranization;
            configUpdateDisabled = false;
        }

        private void UpdateUI()
        {
			if (bootMode)
			{
				connectionStatusLbl.Text = "BOOT MODE";
				runAppBtn.Enabled = true;
			}
			else
			{
				if (communicationType == USB_COMMUNICATION)
				{
					commTypeCmbBox.SelectedIndex = 0;
					commuTypeLbl.Text = "USB";
					if (hidDevice.connected)
					{
						deviceCmdGBox.Enabled = true;
						connectionStatusLbl.Text = "Connected";
						connectionStatusLbl.ForeColor = Color.Green;
					}
					else
					{
						deviceCmdGBox.Enabled = false;
						connectionStatusLbl.Text = "Disconnected";
						connectionStatusLbl.ForeColor = Color.Red;
					}
				}
				else
				{
					commTypeCmbBox.SelectedIndex = 1;
					if ((serialPort != null) && serialPort.IsOpen)
					{
						deviceCmdGBox.Enabled = true;
						commuTypeLbl.Text = serialPort.PortName;
						comPortCmbBox.Text = serialPort.PortName;
						connectionStatusLbl.Text = "Connected";
						connectionStatusLbl.ForeColor = Color.Green;
					}
					else
					{
						deviceCmdGBox.Enabled = false;
						commuTypeLbl.Text = "COM";
						connectionStatusLbl.Text = "Disconnected";
						connectionStatusLbl.ForeColor = Color.Red;
					}
				}
			}
            connectionStatusLbl.Left = statusGrpBox.Width - connectionStatusLbl.Width - 8;
            commuTypeLbl.Left = statusGrpBox.Width - 185;
        }

        void UpdateDownloadStatus(string str)
        {
            if (readReportPercentLbl.InvokeRequired)
            {
                readReportPercentLbl.Invoke((MethodInvoker)delegate
                {
                    readReportPercentLbl.Text = str;
                });
            }
            else
            {
                readReportPercentLbl.Text = str;
            }
        }

        void UpdateProgressBar(int min, int max, int value, bool visible)
        {
            retrieveLogPrgBar.Minimum = min;
            retrieveLogPrgBar.Maximum = max;
            retrieveLogPrgBar.Visible = visible;
            retrieveLogPrgBar.Value = value;
            readReportPercentLbl.Visible = visible;
        }

        void UpdateProgressBar(bool visible)
        {
            if (visible == false)
            {
                retrieveLogPrgBar.Value = 0;
                readReportPercentLbl.Text = "0%";
            }
            retrieveLogPrgBar.Visible = visible;
            readReportPercentLbl.Visible = visible;
            readReportPercentLbl.Visible = visible;
        }

        void LoadBootModeLayout()
        {
            deviceCmdGBox.Enabled = false;
            tabMain.SelectedTab = settingTabPage;
            //sendSMSBtn.Enabled = false;
            readFirmwareVersionBtn.Enabled = false;
            runAppBtn.Enabled = true;
            connectionStatusLbl.Text = "BOOT MODE";
            connectionStatusLbl.ForeColor = Color.Blue;
        }

        void LoadAppModeLayout()
        {
            deviceCmdGBox.Enabled = true;
            //sendSMSBtn.Enabled = true;
            readFirmwareVersionBtn.Enabled = true;
            runAppBtn.Enabled = true;
            UpdateUI();
        }

        public MainForm(bool isInTrial = false)
        {
            InitializeComponent();
            isInTrialPeriod = isInTrial;
        }

        protected override void WndProc(ref Message m)
        {
            if (m.Msg == Win32.WM_DEVICECHANGE)
            {
                if (m.WParam.ToInt32() == Win32.DBT_DEVICEARRIVAL)
                {
                    OpenHIDDevice();
                }
                else if (m.WParam.ToInt32() == Win32.DBT_DEVICEREMOVECOMPLETE)
                {
                    if (HID.DeviceNameMatch(m, hidDevice.pathName))
                    {
                        CloseHIDDevice();
                    }
                }

                UpdateUI();
            }

            base.WndProc(ref m);
        }       

        private bool ParseIP(string ipAddress, ref uint ip)
        {
            try
            {
                IPAddress address = IPAddress.Parse(ipAddress);
                byte[] b = address.GetAddressBytes();

                ip = BitConverter.ToUInt32(b, 0);
            }
            catch
            {
                return false;
            }

            return true;
        }

        #region process received data
        private void ProcessReceivedHIDData()
        {
            byte crc;
            int length;
            byte opcode;
            uint packetNo;
            byte[] bTemp;

            // should have something in the buffer
            length = BitConverter.ToInt16(hidInBuffer, LENGTH_OFFSET);
            if (length <= 0)
            {
                return;
            }
            // validate the CRC
            crc = hidInBuffer[HEADER_SIZE + length];
            if (crc != CalcCRC(hidInBuffer, DATA_OFFSET, length))
            {
                return;
            }

            opcode = hidInBuffer[OPCODE_OFFSET];
            switch (opcode)
            {
                case READ_LOG_OPCODE:             // debug data
                    this.Invoke(new MethodInvoker(UpdateTextBox));
					Logger.log.Info(Encoding.ASCII.GetString(hidInBuffer, DATA_OFFSET, length));
                    break;

                case BOOT_MODE_OPCODE:
                    bootMode = true;
                    this.Invoke((MethodInvoker)delegate
                    {
                        LoadBootModeLayout();
                    });
                    break;

                case WRITE_CONFIG_OPCODE:
                    operationTimer.Stop();
                    packetNo = BitConverter.ToUInt32(hidInBuffer, PACKET_NO_OFFSET);
                    if (packetNo == FINISH_PACKET_NO)
                    {
                        MessageBox.Show("Configuration written successfully!");
                        this.Invoke((MethodInvoker)delegate
                        {
                            EnableDeviceCmd(true);
                        });
                        return;
                    }
                    if (packetNo == ERROR_PACKET_NO)
                    {
                        return;
                    }
                    if (packetNo == DEFAULT_PACKET_NO)
                    {
                        cfgDataSize = BitConverter.ToUInt32(hidInBuffer, DATA_WITH_PACKET_NO_OFFSET);
                        cfgDeviceBuffSize = BitConverter.ToUInt32(hidInBuffer, DATA_WITH_PACKET_NO_OFFSET + 4);
                        cfgDataOffset = 0;
                        length = (int)cfgDeviceBuffSize;
                    }
                    else
                    {
                        cfgNumByteToSend = BitConverter.ToUInt32(hidInBuffer, DATA_WITH_PACKET_NO_OFFSET);
                        cfgDataOffset = (int)packetNo;
                        length = (int)cfgNumByteToSend;
                    }

                    if (cfgDataOffset >= cfgDataSize)         // last packet
                    {
                        MessageBox.Show("Configuration written successfully!");
                        this.Invoke((MethodInvoker)delegate
                        {
                            EnableDeviceCmd(true);
                        });
                        return;
                    }
					if (length > (cfgDataSize - cfgDataOffset))
					{
						length = (int)(cfgDataSize - cfgDataOffset);
					}
                    CreateHIDBuffer(PACKET_NO_SIZE + length, WRITE_CONFIG_OPCODE, (uint)cfgDataOffset);
                    Array.Copy(cfgData, cfgDataOffset, hidOutBuffer, DATA_WITH_PACKET_NO_OFFSET, length);
                    hidOutBuffer[DATA_WITH_PACKET_NO_OFFSET + length] = CalcCRC(hidOutBuffer, DATA_OFFSET, PACKET_NO_SIZE + length);
                    SendHIDData(hidOutBuffer, 0, HEADER_SIZE + PACKET_NO_SIZE + length + CRC_SIZE);
                    break;

                case READ_CONFIG_OPCODE:          // config data
                    operationTimer.Stop();
                    packetNo = BitConverter.ToUInt32(hidInBuffer, PACKET_NO_OFFSET);
                    if (packetNo == FINISH_PACKET_NO)
                    {
                        cfg = (DEVICE_CONFIG_DATA)Lib.ByteArrayToObject(cfgData, typeof(DEVICE_CONFIG_DATA));
                        this.Invoke(new MethodInvoker(UpdateConfigUI));
                    }
                    if (packetNo == ERROR_PACKET_NO)
                    {
                        return;
                    }
                    if (packetNo == DEFAULT_PACKET_NO)
                    {
                        cfgDataSize = BitConverter.ToUInt32(hidInBuffer, DATA_WITH_PACKET_NO_OFFSET);
                        cfgDeviceBuffSize = BitConverter.ToUInt32(hidInBuffer, DATA_WITH_PACKET_NO_OFFSET + 4);
                        cfgDataOffset = 0;
                        length = (int)cfgDeviceBuffSize;
                    }
                    else
                    {
                        Array.Copy(hidInBuffer, DATA_WITH_PACKET_NO_OFFSET, cfgData, packetNo, length - PACKET_NO_SIZE);
                        cfgDataOffset = (int)packetNo + length - PACKET_NO_SIZE;
                        length = (int)cfgDeviceBuffSize;
                        if (cfgDataOffset > cfgDataSize - cfgDeviceBuffSize)
                            length = (int)cfgDataSize - cfgDataOffset;
                    }

                    if (cfgDataOffset >= cfgDataSize)         // last packet
                    {
                        cfg = (DEVICE_CONFIG_DATA)Lib.ByteArrayToObject(cfgData, typeof(DEVICE_CONFIG_DATA));
                        this.Invoke(new MethodInvoker(UpdateConfigUI));
                        // read config successfully
                        if (bootMode)
                        {
                            bootMode = false;
                            this.Invoke((MethodInvoker)delegate
                            {
                                LoadAppModeLayout();
                            });
                        }

                        return;
                    }
					if (length > (cfgDataSize - cfgDataOffset))
					{
						length = (int)(cfgDataSize - cfgDataOffset);
					}
                    CreateHIDBuffer(PACKET_NO_OFFSET + 2, READ_CONFIG_OPCODE, (uint)cfgDataOffset);
                    Array.Copy(BitConverter.GetBytes(length), 0, hidOutBuffer, DATA_WITH_PACKET_NO_OFFSET, 2);
                    hidOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET + 2] = CalcCRC(hidOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE + 2);
                    SendHIDData(hidOutBuffer, 0, SIMPLE_PACKET_SIZE + 2);
                    break;

                case READ_FIRMWARE_VERSION_OPCODE:
                    firmwareVersion = System.Text.Encoding.ASCII.GetString(hidInBuffer, 3, length);
                    this.Invoke((MethodInvoker)delegate
                    {
                        firmwareVersionLbl.Text = firmwareVersion;
                    });
                    break;

                case DOWNLOAD_FIRMWARE_OPCODE:
                    if (!downloadingFirmware)
                    {
                        return;
                    }
                    packetNo = BitConverter.ToUInt32(hidInBuffer, PACKET_NO_OFFSET);
                    if (packetNo == FINISH_PACKET_NO)
                    {
                        // complete
                        downloadingFirmware = false;
                        this.Invoke((MethodInvoker)delegate
                        {
                            EnableDeviceCmd(true);
                            UpdateProgressBar(false);
                        });
                        MessageBox.Show("Firmware downloaded successfully!");
                        try
                        {
                            firmwareReader.Close();
                            return;
                        }
                        catch (Exception)
                        { }
                    }
                    if (packetNo == DEFAULT_PACKET_NO)
                    {
                        fDeviceBuffSize = BitConverter.ToUInt32(hidInBuffer, DATA_WITH_PACKET_NO_OFFSET + 4);
                        packetNo = 0;
                    }
                    this.Invoke((MethodInvoker)delegate
                    {
                        retrieveLogPrgBar.Value = (int)(packetNo / 100);
                        readReportPercentLbl.Text = (retrieveLogPrgBar.Value * 100 / retrieveLogPrgBar.Maximum).ToString() + "%";
                    });
                    try
                    {
                        byte[] temp;

                        firmwareReader.BaseStream.Seek(packetNo, SeekOrigin.Begin);
                        //bTemp = firmwareReader.ReadBytes((int)fDeviceBuffSize);
                        temp = firmwareReader.ReadBytes((int)fDeviceBuffSize);
                        bTemp = Enumerable.Repeat<byte>(0xFF, (int)fDeviceBuffSize).ToArray();
                        Buffer.BlockCopy(temp, 0, bTemp, 0, temp.Length);
                        length = bTemp.Length + 4;
                        CreateHIDBuffer(length, DOWNLOAD_FIRMWARE_OPCODE, packetNo);
                        Array.Copy(bTemp, 0, hidOutBuffer, DATA_WITH_PACKET_NO_OFFSET, bTemp.Length);
                        hidOutBuffer[length + HEADER_SIZE] = CalcCRC(hidOutBuffer, DATA_OFFSET, length);
                        SendHIDData(hidOutBuffer, 0, length + HEADER_SIZE + CRC_SIZE);
                    }
                    catch (Exception)
                    {
                        downloadingFirmware = false;
                        this.Invoke((MethodInvoker)delegate
                        {
                            EnableDeviceCmd(true);
                            UpdateProgressBar(false);
                        });
                        MessageBox.Show("Firmware downloaded fail!");
                        return;
                    }
                    break;
                default:
					break;
            }
        }

		private void ProcessReceivedCOMData()
		{
			byte crc;
			int length;
			byte opcode;
			uint packetNo;
			byte[] bTemp;

			// should have something in the buffer
			length = BitConverter.ToInt16(comInBuffer, LENGTH_OFFSET);
			if (length <= 0)
			{
				return;
			}
			// validate the CRC
			crc = comInBuffer[HEADER_SIZE + length];
			if (crc != CalcCRC(comInBuffer, DATA_OFFSET, length))
			{
				return;
			}

			opcode = comInBuffer[OPCODE_OFFSET];
			switch (opcode)
			{
				case READ_LOG_OPCODE:             // debug data
					//this.Invoke(new MethodInvoker(UpdateTextBox));
					Logger.log.Info(Encoding.ASCII.GetString(comInBuffer, DATA_OFFSET, length));
					break;

				case BOOT_MODE_OPCODE:
					CreateCOMBuffer(PACKET_NO_SIZE, SERIAL_COMM_NTF_OPCODE, DEFAULT_PACKET_NO);
					comOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(comOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
					SendCOMData(comOutBuffer, 0, SIMPLE_PACKET_SIZE);
					bootMode = true;
					this.Invoke((MethodInvoker)delegate
					{
						LoadBootModeLayout();
					});
					break;

				case WRITE_CONFIG_OPCODE:
					operationTimer.Stop();
					packetNo = BitConverter.ToUInt32(comInBuffer, PACKET_NO_OFFSET);
					if (packetNo == FINISH_PACKET_NO)
					{
						MessageBox.Show("Configuration written successfully!");
						this.Invoke((MethodInvoker)delegate
						{
							EnableDeviceCmd(true);
						});
						return;
					}
					if (packetNo == ERROR_PACKET_NO)
					{
						return;
					}
					if (packetNo == DEFAULT_PACKET_NO)
					{
						cfgDataSize = BitConverter.ToUInt32(comInBuffer, DATA_WITH_PACKET_NO_OFFSET);
						cfgDeviceBuffSize = BitConverter.ToUInt32(comInBuffer, DATA_WITH_PACKET_NO_OFFSET + 4);
						cfgDataOffset = 0;
						length = (int)cfgDeviceBuffSize;
					}
					else
					{
						cfgNumByteToSend = BitConverter.ToUInt32(comInBuffer, DATA_WITH_PACKET_NO_OFFSET);
						cfgDataOffset = (int)packetNo;
						length = (int)cfgNumByteToSend;
					}

					if (cfgDataOffset >= cfgDataSize)         // last packet
					{
						MessageBox.Show("Configuration written successfully!");
						this.Invoke((MethodInvoker)delegate
						{
							EnableDeviceCmd(true);
						});
						return;
					}
					if (length > (cfgDataSize - cfgDataOffset))
					{
						length = (int)(cfgDataSize - cfgDataOffset);
					}
					CreateCOMBuffer(PACKET_NO_SIZE + length, WRITE_CONFIG_OPCODE, (uint)cfgDataOffset);
					Array.Copy(cfgData, cfgDataOffset, comOutBuffer, DATA_WITH_PACKET_NO_OFFSET, length);
					comOutBuffer[DATA_WITH_PACKET_NO_OFFSET + length] = CalcCRC(comOutBuffer, DATA_OFFSET, PACKET_NO_SIZE + length);
					SendCOMData(comOutBuffer, 0, HEADER_SIZE + PACKET_NO_SIZE + length + CRC_SIZE);
					break;

				case READ_CONFIG_OPCODE:          // config data
					operationTimer.Stop();
					packetNo = BitConverter.ToUInt32(comInBuffer, PACKET_NO_OFFSET);
					if (packetNo == FINISH_PACKET_NO)
					{
						cfg = (DEVICE_CONFIG_DATA)Lib.ByteArrayToObject(cfgData, typeof(DEVICE_CONFIG_DATA));
						this.Invoke(new MethodInvoker(UpdateConfigUI));
					}
					if (packetNo == ERROR_PACKET_NO)
					{
						return;
					}
					if (packetNo == DEFAULT_PACKET_NO)
					{
						cfgDataSize = BitConverter.ToUInt32(comInBuffer, DATA_WITH_PACKET_NO_OFFSET);
						cfgDeviceBuffSize = BitConverter.ToUInt32(comInBuffer, DATA_WITH_PACKET_NO_OFFSET + 4);
						cfgDataOffset = 0;
						length = (int)cfgDeviceBuffSize;
					}
					else
					{
						Array.Copy(comInBuffer, DATA_WITH_PACKET_NO_OFFSET, cfgData, packetNo, length - PACKET_NO_SIZE);
						cfgDataOffset = (int)packetNo + length - PACKET_NO_SIZE;
						length = (int)cfgDeviceBuffSize;
						if (cfgDataOffset > cfgDataSize - cfgDeviceBuffSize)
							length = (int)cfgDataSize - cfgDataOffset;
					}

					if (cfgDataOffset >= cfgDataSize)         // last packet
					{
						cfg = (DEVICE_CONFIG_DATA)Lib.ByteArrayToObject(cfgData, typeof(DEVICE_CONFIG_DATA));
						this.Invoke(new MethodInvoker(UpdateConfigUI));
						// read config successfully
						if (bootMode)
						{
							bootMode = false;
							this.Invoke((MethodInvoker)delegate
							{
								LoadAppModeLayout();
							});
						}

						return;
					}
					if (length > (cfgDataSize - cfgDataOffset))
					{
						length = (int)(cfgDataSize - cfgDataOffset);
					}
					CreateCOMBuffer(PACKET_NO_OFFSET + 2, READ_CONFIG_OPCODE, (uint)cfgDataOffset);
					Array.Copy(BitConverter.GetBytes(length), 0, comOutBuffer, DATA_WITH_PACKET_NO_OFFSET, 2);
					comOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET + 2] = CalcCRC(comOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE + 2);
					SendCOMData(comOutBuffer, 0, SIMPLE_PACKET_SIZE + 2);
					break;

				case READ_FIRMWARE_VERSION_OPCODE:
					firmwareVersion = System.Text.Encoding.ASCII.GetString(comInBuffer, 3, length+1);
					this.Invoke((MethodInvoker)delegate
					{
						firmwareVersionLbl.Text = firmwareVersion;
					});
					break;

				case DOWNLOAD_FIRMWARE_OPCODE:
					if (!downloadingFirmware)
					{
						return;
					}
					packetNo = BitConverter.ToUInt32(comInBuffer, PACKET_NO_OFFSET);
					if (packetNo == FINISH_PACKET_NO)
					{
						// complete
						downloadingFirmware = false;
						this.Invoke((MethodInvoker)delegate
						{
							EnableDeviceCmd(true);
							UpdateProgressBar(false);
						});
						MessageBox.Show("Firmware downloaded successfully!");
						try
						{
							firmwareReader.Close();
							return;
						}
						catch (Exception)
						{ }
					}
					if (packetNo == DEFAULT_PACKET_NO)
					{
						fDeviceBuffSize = BitConverter.ToUInt32(comInBuffer, DATA_WITH_PACKET_NO_OFFSET + 4);
						packetNo = 0;
					}
					this.Invoke((MethodInvoker)delegate
					{
						retrieveLogPrgBar.Value = (int)(packetNo / 100);
						readReportPercentLbl.Text = (retrieveLogPrgBar.Value * 100 / retrieveLogPrgBar.Maximum).ToString() + "%";
					});
					try
					{
						byte[] temp;

						firmwareReader.BaseStream.Seek(packetNo, SeekOrigin.Begin);
						//bTemp = firmwareReader.ReadBytes((int)fDeviceBuffSize);
						temp = firmwareReader.ReadBytes((int)fDeviceBuffSize);
						bTemp = Enumerable.Repeat<byte>(0xFF, (int)fDeviceBuffSize).ToArray();
						Buffer.BlockCopy(temp, 0, bTemp, 0, temp.Length);
						length = bTemp.Length + 4;
						CreateCOMBuffer(length, DOWNLOAD_FIRMWARE_OPCODE, packetNo);
						Array.Copy(bTemp, 0, comOutBuffer, DATA_WITH_PACKET_NO_OFFSET, bTemp.Length);
						comOutBuffer[length + HEADER_SIZE] = CalcCRC(comOutBuffer, DATA_OFFSET, length);
						SendCOMData(comOutBuffer, 0, length + HEADER_SIZE + CRC_SIZE);
					}
					catch (Exception)
					{
						downloadingFirmware = false;
						this.Invoke((MethodInvoker)delegate
						{
							EnableDeviceCmd(true);
							UpdateProgressBar(false);
						});
						MessageBox.Show("Firmware downloaded fail!");
						return;
					}
					break;
				default:
					break;
			}
		}
        #endregion

        private void SendHIDData(byte[] buffer, int offset, int count, int timeout = DEFAULT_COMM_TIMEOUT)
        {
            int retryCount, copyIndex, byteToSend;

            copyIndex = 0;
            while (copyIndex < count)
            {
                byteToSend = count - copyIndex;
                byteToSend = (byteToSend > 64) ? 64 : byteToSend;
                Array.Clear(hidSendBuffer, 0, hidSendBuffer.Length);
                Array.Copy(buffer, offset + copyIndex, hidSendBuffer, 1, byteToSend);
                copyIndex += byteToSend;
                retryCount = 0;
                operationTimer.Interval = timeout;
                operationTimer.Start();
                while (retryCount < MAX_COMM_RETRY)
                {
                    if (StartBlockingAction(delegate
                    {
                        try
                        {
                            hidDevice.writeStream.Write(hidSendBuffer, 0, hidSendBuffer.Length);
                        }
                        catch (Exception)
                        { }
                    }, timeout))
                    {
                        // the action completed successfully
                        break;
                    }
                    // the action timed-out
                    retryCount++;
                }

                if (retryCount >= MAX_COMM_RETRY)
                {
                    ReportHIDOperationTimedOut();
                    break;
                }
            }
        }

		private void SendCOMData(byte[] buffer, int offset, int count, int timeout = DEFAULT_COMM_TIMEOUT)
		{
			int retryCount;

			try
			{
				serialPort.WriteTimeout = timeout;
			}
			catch { }
			retryCount = 0;
			while (true)
			{
				try
				{
					serialPort.Write(buffer, offset, count);
					break;
				}
				catch (Exception)
				{
					if (retryCount++ > MAX_COMM_RETRY)
					{
						ReportCOMWriteTimedOut();
						return;
					}
				}
			}
		}

        private bool StartBlockingAction(Action action, int timeout)
        {
            bool isCompleted = false;

            // start a thread to perform the action
            Thread t = new Thread(new ThreadStart(delegate
                {
                    try
                    {
                        action();
                        isCompleted = true;
                    }
                    catch (ThreadAbortException)
                    { }
                }));
            t.Start();
            t.Join(timeout);
            if (t.IsAlive)
            {
                // timed-out, abord the thread
                t.Abort();
            }

            return isCompleted;
        }

        private void ReportHIDOperationTimedOut()
        {
            switch (currentOperation)
            { 
                case READ_CONFIG_OPCODE:
                    break;
                case WRITE_CONFIG_OPCODE:
                    MessageBox.Show("Write config failed!");
                    break;
            }
        }

		private void ReportCOMWriteTimedOut()
		{
			if (currentOperation == READ_CONFIG_OPCODE)
			{
				EnableDeviceCmd(true);
				return;
			}
			if (currentOperation == WRITE_CONFIG_OPCODE)
			{
				MessageBox.Show("Write config failed.");
				EnableDeviceCmd(true);
				return;
			}
			if (currentOperation == DOWNLOAD_FIRMWARE_OPCODE)
			{
				if (downloadingFirmware)
				{
					downloadingFirmware = false;
					EnableDeviceCmd(true);
					UpdateProgressBar(false);
					MessageBox.Show("Download firmware failed.");
				}
				return;
			}
		}

        #region Event handler
        private void readSettingsBtn_Clicked(object sender, EventArgs e)
        {
            ReadConfig();
        }

        private void GetConfigFromUI()
        {
            cfg = tempCfg;
        }

        private void writeSettingsBtn_Clicked(object sender, EventArgs e)
        {
            uint crc;

            GetConfigFromUI();

			if (communicationType == USB_COMMUNICATION)
			{
				if (!hidDevice.connected)
				{
					return;
				}

				EnableDeviceCmd(true);
				cfgData = Lib.ObjectToByteArray(cfg);
				crc = CalcCRC32(cfgData, 0, cfgData.Length - 4);
				cfg.crc = crc;
				cfgData = Lib.ObjectToByteArray(cfg);

				CreateHIDBuffer(PACKET_NO_SIZE, WRITE_CONFIG_OPCODE, DEFAULT_PACKET_NO);
				hidOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(hidOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				cfgDataOffset = 0;
				cfgDeviceBuffSize = 0;
				cfgDataSize = 0;
				SendHIDData(hidOutBuffer, 0, SIMPLE_PACKET_SIZE, 2000);
			}
			else
			{
				if (!serialPort.IsOpen)
				{
					return;
				}

				EnableDeviceCmd(false);
				cfgData = Lib.ObjectToByteArray(cfg);
				crc = CalcCRC32(cfgData, 0, cfgData.Length - 4);
				cfg.crc = crc;
				cfgData = Lib.ObjectToByteArray(cfg);

				CreateCOMBuffer(PACKET_NO_SIZE, WRITE_CONFIG_OPCODE, DEFAULT_PACKET_NO);
				comOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(comOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				cfgDataOffset = 0;
				cfgDeviceBuffSize = 0;
				cfgDataSize = 0;
				SendCOMData(comOutBuffer, 0, SIMPLE_PACKET_SIZE, 1000);
			}
        }

        private void EnableDeviceCmd(bool enabled)
        {
            if (this.InvokeRequired)
            {
                this.Invoke((MethodInvoker)delegate
                {
                    deviceCmdGBox.Enabled = enabled;
                    readFirmwareVersionBtn.Enabled = enabled;
                    downloadFirmwareBtn.Enabled = enabled;
                });
            }
            else
            {
                deviceCmdGBox.Enabled = enabled;
                readFirmwareVersionBtn.Enabled = enabled;
                downloadFirmwareBtn.Enabled = enabled;
            }
        }

        private void resetCPUBtn_Clicked(object sender, EventArgs e)
        {
			if (communicationType == USB_COMMUNICATION)
			{
				if (!hidDevice.connected)
				{
					return;
				}

				CreateHIDBuffer(PACKET_NO_SIZE, RESET_DEVICE_OPCODE, DEFAULT_PACKET_NO);
				hidOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(hidOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				SendHIDData(hidOutBuffer, 0, SIMPLE_PACKET_SIZE);
			}
			else
			{
				if (!serialPort.IsOpen)
				{
					return;
				}

				CreateCOMBuffer(PACKET_NO_SIZE, RESET_DEVICE_OPCODE, DEFAULT_PACKET_NO);
				comOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(comOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				SendCOMData(comOutBuffer, 0, SIMPLE_PACKET_SIZE);
			}
        }

        private void factoryResetBtn_Clicked(object sender, EventArgs e)
        {
			if (communicationType == USB_COMMUNICATION)
			{
				if (!hidDevice.connected)
				{
					return;
				}

				CreateHIDBuffer(PACKET_NO_OFFSET, RESET_FACTORY_OPCODE, DEFAULT_PACKET_NO);
				hidOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(hidOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				SendHIDData(hidOutBuffer, 0, SIMPLE_PACKET_SIZE);
			}
			else
			{
				if (!serialPort.IsOpen)
				{
					return;
				}

				CreateCOMBuffer(PACKET_NO_SIZE, RESET_FACTORY_OPCODE, DEFAULT_PACKET_NO);
				comOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(comOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				SendCOMData(comOutBuffer, 0, SIMPLE_PACKET_SIZE);
			}
        }
        #endregion

        private void formatSDCardBtn_Clicked(object sender, EventArgs e)
        {
            if (!hidDevice.connected)
            {
                return;
            }
            // "Format the SD card will take several minutes to complete. Do not disconnect the power supply during the formatting operation!"
            if (MessageBox.Show("Format the SD card will take several minutes to complete. Do not disconnect the power supply during the formatting operation!",
                    "SENIORCARE PARAMETER SETUP", MessageBoxButtons.OKCancel, MessageBoxIcon.Warning) != System.Windows.Forms.DialogResult.OK) return;

            CreateHIDBuffer(PACKET_NO_SIZE, FORMAT_SDCARD_OPCODE, DEFAULT_PACKET_NO);
            hidOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(hidOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
            SendHIDData(hidOutBuffer, 0, SIMPLE_PACKET_SIZE);
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            HID.UnregisterDeviceNotification(notificationHandle);
			Logger.removeTextBoxAppender();
            Environment.Exit(0);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Environment.Exit(0);
        }

        private byte CalcCRC(byte[] buf, int index, int length)
        {
            byte crc;

            crc = 0;
            for (int i = index; i < index + length; i++)
            {
                crc += buf[i];
            }

            return crc;
        }

        private uint CalcCRC32(byte[] buf, int index, int length)
        {
            uint crc;

            crc = 0;
            for (int i = index; i < index + length; i++)
            {
                crc += buf[i];
            }

            return crc;
        }

        private void gprsSettings_Changed(object sender, EventArgs e)
        {
            if (configUpdateDisabled)
            {
                return;
            }

            // gprs config
            tempCfg.gprsApn = gprsApnTxtBox.Text;
            tempCfg.gprsUser = gprsUserTxtBox.Text;
            tempCfg.gprsPass = gprsPwdTxtBox.Text;
        }

        private void serverParameter_Changed(object sender, EventArgs e)
        {


            if (configUpdateDisabled)
            {
                return;
            }
            
            tempCfg.otaMainServerName = otaMainServerDomainTxtBox.Text;
            tempCfg.otaMainServerPort = ushort.Parse(otaMainServerPortTxtBox.Text);
            tempCfg.mqttServerDomain = mqttDomainTxtBox.Text;
            tempCfg.mqttServerPort = ushort.Parse(mqttPortTxtBox.Text);
            tempCfg.bigBoss = bigBossNumberTxt.Text;
            if ((ownerNumberCbBox.SelectedIndex >= 0) && (ownerNumberCbBox.SelectedIndex < BOSS_PHONE_MAX))
            {
                tempCfg.ownerNumber[ownerNumberCbBox.SelectedIndex].phoneNo = ownerNumberTxt.Text;
            }
            tempCfg.smsBurner = smsBurnerNumberTxt.Text;
            tempCfg.reportInterval = ushort.Parse(reportIntervalTxtBox.Text);
        }

        private void serverSelection_Changed(object sender, EventArgs e)
        {
            bool configUpdateAlreadyDisabled;

            configUpdateAlreadyDisabled = configUpdateDisabled;
            configUpdateDisabled = true;

            otaMainServerDomainTxtBox.Text = tempCfg.otaMainServerName;
            otaMainServerPortTxtBox.Text = tempCfg.otaMainServerPort.ToString();

            mqttDomainTxtBox.Text = tempCfg.mqttServerDomain;
            mqttPortTxtBox.Text = tempCfg.mqttServerPort.ToString();
            bigBossNumberTxt.Text = tempCfg.bigBoss.ToString();
            if ((ownerNumberCbBox.SelectedIndex >= 0) && (ownerNumberCbBox.SelectedIndex < BOSS_PHONE_MAX))
            {
                ownerNumberTxt.Text = tempCfg.ownerNumber[ownerNumberCbBox.SelectedIndex].phoneNo;
            }
            smsBurnerNumberTxt.Text = tempCfg.smsBurner.ToString();
            reportIntervalTxtBox.Text = tempCfg.reportInterval.ToString();

            if (!configUpdateAlreadyDisabled)
            {
                configUpdateDisabled = false;
            }
        }

        private void selectLogTypeCmbBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            int logOpcode;

            switch (selectLogTypeCmbBox.SelectedIndex)
            { 
                case 0:
                    logOpcode = DISABLE_LOG_OPCODE;
                    break;
                case 1:
                    logOpcode = ENABLE_LOG_OPCODE;
                    break;
                default:
                    logOpcode = DISABLE_LOG_OPCODE;
                    break;
            }

			if (communicationType == USB_COMMUNICATION)
			{
				if (!hidDevice.connected)
				{
					return;
				}

				CreateHIDBuffer(PACKET_NO_SIZE, (byte)logOpcode, DEFAULT_PACKET_NO);
				hidOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(hidOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				SendHIDData(hidOutBuffer, 0, SIMPLE_PACKET_SIZE);
			}
			else 
			{
				if (!serialPort.IsOpen)
				{
					return;
				}

				CreateCOMBuffer(PACKET_NO_SIZE, (byte)logOpcode, DEFAULT_PACKET_NO);
				comOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(comOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				SendCOMData(comOutBuffer, 0, SIMPLE_PACKET_SIZE);
			}
        }

        private void saveSettingsBtn_Click(object sender, EventArgs e)
        {
            string fileName;
            DialogResult res;
            BinaryWriter writer;

            saveFileDialog1.Filter = "CFG file (*.CFG)|*.CFG";
            res = saveFileDialog1.ShowDialog();
            if (res != DialogResult.OK)
            {
                return;
            }
            fileName = saveFileDialog1.FileName;
            writer = new BinaryWriter(new FileStream(fileName, FileMode.Create));
            cfg = tempCfg;
            writer.Write(Lib.ObjectToByteArray(cfg));
            writer.Close();
            MessageBox.Show("Configuration saved successfully!");
        }

        private void loadSettingsBtn_Click(object sender, EventArgs e)
        {
            string fileName;
            DialogResult res;
            BinaryReader reader;
            byte[] b;

            openFileDialog1.Filter = "CFG file (*.CFG)|*.CFG";
            res = openFileDialog1.ShowDialog();
            if (res != DialogResult.OK)
            {
                return;
            }
            fileName = openFileDialog1.FileName;
            if (!File.Exists(fileName))
            {
                return;
            }
            try
            {
                reader = new BinaryReader(new FileStream(fileName, FileMode.Open));
                b = reader.ReadBytes(GPS_CONFIG_DATA_SIZE);
                cfg = (DEVICE_CONFIG_DATA)Lib.ByteArrayToObject(b, typeof(DEVICE_CONFIG_DATA));
                reader.Close();
                UpdateConfigUI();
                MessageBox.Show("Configuration loaded successfully!");
            }
            catch (Exception)
            { }
        }

        private void ApplyControlResources(Control control, ComponentResourceManager resources, CultureInfo culture)
        {
            resources.ApplyResources(control, control.Name, culture);
            foreach (Control c in control.Controls)
            {
                ApplyControlResources(c, resources, culture);
            }
        }

        private void ApplyMenuItemResources(ToolStripMenuItem item, ComponentResourceManager resources, CultureInfo culture)
        {
            resources.ApplyResources(item, item.Name, culture);
            foreach (ToolStripMenuItem i in item.DropDownItems)
            {
                ApplyMenuItemResources(i, resources, culture);
            }
        }

        private void ApplyLayoutChanges()
        {
            tabMain.Height = statusGrpBox.Top - 106;
            tabMain.Width = statusGrpBox.Width - 10;
        }

        private void readFirmwareVersionBtn_Click(object sender, EventArgs e)
        {
			if (communicationType == USB_COMMUNICATION)
			{
				if (!hidDevice.connected)
				{
					return;
				}

				CreateHIDBuffer(PACKET_NO_SIZE, READ_FIRMWARE_VERSION_OPCODE, DEFAULT_PACKET_NO);
				hidOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(hidOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				SendHIDData(hidOutBuffer, 0, SIMPLE_PACKET_SIZE);
			}
			else
			{
				if (!serialPort.IsOpen)
				{
					return;
				}

				CreateCOMBuffer(PACKET_NO_SIZE, READ_FIRMWARE_VERSION_OPCODE, DEFAULT_PACKET_NO);
				comOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(comOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				SendCOMData(comOutBuffer, 0, SIMPLE_PACKET_SIZE);
			}
        }

        private void browseFirmwareBtn_Click(object sender, EventArgs e)
        {
            string fileName;
            DialogResult res;

            openFileDialog1.Filter = "BIN file (*.BIN)|*.BIN";
            res = openFileDialog1.ShowDialog();
            if (res != DialogResult.OK)
            {
                return;
            }
            fileName = openFileDialog1.FileName;
            if (!File.Exists(fileName))
            {
                return;
            }
            firmwarePathTxtBox.Text = fileName;
        }

        private void downloadFirmwareBtn_Click(object sender, EventArgs e)
        {
            string fileName;
            byte[] fileSize;
            byte[] fileCRC;

            fileName = firmwarePathTxtBox.Text;
            if (!File.Exists(fileName))
            {
                MessageBox.Show("Invalid file name!");
                return;
            }

			if (communicationType == USB_COMMUNICATION)
			{
				if (!hidDevice.connected)
				{
					return;
				}

				if (firmwareReader != null)
				{
					try
					{
						firmwareReader.Close();
					}
					catch (Exception)
					{ }
				}

				fileCRC = BitConverter.GetBytes(CalcFileCRC(firmwarePathTxtBox.Text));
				try
				{
					firmwareReader = new BinaryReader(new FileStream(firmwarePathTxtBox.Text, FileMode.Open));
				}
				catch (Exception)
				{
					MessageBox.Show("Cannot open \"" + fileName + "\"!");
					return;
				}

				downloadingFirmware = true;
				EnableDeviceCmd(false);
				fileSize = BitConverter.GetBytes(firmwareReader.BaseStream.Length);
				CreateHIDBuffer(PACKET_NO_SIZE + 8, DOWNLOAD_FIRMWARE_OPCODE, DEFAULT_PACKET_NO);
				Array.Copy(fileSize, 0, hidOutBuffer, DATA_WITH_PACKET_NO_OFFSET, 4);
				Array.Copy(fileCRC, 0, hidOutBuffer, DATA_WITH_PACKET_NO_OFFSET + 4, 4);
				hidOutBuffer[DATA_WITH_PACKET_NO_OFFSET + 8] = CalcCRC(hidOutBuffer, DATA_OFFSET, PACKET_NO_SIZE + 8);
				SendHIDData(hidOutBuffer, 0, HEADER_SIZE + PACKET_NO_SIZE + 8 + CRC_SIZE);
				retrieveLogPrgBar.Maximum = (int)(firmwareReader.BaseStream.Length / 100);
				readReportPercentLbl.Text = "Firmware downloading...";
				UpdateProgressBar(true);
			}
			else
			{
				if (!serialPort.IsOpen)
				{
					return;
				}

				if (firmwareReader != null)
				{
					try
					{
						firmwareReader.Close();
					}
					catch (Exception)
					{ }
				}

				fileCRC = BitConverter.GetBytes(CalcFileCRC(firmwarePathTxtBox.Text));
				try
				{
					firmwareReader = new BinaryReader(new FileStream(firmwarePathTxtBox.Text, FileMode.Open));
				}
				catch (Exception)
				{
					MessageBox.Show("Cannot open \"" + fileName + "\"!");
					return;
				}

				downloadingFirmware = true;
				EnableDeviceCmd(false);
				fileSize = BitConverter.GetBytes(firmwareReader.BaseStream.Length);
				CreateCOMBuffer(PACKET_NO_SIZE + 8, DOWNLOAD_FIRMWARE_OPCODE, DEFAULT_PACKET_NO);
				Array.Copy(fileSize, 0, comOutBuffer, DATA_WITH_PACKET_NO_OFFSET, 4);
				Array.Copy(fileCRC, 0, comOutBuffer, DATA_WITH_PACKET_NO_OFFSET + 4, 4);
				comOutBuffer[DATA_WITH_PACKET_NO_OFFSET + 8] = CalcCRC(comOutBuffer, DATA_OFFSET, PACKET_NO_SIZE + 8);
				SendCOMData(comOutBuffer, 0, HEADER_SIZE + PACKET_NO_SIZE + 8 + CRC_SIZE);
				retrieveLogPrgBar.Maximum = (int)(firmwareReader.BaseStream.Length / 100);
				readReportPercentLbl.Text = "Firmware downloading...";
				UpdateProgressBar(true);
			}
        }

        private void clearLogBtn_Click(object sender, EventArgs e)
        {
            lock (logTxtBox)
            {
                logTxtBox.Clear();
            }
        }

        private void jumpToAppBtn_Click(object sender, EventArgs e)
        {
			if (communicationType == USB_COMMUNICATION)
			{
				if (!hidDevice.connected)
				{
					return;
				}

				CreateHIDBuffer(PACKET_NO_SIZE, JUMP_TO_APP_OPCODE, DEFAULT_PACKET_NO);
				hidOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(hidOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				SendHIDData(hidOutBuffer, 0, SIMPLE_PACKET_SIZE);
				Thread.Sleep(1000);
				ReadConfig();
			}
			else
			{
				if (!serialPort.IsOpen)
				{
					return;
				}

				CreateCOMBuffer(PACKET_NO_SIZE, JUMP_TO_APP_OPCODE, DEFAULT_PACKET_NO);
				comOutBuffer[SIMPLE_CRC_WITH_PACKET_NO_OFFSET] = CalcCRC(comOutBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
				SendCOMData(comOutBuffer, 0, SIMPLE_PACKET_SIZE);
				Thread.Sleep(1000);
				ReadConfig();
			}
        }

        private uint CalcFileCRC(string fileName)
        {
            List<byte> temp;
            uint crc;

            temp = ReadAllByteInFile(fileName);
            crc = CalcCRC32(temp.ToArray(), 0, temp.Count);
            temp.Clear();
            temp = null;

            return crc;
        }

        private List<byte> ReadAllByteInFile(string fileName)
        {
            BinaryReader reader;
            long fileSize;
            List<byte> buffer;
            byte[] tempBuf;
            int bufIndex;

            reader = null;
            try
            {
                reader = new BinaryReader(new FileStream(fileName, FileMode.Open));
            }
            catch (Exception)
            {
                return null;
            }
            if (reader == null)
            {
                return null;
            }

            fileSize = reader.BaseStream.Length;
            buffer = new List<byte>();
            bufIndex = 0;
            while (bufIndex < fileSize)
            {
                tempBuf = reader.ReadBytes(1024);
                buffer.AddRange(tempBuf);
                bufIndex += tempBuf.Length;
            }
            reader.Close();
            reader = null;

            return buffer;
        }

        private void registerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RegisterForm rForm;
            
            rForm = new RegisterForm();
            rForm.ShowDialog();
            registerToolStripMenuItem.Visible = !rForm.isRegistered;
            rForm.Dispose();
            rForm = null;
        }

        private void otherSettings_Changed(object sender, EventArgs e)
        {
            if (configUpdateDisabled)
            {
                return;
            }
            tempCfg.mqttUsername = mqttUserTxtBox.Text;
            tempCfg.mqttPassword = mqttUserPassTxtBox.Text;
            tempCfg.id = idTxt.Text;
            tempCfg.thingName = thingNameTxtBox.Text;
            tempCfg.thingOrgranization = thingOrgTxtBox.Text;
            tempCfg.serverCA = serverCATextBox.Text;
        }

        public void operationTimer_Elapsed(object sender, EventArgs e)
        { 
            // operation timed-out
            switch (currentOperation)
            {
                case READ_CONFIG_OPCODE:
                    break;
                case WRITE_CONFIG_OPCODE:
                    break;
            }
        }

        private void ownerNumber_SelectionChanged(object sender, EventArgs e)
        {
            bool configUpdateAlreadyDisabled;

            configUpdateAlreadyDisabled = configUpdateDisabled;
            configUpdateDisabled = true;


            if ((ownerNumberCbBox.SelectedIndex >= 0) && (ownerNumberCbBox.SelectedIndex < BOSS_PHONE_MAX))
            {
                ownerNumberTxt.Text = tempCfg.ownerNumber[ownerNumberCbBox.SelectedIndex].phoneNo;
            }

            if (!configUpdateAlreadyDisabled)
            {
                configUpdateDisabled = false;
            }
        }

        private void prevownerNumberBtn_Click(object sender, EventArgs e)
        {
            if (ownerNumberCbBox.SelectedIndex > 0)
            {
                ownerNumberCbBox.SelectedIndex--;
            }
        }

        private void nextownerNumberBtn_Click(object sender, EventArgs e)
        {
            if (ownerNumberCbBox.SelectedIndex <( BOSS_PHONE_MAX - 1))
            {
                ownerNumberCbBox.SelectedIndex++;
            }
        }

		private void commTypeCmbBox_SelectedIndexChanged(object sender, EventArgs e)
		{
			communicationType = commTypeCmbBox.SelectedIndex;
			if (communicationType == USB_COMMUNICATION)
			{
				comPortCmbBox.Enabled = false;
				baudrateCbBox.Enabled = false;
				parityCbBox.Enabled = false;
				dataBitsCbBox.Enabled = false;
				stopBitsCbBox.Enabled = false;
				openPortBtn.Enabled = false;
				if (hidDevice.connected)
				{
					return;
				}

				commuTypeLbl.Text = "USB";
				communicationType = USB_COMMUNICATION;
				HID.RegisterDeviceNotification(this.Handle, ref notificationHandle);
				OpenHIDDevice();

				// close COM port
				if ((serialPort != null) && (serialPort.IsOpen))
				{
					serialPort.Close();
					serialPort = null;
				}
			}
			else
			{
				comPortCmbBox.Items.Clear();
				comPortCmbBox.Enabled = true;
				baudrateCbBox.Enabled = true;
				parityCbBox.Enabled = true;
				dataBitsCbBox.Enabled = true;
				stopBitsCbBox.Enabled = true;
				openPortBtn.Enabled = true;
				foreach (string portName in SerialPort.GetPortNames())
				{
					comPortCmbBox.Items.Add(portName);
				}
				if (comPortCmbBox.Items.Count > 0)
				{
					comPortCmbBox.SelectedIndex = 0;
				}
				if (baudrateCbBox.Text.Equals(string.Empty))
				{
					baudrateCbBox.SelectedIndex = 7; // default: 115200
				}
				if (parityCbBox.Text.Equals(string.Empty))
				{
					parityCbBox.SelectedIndex = 0; // default: none
				}
				if (dataBitsCbBox.Text.Equals(string.Empty))
				{
					dataBitsCbBox.SelectedIndex = 3; // default: 8 bits
				}
				if (stopBitsCbBox.Text.Equals(string.Empty))
				{
					stopBitsCbBox.SelectedIndex = 0; // default: 1 stop bits
				}

				// close HID device
				if ((hidDevice != null) && (hidDevice.connected))
				{
					CloseHIDDevice();
					HID.UnregisterDeviceNotification(notificationHandle);
				}
			}

			UpdateUI();
		}

		private void openPortBtn_Click(object sender, EventArgs e)
		{
			int baudrate;
			int dataBits;

			if (comPortCmbBox.Text.Equals(string.Empty))
			{
				return;
			}
			if ((serialPort != null) && (serialPort.IsOpen))
			{
				serialPort.Close();
			}

			// open COM Port
			serialPort.PortName = comPortCmbBox.Text;
			baudrate = 0;
			int.TryParse(baudrateCbBox.Text, out baudrate);
			if (baudrate != 0)
			{
				serialPort.BaudRate = baudrate;
			}
			else
			{
				serialPort.BaudRate = 115200;
			}
			switch (parityCbBox.SelectedIndex)
			{ 
				case 0:
					serialPort.Parity = Parity.None;
					break;
				case 1:
					serialPort.Parity = Parity.Odd;
					break;
				case 2:
					serialPort.Parity = Parity.Even;
					break;
				case 3:
					serialPort.Parity = Parity.Mark;
					break;
				case 4:
					serialPort.Parity = Parity.Space;
					break;
				default:
					serialPort.Parity = Parity.None;
					break;
			}
			dataBits = 0;
			int.TryParse(dataBitsCbBox.Text, out dataBits);
			if (dataBits != 0)
			{
				serialPort.DataBits = dataBits;
			}
			else
			{
				serialPort.DataBits = 8;
			}
			switch (stopBitsCbBox.SelectedIndex)
			{ 
				case 0:
					serialPort.StopBits = StopBits.One;
					break;
				case 1:
					serialPort.StopBits = StopBits.OnePointFive;
					break;
				case 2:
					serialPort.StopBits = StopBits.Two;
					break;
				default:
					serialPort.StopBits = StopBits.One;
					break;
			}
			//serialPort.WriteTimeout = 1000;
			try
			{
				serialPort.Open();
			}
			catch (Exception)
			{
				// "Cannot open port "
				MessageBox.Show("Cannot open port " + serialPort.PortName);
				return;
			}
			commuTypeLbl.Text = serialPort.PortName;
			communicationType = COM_COMMUNICATION;

			// stop USB communication
			CloseHIDDevice();
			HID.UnregisterDeviceNotification(notificationHandle);

			// start COM communication
			comBufIndex = 0;
			ReadConfig();
		}
    }
}
