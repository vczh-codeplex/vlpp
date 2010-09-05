using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UITesting;
using System.Windows.Input;
using CodeBoxControlTest.TextEditorServiceReference;

namespace CodeBoxControlTest
{
    [TestClass]
    public class UILaunchTest
    {
        private ApplicationUnderTest application;
        private CodeFormWindow window;
        private TextEditorServiceClient service;

        [TestInitialize]
        public void Initialize()
        {
            Playback.Initialize();
            application = ApplicationUnderTest.Launch("CodeForm.exe");
            window = new CodeFormWindow();
            service = new TextEditorServiceClient();
            service.Open();
        }

        [TestCleanup]
        public void Cleanup()
        {
            service.Close();
            application.Close();
            Playback.Cleanup();
        }

        [TestMethod]
        public void UITestHelperOpenForm()
        {
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

            Assert.AreEqual("TextEditorService", service.GetName());
        }
    }
}
