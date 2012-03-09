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
            if (args.Length == 2)
            {
                XDocument xmlPdb = XDocument.Load(args[0]);
                XDocument xmlDoc = XDocument.Load(args[1]);
                var udts = TypeDefinitions.LoadTypes(xmlPdb);
                var funcs = TypeDefinitions.LoadFunctions(udts, xmlPdb);
            }
            else
            {
                Console.WriteLine("Xml2Doc.exe <pdb-xml> <doc-xml>");
            }
        }
    }
}
