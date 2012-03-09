using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace Xml2Doc
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 3)
            {
                Console.WriteLine("Parsing XMLs...");
                XDocument xmlPdb = XDocument.Load(args[0]);
                XDocument xmlDoc = XDocument.Load(args[1]);

                Console.WriteLine("Loading metadata from XMLs...");
                var udts = TypeDefinitions.LoadTypes(xmlPdb);
                var funcs = TypeDefinitions.LoadFunctions(udts, xmlPdb);
                var docItems = xmlDoc
                    .Root
                    .Elements("members")
                    .SelectMany(e => e.Elements("member"))
                    .ToDictionary(e => e.Attribute("name").Value, e => e);

                Console.WriteLine("Matching document items with pdb symbols...");
                var matchedDocItems = docItems
                    .ToDictionary(p => p.Key, p => Tuple.Create(p.Value, DocItemProvider.FindSymbol(p.Value, udts, funcs)));

                Console.WriteLine("Writing merged xml documentation...");
            }
            else
            {
                Console.WriteLine("Xml2Doc.exe <pdb-xml> <doc-xml>");
            }
        }
    }
}
