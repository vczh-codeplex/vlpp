using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Runtime.Serialization;
using System.IO;
using System.Xml.Linq;

namespace TokenizerBuilder.Shape
{
    public abstract class ShapeBase
    {
        public ShapeManager Manager { get; internal set; }
        public abstract int Priority { get; }
        public abstract Rectangle SelectionBounds { get; }
        public abstract bool IsInShape(Graphics g, Point p);
        public abstract void Draw(Graphics g, Font font, Size offset);
        public abstract void Move(Size size);
    }

    public class ShapeManager
    {
        public SortedList<int, List<ShapeBase>> Shapes { get; private set; }

        public ShapeManager()
        {
            this.Shapes = new SortedList<int, List<ShapeBase>>();
        }

        public void AddShape(ShapeBase shape)
        {
            shape.Manager = this;
            if (!this.Shapes.ContainsKey(shape.Priority))
            {
                this.Shapes.Add(shape.Priority, new List<ShapeBase>());
            }
            this.Shapes[shape.Priority].Add(shape);
        }

        public void RemoveShape(ShapeBase shape)
        {
            shape.Manager = null;
            this.Shapes[shape.Priority].Remove(shape);
        }

        public ShapeBase Pick(Graphics g, Point p)
        {
            foreach (var pair in this.Shapes)
            {
                foreach (var shape in pair.Value.Reverse<ShapeBase>())
                {
                    if (shape.IsInShape(g, p))
                    {
                        return shape;
                    }
                }
            }
            return null;
        }

        public void Draw(Graphics g, Font font, Size offset)
        {
            foreach (var pair in this.Shapes.Reverse())
            {
                foreach (var shape in pair.Value)
                {
                    shape.Draw(g, font, offset);
                }
            }
        }

        public void Move(Size offset)
        {
            foreach (var pair in this.Shapes)
            {
                foreach (var shape in pair.Value)
                {
                    shape.Move(offset);
                }
            }
        }

        public void SaveToFile(string filename)
        {
            XDocument document = new XDocument(
                new XElement("ShapeManager",
                    this.Shapes.Values.SelectMany(v => v).Select(s =>
                    {
                        StateShape state = s as StateShape;
                        ArrowShape arrow = s as ArrowShape;
                        if (state != null)
                        {
                            return new XElement("StateShape",
                                new XAttribute("Name", state.Name),
                                new XAttribute("Color", state.Color),
                                new XAttribute("Token", state.Token),
                                new XAttribute("X", state.Position.X),
                                new XAttribute("Y", state.Position.Y),
                                new XAttribute("R", state.Radius),
                                new XAttribute("Type", state.Type)
                                );
                        }
                        else
                        {
                            return new XElement("ArrowShape",
                                new XAttribute("Name", arrow.Name),
                                new XAttribute("X", arrow.ControlPoint.X),
                                new XAttribute("Y", arrow.ControlPoint.Y),
                                new XAttribute("R", arrow.Radius),
                                new XAttribute("Start", this.Shapes[StateShape.StateShapeProprity].IndexOf(arrow.Start)),
                                new XAttribute("End", this.Shapes[StateShape.StateShapeProprity].IndexOf(arrow.End))
                                );
                        }
                    })
                    )
                );
            document.Save(filename);
        }

        public static ShapeManager LoadFromFile(string filename)
        {
            XDocument document = XDocument.Load(filename);
            ShapeManager manager = new ShapeManager();
            foreach (XElement shapeElement in document.Root.Elements())
            {
                if (shapeElement.Name == "StateShape")
                {
                    StateShape state = new StateShape();
                    state.Name = shapeElement.Attribute("Name").Value;
                    state.Color = shapeElement.Attribute("Color").Value;
                    state.Token = shapeElement.Attribute("Token").Value;
                    state.Position = new Point(int.Parse(shapeElement.Attribute("X").Value), int.Parse(shapeElement.Attribute("Y").Value));
                    state.Radius = int.Parse(shapeElement.Attribute("R").Value);
                    state.Type = (StateType)Enum.Parse(typeof(StateType), shapeElement.Attribute("Type").Value);

                    manager.AddShape(state);
                }
                else if (shapeElement.Name == "ArrowShape")
                {
                    ArrowShape arrow = new ArrowShape();
                    arrow.Name = shapeElement.Attribute("Name").Value;
                    arrow.ControlPoint = new Point(int.Parse(shapeElement.Attribute("X").Value), int.Parse(shapeElement.Attribute("Y").Value));
                    arrow.Radius = int.Parse(shapeElement.Attribute("R").Value);
                    arrow.Start = (StateShape)manager.Shapes[StateShape.StateShapeProprity][int.Parse(shapeElement.Attribute("Start").Value)];
                    arrow.End = (StateShape)manager.Shapes[StateShape.StateShapeProprity][int.Parse(shapeElement.Attribute("End").Value)];

                    arrow.Start.OutArrows.Add(arrow);
                    arrow.End.InArrows.Add(arrow);
                    manager.AddShape(arrow);
                }
            }
            return manager;
        }
    }
}
