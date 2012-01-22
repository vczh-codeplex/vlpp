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
            var input = new ReflectedTypeAnalyzerInput
            {
                DescriptableUdts = analyzerResult.DescriptableUdts.ToDictionary(t => t.Name),
                ExportableClasses = analyzerResult.ExportableClasses.ToDictionary(t => t.Name),
                ExportableEnums = analyzerResult.ExportableEnums.ToDictionary(t => t.Name),
                ExportableStructs = analyzerResult.ExportableStructs.ToDictionary(t => t.Name),
                AvailableUdts = analyzerResult.AvailableUdts.ToDictionary(t => t.Name),
            };
            var exportingResult = ReflectedTypeAnalyzer.Analyze(input);

            string analysysPath = xmlPath + ".analysis.xml";
            Console.WriteLine("exporting analysis result to {0}", analysysPath);
            exportingResult.Export().Save(analysysPath);

            Console.WriteLine("generating code");
        }
    }
}
