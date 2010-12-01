using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace Developer.WinFormControls
{
    public class SnippetContent
    {
        #region Snippet Builder

        public class Text : XCData
        {
            public Text(string value)
                : base(value)
            {
            }
        }

        public class EditableItem : XElement
        {
            public EditableItem(string name, string defaultValue)
                : base("editable", new XAttribute("name", name), new XAttribute("default", defaultValue))
            {
            }

            public EditableItem(string name)
                : base("editable", new XAttribute("name", name))
            {
            }
        }

        public class Tab : XElement
        {
            public Tab()
                : base("tab")
            {
            }
        }

        public class Stop : XElement
        {
            public Stop()
                : base("stop")
            {
            }
        }

        public class Snippet : XDocument
        {
            public Snippet(string name, params object[] contents)
                : base(new XElement("snippet", new XAttribute("name", name), contents))
            {
            }
        }

        #endregion

        #region Snippet Segment

        public enum SegmentType
        {
            Text,
            Editable,
            Stop
        }

        public class Segment
        {
            public SegmentType Type { get; set; }
            public Segment AssociatedSegment { get; set; }
            public string Value { get; set; }
        }

        #endregion

        private XDocument snippet = null;

        public SnippetContent(XDocument snippet)
        {
            this.snippet = snippet;
            CheckContent(this.snippet.Root);
        }

        public string Name
        {
            get
            {
                return this.snippet.Root.Attribute("name").Value;
            }
        }

        public List<List<Segment>> Compile(string tabPrefix)
        {
            List<List<Segment>> lines = new List<List<Segment>>();
            List<Segment> current = new List<Segment>();
            lines.Add(current);

            Dictionary<string, Segment> editables = new Dictionary<string, Segment>();

            foreach (XNode node in this.snippet.Root.Nodes())
            {
                XElement element = node as XElement;
                if (element != null)
                {
                    switch (element.Name.LocalName)
                    {
                        case "editable":
                            {
                                Segment segment = null;
                                string name = element.Attribute("name").Value;
                                if (editables.ContainsKey(name))
                                {
                                    segment = new Segment()
                                    {
                                        Type = SegmentType.Editable,
                                        AssociatedSegment = editables[name]
                                    };
                                }
                                else
                                {
                                    XAttribute defaultAttribute = element.Attribute("default");
                                    segment = new Segment()
                                    {
                                        Type = SegmentType.Editable,
                                        Value = defaultAttribute == null ? "" : defaultAttribute.Value
                                    };
                                    editables.Add(name, segment);
                                }
                                current.Add(segment);
                            }
                            break;
                        case "tab":
                            {
                                Segment segment = new Segment()
                                {
                                    Type = SegmentType.Text,
                                    Value = tabPrefix
                                };
                                current.Add(segment);
                            }
                            break;
                        case "stop":
                            {
                                Segment segment = new Segment()
                                {
                                    Type = SegmentType.Stop
                                };
                                current.Add(segment);
                            }
                            break;
                        default:
                            throw new ArgumentException("Wrong schema.");
                    }
                }
                else
                {
                    string[] textLines = ((XText)node).Value
                        .Split(new string[] { "\r\n" }, StringSplitOptions.None)
                        .SelectMany(s => s.Split(new string[] { "\r", "\n" }, StringSplitOptions.None))
                        .Select(s => string.IsNullOrEmpty(s) ? "" : s)
                        .ToArray();
                    for (int i = 0; i < textLines.Length; i++)
                    {
                        if (i > 0)
                        {
                            current = new List<Segment>();
                            lines.Add(current);
                        }
                        Segment segment = new Segment()
                        {
                            Type = SegmentType.Text,
                            Value = textLines[i]
                        };
                        current.Add(segment);
                    }
                }
            }
            return lines;
        }

        private void CheckContent(XElement element)
        {
            if (element.Name != "snippet") throw new ArgumentException("Wrong schema.");
            if (element.Attributes().Count() != 1) throw new ArgumentException("Wrong schema.");
            if (element.FirstAttribute.Name.LocalName != "name") throw new ArgumentException("Wrong schema.");
            foreach (var node in element.Nodes())
            {
                CheckSubContent(node);
            }
        }

        private void CheckSubContent(XNode node)
        {
            XElement element = node as XElement;
            if (element != null)
            {
                switch (element.Name.LocalName)
                {
                    case "editable":
                        {
                            switch (element.Attributes().Count())
                            {
                                case 1:
                                    {
                                        string[] attributes = element.Attributes().Select(a => a.Name.LocalName).ToArray();
                                        if (attributes[0] != "name") throw new ArgumentException("Wrong schema.");
                                    }
                                    break;
                                case 2:
                                    {
                                        string[] attributes = element.Attributes().Select(a => a.Name.LocalName).ToArray();
                                        if (attributes[0] != "name") throw new ArgumentException("Wrong schema.");
                                        if (attributes[1] != "default") throw new ArgumentException("Wrong schema.");
                                    }
                                    break;
                                default:
                                    throw new ArgumentException("Wrong schema.");
                            }
                            if (element.Nodes().Count() != 0)
                            {
                                throw new ArgumentException("Wrong schema.");
                            }
                        }
                        break;
                    case "tab":
                    case "stop":
                        {
                            if (element.Attributes().Count() != 0) throw new ArgumentException("Wrong schema.");
                            if (element.Nodes().Count() != 0)
                            {
                                throw new ArgumentException("Wrong schema.");
                            }
                        }
                        break;
                    default:
                        throw new ArgumentException("Wrong schema.");
                }
            }
            else
            {
                if (!(node is XText))
                {
                    throw new ArgumentException("Wrong schema.");
                }
            }
        }
    }
}
