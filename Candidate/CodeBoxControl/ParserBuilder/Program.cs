using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CodeForm.NativeX;

namespace ParserBuilder
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
