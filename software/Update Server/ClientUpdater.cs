using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Net.Sockets;
using System.Net;
using System.Windows.Forms;
using System.Threading;
using System.Runtime.InteropServices;

namespace PJ1000_Update_Server
{
    public class ClientUpdater
    {
        const int BUFFER_SIZE = 1024;

        const int DEFAULT_WRITE_TIMEOUT = 500;
        const int MAX_RETRY_COUNT = 5;

        const int START_CODE = 0xCA;

        const byte READ_LOG_OPCODE = 0x03;
        const int DOWNLOAD_FIRMWARE_OPCODE = 0x12;
        const int RETRIEVE_IMEI_OPCODE = 0x77;

        const UInt32 DEFAULT_PACKET_NO = 0xA5A5A5A5;
        const UInt32 ERROR_PACKET_NO = 0xffffffff;
        const UInt32 FINISH_PACKET_NO = 0x5A5A5A5A;
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

        public TcpClient client;
        public string imei;
        public string name;
        public string version;
        public bool infoAvailable;
        public TimeSpan elapsedTime;
        private NetworkStream stream;
        private MainForm mainForm;
        private TextBox logTxtBox;
        private byte[] inputBuffer;
        private byte[] outputBuffer;
        private byte[] receivedBuffer;
        private int deviceBufferSize;
        private List<byte> loadedFirmware;
        private bool downloadingFirmware;
        private DateTime startDownloadTime;

        public ClientUpdater()
        {
            client = null;
        }

        public ClientUpdater(TcpClient client, MainForm mainForm)
        {
            this.imei = null;
            this.client = client;
            this.mainForm = mainForm;
            this.logTxtBox = mainForm.logTxtBox;
            this.infoAvailable = false;
        }

        public void Start()
        {
            if (client == null)
            {
                return;
            }

            inputBuffer = new byte[client.ReceiveBufferSize];
            outputBuffer = new byte[client.SendBufferSize];
            receivedBuffer = new byte[client.ReceiveBufferSize];
            Thread.Sleep(500);
            try
            {
                stream = client.GetStream();
                stream.BeginRead(receivedBuffer, 0, receivedBuffer.Length, new AsyncCallback(dataReceived), null);
            }
            catch (Exception)
            {
                Console.WriteLine("Unable to start reading. Close the connection");
                Stop();
            }
        }

        public void UpdateData(ClientUpdater updater)
        {
            elapsedTime = updater.elapsedTime;
            deviceBufferSize = updater.deviceBufferSize;
            downloadingFirmware = updater.downloadingFirmware;
            loadedFirmware = updater.loadedFirmware;
            startDownloadTime = updater.startDownloadTime;
        }

        public void Stop()
        {
            try
            {
                stream.Close();
                stream = null;
            }
            catch (Exception)
            { }
            try
            {
                client.Close();
                client = null;
            }
            catch (Exception)
            { }
        }

        public void DownloadFirmware(string binPath)
        {
            byte[] fileSize;
            byte[] fileCRC;

            if (!File.Exists(binPath))
            {
                MessageBox.Show("Invalid file path!");
                return;
            }

           // if (loadedFirmware == null)
            {
                loadedFirmware = ReadAllByteInFile(binPath);
                if (loadedFirmware == null)
                {
                    MessageBox.Show("Cannot open file " + binPath + "!");
                    return;
                }
            }

            fileSize = BitConverter.GetBytes(loadedFirmware.Count);
            CreatePacketHeader(PACKET_NO_SIZE + 8, DOWNLOAD_FIRMWARE_OPCODE, DEFAULT_PACKET_NO);
            Array.Copy(fileSize, 0, outputBuffer, DATA_WITH_PACKET_NO_OFFSET, 4);
            fileCRC = BitConverter.GetBytes(calcCRC32(loadedFirmware.ToArray(), 0, loadedFirmware.Count));
            Array.Copy(fileCRC, 0, outputBuffer, DATA_WITH_PACKET_NO_OFFSET + 4, 4);
            outputBuffer[DATA_WITH_PACKET_NO_OFFSET + 8] = calcCRC(outputBuffer, DATA_OFFSET, PACKET_NO_SIZE + 8);
            SendTCPData(outputBuffer, 0, HEADER_SIZE + PACKET_NO_SIZE + 8 + CRC_SIZE);
            downloadingFirmware = true;
            startDownloadTime = DateTime.Now;
            elapsedTime = new TimeSpan(0);
        }

        private void dataReceived(IAsyncResult ar)
        {
            int copyIndex;
            int len;

            try
            {
                len = stream.EndRead(ar);
                copyIndex = -1;
                for (int i = 0; i < len; i++)
                {
                    if (receivedBuffer[i] == START_CODE)
                    {
                        copyIndex = i;
                        break;
                    }
                }
                if (copyIndex >= 0)
                {
                    Array.Clear(inputBuffer, 0, inputBuffer.Length);
                    Array.Copy(receivedBuffer, copyIndex, inputBuffer, 0, len - copyIndex);
                    Console.WriteLine("Received: ");
                    for (int i = copyIndex; i < len; i++)
                    {
                        Console.Write(receivedBuffer[i].ToString("X2") + " ");
                    }
                    Console.WriteLine();
                    ProcessReceivedData();
                }

                // continue to receive data
                Array.Clear(receivedBuffer, 0, len);
                stream.BeginRead(receivedBuffer, 0, receivedBuffer.Length, new AsyncCallback(dataReceived), null);
            }
            catch (Exception)
            { }
        }

        UInt32 oldPacketNo;
        int oldLength;
        private void ProcessReceivedData()
        {
            byte crc;
            int length;
            byte opcode;
            UInt32 packetNo;
            byte[] bTemp;
            string receivedStr;

            // should have something in the buffer
            length = BitConverter.ToInt16(inputBuffer, LENGTH_OFFSET);
            if ((length <= 0) && (length > MAX_DATA_SIZE))
            {
                return;
            }
            // validate the CRC
            crc = inputBuffer[DATA_OFFSET + length];
            if (crc != calcCRC(inputBuffer, DATA_OFFSET, length))
            {
                return;
            }

            opcode = inputBuffer[OPCODE_OFFSET];
            switch (opcode)
            {
                case READ_LOG_OPCODE:
                    receivedStr = Encoding.ASCII.GetString(inputBuffer, DATA_OFFSET, length);
                    AppendLog(receivedStr);
                    break;

                case RETRIEVE_IMEI_OPCODE:
                    byte[] temp = new byte[Marshal.SizeOf(typeof(INFO_PACKET))];
                    Buffer.BlockCopy(inputBuffer, DATA_OFFSET, temp, 0, temp.Length);
                    INFO_PACKET info = (INFO_PACKET)Lib.ByteArrayToObject(temp, typeof(INFO_PACKET));
                    imei = info.imei;
                    name = info.name;
                    version = info.version;
                    infoAvailable = true;
                    break;

                case DOWNLOAD_FIRMWARE_OPCODE:
                    if (!downloadingFirmware)
                    {
                        return;
                    }
                    elapsedTime = DateTime.Now.Subtract(startDownloadTime);
                    packetNo = BitConverter.ToUInt32(inputBuffer, PACKET_NO_OFFSET);
                    if (packetNo == FINISH_PACKET_NO)
                    {
                        downloadingFirmware = false;
                        mainForm.Invoke((MethodInvoker)delegate
                        {
                            mainForm.UpdateDownloadProgress(imei, 100, elapsedTime);
                        });
                        MessageBox.Show("Firmware downloaded successfully!");
                        return;
                    }
                    if (packetNo == DEFAULT_PACKET_NO)
                    {
                        deviceBufferSize = BitConverter.ToInt32(inputBuffer, DATA_WITH_PACKET_NO_OFFSET + 4);
                        packetNo = 0;
                    }
                    try
                    {
                        if (loadedFirmware == null)
                        {
                            downloadingFirmware = false;
                            MessageBox.Show("Firware is not loaded!");
                            return;
                        }
                        length = loadedFirmware.Count - (int)packetNo;
                        length = (length > deviceBufferSize) ? deviceBufferSize : length;
                        //bTemp = loadedFirmware.GetRange((int)packetNo, length).ToArray();
                        bTemp = Enumerable.Repeat<byte>(0xFF, deviceBufferSize).ToArray();
                        Buffer.BlockCopy(loadedFirmware.GetRange((int)packetNo, length).ToArray(), 0, bTemp, 0, length);
                        //length = bTemp.Length + 4;
                        length = deviceBufferSize + 4;
                        CreatePacketHeader(length, DOWNLOAD_FIRMWARE_OPCODE, packetNo);
                        Array.Copy(bTemp, 0, outputBuffer, DATA_WITH_PACKET_NO_OFFSET, bTemp.Length);
                        outputBuffer[length + HEADER_SIZE] = calcCRC(outputBuffer, DATA_OFFSET, length);
                        SendTCPData(outputBuffer, 0, length + HEADER_SIZE + CRC_SIZE);
                        mainForm.Invoke((MethodInvoker)delegate
                        {
                            mainForm.UpdateDownloadProgress(imei, (int)(packetNo * 100 / loadedFirmware.Count), elapsedTime);
                        });
                        oldPacketNo = packetNo;
                        oldLength = length;
                    }
                    catch (Exception)
                    {
                        downloadingFirmware = false;
                        MessageBox.Show("Firmware downloaded failed!");
                        return;
                    }
                    break;

                default:
                    break;
            }
        }

        private void CreatePacketHeader(int length, byte opcode, UInt32 packetNo)
        {
            Array.Clear(outputBuffer, 0, outputBuffer.Length);
            outputBuffer[START_CODE_OFFSET] = START_CODE;
            Array.Copy(BitConverter.GetBytes(length), 0, outputBuffer, LENGTH_OFFSET, LENGTH_SIZE);
            outputBuffer[OPCODE_OFFSET] = opcode;
            Array.Copy(BitConverter.GetBytes(packetNo), 0, outputBuffer, PACKET_NO_OFFSET, PACKET_NO_SIZE);
        }

        private void SendTCPData(byte[] buffer, int offset, int count)
        {
            try
            {
                lock (client)
                {
                    stream.Write(outputBuffer, offset, count);
                    Console.WriteLine("Sent: ");
                    for (int i = 0; i < count; i++)
                    {
                        Console.Write(outputBuffer[i + offset].ToString("X2") + " ");
                    }
                    Console.WriteLine();
                }
            }
            catch (Exception)
            { }
        }

        private byte calcCRC(byte[] buf, int index, int length)
        {
            byte crc;

            crc = 0;
            for (int i = index; i < index + length; i++)
            {
                crc += buf[i];
            }

            return crc;
        }

        private uint calcCRC32(byte[] buf, int index, int length)
        {
            uint crc;

            crc = 0;
            for (int i = index; i < index + length; i++)
            {
                crc += buf[i];
            }

            return crc;
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

        public static List<byte> ReadAllByteInFile(string fileName)
        {
            BinaryReader reader;
            long fileSize;
            int retry;
            List<byte> buffer;
            byte[] tempBuf;
            int bufIndex;

            reader = null;
            retry = 0;
            while (retry < MAX_RETRY_COUNT)
            {
                try
                {
                    reader = new BinaryReader(new FileStream(fileName, FileMode.Open));
                }
                catch (Exception)
                {
                    retry++;
                    Thread.Sleep(100);
                }
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
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
    internal struct INFO_PACKET
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 18)]
        public string imei;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 18)]
        public string name;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string version;
    }

    class Lib
    {
        internal static byte[] ObjectToByteArray(object obj)
        {
            IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf(obj));

            Marshal.StructureToPtr(obj, ptr, false);

            byte[] arr = new byte[Marshal.SizeOf(obj)];

            Marshal.Copy(ptr, arr, 0, arr.Length);

            Marshal.FreeHGlobal(ptr);

            return arr;
        }

        internal static object ByteArrayToObject(byte[] buf, Type objectType)
        {
            IntPtr ptr = Marshal.AllocHGlobal(buf.Length);
            Marshal.Copy(buf, 0, ptr, buf.Length);

            object Obj = Marshal.PtrToStructure(ptr, objectType);

            Marshal.FreeHGlobal(ptr);

            return Obj;
        }
    }
}
