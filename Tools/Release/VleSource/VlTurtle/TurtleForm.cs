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
using System.Threading;

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

        private Bitmap finalBuffer = null;
        private Graphics finalCanvas = null;

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
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.UserPaint, true);
            this.ClientSize = new System.Drawing.Size(800, 600);

            this.buffer = new Bitmap(this.ClientSize.Width, this.ClientSize.Height);
            this.canvas = Graphics.FromImage(this.buffer);
            this.canvas.FillRectangle(Brushes.White, 0, 0, this.ClientSize.Width, this.ClientSize.Height);
            this.canvas.CompositingQuality = CompositingQuality.HighQuality;
            this.canvas.SmoothingMode = SmoothingMode.AntiAlias;

            this.finalBuffer = new Bitmap(this.ClientSize.Width, this.ClientSize.Height);
            this.finalCanvas = Graphics.FromImage(this.finalBuffer);
            this.finalCanvas.FillRectangle(Brushes.White, 0, 0, this.ClientSize.Width, this.ClientSize.Height);
            this.finalCanvas.CompositingQuality = CompositingQuality.HighQuality;
            this.canvas.SmoothingMode = SmoothingMode.AntiAlias;

            this.turtleX = this.ClientSize.Width / 2;
            this.turtleY = this.ClientSize.Height / 2;
            this.turtlePen = new Pen(Color.Black, 3);
            LoadProgram();
        }

        #region API

        private double turtleX;
        private double turtleY;
        private bool turtleDrawing = false;
        private double turtleDirection = -90;
        private Pen turtlePen = null;
        private double turtleSpeed = 100;
        private int turtleFPS = 20;

        private void TurtleGetDestination(double length, out double x, out double y)
        {
            double t = this.turtleDirection / 180 * Math.PI;
            double dx = length * Math.Cos(t);
            double dy = length * Math.Sin(t);
            x = this.turtleX + dx;
            y = this.turtleY + dy;
        }

        private void TurtleGo(double length, double originX, double originY)
        {
            double tempX, tempY;
            TurtleGetDestination(length, out tempX, out tempY);
            if (this.turtleDrawing)
            {
                this.canvas.DrawLine(this.turtlePen, (float)originX, (float)originY, (float)tempX, (float)tempY);
            }
            this.turtleX = tempX;
            this.turtleY = tempY;
            this.Refresh();
            Application.DoEvents();
            Thread.Sleep(1000 / this.turtleFPS);
        }

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
            double originX, originY, finalX, finalY;
            originX = this.turtleX;
            originY = this.turtleY;
            TurtleGetDestination(length, out finalX, out finalY);

            int steps = (int)(this.turtleFPS * Math.Round(length / this.turtleSpeed));
            double stepLength = this.turtleSpeed / this.turtleFPS;
            for (int i = 0; i < steps; i++)
            {
                TurtleGo(stepLength, originX, originY);
            }

            this.turtleX = originX;
            this.turtleY = originY;
            TurtleGo(length, originX, originY);

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
            this.finalCanvas.Dispose();
            this.finalBuffer.Dispose();
            this.canvas.Dispose();
            this.buffer.Dispose();
        }

        private void TurtleForm_Paint(object sender, PaintEventArgs e)
        {
            this.finalCanvas.DrawImage(this.buffer, 0, 0);
            RectangleF turtle = new RectangleF((float)this.turtleX - 3, (float)this.turtleY - 3, 6, 6);
            this.finalCanvas.FillEllipse(Brushes.White, turtle);
            this.finalCanvas.DrawEllipse(Pens.Black, turtle);
            e.Graphics.DrawImage(this.finalBuffer, 0, 0);
        }

        private void TurtleForm_Shown(object sender, EventArgs e)
        {
            RunProgram();
        }
    }
}
