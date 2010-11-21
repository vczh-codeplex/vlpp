using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace Developer.WinFormControls
{
    public class RichContent
    {
        public class Text : XCData
        {
            public Text(string value)
                : base(value)
            {
            }
        }

        public class Bold : XElement
        {
            public Bold(params object[] contents)
                : base("b", contents)
            {
            }
        }

        public class Italic : XElement
        {
            public Italic(params object[] contents)
                : base("i", contents)
            {
            }
        }

        public class Underline : XElement
        {
            public Underline(params object[] contents)
                : base("u", contents)
            {
            }
        }

        public class Content : XDocument
        {
            public Content(params object[] contents)
                : base(new XElement("content", contents))
            {
            }
        }

        private XDocument content = null;

        public RichContent(XDocument content)
        {
            CheckContent(content.Root);
            this.content = content;
        }

        private void CheckContent(XElement element)
        {
            if (element.Name != "content") throw new ArgumentException("Wrong schema.");
            if (element.Attributes().Count() != 0) throw new ArgumentException("Wrong schema.");
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
                if (element.Name != "b" && element.Name != "i" && element.Name != "u") throw new ArgumentException("Wrong schema.");
                if (element.Attributes().Count() != 0) throw new ArgumentException("Wrong schema.");
            }
            else
            {
                if (!(node is XCData || node is XText))
                {
                    throw new ArgumentException("Wrong schema.");
                }
            }
        }
    }
}
