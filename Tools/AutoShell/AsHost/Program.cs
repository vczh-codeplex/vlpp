using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using AutoShell.Services;
using System.Reflection;
using System.IO;

namespace AsHost
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Shell shell = new Shell();
            shell.LoadPackage(Assembly.LoadFrom("AutoShell.Document.dll"));
            shell.LoadPackage(Assembly.LoadFrom("AutoShell.TextEditor.dll"));
            shell.LoadPackage(Assembly.LoadFrom("AutoShell.CodeEditor.dll"));
            shell.LoadPackage(Assembly.LoadFrom("AsHost.exe"));

            IOptionService optionService = shell.Services.QueryService<IOptionService>();
            string folder = Path.GetDirectoryName(Application.ExecutablePath);
            if (!folder.EndsWith("\\"))
            {
                folder += "\\";
            }
            optionService.LoadOptions(folder + "Options.xml");

            shell.Initialize();
            Application.Run(shell.Context);
        }
    }
}