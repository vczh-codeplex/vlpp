using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace VlTurtle
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
            if (args.SequenceEqual(new string[] { "Execute" }))
            {
                Application.Run(new TurtleForm());
            }
            else
            {
                Application.Run(new TurtleIdeForm());
            }
        }
    }
}
