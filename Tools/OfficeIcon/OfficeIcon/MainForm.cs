using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Reflection;

namespace OfficeIcon
{
    public partial class MainForm : Form
    {
        const int IconCols = 50;
        const int IconRows = 50;
        const int IconSize = 16;
        const int IconBorder = 2;

        private Bitmap icons = null;
        private int lastCol = 0;
        private int lastRow = 0;

        public MainForm()
        {
            InitializeComponent();
            this.DoubleBuffered = true;
            panelIcons.Location = new Point(0, 0);
            panelIcons.Size = new Size(IconCols * (IconSize + IconBorder) + IconBorder, IconRows * (IconSize + IconBorder) + IconBorder);
            this.icons = new Bitmap(panelIcons.Size.Width, panelIcons.Size.Height);
            using (Graphics canvas = Graphics.FromImage(this.icons))
            {
                for (int x = 0; x < IconCols; x++)
                {
                    for (int y = 0; y < IconCols; y++)
                    {
                        using (Bitmap icon = new Bitmap(GetIconPath(x, y)))
                        {
                            canvas.DrawImage(icon, GetIconPosition(x, y).Location);
                        }
                    }
                }
            }
            panelIcons.GetType().GetProperty("DoubleBuffered", BindingFlags.Instance | BindingFlags.NonPublic).SetValue(panelIcons, true, new object[] { });
        }

        private string itoa2(int i)
        {
            string result = i.ToString();
            if (result.Length == 1)
            {
                result = "0" + result;
            }
            return result;
        }

        private string GetIconPath(int col, int row)
        {
            return string.Format(Path.GetDirectoryName(Application.ExecutablePath) + @"\..\..\Office2003Icons\({0},{1}).png", itoa2(col), itoa2(row));
        }

        private Rectangle GetIconPosition(int col, int row)
        {
            return new Rectangle(col * (IconSize + IconBorder) + IconBorder, row * (IconSize + IconBorder) + IconBorder, IconSize, IconSize);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
        }

        private void panelIcons_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(this.icons, 0, 0);
            e.Graphics.DrawRectangle(Pens.Red, GetIconPosition(this.lastCol, this.lastRow));
        }

        private void panelIcons_MouseMove(object sender, MouseEventArgs e)
        {
            int col = e.X / (IconSize + IconBorder);
            int row = e.Y / (IconSize + IconBorder);
            if (col != this.lastCol || row != this.lastRow)
            {
                this.lastCol = col;
                this.lastRow = row;
                this.Text = string.Format("Office Icons ({0} - {1})", this.lastCol, this.lastRow);
                Refresh();
            }
        }

        private void contextSaveAs_Click(object sender, EventArgs e)
        {
            if (this.lastCol >= 0 && this.lastCol < IconCols && this.lastRow >= 0 && this.lastRow < IconRows)
            {
                if (dialogSave.ShowDialog() == DialogResult.OK)
                {
                    File.Copy(GetIconPath(this.lastCol, this.lastRow), dialogSave.FileName, true);
                }
            }
            else
            {
                MessageBox.Show("Wrong position.");
            }
        }
    }
}
