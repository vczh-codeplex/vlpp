using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UITesting;
using System.Windows.Input;

namespace CodeBoxControlTest
{
    [TestClass]
    public class UILaunchTest
    {
        [TestInitialize]
        public void Initialize()
        {
            Playback.Initialize();
        }

        [TestCleanup]
        public void Cleanup()
        {
            Playback.Cleanup();
        }

        [TestMethod]
        public void UITestHelperOpenForm()
        {
            ApplicationUnderTest application = ApplicationUnderTest.Launch("CodeForm.exe");
            CodeFormWindow window = new CodeFormWindow();

            window.TypeAndEnter("using System;");
            window.TypeAndEnter("");
            window.TypeAndEnter("namespace Program");
            window.TypeAndEnter("{");
            window.TypeAndEnter("    public class Program");
            window.TypeAndEnter("    {");
            window.TypeAndEnter("        public void Main(string[] args)");
            window.TypeAndEnter("        {");
            window.TypeAndEnter("            Console.WriteLine(\"Hello world!\");");
            window.TypeAndEnter("        }");
            window.TypeAndEnter("    }");
            window.TypeAndEnter("}");

            window.Wait(1000);
            application.Close();
        }
    }
}
