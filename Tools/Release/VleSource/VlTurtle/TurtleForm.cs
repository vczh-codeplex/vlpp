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

namespace VlTurtle
{
    public partial class TurtleForm : Form
    {
        private VlsHost host = null;
        private VlsState state = null;
        private VlsAssembly assemblySyscrnat = null;
        private VlsAssembly assemblyTurtle = null;
        private VlsAssembly assemblyMain = null;

        private int TurtleMessageBox(IntPtr result, IntPtr arguments, IntPtr userData)
        {
            IntPtr ptr = (IntPtr)Marshal.PtrToStructure(arguments, typeof(IntPtr));
            string str = Marshal.PtrToStringUni(ptr);
            MessageBox.Show(str, this.Text);
            return Marshal.SizeOf(typeof(IntPtr));
        }

        public TurtleForm()
        {
            InitializeComponent();
            string path = Path.GetDirectoryName(Application.ExecutablePath) + "\\";

            this.host = new VlsHost(65536);
            this.state = new VlsState(this.host);
            this.assemblySyscrnat = new VlsAssembly(this.host, path + "Assembly.syscrnat.assembly");
            this.assemblyTurtle = new VlsAssembly(this.host, path + "Script\\Turtle.assembly");
            this.assemblyMain = new VlsAssembly(this.host, path + "Script\\NativeX\\TurtleController.assembly");

            this.host.InstallCoreNativePlugin();
            this.host.RegisterForeignFunction("Turtle", "MessageBox", TurtleMessageBox, IntPtr.Zero);
            this.host.InstallAssembly(this.assemblySyscrnat);
            this.host.InstallAssembly(this.assemblyTurtle);
            this.host.InstallAssembly(this.assemblyMain);
            this.state.InitializeInstalledAssembly(this.assemblySyscrnat);
            this.state.InitializeInstalledAssembly(this.assemblyTurtle);
            this.state.InitializeInstalledAssembly(this.assemblyMain);

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

        private void TurtleForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.assemblyMain.Dispose();
            this.assemblyTurtle.Dispose();
            this.assemblySyscrnat.Dispose();
            this.state.Dispose();
            this.host.Dispose();
        }
    }
}
