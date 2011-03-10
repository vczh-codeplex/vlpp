using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using VlScriptDotNet;
using System.IO;
using System.Runtime.InteropServices;
using System.Drawing.Drawing2D;

namespace VlTurtle
{
    public partial class TurtleForm : Form
    {
        private VlsHost host = null;
        private VlsState state = null;
        private VlsAssembly assemblySyscrnat = null;
        private VlsAssembly assemblyTurtle = null;
        private VlsAssembly assemblyMain = null;

        private Bitmap buffer = null;
        private Graphics canvas = null;

        private void LoadProgram()
        {
            string path = Path.GetDirectoryName(Application.ExecutablePath) + "\\";
            this.host = new VlsHost(65536);
            this.state = new VlsState(this.host);
            this.assemblySyscrnat = new VlsAssembly(this.host, path + "Assembly.syscrnat.assembly");
            this.assemblyTurtle = new VlsAssembly(this.host, path + "Script\\Turtle.assembly");
            this.assemblyMain = new VlsAssembly(this.host, path + "Script\\NativeX\\TurtleController.assembly");

            this.host.InstallCoreNativePlugin();
            this.host.RegisterForeignFunction("Turtle", "MessageBox", TurtleMessageBox, IntPtr.Zero);
            this.host.RegisterForeignFunction("Turtle", "Down", TurtleDown, IntPtr.Zero);
            this.host.RegisterForeignFunction("Turtle", "Up", TurtleUp, IntPtr.Zero);
            this.host.RegisterForeignFunction("Turtle", "Move", TurtleMove, IntPtr.Zero);
            this.host.RegisterForeignFunction("Turtle", "TurnLeft", TurtleTurnLeft, IntPtr.Zero);
            this.host.RegisterForeignFunction("Turtle", "TurnRight", TurtleTurnRight, IntPtr.Zero);
            this.host.InstallAssembly(this.assemblySyscrnat);
            this.host.InstallAssembly(this.assemblyTurtle);
            this.host.InstallAssembly(this.assemblyMain);
            this.state.InitializeInstalledAssembly(this.assemblySyscrnat);
            this.state.InitializeInstalledAssembly(this.assemblyTurtle);
            this.state.InitializeInstalledAssembly(this.assemblyMain);
        }

        private void RunProgram()
        {
            VlsBasicFunction mainFunction = this.assemblyMain.BasicFunctions.Where(f => f.Name == "main").FirstOrDefault();
            if (mainFunction == null)
            {
                MessageBox.Show("Cannot find main function.", this.Text);
            }
            else
            {
                this.state.PrepareToCall(mainFunction, IntPtr.Zero);
                this.state.Call();
            }
        }

        public TurtleForm()
        {
            InitializeComponent();
            this.ClientSize = new System.Drawing.Size(800, 600);
            this.buffer = new Bitmap(this.ClientSize.Width, this.ClientSize.Height);
            this.canvas = Graphics.FromImage(this.buffer);
            this.canvas.FillRectangle(Brushes.White, 0, 0, this.ClientSize.Width, this.ClientSize.Height);
            this.canvas.CompositingQuality = CompositingQuality.HighQuality;
            this.turtleX = this.ClientSize.Width / 2;
            this.turtleY = this.ClientSize.Height / 2;
            this.turtlePen = new Pen(Color.Black, 3);
            LoadProgram();
            RunProgram();
        }

        #region API

        private double turtleX;
        private double turtleY;
        private bool turtleDrawing = false;
        private double turtleDirection = -90;
        private Pen turtlePen = null;

        private int TurtleMessageBox(IntPtr result, IntPtr arguments, IntPtr userData)
        {
            VlsForeignAccessor accessor = new VlsForeignAccessor(result, arguments);
            string message = accessor.GetWStr();
            MessageBox.Show(message, this.Text);
            return accessor.Size;
        }

        private int TurtleDown(IntPtr result, IntPtr arguments, IntPtr userData)
        {
            this.turtleDrawing = true;
            return 0;
        }

        private int TurtleUp(IntPtr result, IntPtr arguments, IntPtr userData)
        {
            this.turtleDrawing = false;
            return 0;
        }

        private int TurtleMove(IntPtr result, IntPtr arguments, IntPtr userData)
        {
            VlsForeignAccessor accessor = new VlsForeignAccessor(result, arguments);
            double length = accessor.Get<double>();

            double t = this.turtleDirection / 180 * Math.PI;
            double dx = length * Math.Cos(t);
            double dy = length * Math.Sin(t);
            double x = this.turtleX + dx;
            double y = this.turtleY + dy;
            if (this.turtleDrawing)
            {
                this.canvas.DrawLine(this.turtlePen, (float)this.turtleX, (float)this.turtleY, (float)x, (float)y);
                this.Refresh();
            }
            this.turtleX = x;
            this.turtleY = y;

            return accessor.Size;
        }

        private int TurtleTurnLeft(IntPtr result, IntPtr arguments, IntPtr userData)
        {
            VlsForeignAccessor accessor = new VlsForeignAccessor(result, arguments);
            double angle = accessor.Get<double>();
            this.turtleDirection -= angle;
            return accessor.Size;
        }

        private int TurtleTurnRight(IntPtr result, IntPtr arguments, IntPtr userData)
        {
            VlsForeignAccessor accessor = new VlsForeignAccessor(result, arguments);
            double angle = accessor.Get<double>();
            this.turtleDirection += angle;
            return accessor.Size;
        }

        #endregion

        private void TurtleForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.assemblyMain.Dispose();
            this.assemblyTurtle.Dispose();
            this.assemblySyscrnat.Dispose();
            this.state.Dispose();
            this.host.Dispose();
            this.turtlePen.Dispose();
            this.canvas.Dispose();
            this.buffer.Dispose();
        }

        private void TurtleForm_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(this.buffer, 0, 0);
        }
    }
}
