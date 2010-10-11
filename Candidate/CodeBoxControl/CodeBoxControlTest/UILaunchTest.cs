using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UITesting;
using System.Windows.Input;
using CodeBoxControlTest.TextEditorServiceReference;
using CodeBoxControl.Core;
using System.Windows.Forms;

namespace CodeBoxControlTest
{
    [TestClass]
    public class UILaunchTest
    {
        private CodeFormWindow window;

        [TestInitialize]
        public void Initialize()
        {
            Playback.Initialize();
            window = new CodeFormWindow("", "NativeX Form");
        }

        [TestCleanup]
        public void Cleanup()
        {
            window.Close();
            Playback.Cleanup();
        }

        private void AssertController(string a, string b, string c)
        {
            TextPosition p0 = new TextPosition(0, 0);
            TextPosition p1 = window.SelectionStart;
            TextPosition p2 = window.SelectionEnd;
            TextPosition p3 = new TextPosition(window.LineCount - 1, window.GetLineLength(window.LineCount - 1));
            Assert.AreEqual(a, window.GetString(p0, p1));
            Assert.AreEqual(b, window.GetString(p1, p2));
            Assert.AreEqual(c, window.GetString(p2, p3));
        }

        private void AssertTextPosition(TextPosition a, TextPosition b)
        {
            Assert.AreEqual(a, b);
        }

        [TestMethod()]
        public void InputTest()
        {
            AssertController("", "", "");
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            AssertTextPosition(new TextPosition(3, 6), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(3, 6), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", "", "");

            this.window.Move(new TextPosition(1, 4), false, false);
            this.window.Move(new TextPosition(2, 3), false, true);
            AssertTextPosition(new TextPosition(1, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJK", "LMN\r\nOPQ", "RST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 3), false, false);
            this.window.Move(new TextPosition(1, 4), false, true);
            AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(1, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJK", "LMN\r\nOPQ", "RST\r\nUVWXYZ");

            this.window.Input("...");
            AssertTextPosition(new TextPosition(1, 7), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJK...", "", "RST\r\nUVWXYZ");
        }

        [TestMethod()]
        public void PressUpTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            {
                this.window.Move(new TextPosition(1, 3), false, false);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressUp(false, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressUp(false, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(1, 3), false, false);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressUp(true, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressUp(true, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(1, 3), false, false);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressUp(false, true);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressUp(false, true);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(1, 3), false, false);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressUp(true, true);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressUp(true, true);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ");
            }
        }

        [TestMethod]
        public void PressDownTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressDown(false, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.window.PressDown(false, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressDown(true, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.window.PressDown(true, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressDown(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVW", "XYZ");

                this.window.PressDown(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVW", "XYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressDown(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVW", "XYZ");

                this.window.PressDown(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVW", "XYZ");
            }
        }

        [TestMethod]
        public void PressLeftTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressLeft(false, false);
                AssertTextPosition(new TextPosition(2, 2), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 2), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOP", "", "QRST\r\nUVWXYZ");

                this.window.PressLeft(false, false);
                AssertTextPosition(new TextPosition(2, 1), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 1), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "", "PQRST\r\nUVWXYZ");

                this.window.PressLeft(false, false);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "OPQRST\r\nUVWXYZ");

                this.window.PressLeft(false, false);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "", "\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressLeft(true, false);
                AssertTextPosition(new TextPosition(2, 2), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 2), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOP", "", "QRST\r\nUVWXYZ");

                this.window.PressLeft(true, false);
                AssertTextPosition(new TextPosition(2, 1), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 1), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "", "PQRST\r\nUVWXYZ");

                this.window.PressLeft(true, false);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "OPQRST\r\nUVWXYZ");

                this.window.PressLeft(true, false);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "", "\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressLeft(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 2), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOP", "Q", "RST\r\nUVWXYZ");

                this.window.PressLeft(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 1), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "PQ", "RST\r\nUVWXYZ");

                this.window.PressLeft(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "OPQ", "RST\r\nUVWXYZ");

                this.window.PressLeft(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "\r\nOPQ", "RST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressLeft(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 2), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOP", "Q", "RST\r\nUVWXYZ");

                this.window.PressLeft(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 1), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "PQ", "RST\r\nUVWXYZ");

                this.window.PressLeft(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "OPQ", "RST\r\nUVWXYZ");

                this.window.PressLeft(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "\r\nOPQ", "RST\r\nUVWXYZ");
            }
        }

        [TestMethod]
        public void PressRightTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressRight(false, false);
                AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

                this.window.PressRight(false, false);
                AssertTextPosition(new TextPosition(2, 5), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 5), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRS", "", "T\r\nUVWXYZ");

                this.window.PressRight(false, false);
                AssertTextPosition(new TextPosition(2, 6), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 6), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST", "", "\r\nUVWXYZ");

                this.window.PressRight(false, false);
                AssertTextPosition(new TextPosition(3, 0), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\n", "", "UVWXYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressRight(true, false);
                AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

                this.window.PressRight(true, false);
                AssertTextPosition(new TextPosition(2, 5), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 5), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRS", "", "T\r\nUVWXYZ");

                this.window.PressRight(true, false);
                AssertTextPosition(new TextPosition(2, 6), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 6), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST", "", "\r\nUVWXYZ");

                this.window.PressRight(true, false);
                AssertTextPosition(new TextPosition(3, 0), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\n", "", "UVWXYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressRight(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "R", "ST\r\nUVWXYZ");

                this.window.PressRight(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 5), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RS", "T\r\nUVWXYZ");

                this.window.PressRight(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 6), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST", "\r\nUVWXYZ");

                this.window.PressRight(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\n", "UVWXYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressRight(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "R", "ST\r\nUVWXYZ");

                this.window.PressRight(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 5), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RS", "T\r\nUVWXYZ");

                this.window.PressRight(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 6), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST", "\r\nUVWXYZ");

                this.window.PressRight(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\n", "UVWXYZ");
            }
        }

        [TestMethod]
        public void PressHomeTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressHome(false, false);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "OPQRST\r\nUVWXYZ");

                this.window.PressHome(false, false);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "OPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressHome(true, false);
                AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
                AssertController("", "", "ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressHome(true, false);
                AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
                AssertController("", "", "ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressHome(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "OPQ", "RST\r\nUVWXYZ");

                this.window.PressHome(false, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "OPQ", "RST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(2, 3), false, false);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.window.PressHome(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
                AssertController("", "ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVWXYZ");

                this.window.PressHome(true, true);
                AssertTextPosition(new TextPosition(2, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
                AssertController("", "ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVWXYZ");
            }
        }

        [TestMethod]
        public void PressEndTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            {
                this.window.Move(new TextPosition(1, 3), false, false);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressEnd(false, false);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "", "\r\nOPQRST\r\nUVWXYZ");

                this.window.PressEnd(false, false);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "", "\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(1, 3), false, false);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressEnd(true, false);
                AssertTextPosition(new TextPosition(3, 6), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 6), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", "", "");

                this.window.PressEnd(true, false);
                AssertTextPosition(new TextPosition(3, 6), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 6), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", "", "");
            }
            {
                this.window.Move(new TextPosition(1, 3), false, false);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressEnd(false, true);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "KLMN", "\r\nOPQRST\r\nUVWXYZ");

                this.window.PressEnd(false, true);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "KLMN", "\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(1, 3), false, false);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressEnd(true, true);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 6), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ", "");

                this.window.PressEnd(true, true);
                AssertTextPosition(new TextPosition(1, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 6), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ", "");
            }
        }

        [TestMethod]
        public void PressPageUpTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            {
                this.window.Move(new TextPosition(3, 3), false, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.window.PressPageUp(false, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressPageUp(false, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(3, 3), false, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.window.PressPageUp(true, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressPageUp(true, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.window.Move(new TextPosition(3, 3), false, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.window.PressPageUp(false, true);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");

                this.window.PressPageUp(false, true);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");
            }
            {
                this.window.Move(new TextPosition(3, 3), false, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.window.PressPageUp(true, true);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");

                this.window.PressPageUp(true, true);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");
            }
        }

        [TestMethod]
        public void PressPageDownTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            {
                this.window.Move(new TextPosition(0, 3), false, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressPageDown(false, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.window.PressPageDown(false, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");
            }
            {
                this.window.Move(new TextPosition(0, 3), false, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressPageDown(true, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.window.PressPageDown(true, false);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");
            }
            {
                this.window.Move(new TextPosition(0, 3), false, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressPageDown(false, true);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");

                this.window.PressPageDown(false, true);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");
            }
            {
                this.window.Move(new TextPosition(0, 3), false, false);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.window.PressPageDown(true, true);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");

                this.window.PressPageDown(true, true);
                AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
                AssertTextPosition(new TextPosition(3, 3), this.window.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");
            }
        }

        [TestMethod]
        public void PressDeleteNormalTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 4), false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.window.PressDelete(false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "T\r\nUVWXYZ");

            this.window.PressDelete(false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "\r\nUVWXYZ");

            this.window.PressDelete(false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "UVWXYZ");

            this.window.Move(new TextPosition(2, 10), false, false);
            AssertTextPosition(new TextPosition(2, 10), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 10), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRUVWXYZ", "", "");

            this.window.PressDelete(false, false);
            AssertTextPosition(new TextPosition(2, 10), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 10), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRUVWXYZ", "", "");
        }

        [TestMethod]
        public void PressDeleteControlTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 4), false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.window.PressDelete(true, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "T\r\nUVWXYZ");

            this.window.PressDelete(true, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "\r\nUVWXYZ");

            this.window.PressDelete(true, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "UVWXYZ");

            this.window.Move(new TextPosition(2, 10), false, false);
            AssertTextPosition(new TextPosition(2, 10), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 10), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRUVWXYZ", "", "");

            this.window.PressDelete(true, false);
            AssertTextPosition(new TextPosition(2, 10), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 10), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRUVWXYZ", "", "");
        }

        [TestMethod]
        public void PressDeleteShiftTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 4), false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.window.PressDelete(false, true);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "UVWXYZ");

            this.window.Move(new TextPosition(2, 6), false, false);
            AssertTextPosition(new TextPosition(2, 6), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 6), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nUVWXYZ", "", "");

            this.window.PressDelete(false, true);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "");
        }

        [TestMethod]
        public void PressDeleteControlShiftTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 4), false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.window.PressDelete(true, true);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "UVWXYZ");

            this.window.Move(new TextPosition(2, 6), false, false);
            AssertTextPosition(new TextPosition(2, 6), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 6), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nUVWXYZ", "", "");

            this.window.PressDelete(true, true);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "");
        }

        [TestMethod]
        public void PressBackspaceNormalTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 2), false, false);
            AssertTextPosition(new TextPosition(2, 2), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 2), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOP", "", "QRST\r\nUVWXYZ");

            this.window.PressBackspace(false, false);
            AssertTextPosition(new TextPosition(2, 1), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 1), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "", "QRST\r\nUVWXYZ");

            this.window.PressBackspace(false, false);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "QRST\r\nUVWXYZ");

            this.window.PressBackspace(false, false);
            AssertTextPosition(new TextPosition(1, 7), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN", "", "QRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(0, 0), false, false);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMNQRST\r\nUVWXYZ");

            this.window.PressBackspace(false, false);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMNQRST\r\nUVWXYZ");
        }

        [TestMethod]
        public void PressBackspaceControlTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 2), false, false);
            AssertTextPosition(new TextPosition(2, 2), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 2), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOP", "", "QRST\r\nUVWXYZ");

            this.window.PressBackspace(true, false);
            AssertTextPosition(new TextPosition(2, 1), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 1), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "", "QRST\r\nUVWXYZ");

            this.window.PressBackspace(true, false);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "QRST\r\nUVWXYZ");

            this.window.PressBackspace(true, false);
            AssertTextPosition(new TextPosition(1, 7), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(1, 7), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN", "", "QRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(0, 0), false, false);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMNQRST\r\nUVWXYZ");

            this.window.PressBackspace(true, false);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMNQRST\r\nUVWXYZ");
        }

        [TestMethod]
        public void PressBackspaceShiftTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 4), false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.window.PressBackspace(false, true);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "UVWXYZ");

            this.window.Move(new TextPosition(0, 3), false, false);
            AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
            AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nUVWXYZ");

            this.window.PressBackspace(false, true);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
            AssertController("", "", "HIJKLMN\r\nUVWXYZ");
        }

        [TestMethod]
        public void PressBackspaceControlShiftTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 4), false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.window.PressBackspace(true, true);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "UVWXYZ");

            this.window.Move(new TextPosition(0, 3), false, false);
            AssertTextPosition(new TextPosition(0, 3), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 3), this.window.SelectionCaret);
            AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nUVWXYZ");

            this.window.PressBackspace(true, true);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
            AssertController("", "", "HIJKLMN\r\nUVWXYZ");
        }

        [TestMethod]
        public void PressEnterNormalTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 4), false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.window.PressEnter(false, false);
            AssertTextPosition(new TextPosition(3, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(3, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR\r\n", "", "ST\r\nUVWXYZ");

            this.window.Move(new TextPosition(0, 0), false, false);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ");

            this.window.PressEnter(false, false);
            AssertTextPosition(new TextPosition(1, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(1, 0), this.window.SelectionCaret);
            AssertController("\r\n", "", "ABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ");

            this.window.Move(new TextPosition(5, 6), false, false);
            AssertTextPosition(new TextPosition(5, 6), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(5, 6), this.window.SelectionCaret);
            AssertController("\r\nABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ", "", "");

            this.window.PressEnter(false, false);
            AssertTextPosition(new TextPosition(6, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(6, 0), this.window.SelectionCaret);
            AssertController("\r\nABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ\r\n", "", "");
        }

        [TestMethod]
        public void PressEnterShiftTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 4), false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.window.PressEnter(false, true);
            AssertTextPosition(new TextPosition(3, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(3, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR\r\n", "", "ST\r\nUVWXYZ");

            this.window.Move(new TextPosition(0, 0), false, false);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ");

            this.window.PressEnter(false, true);
            AssertTextPosition(new TextPosition(1, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(1, 0), this.window.SelectionCaret);
            AssertController("\r\n", "", "ABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ");

            this.window.Move(new TextPosition(5, 6), false, false);
            AssertTextPosition(new TextPosition(5, 6), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(5, 6), this.window.SelectionCaret);
            AssertController("\r\nABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ", "", "");

            this.window.PressEnter(false, true);
            AssertTextPosition(new TextPosition(6, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(6, 0), this.window.SelectionCaret);
            AssertController("\r\nABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ\r\n", "", "");
        }

        [TestMethod]
        public void PressEnterControlTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 4), false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.window.PressEnter(true, false);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(0, 0), false, false);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMN\r\n\r\nOPQRST\r\nUVWXYZ");

            this.window.PressEnter(true, false);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(0, 0), this.window.SelectionCaret);
            AssertController("", "", "\r\nABCDEFG\r\nHIJKLMN\r\n\r\nOPQRST\r\nUVWXYZ");
        }

        [TestMethod]
        public void PressEnterControlShiftTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

            this.window.Move(new TextPosition(2, 4), false, false);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(2, 4), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.window.PressEnter(true, true);
            AssertTextPosition(new TextPosition(3, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(3, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\n", "", "\r\nUVWXYZ");

            this.window.Move(new TextPosition(4, 6), false, false);
            AssertTextPosition(new TextPosition(4, 6), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(4, 6), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\n\r\nUVWXYZ", "", "");

            this.window.PressEnter(true, true);
            AssertTextPosition(new TextPosition(5, 0), this.window.SelectionAnchor);
            AssertTextPosition(new TextPosition(5, 0), this.window.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\n\r\nUVWXYZ\r\n", "", "");
        }

        [TestMethod]
        public void CutTest()
        {
            Clipboard.Clear();
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            this.window.Move(new TextPosition(2, 4), false, false);
            this.window.Move(new TextPosition(1, 3), false, true);
            this.window.Press("X", ModifierKeys.Control);
            AssertController("ABCDEFG\r\nHIJ", "", "ST\r\nUVWXYZ");
            Assert.AreEqual("KLMN\r\nOPQR", Clipboard.GetText());
        }

        [TestMethod]
        public void CopyTest()
        {
            Clipboard.Clear();
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            this.window.Move(new TextPosition(2, 4), false, false);
            this.window.Move(new TextPosition(1, 3), false, true);
            this.window.Press("C", ModifierKeys.Control);
            AssertController("ABCDEFG\r\nHIJ", "KLMN\r\nOPQR", "ST\r\nUVWXYZ");
            Assert.AreEqual("KLMN\r\nOPQR", Clipboard.GetText());
        }

        [TestMethod]
        public void PasteTest()
        {
            Clipboard.SetText("Vczh is a genius!");
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            this.window.Move(new TextPosition(2, 4), false, false);
            this.window.Move(new TextPosition(1, 3), false, true);
            this.window.Press("V", ModifierKeys.Control);
            AssertController("ABCDEFG\r\nHIJVczh is a genius!", "", "ST\r\nUVWXYZ");
        }

        [TestMethod]
        public void SelectAllTest()
        {
            this.window.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            this.window.Press("A", ModifierKeys.Control);
            AssertController("", "ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", "");
        }
    }
}
