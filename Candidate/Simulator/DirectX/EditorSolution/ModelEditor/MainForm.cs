using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Reflection;
using ModelEditor.GeometryForms;

namespace ModelEditor
{
    public partial class MainForm : Form
    {
        private IntPtr editorWindow = IntPtr.Zero;
        private string workingDirectory = null;

        public MainForm()
        {
            InitializeComponent();
            panelEditorWindow.GetType()
                .GetMethod("SetStyle", BindingFlags.NonPublic | BindingFlags.Instance)
                .Invoke(panelEditorWindow, new object[] { ControlStyles.Selectable, true });
            panelEditorWindow.Select();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            this.workingDirectory = Path.GetDirectoryName(Application.ExecutablePath) + "\\";
            this.editorWindow = ModelEditorCore.CreateEditorWindow(panelEditorWindow.Handle, this.workingDirectory);
            ModelEditorCore.RenderEditorWindow(this.editorWindow);
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            ModelEditorCore.DestroyEditorWindow(this.editorWindow);
            this.editorWindow = IntPtr.Zero;
        }

        private void timerRenderer_Tick(object sender, EventArgs e)
        {
            ModelEditorCore.RenderEditorWindow(this.editorWindow);
        }

        private void resetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ModelEditorCore.ResetView(this.editorWindow);
        }

        private void cSelectionModeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ModelEditorCore.EditorModeSelection(this.editorWindow);
        }

        private void tTranslationModeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ModelEditorCore.EditorModeTranslation(this.editorWindow);
        }

        private void rRotationModeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ModelEditorCore.EditorModeRotation(this.editorWindow);
        }

        private void sScalingModeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ModelEditorCore.EditorModeScaling(this.editorWindow);
        }

        private void gGlobalAxisToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ModelEditorCore.EditorAxisGlobal(this.editorWindow);
        }

        private void lLocalAxisToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ModelEditorCore.EditorAxisLocal(this.editorWindow);
        }

        private void cubeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ModelEditorCore.CreateModelCube(this.editorWindow);
        }

        private void CreateConicCommon(GeometryConicType geometry)
        {
            using (GeometryConicForm form = new GeometryConicForm(geometry))
            {
                if (form.ShowDialog() == DialogResult.OK)
                {
                    switch (form.Geometry)
                    {
                        case GeometryConicType.Sphere:
                            ModelEditorCore.CreateModelSphere(this.editorWindow, form.Rows, form.Cols);
                            break;
                        case GeometryConicType.Cylinder:
                            ModelEditorCore.CreateModelCylinder(this.editorWindow, form.Rows, form.Cols);
                            break;
                        case GeometryConicType.Cone:
                            ModelEditorCore.CreateModelCone(this.editorWindow, form.Rows, form.Cols);
                            break;
                    }
                }
            }
        }

        private void sphereToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateConicCommon(GeometryConicType.Sphere);
        }

        private void cylnderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateConicCommon(GeometryConicType.Cylinder);
        }

        private void coneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateConicCommon(GeometryConicType.Cone);
        }
    }
}
