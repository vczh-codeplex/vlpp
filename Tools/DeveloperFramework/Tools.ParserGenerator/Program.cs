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
            Clipboard.SetText(NativeXCodeParserGrammar.Build());
            Console.WriteLine("Generated to Clipboard.");
            Console.ReadKey();
        }
    }
}
