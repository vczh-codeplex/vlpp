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
            if (parent.Name == name)
            {
                yield return parent;
                yield break;
            }
            foreach (var e in parent.Elements())
            {
                if (e.Name == name)
                {
                    yield return e;
                }
                else
                {
                    foreach (var se in RecursiveElements(e, name))
                    {
                        yield return se;
                    }
                }
            }
        }

        const string fo = "urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0";
        const string office = "urn:oasis:names:tc:opendocument:xmlns:office:1.0";
        const string style = "urn:oasis:names:tc:opendocument:xmlns:style:1.0";
        const string table = "urn:oasis:names:tc:opendocument:xmlns:table:1.0";
        const string text = "urn:oasis:names:tc:opendocument:xmlns:text:1.0";

        static Tuple<string, string, string> GetTextItem(string paragraphStyle, string textStyle, string text)
        {
            if (paragraphStyle == null || textStyle == null) throw new ArgumentException();
            return Tuple.Create(paragraphStyle, textStyle, text);
        }

        static IEnumerable<Tuple<string, string, string>> ExtractText(XElement paragraphParent, string paragraphStyle, string textStyle, int listIndex)
        {
            foreach (XNode node in paragraphParent.Nodes())
            {
                if (node is XElement)
                {
                    XElement element = node as XElement;
                    switch (element.Name.ToString())
                    {
                        case "{" + text + "}p":
                            {
                                var styleName = element.Attribute(XName.Get("style-name", text)).Value;
                                foreach (var p in ExtractText(element, styleName, styleName, listIndex))
                                {
                                    yield return p;
                                }
                            }
                            break;
                        case "{" + text + "}span":
                            {
                                var styleName = element.Attribute(XName.Get("style-name", text)).Value;
                                foreach (var p in ExtractText(element, paragraphStyle, styleName, listIndex))
                                {
                                    yield return p;
                                }
                            }
                            break;
                        case "{" + text + "}line-break":
                            yield return GetTextItem(paragraphStyle, textStyle, "\r\n");
                            break;
                        case "{" + text + "}s":
                            yield return GetTextItem(paragraphStyle, textStyle, " ");
                            break;
                        case "{" + text + "}tab":
                            yield return GetTextItem(paragraphStyle, textStyle, "\t");
                            break;
                        case "{" + text + "}a":
                        case "{" + table + "}table":
                        case "{" + table + "}table-row":
                        case "{" + table + "}table-cell":
                            foreach (var p in ExtractText(element, paragraphStyle, textStyle, listIndex))
                            {
                                yield return p;
                            }
                            break;
                        case "{" + text + "}list":
                            {
                                listIndex = 0;
                                foreach (XElement item in element.Elements(XName.Get("list-item", text)))
                                {
                                    foreach (var p in ExtractText(item, paragraphStyle, textStyle, ++listIndex))
                                    {
                                        yield return p;
                                    }
                                }
                            }
                            break;
                        case "{" + text + "}list-item":
                            listIndex += 1;
                            foreach (var p in ExtractText(element, paragraphStyle, textStyle, listIndex))
                            {
                                yield return p;
                            }
                            break;
                        case "{urn:oasis:names:tc:opendocument:xmlns:drawing:1.0}custom-shape":
                        case "{urn:oasis:names:tc:opendocument:xmlns:drawing:1.0}frame":
                        case "{" + text + "}soft-page-break":
                        case "{" + text + "}bookmark-start":
                        case "{" + text + "}bookmark-end":
                        case "{" + table + "}table-columns":
                            break;
                        default:
                            throw new ArgumentException();
                    }
                }
                else if (node is XText)
                {
                    yield return GetTextItem(paragraphStyle, textStyle, node.ToString().Trim());
                }
                else
                {
                    throw new ArgumentException();
                }
            }
        }

        static void Convert(string from, string to)
        {
            XDocument document = XDocument.Load(from);

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

            var paragraphParent = document
                .Root
                .Element(XName.Get("body", office))
                .Element(XName.Get("text", office));

            var spans = ExtractText(paragraphParent, null, null, 0)
                .GroupBy(s => s.Item1)
                .Select(g =>
                    Tuple.Create(
                        g.Key,
                        g.Select(t => Tuple.Create(t.Item2, t.Item3)).ToArray()
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

            using (StreamWriter writer = new StreamWriter(to))
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

        static void Main(string[] args)
        {
            Convert("content.xml", "document.txt");
            Convert("content2.xml", "document2.txt");
        }
    }
}
