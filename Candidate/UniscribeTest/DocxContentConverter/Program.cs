using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace DocxContentConverter
{
    class Program
    {
        class FontInfo
        {
            public string Name { get; set; }
            public bool Bold { get; set; }
            public string Color { get; set; }
            public string Size { get; set; }

            public override string ToString()
            {
                return Name + "#" + Bold + "#" + Color + "#" + Size;
            }
        }

        static IEnumerable<XElement> RecursiveElements(XElement parent, XName name)
        {
            foreach (var e in parent.Elements())
            {
                if (e.Name == name)
                {
                    yield return e;
                }
                foreach (var se in RecursiveElements(e, name))
                {
                    yield return se;
                }
            }
        }

        static void Main(string[] args)
        {
            XDocument document = XDocument.Load("content.xml");
            string fo = "urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0";
            string office = "urn:oasis:names:tc:opendocument:xmlns:office:1.0";
            string style = "urn:oasis:names:tc:opendocument:xmlns:style:1.0";
            string text = "urn:oasis:names:tc:opendocument:xmlns:text:1.0";

            var styles = document
                .Root
                .Element(XName.Get("automatic-styles", office))
                .Elements(XName.Get("style", style))
                .Select(s =>
                    Tuple.Create(
                        s.Attribute(XName.Get("name", style)).Value,
                        s.Element(XName.Get("text-properties", style))
                        )
                    )
                .Where(t => t.Item2 != null)
                .ToDictionary(
                    t => t.Item1,
                    t => new FontInfo
                    {
                        Name = t.Item2.Attribute(XName.Get("font-name", style)).Value,
                        Bold = t.Item2.Attribute(XName.Get("font-weight", fo)) != null && t.Item2.Attribute(XName.Get("font-weight", fo)).Value == "bold",
                        Color = t.Item2.Attribute(XName.Get("color", fo)) != null ? t.Item2.Attribute(XName.Get("color", fo)).Value : "#000000",
                        Size = t.Item2.Attribute(XName.Get("font-size", fo)).Value,
                    });

            var paragraphs = document
                .Root
                .Element(XName.Get("body", office))
                .Element(XName.Get("text", office))
                .Elements(XName.Get("p", text))
                .ToArray();

            var spans = paragraphs
                .Select(p =>
                    Tuple.Create(
                        p.Attribute(XName.Get("style-name", text)).Value,
                        RecursiveElements(p, XName.Get("span", text))
                            .Select(s =>
                                Tuple.Create(
                                    s.Attribute(XName.Get("style-name", text)).Value,
                                    s.Value
                                    )
                                )
                            .ToArray()
                        )
                    )
                .ToArray();

            XDocument output = new XDocument();
            output.Add(
                new XElement("document",
                    spans.Select(p =>
                        new XElement(
                            "p",
                            p.Item2
                                .Aggregate(new Tuple<string, string>[] { }, (a, b) =>
                                    {
                                        if (a.Length == 0)
                                        {
                                            return new Tuple<string, string>[] { b };
                                        }
                                        else
                                        {
                                            var last = a.Last();
                                            var fontInfo1 = styles[last.Item1];
                                            var fontInfo2 = styles[b.Item1];
                                            if (fontInfo1.ToString() == fontInfo2.ToString())
                                            {
                                                a[a.Length - 1] = Tuple.Create(last.Item1, last.Item2 + b.Item2);
                                                return a;
                                            }
                                            else
                                            {
                                                return a.Concat(new Tuple<string, string>[] { b }).ToArray();
                                            }
                                        }
                                    })
                                .Select(s =>
                                    {
                                        var fontInfo = styles[s.Item1];
                                        return new XElement(
                                            "s",
                                            new XAttribute("font", fontInfo.Name),
                                            new XAttribute("bold", fontInfo.Bold),
                                            new XAttribute("color", fontInfo.Color),
                                            new XAttribute("size", fontInfo.Size),
                                            s.Item2
                                            );
                                    })
                            )
                        )
                    )
                );
            output.Save("document.xml");

            using (StreamWriter writer = new StreamWriter("document.txt"))
            {
                foreach (var p in output.Root.Elements("p"))
                {
                    foreach (var s in p.Elements("s"))
                    {
                        writer.WriteLine(
                            "<s>{0}:{1}:{2}:{3}:{4}</s>",
                            s.Attribute("font").Value,
                            s.Attribute("bold").Value,
                            s.Attribute("color").Value,
                            s.Attribute("size").Value,
                            s.Value
                            );
                    }
                    writer.WriteLine("<p/>");
                }
            }
        }
    }
}
