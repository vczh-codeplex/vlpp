using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ParserBuilder
{
    class Program
    {
        static void Main(string[] args)
        {
            CodeParserTestParserBuilder.Build();
            Console.WriteLine("Generated to Clipboard.");
            Console.ReadKey();
        }
    }
}
