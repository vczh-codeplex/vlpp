using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using Developer.WinFormControls;
using Developer.LanguageServices.NativeX;
using Test.Host.LanguageForms.NativeX;

namespace Test.Host.LanguageForms
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            if (args.SequenceEqual(new string[] { }))
            {
                Application.Run(new NativeXForm());
            }
            else if (args.SequenceEqual(new string[] { "PlanText" }))
            {
                Application.Run(new PlanTextForm());
            }
            else if (args.SequenceEqual(new string[] { "NativeX" }))
            {
                Application.Run(new NativeXForm());
            }
        }
    }
}
