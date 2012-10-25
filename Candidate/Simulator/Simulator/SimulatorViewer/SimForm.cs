using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SimulatorViewer
{
    public partial class SimForm : Form
    {
        private IntPtr simulator = IntPtr.Zero;
        private IntPtr simulatorFSAA = IntPtr.Zero;
        private List<Tuple<string, IntPtr>> renderers = new List<Tuple<string, IntPtr>>();
        private List<Tuple<string, IntPtr>> scenes = new List<Tuple<string, IntPtr>>();

        private IntPtr currentScene = IntPtr.Zero;
        private IntPtr currentRenderer = IntPtr.Zero;
        private bool rotating = false;
        private Point rotatingPosition;
        private double renderMilliseconds = 0;

        private bool debuggerIntersectPixel = false;

        public SimForm()
        {
            InitializeComponent();
            this.DoubleBuffered = true;
            this.ClientSize = new Size(800, 600);
        }

        private void Render()
        {
            DateTime a = DateTime.Now;
            SimulatorAPI.RenderScene(this.simulator, this.currentScene, this.currentRenderer);
            DateTime b = DateTime.Now;
            this.renderMilliseconds = (b - a).TotalMilliseconds;
            Refresh();
        }

        private void SimForm_Load(object sender, EventArgs e)
        {
            this.simulator = SimulatorAPI.CreateSimulator(false);
            SimulatorAPI.SetSimulatorBuffer(this.simulator, this.ClientSize.Width, this.ClientSize.Height);
            SimulatorAPI.ClearSimulatorBuffer(this.simulator);
            this.simulatorFSAA = SimulatorAPI.CreateSimulator(true);
            SimulatorAPI.SetSimulatorBuffer(this.simulatorFSAA, this.ClientSize.Width, this.ClientSize.Height);
            SimulatorAPI.ClearSimulatorBuffer(this.simulatorFSAA);

            this.renderers.Add(Tuple.Create("Frame", IntPtr.Zero));
            this.renderers.Add(Tuple.Create("Diffuse", SimulatorAPI.CreateDirectTrayRenderer(0)));
            this.renderers.Add(Tuple.Create("Shadow", SimulatorAPI.CreateDirectTrayRenderer(1)));
            this.renderers.Add(Tuple.Create("Reflection & Refraction", SimulatorAPI.CreateDirectTrayRenderer(2)));
            this.renderers.Add(Tuple.Create("AO & BRDF & BSSRDF", SimulatorAPI.CreateDirectTrayRenderer(3)));

            this.scenes.Add(Tuple.Create("Geometries and Glasses", SimulatorAPI.CreateScene1()));
            this.scenes.Add(Tuple.Create("Earth", SimulatorAPI.CreateScene2()));
            this.scenes.Add(Tuple.Create("Earth and Glasses", SimulatorAPI.CreateScene3()));
            this.scenes.Add(Tuple.Create("Cylinders", SimulatorAPI.CreateScene4()));
            this.scenes.Add(Tuple.Create("Menger Sponge", SimulatorAPI.CreateScene5()));
            this.scenes.Add(Tuple.Create("Balls and Walls", SimulatorAPI.CreateScene6()));
            this.scenes.Add(Tuple.Create("Desktop", SimulatorAPI.CreateScene7()));
            //this.scenes.Add(Tuple.Create("Sponza", SimulatorAPI.CreateScene8()));
            //this.scenes.Add(Tuple.Create("Conference", SimulatorAPI.CreateScene9()));

            this.currentScene = this.scenes[0].Item2;
            this.currentRenderer = this.renderers[0].Item2;
            Render();
        }

        private void SimForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            foreach (var s in this.scenes)
            {
                SimulatorAPI.DestroyScene(s.Item2);
            }
            foreach (var r in this.renderers)
            {
                if (r.Item2 != IntPtr.Zero)
                {
                    SimulatorAPI.DestroyRenderer(r.Item2);
                }
            }
            SimulatorAPI.DestroySimulator(this.simulator);
            SimulatorAPI.DestroySimulator(this.simulatorFSAA);
        }

        private void SimForm_Paint(object sender, PaintEventArgs e)
        {
            using (Image image = Image.FromHbitmap(SimulatorAPI.GetSimulatorBitmap(this.simulator)))
            {
                e.Graphics.DrawImage(image, 0, 0);
                e.Graphics.DrawString("FPS: " + Math.Round(100000 / this.renderMilliseconds) / 100 + " (" + Math.Round(this.renderMilliseconds / 10) / 100 + " seconds)",
                    this.Font, Brushes.Yellow, 5, 5);
                e.Graphics.DrawString("Mouse Dragging: Rotation",
                    this.Font, Brushes.Yellow, 5, 20);

                for (int i = 0; i < this.renderers.Count; i++)
                {
                    string name = this.renderers[i].Item1;
                    IntPtr renderer = this.renderers[i].Item2;

                    e.Graphics.DrawString("F" + (i + 1).ToString() + ": " + name,
                        this.Font, (this.currentRenderer == renderer ? Brushes.Red : Brushes.White), 5, 40 + i * 15);
                }

                for (int i = 0; i < this.scenes.Count; i++)
                {
                    string name = this.scenes[i].Item1;
                    IntPtr scene = this.scenes[i].Item2;

                    e.Graphics.DrawString((i + 1).ToString() + ": " + name,
                        this.Font, (this.currentScene == scene ? Brushes.Blue : Brushes.White), 5, 45 + (this.renderers.Count + i) * 15);
                }

                e.Graphics.DrawString("A: Full Screen Anti-Alias",
                    this.Font, Brushes.Yellow, 5, 50 + (this.renderers.Count + this.scenes.Count) * 15);
            }
        }

        private void SimForm_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Control && !e.Shift & !e.Alt && e.KeyCode == Keys.ControlKey)
            {
                this.debuggerIntersectPixel = true;
                this.Cursor = Cursors.Cross;
            }
        }

        private void SimForm_KeyUp(object sender, KeyEventArgs e)
        {
            if (this.debuggerIntersectPixel)
            {
                this.debuggerIntersectPixel = false;
                this.Cursor = Cursors.SizeAll;
            }
            if (!e.Control && !e.Shift && !e.Alt)
            {
                int code = (int)e.KeyCode;
                if ((int)Keys.F1 <= code && code < (int)Keys.F1 + this.renderers.Count)
                {
                    this.currentRenderer = this.renderers[code - (int)Keys.F1].Item2;
                    Render();
                }
                else if ((int)Keys.D1 <= code && code < (int)Keys.D1 + this.scenes.Count)
                {
                    this.currentScene = this.scenes[code - (int)Keys.D1].Item2;
                    Render();
                }
                else if ((int)Keys.NumPad1 <= code && code < (int)Keys.NumPad1 + this.scenes.Count)
                {
                    this.currentScene = this.scenes[code - (int)Keys.NumPad1].Item2;
                    Render();
                }
                else
                {
                    switch (e.KeyCode)
                    {
                        case Keys.A:
                            if (this.currentRenderer != IntPtr.Zero)
                            {
                                SimulatorAPI.RenderScene(this.simulatorFSAA, this.currentScene, this.currentRenderer);
                                Bitmap bitmapFSAA = new Bitmap(this.ClientSize.Width, this.ClientSize.Height);
                                using (Graphics gFSAA = Graphics.FromImage(bitmapFSAA))
                                using (Image image = Image.FromHbitmap(SimulatorAPI.GetSimulatorBitmap(this.simulatorFSAA)))
                                {
                                    gFSAA.DrawImage(image, 0, 0);
                                }
                                new ResultForm(bitmapFSAA).Show();
                            }
                            break;
                    }
                }
            }
        }

        private void SimForm_MouseDown(object sender, MouseEventArgs e)
        {
            if (this.debuggerIntersectPixel)
            {
                int result = SimulatorAPI.DebuggerIntersect(this.simulator, this.currentScene, this.currentRenderer, e.X, e.Y);
                Color color = Color.FromArgb(result % 256, (result >> 8) % 256, (result >> 16));
                this.Text = string.Format("Vczh Simulator 3.0 ({0}, {1}) -> {2}", e.X, e.Y, color);
            }
            else
            {
                if (e.Button == MouseButtons.Left)
                {
                    this.rotating = true;
                    this.rotatingPosition = e.Location;
                }
            }
        }

        private void SimForm_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.debuggerIntersectPixel)
            {
            }
            else
            {
                if (this.rotating)
                {
                    double x = (double)(e.Location.X - this.rotatingPosition.X) / this.ClientSize.Width;
                    double y = (double)(e.Location.Y - this.rotatingPosition.Y) / this.ClientSize.Height;
                    SimulatorAPI.RotateScene(this.currentScene, x * 2 * Math.PI, y * 2 * Math.PI, 0);
                    this.rotatingPosition = e.Location;
                    Render();
                }
            }
        }

        private void SimForm_MouseUp(object sender, MouseEventArgs e)
        {
            if (this.debuggerIntersectPixel)
            {
            }
            else
            {
                this.rotating = false;
            }
        }
    }
}
