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

        private Bitmap finalBuffer = null;
        private Graphics finalCanvas = null;
        private Bitmap cachedBuffer = null;
        private Graphics cachedCanvas = null;

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
            this.DoubleBuffered = true;
            this.ClientSize = new System.Drawing.Size(800, 600);

            this.cachedBuffer = new Bitmap(this.ClientSize.Width, this.ClientSize.Height);
            this.cachedCanvas = Graphics.FromImage(this.cachedBuffer);
            this.cachedCanvas.FillRectangle(Brushes.White, 0, 0, this.ClientSize.Width, this.ClientSize.Height);
            this.cachedCanvas.CompositingQuality = CompositingQuality.HighQuality;
            this.cachedCanvas.SmoothingMode = SmoothingMode.AntiAlias;

            this.finalBuffer = new Bitmap(this.ClientSize.Width, this.ClientSize.Height);
            this.finalCanvas = Graphics.FromImage(this.finalBuffer);
            this.finalCanvas.FillRectangle(Brushes.White, 0, 0, this.ClientSize.Width, this.ClientSize.Height);
            this.finalCanvas.CompositingQuality = CompositingQuality.HighQuality;
            this.finalCanvas.SmoothingMode = SmoothingMode.AntiAlias;

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
        private double turtleSpeed = 200;
        private int turtleFPS = 20;

        private void TurtleGetDestination(double length, out double x, out double y)
        {
            double t = this.turtleDirection / 180 * Math.PI;
            double dx = length * Math.Cos(t);
            double dy = length * Math.Sin(t);
            x = this.turtleX + dx;
            y = this.turtleY + dy;
        }

        private void TurtleGo(double length, double originX, double originY, bool final)
        {

            double tempX, tempY;
            TurtleGetDestination(length, out tempX, out tempY);
            if (!final)
            {
                this.finalCanvas.DrawImage(this.cachedBuffer, 0, 0);
            }
            if (this.turtleDrawing)
            {
                if (final)
                {
                    this.cachedCanvas.DrawLine(this.turtlePen, (float)originX, (float)originY, (float)tempX, (float)tempY);
                }
                else
                {
                    this.finalCanvas.DrawLine(this.turtlePen, (float)originX, (float)originY, (float)tempX, (float)tempY);
                }
            }
            if (final)
            {
                this.finalCanvas.DrawImage(this.cachedBuffer, 0, 0);
            }
            this.turtleX = tempX;
            this.turtleY = tempY;

            RectangleF turtle = new RectangleF((float)this.turtleX - 3, (float)this.turtleY - 3, 6, 6);
            this.finalCanvas.FillEllipse(Brushes.White, turtle);
            this.finalCanvas.DrawEllipse(Pens.Black, turtle);

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

            int steps = (int)Math.Round(this.turtleFPS * (length / this.turtleSpeed));
            double stepLength = this.turtleSpeed / this.turtleFPS;
            for (int i = 0; i < steps; i++)
            {
                TurtleGo(stepLength, originX, originY, false);
            }

            this.turtleX = originX;
            this.turtleY = originY;
            TurtleGo(length, originX, originY, true);

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
            this.cachedCanvas.Dispose();
            this.cachedBuffer.Dispose();
        }

        private void TurtleForm_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(this.finalBuffer, 0, 0);
        }

        private void TurtleForm_Shown(object sender, EventArgs e)
        {
            RunProgram();
        }
    }
}
