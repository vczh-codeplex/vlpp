using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using _TranslateXMLtoCode.Codegen;

namespace _TranslateXMLtoCode
{
    /// <summary>
    /// This comment is check-ed at the first minute of the Spring Festival of China!
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {
            // analyze _GenPDB.xml
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

            // expoort analyzing result
            string analysysPath = xmlPath + ".analysis.xml";
            Console.WriteLine("exporting analysis result to {0}", analysysPath);
            exportingResult.Export().Save(analysysPath);

            // configurate to generate code
            Console.WriteLine("generating code");
            CodeGeneratorOptions options = new CodeGeneratorOptions
            {
                ReflectionPath = args[0] + @"..\..\..\..\Candidate\GUI\GUI\Reflection\",
                GacuiPath = args[0] + @"GacUI\",
                Udts = exportingResult.Udts,
            };
            Console.WriteLine("reflection folder = {0}", options.ReflectionPath);
            Console.WriteLine("gacui folder = {0}", options.GacuiPath);

            // prepare code generators
            List<CodeGenerator> generators = new List<CodeGenerator>();
            generators.Add(new Codegen_GuiTypeDescriptorImpProvider_codegen_h(options));
            generators.Add(new Codegen_GuiTypeDescriptorImpProvider_codegen_cpp(options));

            // generate code
            foreach (var codegen in generators)
            {
                Console.WriteLine("Generating {0}", codegen.FileName);
                if (codegen.CanWrite)
                {
                    codegen.GenerateCode();
                    Console.WriteLine("    Succeeded!");
                }
                else
                {
                    Console.WriteLine("    Failed: {0}", codegen.Error.Message);
                }
            }
            foreach (var codegen in generators)
            {
                codegen.Dispose();
            }
        }
    }
}
