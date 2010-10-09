using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using CodeBoxControl;

namespace CodeForm
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
                Application.Run(new CSharpForm());
            }
            else if (args.SequenceEqual(new string[] { "C#" }))
            {
                Application.Run(new CSharpForm());
            }
        }
    }
}
