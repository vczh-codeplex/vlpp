using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.LanguageServices.NativeX;
using System.Windows.Forms;

namespace Tools.ParserGenerator
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Console.WriteLine("1: With Implementation.");
            Console.WriteLine("2: Without Implementation.");
            Console.Write(">");
            string input = Console.ReadLine();
            switch (input)
            {
                case "1":
                    Clipboard.SetText(NativeXCodeParserGrammar.Build(true));
                    break;
                case "2":
                    Clipboard.SetText(NativeXCodeParserGrammar.Build(false));
                    break;
                default:
                    Console.WriteLine("You should enter 1 or 2.");
                    return;
            }
            Console.WriteLine("Generated to Clipboard.");
            Console.ReadKey();
        }
    }
}
