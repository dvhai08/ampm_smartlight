using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;
using System.Collections;
using DevComponents.DotNetBar.Controls;
using System.Text.RegularExpressions;

namespace PJ1000_Update_Server
{
    public partial class MainForm : Form
    {
        const int ACCEPT_COLUMN = 0;
        const int IMEI_COLUMN = 1;
        const int DEVICE_NAME_COLUMN = 2;
        const int IP_COLUMN = 3;
        const int PORT_COLUMN = 4;
        const int FIRMWARE_VERSION_COLUMN = 5;
        const int NEW_FIRMWARE_COLUMN = 6;
        const int ELAPSED_TIME_COLUMN = 7;
        const int PROGRESS_COLUMN = 8;

        bool isInTrialPeriod = false;
        TcpListener tcpServer;
        Hashtable clientMonitors;
        Thread listeningThread;
        Thread monitoringThread;
        Regex imeiFilter, deviceNameFilter, ipFilter, firmwareVersionFilter;
        int lastFoundRow = -1;

        public MainForm(bool isInTrial = false)
        {
            InitializeComponent();
            isInTrialPeriod = isInTrial;
        }

        private void startServerBtn_Click(object sender, EventArgs e)
        {
            int port;

            if (tcpServer != null)
            {
                try
                {
                    if (listeningThread.IsAlive)
                    {
                        listeningThread.Abort();
                    }
                    tcpServer.Stop();
                }
                catch (Exception)
                { }
                tcpServer = null;
            }

            if (int.TryParse(portTxtBox.Text, out port) == false)
            {
                MessageBox.Show("Invalid port!");
                return;
            }

            try
            {
                tcpServer = new TcpListener(IPAddress.Any, port);
                tcpServer.Start();
                listeningThread = new Thread(ServerListen);
                listeningThread.IsBackground = true;
                listeningThread.Start();
                monitoringThread = new Thread(MonitorServer);
                //monitoringThread.Start();
                if (clientMonitors == null)
                {
                    clientMonitors = new Hashtable();
                }
                startServerBtn.Enabled = false;
                stopServerBtn.Enabled = true;
                AppendLog("Server started at port " + port + ".\n");
            }
            catch (Exception)
            {
                MessageBox.Show("Cannot open port " + port + "!");
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            registerBtn.Visible = isInTrialPeriod;

            //int row;
            //// test
            //clientGridView.Rows.Add();
            //row = clientGridView.Rows.Count - 1;
            //clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = "imei1";
            //clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = "name1";
            //clientGridView.Rows[row].Cells[IP_COLUMN].Value = "1.1.1.1";
            //clientGridView.Rows[row].Cells[PORT_COLUMN].Value = "2000";
            //clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = "9.28";

            //clientGridView.Rows.Add();
            //row = clientGridView.Rows.Count - 1;
            //clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = "imei2";
            //clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = "name2";
            //clientGridView.Rows[row].Cells[IP_COLUMN].Value = "1.1.1.2";
            //clientGridView.Rows[row].Cells[PORT_COLUMN].Value = "2000";
            //clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = "9.22";

            //clientGridView.Rows.Add();
            //row = clientGridView.Rows.Count - 1;
            //clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = "imei3";
            //clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = "name3";
            //clientGridView.Rows[row].Cells[IP_COLUMN].Value = "1.1.1.4";
            //clientGridView.Rows[row].Cells[PORT_COLUMN].Value = "2000";
            //clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = "9.23";

            //clientGridView.Rows.Add();
            //row = clientGridView.Rows.Count - 1;
            //clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = "imei4";
            //clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = "name4";
            //clientGridView.Rows[row].Cells[IP_COLUMN].Value = "1.1.1.4";
            //clientGridView.Rows[row].Cells[PORT_COLUMN].Value = "2000";
            //clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = "9.28";

            //clientGridView.Rows.Add();
            //row = clientGridView.Rows.Count - 1;
            //clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = "imei5";
            //clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = "name5";
            //clientGridView.Rows[row].Cells[IP_COLUMN].Value = "1.1.1.1";
            //clientGridView.Rows[row].Cells[PORT_COLUMN].Value = "2000";
            //clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = "9.25";

            //clientGridView.Rows.Add();
            //row = clientGridView.Rows.Count - 1;
            //clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = "imei6";
            //clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = "name6";
            //clientGridView.Rows[row].Cells[IP_COLUMN].Value = "1.1.1.1";
            //clientGridView.Rows[row].Cells[PORT_COLUMN].Value = "2000";
            //clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = "9.28";

            //clientGridView.Rows.Add();
            //row = clientGridView.Rows.Count - 1;
            //clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = "imei7";
            //clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = "name7";
            //clientGridView.Rows[row].Cells[IP_COLUMN].Value = "1.1.1.7";
            //clientGridView.Rows[row].Cells[PORT_COLUMN].Value = "2000";
            //clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = "9.28";

            //clientGridView.Rows.Add();
            //row = clientGridView.Rows.Count - 1;
            //clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = "imei8";
            //clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = "name8";
            //clientGridView.Rows[row].Cells[IP_COLUMN].Value = "1.1.1.8";
            //clientGridView.Rows[row].Cells[PORT_COLUMN].Value = "2000";
            //clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = "9.28";

            //clientGridView.Rows.Add();
            //row = clientGridView.Rows.Count - 1;
            //clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = "imei9";
            //clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = "name1";
            //clientGridView.Rows[row].Cells[IP_COLUMN].Value = "1.1.1.9";
            //clientGridView.Rows[row].Cells[PORT_COLUMN].Value = "2000";
            //clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = "9.27";

            //clientGridView.Rows.Add();
            //row = clientGridView.Rows.Count - 1;
            //clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = "imei10";
            //clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = "name10";
            //clientGridView.Rows[row].Cells[IP_COLUMN].Value = "1.1.1.10";
            //clientGridView.Rows[row].Cells[PORT_COLUMN].Value = "2000";
            //clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = "9.28";
        }

        private void ServerListen()
        {
            TcpClient client;
            Thread updateInfoThread;

            while (true)
            {
                try
                {
                    client = tcpServer.AcceptTcpClient();
                    AppendLog("Accepted connection form " + GetClientIP(client).ToString() + ".\n");
                    //updater = new ClientUpdater(client, this);
                    //updater.Start();
                    //if (!updater.RetrieveInfo())
                    //{
                    //    client.Close();
                    //    updater = null;
                    //    continue;
                    //}
                    //lock (clientMonitors)
                    //{
                    //    if (clientMonitors.Contains(updater.imei))
                    //    {
                    //        ((ClientUpdater)clientMonitors[updater.imei]).Stop();
                    //        updater.UpdateData((ClientUpdater)clientMonitors[updater.imei]);
                    //        clientMonitors.Remove(updater.imei);
                    //        clientMonitors.Add(updater.imei, updater);
                    //        UpdateConnectionTable(updater);
                    //    }
                    //    else
                    //    {
                    //        clientMonitors.Add(updater.imei, updater);
                    //        this.Invoke((MethodInvoker)delegate
                    //        {
                    //            AddConnectionToTable(updater);
                    //        });
                    //    }
                    //}
                    updateInfoThread = new Thread(UpdateClientInfo);
                    updateInfoThread.Start(client);
                    updateInfoThread.IsBackground = true;
                }
                catch (Exception)
                { }
            }
        }

        private void UpdateClientInfo(object clientObj)
        {
            TcpClient client;
            ClientUpdater updater;

            client = (TcpClient)clientObj;
            updater = new ClientUpdater(client, this);
            updater.Start();
            for (int retry = 0; retry < 10; retry++)
            {
                if (updater.infoAvailable)
                {
                    if (imeiFilter != null)
                    {
                        if (!imeiFilter.Match(updater.imei).Success)
                        {
                            updater.Stop();
                            AppendLog("Connection filtered.\n");
                            return;
                        }
                    }
                    if (deviceNameFilter != null)
                    {
                        if (!deviceNameFilter.Match(updater.name).Success)
                        {
                            updater.Stop();
                            AppendLog("Connection filtered.\n");
                            return;
                        }
                    }
                    if (ipFilter != null)
                    {
                        if (!ipFilter.Match(GetClientIP(updater.client).ToString()).Success)
                        {
                            updater.Stop();
                            AppendLog("Connection filtered.\n");
                            return;
                        }
                    }
                    if (firmwareVersionFilter != null)
                    {
                        if (!firmwareVersionFilter.Match(updater.version).Success)
                        {
                            updater.Stop();
                            AppendLog("Connection filtered.\n");
                            return;
                        }
                    }
                    lock (clientMonitors)
                    {
                        if (clientMonitors.Contains(updater.imei))
                        {
                            updater.UpdateData((ClientUpdater)clientMonitors[updater.imei]);
                            ((ClientUpdater)clientMonitors[updater.imei]).Stop();                            
                            clientMonitors.Remove(updater.imei);
                            clientMonitors.Add(updater.imei, updater);
                            UpdateConnectionTable(updater);
                            AppendLog("Update existing connection.\n");
                        }
                        else
                        {
                            clientMonitors.Add(updater.imei, updater);
                            this.Invoke((MethodInvoker)delegate
                            {
                                AddConnectionToTable(updater);
                            });
                            AppendLog("Add new connection.\n");
                        }
                    }
                    return;
                }
                Thread.Sleep(1000);
            }
            AppendLog("Cannot retrieve device infomation!\n");
            updater.Stop();
        }

        private void MonitorServer()
        { 
            
        }

        private void AddConnectionToTable(ClientUpdater updater)
        {
            int row;

            clientGridView.Rows.Add();
            row = clientGridView.Rows.Count - 1;
            clientGridView.Rows[row].Cells[IMEI_COLUMN].Value = updater.imei;
            clientGridView.Rows[row].Cells[DEVICE_NAME_COLUMN].Value = updater.name;
            clientGridView.Rows[row].Cells[IP_COLUMN].Value = GetClientIP(updater.client).ToString();
            clientGridView.Rows[row].Cells[PORT_COLUMN].Value = GetClientPort(updater.client).ToString();
            clientGridView.Rows[row].Cells[FIRMWARE_VERSION_COLUMN].Value = updater.version;
        }

        private void UpdateConnectionTable(ClientUpdater updater)
        {
            foreach (DataGridViewRow row in clientGridView.Rows)
            {
                if (row.Cells[IMEI_COLUMN].Value.ToString().Equals(updater.imei))
                {
                    row.Cells[IP_COLUMN].Value = GetClientIP(updater.client).ToString();
                    row.Cells[PORT_COLUMN].Value = GetClientPort(updater.client).ToString();
                    row.Cells[FIRMWARE_VERSION_COLUMN].Value = updater.version;
                    break;
                }
            }
        }

        public void UpdateDownloadProgress(string imei, int value, TimeSpan elapsedTime)
        {
            DataGridViewProgressBarXCell progCell;

            foreach (DataGridViewRow row in clientGridView.Rows)
            {
                if (row.Cells[IMEI_COLUMN].Value.ToString().Equals(imei))
                {
                    progCell = (DataGridViewProgressBarXCell)row.Cells[PROGRESS_COLUMN];
                    progCell.Value = value;
                    row.Cells[ELAPSED_TIME_COLUMN].Value = string.Format("{0:D2}:{1:D2}", elapsedTime.Minutes, elapsedTime.Seconds);
                    break;
                }
            }
        }

        private IPAddress GetClientIP(TcpClient client)
        {
            return (((IPEndPoint)((TcpClient)client).Client.RemoteEndPoint).Address);
        }

        private int GetClientPort(TcpClient client)
        {
            return (((IPEndPoint)((TcpClient)client).Client.RemoteEndPoint).Port);
        }

        private delegate void AppendLogDelegate(string msg);
        private void AppendLog(string msg)
        {
            if (logTxtBox.InvokeRequired)
            {
                logTxtBox.Invoke(new AppendLogDelegate(AppendLog), msg);
                return;
            }

            logTxtBox.AppendText(msg);
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Environment.Exit(0);
        }

        private void browseBinBtn_Click(object sender, EventArgs e)
        {
            string binPath;
            DialogResult res;

            res = openBinDialog.ShowDialog();
            if (res != DialogResult.OK)
            {
                return;
            }

            binPath = openBinDialog.FileName;
            if (!File.Exists(binPath))
            {
                return;
            }
            binPathTxtBox.Text = binPath;

            foreach (DataGridViewRow row in clientGridView.Rows)
            {
                if ((row.Cells[ACCEPT_COLUMN].Value != null) && ((bool)row.Cells[ACCEPT_COLUMN].Value))
                {
                    row.Cells[NEW_FIRMWARE_COLUMN].Tag = binPath;
                    row.Cells[NEW_FIRMWARE_COLUMN].Value = openBinDialog.SafeFileName;
                }
            }
        }

        private void stopServerBtn_Click(object sender, EventArgs e)
        {
            ClientUpdater updater;

            if ((clientMonitors != null) && (clientMonitors.Count > 0))
            {
                foreach (string imei in clientMonitors.Keys)
                {
                    updater = (ClientUpdater)clientMonitors[imei];
                    updater.Stop();
                }
            }
            try
            {
                listeningThread.Abort();
                tcpServer.Stop();
                tcpServer.Server.Close();
                tcpServer = null;
            }
            catch (Exception)
            { }

            startServerBtn.Enabled = true;
            stopServerBtn.Enabled = false;
        }

        private void selectAllBtn_Click(object sender, EventArgs e)
        {
            foreach (DataGridViewRow row in clientGridView.Rows)
            {
                row.Cells[ACCEPT_COLUMN].Value = true;
            }
        }

        private void selectNoneBtn_Click(object sender, EventArgs e)
        {
            foreach (DataGridViewRow row in clientGridView.Rows)
            {
                row.Cells[ACCEPT_COLUMN].Value = false;
            }
        }

        private void downloadFirmwareBtn_Click(object sender, EventArgs e)
        {
            string imei;

            foreach (DataGridViewRow row in clientGridView.Rows)
            {
                if ((row.Cells[ACCEPT_COLUMN].Value != null) && ((bool)row.Cells[ACCEPT_COLUMN].Value))
                {
                    imei = row.Cells[IMEI_COLUMN].Value.ToString();
                    if (row.Cells[NEW_FIRMWARE_COLUMN].Tag == null)
                    {
                        MessageBox.Show("Firmware path missing!");
                        return;
                    }
                    ((ClientUpdater)clientMonitors[imei]).DownloadFirmware(row.Cells[NEW_FIRMWARE_COLUMN].Tag.ToString());
                }
            }
        }

        private void setFilterBtn_Click(object sender, EventArgs e)
        {
            DialogResult res;

            res = MessageBox.Show("Changing filter settings will cause the server to be restarted and all existing connections will be aborted. Do you want to continue?", "Confirm", MessageBoxButtons.YesNo);
            if (res != DialogResult.Yes)
            {
                return;
            }
            stopServerBtn_Click(null, null);
            Thread.Sleep(500);
            startServerBtn_Click(null, null);
            clientMonitors.Clear();
            clientGridView.Rows.Clear();
            imeiFilter = CreateRegEx(imeiFilterTxt.Text);
            deviceNameFilter = CreateRegEx(deviceNameFilterTxt.Text);
            ipFilter = CreateRegEx(ipFilterTxt.Text);
            firmwareVersionFilter = CreateRegEx(firmwareVersionFilterTxt.Text);
        }

        private Regex CreateRegEx(string inputStr)
        {
            string res;

            if ((inputStr == null) || (inputStr.Length == 0))
            {
                return null;
            }
            
            res = inputStr;
            if (!res.StartsWith("*"))
            {
                res = "^" + res;
            }
            if (!res.EndsWith("*"))
            {
                res += "$";
            }
            res = res.Replace("*", ".*");
            res = res.Replace("?", ".");

            return (new Regex(res));
        }

        private void registerBtn_Click(object sender, EventArgs e)
        {
            /*RegisterForm rForm;

            rForm = new RegisterForm();
            rForm.ShowDialog();
            registerBtn.Visible = !rForm.isRegistered;
            rForm.Dispose();
            rForm = null;*/
        }

        private void clientGridView_RowsAdded(object sender, DataGridViewRowsAddedEventArgs e)
        {
            nConnectedDevicesLbl.Text = "Connected Devices: " + clientGridView.Rows.Count.ToString();
        }

        private void clientGridView_RowsRemoved(object sender, DataGridViewRowsRemovedEventArgs e)
        {
            nConnectedDevicesLbl.Text = "Connected Devices: " + clientGridView.Rows.Count.ToString();
        }

        private void searchBtn_Click(object sender, EventArgs e)
        {
            string imei;
            string deviceName;
            string ip;
            string firmwareVersion;
            bool found = false;
            bool allMatch;
            DataGridViewRow row;
            int firstFoundIndex = -1;
            int foundIndex = -1;

            imei = imeiSearchTxt.Text;
            deviceName = deviceNameSearchTxt.Text;
            ip = ipSearchTxt.Text;
            firmwareVersion = firmwareVersionSearchTxt.Text;
            for (int i = 0; i < clientGridView.Rows.Count; i++)
            {
                allMatch = true;
                row = clientGridView.Rows[i];
                if (imei.Trim().Length > 0)
                {
                    if (imei.Equals(row.Cells[IMEI_COLUMN].Value))
                    {
                        found = true;
                    }
                    else
                    {
                        found = false;
                        allMatch = false;
                    }
                }
                if (deviceName.Trim().Length > 0)
                {
                    if (deviceName.Equals(row.Cells[DEVICE_NAME_COLUMN].Value))
                    {
                        found = true;
                    }
                    else
                    {
                        found = false;
                        allMatch = false;
                    }
                }
                if (ip.Trim().Length > 0)
                {
                    if (ip.Equals(row.Cells[IP_COLUMN].Value))
                    {
                        found = true;
                    }
                    else
                    {
                        found = false;
                        allMatch = false;
                    }
                }
                if (firmwareVersion.Trim().Length > 0)
                {
                    if (firmwareVersion.Equals(row.Cells[FIRMWARE_VERSION_COLUMN].Value))
                    {
                        found = true;
                    }
                    else
                    {
                        found = false;
                        allMatch = false;
                    }
                }
                if (found && allMatch)
                {
                    foundIndex = i;
                    if (firstFoundIndex < 0)
                    {
                        firstFoundIndex = foundIndex;
                    }
                    if (lastFoundRow >= 0)
                    {
                        if (foundIndex > lastFoundRow)
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    found = false;
                }
            }

            if (foundIndex >= 0)
            {
                if (foundIndex == lastFoundRow)
                {
                    foundIndex = firstFoundIndex;
                }
                clientGridView.FirstDisplayedScrollingRowIndex = foundIndex;
                foreach (DataGridViewRow selectedRow in clientGridView.SelectedRows)
                {
                    selectedRow.Selected = false;
                }
                foreach (DataGridViewCell selectedCell in clientGridView.SelectedCells)
                {
                    selectedCell.Selected = false;
                }
                clientGridView.Rows[foundIndex].Selected = true;
                lastFoundRow = foundIndex;
            }
        }

        private void searchDetails_Changed(object sender, EventArgs e)
        {
            lastFoundRow = -1;
        }
    }
}
