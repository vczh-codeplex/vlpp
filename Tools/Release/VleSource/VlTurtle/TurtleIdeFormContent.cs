using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Developer.LanguageProvider;
using VlTurtle.EditorControls;
using System.IO;
using System.Diagnostics;
using System.Threading;

namespace VlTurtle
{
    public partial class TurtleIdeFormContent : UserControl
    {
        private ClipboardMonitor clipboardMonitor = null;
        private readonly string workingFolder = Path.GetDirectoryName(typeof(TurtleIdeFormContent).Assembly.Location) + "\\";
        private string fileName = "";
        private Process turtleProcess = null;

        public TurtleIdeFormContent()
        {
            InitializeComponent();
            if (this.components == null)
            {
                this.components = new Container();
            }
            this.clipboardMonitor = new ClipboardMonitor(this);
            this.clipboardMonitor.ClipboardChanged += new EventHandler(clipboardMonitor_ClipboardChanged);
            this.components.Add(this.clipboardMonitor);
        }

        private void clipboardMonitor_ClipboardChanged(object sender, EventArgs e)
        {
            InvokeButtonStateUpdated();
        }

        private void codeEditorNativeX_SelectionChanged(object sender, EventArgs e)
        {
            InvokeButtonStateUpdated();
        }

        protected override void WndProc(ref Message m)
        {
            if (this.clipboardMonitor != null)
            {
                this.clipboardMonitor.WndProc(ref m);
            }
            base.WndProc(ref m);
        }

        private void InvokeButtonStateUpdated()
        {
            if (this.ButtonStateUpdated != null)
            {
                this.ButtonStateUpdated(this, new EventArgs());
            }
        }

        private void LoadDefaultCode()
        {
            this.codeEditorNativeX.Text =
                "unit MyTurtleProgram;\r\n\r\n" +
                "uses syscrnat, Turtle;\r\n\r\n" +
                "function void main()\r\n" +
                "{\r\n" +
                "\t\r\n" +
                "}\r\n";
            this.codeEditorNativeX.ClearUndoRedoHistory();
            this.codeEditorNativeX.Controller.Select(new TextPosition(6, 1), new TextPosition(6, 1));
        }

        public override string Text
        {
            get
            {
                return this.codeEditorNativeX.Text;
            }
            set
            {
                this.codeEditorNativeX.Text = value;
            }
        }

        #region Operations

        public event EventHandler ButtonStateUpdated;

        public void OperationOpenSample(string fullName)
        {
            this.fileName = "";
            using (StreamReader reader = new StreamReader(typeof(TurtleIdeFormContent).Assembly.GetManifestResourceStream(fullName)))
            {
                this.codeEditorNativeX.Text = reader.ReadToEnd();
            }
            this.codeEditorNativeX.ClearUndoRedoHistory();
            InvokeButtonStateUpdated();
        }

        public void OperationNew()
        {
            LoadDefaultCode();
            this.fileName = "";
            InvokeButtonStateUpdated();
        }

        public void OperationOpen()
        {
            if (dialogOpen.ShowDialog() == DialogResult.OK)
            {
                this.fileName = dialogOpen.FileName;
                using (StreamReader reader = new StreamReader(this.fileName))
                {
                    this.codeEditorNativeX.Text = reader.ReadToEnd();
                }
                this.codeEditorNativeX.ClearUndoRedoHistory();
                InvokeButtonStateUpdated();
            }
        }

        public void OperationSave()
        {
            if (this.fileName == "")
            {
                OperationSaveAs();
            }
            else
            {
                using (StreamWriter writer = new StreamWriter(this.fileName))
                {
                    writer.Write(this.codeEditorNativeX.Text);
                }
                InvokeButtonStateUpdated();
            }
        }

        public void OperationSaveAs()
        {
            if (dialogSave.ShowDialog() == DialogResult.OK)
            {
                this.fileName = dialogSave.FileName;
                OperationSave();
            }
        }

        public void OperationCut()
        {
            this.codeEditorNativeX.Cut();
            InvokeButtonStateUpdated();
        }

        public void OperationCopy()
        {
            this.codeEditorNativeX.Copy();
            InvokeButtonStateUpdated();
        }

        public void OperationPaste()
        {
            this.codeEditorNativeX.Paste();
            InvokeButtonStateUpdated();
        }

        public void OperationRedo()
        {
            this.codeEditorNativeX.Redo();
            InvokeButtonStateUpdated();
        }

        public void OperationUndo()
        {
            this.codeEditorNativeX.Undo();
            InvokeButtonStateUpdated();
        }

        public void OperationRun()
        {
            textBoxOutput.Text = "Compling...\r\n";
            try
            {
                string makePath = this.workingFolder + "Script\\NativeX\\Make.txt";
                string codePath = this.workingFolder + "Script\\NativeX\\NativeX.txt";
                string errPath = this.workingFolder + "Script\\NativeX\\Error.txt";
                string binPath = this.workingFolder + "Script\\NativeX\\TurtleController.assembly";

                if (File.Exists(errPath)) File.Delete(errPath);
                if (File.Exists(binPath)) File.Delete(binPath);
                using (StreamWriter writer = new StreamWriter(codePath))
                {
                    writer.Write(this.codeEditorNativeX.Text);
                }
                Execute(workingFolder + "vle.exe", "make \"" + makePath + "\"").WaitForExit();

                if (File.Exists(binPath))
                {
                    textBoxOutput.Text += "Finished.\r\n";
                    textBoxOutput.Text += "Running...\r\n";
                    this.turtleProcess = Execute(Application.ExecutablePath, "Execute");
                    new Thread(new ThreadStart(() =>
                    {
                        this.turtleProcess.WaitForExit();
                        this.Invoke(new MethodInvoker(() =>
                        {
                            InternalStop(true);
                        }));
                    })).Start();
                    InvokeButtonStateUpdated();
                }
                else
                {
                    using (StreamReader reader = new StreamReader(errPath))
                    {
                        textBoxOutput.Text += reader.ReadToEnd();
                    }
                }
            }
            catch (Exception ex)
            {
                textBoxOutput.Text += "ERROR: " + ex.Message;
            }
        }

        private void InternalStop(bool internalCall)
        {
            if (this.turtleProcess != null)
            {
                try
                {
                    this.turtleProcess.Kill();
                }
                catch (Exception)
                {
                }
                this.turtleProcess = null;
            }
            if (internalCall)
            {
                textBoxOutput.Text += "Stopped\r\n";
            }
            InvokeButtonStateUpdated();
        }

        public void OperationStop()
        {
            InternalStop(false);
        }

        private Process Execute(string exe, string args)
        {
            ProcessStartInfo info = new ProcessStartInfo();
            info.Arguments = args;
            info.CreateNoWindow = true;
            info.FileName = exe;
            info.UseShellExecute = false;
            info.ErrorDialog = true;

            Process process = new Process();
            process.StartInfo = info;
            process.Start();
            return process;
        }

        #endregion

        #region Operation States

        public string WindowTitle
        {
            get
            {
                return "Turtle IDE v0.1 [" + (this.fileName == "" ? "Untitled" : Path.GetFileName(this.fileName)) + "]";
            }
        }

        public bool AvailableCut()
        {
            return codeEditorNativeX.CanCut;
        }

        public bool AvailableCopy()
        {
            return codeEditorNativeX.CanCopy;
        }

        public bool AvailablePaste()
        {
            return codeEditorNativeX.CanPaste;
        }

        public bool AvailableRedo()
        {
            return codeEditorNativeX.CanRedo;
        }

        public bool AvailableUndo()
        {
            return codeEditorNativeX.CanUndo;
        }

        public bool AvailableRun()
        {
            return this.turtleProcess == null;
        }

        public bool AvailableStop()
        {
            return this.turtleProcess != null;
        }

        #endregion
    }
}
