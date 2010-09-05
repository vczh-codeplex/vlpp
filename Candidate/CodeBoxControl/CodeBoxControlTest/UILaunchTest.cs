using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using CodeBoxControl;
using System.Threading;
using System.Windows.Forms;
using Microsoft.VisualStudio.TestTools.UITesting.WinControls;
using Microsoft.VisualStudio.TestTools.UITesting;

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
        public void UITestHelperOpenFormOnce()
        {
            TextEditorBox editor = UITestHelper.OpenEditor(true);
            Thread.Sleep(5000);
            UITestHelper.CloseEditor(editor);
        }

        [TestMethod]
        public void UITestHelperOpenFormTwice()
        {
            ApplicationUnderTest application = ApplicationUnderTest.Launch("CodeForm.exe");
            CodeFormWindow window = new CodeFormWindow();
            application.Close();
        }
    }
}
