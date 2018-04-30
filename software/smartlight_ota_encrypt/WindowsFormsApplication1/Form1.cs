using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Security.Cryptography;
using System.Threading.Tasks;
using System.IO;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
       // string _key = "&*#@($*&#(*%)&)(";
        //string _iv = "&^&*@!#(&*^%#(*(";
        //sml V2-hcm
       // byte[] key = Encoding.ASCII.GetBytes("&*#@($*&#(*%)&)(");
        //byte[] iv =  Encoding.ASCII.GetBytes("$(*&#@*^!)%)@(*#");
        //sml V1
        byte[] key = Encoding.ASCII.GetBytes("^$&#(@*%%^^%^*^$");
        byte[] iv = Encoding.ASCII.GetBytes("^$&#(@*%%^^%^*^$");
        private StreamReader fileReader = null;
        private String content = "";
        byte[] readBuff;
        public Form1()
        {
            InitializeComponent();
        }

        public byte[] AES_Decrypt(byte[] cipherData, byte[] Key, byte[] IV)
        {
            MemoryStream ms = new MemoryStream();
            Rijndael alg = Rijndael.Create();
            alg.Key = Key;
            alg.IV = IV;
            alg.Padding = PaddingMode.Zeros;
            CryptoStream cs = new CryptoStream(ms,
                alg.CreateDecryptor(), CryptoStreamMode.Write);
            cs.Write(cipherData, 0, cipherData.Length);
            cs.Close();
            byte[] decryptedData = ms.ToArray();

            return decryptedData;
        }

        public byte[] AES_Encrypt(byte[] cipherData, byte[] Key, byte[] IV)
        {
            MemoryStream ms = new MemoryStream();
            Rijndael alg = Rijndael.Create();
            alg.Key = Key;
            alg.IV = IV;
            alg.Padding = PaddingMode.PKCS7;
            alg.Mode = CipherMode.CBC;
            CryptoStream cs = new CryptoStream(ms, alg.CreateEncryptor(), CryptoStreamMode.Write);
            cs.Write(cipherData, 0, cipherData.Length);
            cs.Close();
            byte[] encryptedData = ms.ToArray();

            return encryptedData;
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            FileInfo fi = new FileInfo(System.Reflection.Assembly.GetExecutingAssembly().Location);
            {

                string[] filePaths = Directory.GetFiles(fi.DirectoryName, "*.bin");
                if (filePaths.Length > 0)
                {
                    txtIn.Text = filePaths[0];
                    txtOut.Text = fi.DirectoryName + "\\out.bin";
                    fileReader = new StreamReader(filePaths[0]);
                    content = fileReader.ReadToEnd();
                    readBuff = File.ReadAllBytes(filePaths[0]);
                    fileReader.Close();
                }
                else
                {
                    txtIn.Text = fi.DirectoryName;
                    txtOut.Text = fi.DirectoryName;
                }
                btnEncrypt.Enabled = false;
                btnOut.Enabled = false;
                dlgFile.InitialDirectory = fi.DirectoryName;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] tempBuff;
            int i;
            dlgFile.CheckFileExists = true;
            if (dlgFile.ShowDialog() == DialogResult.OK)
            {
                txtIn.Text = dlgFile.FileName;
                fileReader = new StreamReader(dlgFile.FileName);
                content = fileReader.ReadToEnd();
                readBuff = File.ReadAllBytes(dlgFile.FileName);
                btnOut.Enabled = true;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            dlgFile.CheckFileExists = false;
            if (dlgFile.ShowDialog() == DialogResult.OK)
            {
                txtOut.Text = dlgFile.FileName;
                btnEncrypt.Enabled = true;
            }
        }

        private void btnEncrypt_Click(object sender, EventArgs e)
        {
            int i;
            byte[] tempOut;
            byte[] newBuff = new byte[readBuff.Length];
            tempOut = AES_Encrypt(readBuff, key, iv);
            /*for(i=0;i<readBuff.Length;i+=16)
            {
                byte[] tempIn = new byte[16];
                byte[] tempOut = new byte[16];
                try
                {
                    Buffer.BlockCopy(readBuff, i, tempIn, 0, 16);
                    tempOut = AES_Encrypt(tempIn, key, iv);
                    Buffer.BlockCopy(tempOut, 0, newBuff, i, tempOut.Length);
                }
                catch (System.ArgumentException)
                {

                }
            }*/
            /*
            byte[] temp = new byte[16];
            byte[] Out = new byte[16];
            Array.Clear(temp, 0, 16);
            i -= 16;
            Buffer.BlockCopy(readBuff, i, temp, 0, readBuff.Length - i);
            Out = AES_Encrypt(temp, key, iv);
            Buffer.BlockCopy(temp, 0, newBuff, i, 16);*/
            File.WriteAllBytes(txtOut.Text, tempOut);
            btnEncrypt.Enabled = false;
            btnOut.Enabled = false;
            MessageBox.Show("Done!", "Done");
        }
    }

}
