using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace _TranslateXMLtoCode
{
    class Program
    {

        static void Main(string[] args)
        {
            string xmlPath = args[0] + @"_GenPDB.xml";
            Console.WriteLine("analysing {0}", xmlPath);
            var document = XDocument.Load(xmlPath);
            var analyzerResult = TypeAnalyzer.Analyze(document);
        }
    }
}
