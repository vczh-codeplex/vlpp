using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Gate.BoardComponents
{
    class GateBoard
    {
        private List<GateBoardComponent> components = new List<GateBoardComponent>();
        private GateBoardComponent selectedComponent;

        public GateBoard()
        {
        }

        public GateBoardComponent SelectedComponent
        {
            get
            {
                return this.selectedComponent;
            }
            set
            {
                if (this.components.Contains(value))
                {
                    this.selectedComponent = value;
                }
                else
                {
                    this.selectedComponent = null;
                }
            }
        }

        public void AddComponent(GateBoardComponent component)
        {
            this.components.Add(component);
            this.selectedComponent = null;
        }

        public void RemoveComponent(GateBoardComponent component)
        {
            this.components.Remove(component);
            this.selectedComponent = null;
        }

        public void Clear()
        {
            this.components.Clear();
        }

        public bool IsConflict(Rectangle bounds)
        {
            bounds.Inflate(-1, -1);
            return this.components.Any(c => c.Bounds.IntersectsWith(bounds));
        }

        public GateBoardComponent GetComponent(Point position)
        {
            return this.components.Where(c => c.Bounds.Contains(position)).FirstOrDefault();
        }

        public void PaintSelectBox(Graphics g, Rectangle bounds, Color color)
        {
            bounds.Inflate(10, 10);
            using (Pen pen = new Pen(color, 3))
            {
                pen.DashStyle = DashStyle.DashDotDot;
                g.DrawRectangle(pen, bounds);
            }
        }

        public void Paint(Graphics g, Point offset)
        {
            foreach (var component in this.components)
            {
                Point[] inputs = component.Inputs;
                for (int i = 0; i < inputs.Length; i++)
                {
                    var input = component.GetInputAccess(i);
                    if (input != null)
                    {
                        Point ip = inputs[i] + new Size(offset);
                        Point op = input.Component.Outputs[input.PortIndex] + new Size(offset);
                        g.DrawLine(Pens.Purple, ip, op);
                    }
                }
            }
            foreach (var component in this.components)
            {
                component.Paint(g, offset, false);
            }
            if (this.selectedComponent != null)
            {
                Rectangle bounds = this.selectedComponent.Bounds;
                bounds.Location += new Size(offset);
                PaintSelectBox(g, bounds, Color.Blue);
            }
        }
    }

    class GateBoardAccess
    {
        public GateBoardComponent Component { get; set; }
        public int PortIndex { get; set; }
    }

    abstract class GateBoardComponent
    {
        private Dictionary<int, GateBoardAccess> inputAccesses = new Dictionary<int, GateBoardAccess>();
        private Dictionary<int, List<GateBoardAccess>> outputAccesses = new Dictionary<int, List<GateBoardAccess>>();

        public Point Position { get; set; }

        public abstract Point[] Inputs { get; }
        public abstract Point[] Outputs { get; }
        public abstract Rectangle Bounds { get; }
        public abstract void Paint(Graphics g, Point offset, bool alert);

        public GateBoardAccess GetInputAccess(int portIndex)
        {
            GateBoardAccess access = null;
            if (this.inputAccesses.TryGetValue(portIndex, out access))
            {
                return access;
            }
            else
            {
                return null;
            }
        }

        public GateBoardAccess[] GetOutputAccess(int portIndex)
        {
            List<GateBoardAccess> accesses = null;
            if (this.outputAccesses.TryGetValue(portIndex, out accesses))
            {
                return accesses.ToArray();
            }
            else
            {
                return new GateBoardAccess[0];
            }
        }

        public bool SetInputAccess(int portIndex, GateBoardComponent component, int outputPortIndex)
        {
            if (this.inputAccesses.ContainsKey(portIndex))
            {
                return false;
            }
            else
            {
                GateBoardAccess input = new GateBoardAccess
                {
                    Component = component,
                    PortIndex = outputPortIndex,
                };
                GateBoardAccess output = new GateBoardAccess
                {
                    Component = this,
                    PortIndex = portIndex,
                };
                this.inputAccesses.Add(portIndex, input);

                List<GateBoardAccess> outputs = null;
                if (!component.outputAccesses.TryGetValue(outputPortIndex, out outputs))
                {
                    outputs = new List<GateBoardAccess>();
                    component.outputAccesses.Add(outputPortIndex, outputs);
                }
                outputs.Add(output);
                return true;
            }
        }

        public void ResetInputAccess(int portIndex)
        {
            GateBoardAccess input = null;
            if (this.inputAccesses.TryGetValue(portIndex, out input))
            {
                List<GateBoardAccess> outputs = input.Component.outputAccesses[input.PortIndex];
                this.inputAccesses.Remove(portIndex);
                outputs.Remove(outputs.Where(o => o.Component == this && o.PortIndex == portIndex).First());
                if (outputs.Count == 0)
                {
                    input.Component.outputAccesses.Remove(input.PortIndex);
                }
            }
        }

        public void ClearInputAccess()
        {
            foreach (int i in this.inputAccesses.Keys.ToArray())
            {
                ResetInputAccess(i);
            }
        }

        public void ClearOutputAccess()
        {
            foreach (int i in this.outputAccesses.Keys.ToArray())
            {
                foreach (var access in this.outputAccesses[i].ToArray())
                {
                    access.Component.ResetInputAccess(access.PortIndex);
                }
            }
        }

        public void ClearAccess()
        {
            ClearInputAccess();
            ClearOutputAccess();
        }
    }
}
