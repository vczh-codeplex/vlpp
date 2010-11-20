using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UITesting;
using System.Windows.Input;
using Developer.WinFormControls.Core;
using System.Windows.Forms;
using Developer.LanguageProvider;

namespace Test.DeveloperComponent
{
    [TestClass]
    public class NativeXIntellisenseTest
    {
        private CodeFormWindow window;
        private int sendKeysDelay = 0;

        [TestInitialize]
        public void Initialize()
        {
            Playback.Initialize();
            sendKeysDelay = Keyboard.SendKeysDelay;
            Keyboard.SendKeysDelay = 60;
            window = new CodeFormWindow("NativeX", "NativeX Form");
        }

        [TestCleanup]
        public void Cleanup()
        {
            window.Close();
            Keyboard.SendKeysDelay = sendKeysDelay;
            Playback.Cleanup();
        }

        [TestMethod]
        public void InputCode()
        {
            window.Type(code);
        }

        private const string code =
            "unit program" + "\r\n" +
            "concept T:Eq" + "\r\n" +
            "{" + "\r\n" +
            "	Equals=function bool(T,T);" + "\r\n" +
            "	NotEquals=function bool(T,T);" + "\r\n" +
            "}" + "\r\n" +
            "" + "\r\n" +
            "function bool IntEquals(int a, int b) result=a==b;" + "\r\n" +
            "function bool IntNotEquals(int a, int b) result=a!=b;" + "\r\n" +
            "" + "\r\n" +
            "instance int:Eq" + "\r\n" +
            "{" + "\r\n" +
            "	Equals=IntEquals;" + "\r\n" +
            "	NotEquals=IntNotEquals;" + "\r\n" +
            "}" + "\r\n" +
            "" + "\r\n" +
            "generic<T>" + "\r\n" +
            "structure Vector" + "\r\n" +
            "{" + "\r\n" +
            "	T x;" + "\r\n" +
            "	T y;" + "\r\n" +
            "}" + "\r\n" +
            "" + "\r\n" +
            "generic<T> where T:Eq" + "\r\n" +
            "function bool VectorEquals(Vector<T> a, Vector<T> b)" + "\r\n" +
            "	result=Eq<T>::Equals(a.x,b.x)&&Eq<T>::Equals(a.y,b.y);" + "\r\n" +
            "" + "\r\n" +
            "generic<T> where T:Eq" + "\r\n" +
            "function bool VectorNotEquals(Vector<T> a, Vector<T> b)" + "\r\n" +
            "	result=!VectorEquals<T>(a,b);" + "\r\n" +
            "" + "\r\n" +
            "generic<T> where T:Eq" + "\r\n" +
            "instance Vector:Eq" + "\r\n" +
            "{" + "\r\n" +
            "	Equals=VectorEquals<T>;" + "\r\n" +
            "	NotEquals=VectorNotEquals<T>;" + "\r\n" +
            "}";
    }
}
