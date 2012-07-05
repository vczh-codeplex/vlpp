using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;

namespace ShaofeiCaptcha
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private Bitmap Process(Bitmap sourceBitmap)
        {
            return sourceBitmap;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(@"http://shaofei.name/captcha.php");
            request.Method = "GET";
            var response = request.GetResponse();
            using (var stream = response.GetResponseStream())
            {
                var bitmap = new Bitmap(stream);
                pictureBox1.Image = bitmap;
                pictureBox2.Image = Process(bitmap);
            }
            response.Close();
        }
    }
}
