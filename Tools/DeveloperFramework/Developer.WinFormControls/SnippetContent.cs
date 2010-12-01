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

        public class Snippet : XElement
        {
            public Snippet(string name, params object[] contents)
                : base("snippet", new XAttribute("name", name), contents)
            {
            }
        }

        #endregion

        private XElement snippet = null;

        public SnippetContent(XElement snippet)
        {
            this.snippet = snippet;
            CheckContent(this.snippet);
        }

        public string Name
        {
            get
            {
                return this.snippet.Attribute("name").Value;
            }
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
                            if (element.Attributes().Count() != 1) throw new ArgumentException("Wrong schema.");
                            if (element.FirstAttribute.Name.LocalName != "name") throw new ArgumentException("Wrong schema.");
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
