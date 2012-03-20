using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.IO;

namespace DocRef
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 2)
            {
                XDocument docXml = XDocument.Load(args[0]);
                string outputFolder = Path.GetFullPath(args[1]) + "\\";

                var docRoot = DocItemAnalyzer.GetItems(docXml.Root);
                Dictionary<string, DocItem> uniqueIdItemMap = new Dictionary<string, DocItem>();
                docRoot.BuildIndex(uniqueIdItemMap);
            }
            else
            {
                Console.WriteLine("DocRef.exe <doc-xml> <out-folder>");
            }
        }
    }
}
