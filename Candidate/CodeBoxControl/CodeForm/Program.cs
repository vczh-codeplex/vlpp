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

            ITextEditorColorizer colorizer = null;
            if (args.Length == 0)
            {
                colorizer = new CSharpColorizer();
            }
            else if (args[0] == "normal")
            {
            }
            else if (args[0] == "C#")
            {
                colorizer = new CSharpColorizer();
            }

            CodeForm form = colorizer == null ? new CodeForm() : new CodeForm(colorizer);
            Application.Run(form);
        }
    }
}
