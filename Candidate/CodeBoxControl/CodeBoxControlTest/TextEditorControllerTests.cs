using CodeBoxControl.Core;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace CodeBoxControlTest
{
    [TestClass()]
    public class TextEditorControllerTests
    {
        private MockProvider provider = null;
        private TextEditorController controller = null;

        [TestInitialize]
        public void Initialize()
        {
            provider = new MockProvider();
            controller = new TextEditorController(provider);
        }

        [TestCleanup]
        public void Cleanup()
        {
            provider.Dispose();
            provider = null;
            controller = null;
        }

        private void AssertController(string a, string b, string c)
        {
            TextPosition x1 = new TextPosition(0, 0);
            TextPosition x2 = this.controller.SelectionAnchor;
            TextPosition x3 = this.controller.SelectionCaret;
            TextPosition x4 = new TextPosition(this.provider.GetLineCount() - 1, this.provider.GetLineLength(this.provider.GetLineCount() - 1));
            if (x2 > x3)
            {
                x2 = this.controller.SelectionCaret;
                x3 = this.controller.SelectionAnchor;
            }
            Assert.AreEqual(a, this.provider.provider.GetString(x1, x2));
            Assert.AreEqual(b, this.provider.provider.GetString(x2, x3));
            Assert.AreEqual(c, this.provider.provider.GetString(x3, x4));
        }

        [TestMethod()]
        public void InputTest()
        {
            AssertController("", "", "");
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);
            Assert.AreEqual(new TextPosition(3, 6), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(3, 6), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", "", "");

            this.controller.Move(new TextPosition(1, 4), false, false);
            this.controller.Move(new TextPosition(2, 3), false, true);
            Assert.AreEqual(new TextPosition(1, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJK", "LMN\r\nOPQ", "RST\r\nUVWXYZ");

            this.controller.Move(new TextPosition(2, 3), false, false);
            this.controller.Move(new TextPosition(1, 4), false, true);
            Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(1, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJK", "LMN\r\nOPQ", "RST\r\nUVWXYZ");

            this.controller.Input("...", false);
            Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJK...", "", "RST\r\nUVWXYZ");
        }

        [TestMethod()]
        public void PressUpTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);
            {
                this.controller.Move(new TextPosition(1, 3), false, false);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressUp(false, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressUp(false, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(1, 3), false, false);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressUp(true, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressUp(true, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(1, 3), false, false);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressUp(false, true);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressUp(false, true);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(1, 3), false, false);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressUp(true, true);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressUp(true, true);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ");
            }
        }

        [TestMethod]
        public void PressDownTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressDown(false, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.controller.PressDown(false, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressDown(true, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.controller.PressDown(true, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressDown(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVW", "XYZ");

                this.controller.PressDown(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVW", "XYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressDown(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVW", "XYZ");

                this.controller.PressDown(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVW", "XYZ");
            }
        }

        [TestMethod]
        public void PressLeftTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressLeft(false, false);
                Assert.AreEqual(new TextPosition(2, 2), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 2), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOP", "", "QRST\r\nUVWXYZ");

                this.controller.PressLeft(false, false);
                Assert.AreEqual(new TextPosition(2, 1), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 1), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "", "PQRST\r\nUVWXYZ");

                this.controller.PressLeft(false, false);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "OPQRST\r\nUVWXYZ");

                this.controller.PressLeft(false, false);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "", "\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressLeft(true, false);
                Assert.AreEqual(new TextPosition(2, 1), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 1), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "", "PQRST\r\nUVWXYZ");

                this.controller.PressLeft(true, false);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "OPQRST\r\nUVWXYZ");

                this.controller.PressLeft(true, false);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "", "\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressLeft(true, false);
                Assert.AreEqual(new TextPosition(1, 5), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 5), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKL", "", "MN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressLeft(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 2), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOP", "Q", "RST\r\nUVWXYZ");

                this.controller.PressLeft(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 1), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "PQ", "RST\r\nUVWXYZ");

                this.controller.PressLeft(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "OPQ", "RST\r\nUVWXYZ");

                this.controller.PressLeft(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "\r\nOPQ", "RST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressLeft(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 1), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "PQ", "RST\r\nUVWXYZ");

                this.controller.PressLeft(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "OPQ", "RST\r\nUVWXYZ");

                this.controller.PressLeft(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "\r\nOPQ", "RST\r\nUVWXYZ");

                this.controller.PressLeft(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 5), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKL", "MN\r\nOPQ", "RST\r\nUVWXYZ");
            }
        }

        [TestMethod]
        public void PressRightTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressRight(false, false);
                Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

                this.controller.PressRight(false, false);
                Assert.AreEqual(new TextPosition(2, 5), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 5), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRS", "", "T\r\nUVWXYZ");

                this.controller.PressRight(false, false);
                Assert.AreEqual(new TextPosition(2, 6), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 6), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST", "", "\r\nUVWXYZ");

                this.controller.PressRight(false, false);
                Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\n", "", "UVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressRight(true, false);
                Assert.AreEqual(new TextPosition(2, 5), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 5), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRS", "", "T\r\nUVWXYZ");

                this.controller.PressRight(true, false);
                Assert.AreEqual(new TextPosition(2, 6), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 6), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST", "", "\r\nUVWXYZ");

                this.controller.PressRight(true, false);
                Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\n", "", "UVWXYZ");

                this.controller.PressRight(true, false);
                Assert.AreEqual(new TextPosition(3, 2), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 2), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUV", "", "WXYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressRight(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "R", "ST\r\nUVWXYZ");

                this.controller.PressRight(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 5), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RS", "T\r\nUVWXYZ");

                this.controller.PressRight(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 6), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST", "\r\nUVWXYZ");

                this.controller.PressRight(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\n", "UVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressRight(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 5), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RS", "T\r\nUVWXYZ");

                this.controller.PressRight(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 6), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST", "\r\nUVWXYZ");

                this.controller.PressRight(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\n", "UVWXYZ");

                this.controller.PressRight(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 2), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUV", "WXYZ");
            }
        }

        [TestMethod]
        public void PressHomeTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressHome(false, false);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "OPQRST\r\nUVWXYZ");

                this.controller.PressHome(false, false);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "OPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressHome(true, false);
                Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
                AssertController("", "", "ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressHome(true, false);
                Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
                AssertController("", "", "ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressHome(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "OPQ", "RST\r\nUVWXYZ");

                this.controller.PressHome(false, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\n", "OPQ", "RST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(2, 3), false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressHome(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
                AssertController("", "ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVWXYZ");

                this.controller.PressHome(true, true);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
                AssertController("", "ABCDEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVWXYZ");
            }
        }

        [TestMethod]
        public void PressEndTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);
            {
                this.controller.Move(new TextPosition(1, 3), false, false);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressEnd(false, false);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "", "\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressEnd(false, false);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN", "", "\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(1, 3), false, false);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressEnd(true, false);
                Assert.AreEqual(new TextPosition(3, 6), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 6), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", "", "");

                this.controller.PressEnd(true, false);
                Assert.AreEqual(new TextPosition(3, 6), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 6), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", "", "");
            }
            {
                this.controller.Move(new TextPosition(1, 3), false, false);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressEnd(false, true);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "KLMN", "\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressEnd(false, true);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "KLMN", "\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(1, 3), false, false);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressEnd(true, true);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 6), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ", "");

                this.controller.PressEnd(true, true);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 6), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVWXYZ", "");
            }
        }

        [TestMethod]
        public void PressPageUpTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);
            {
                this.controller.Move(new TextPosition(3, 3), false, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.controller.PressPageUp(false, false);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressPageUp(false, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressPageUp(false, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(3, 3), false, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.controller.PressPageUp(true, false);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "", "KLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressPageUp(true, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressPageUp(true, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");
            }
            {
                this.controller.Move(new TextPosition(3, 3), false, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.controller.PressPageUp(false, true);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVW", "XYZ");

                this.controller.PressPageUp(false, true);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");

                this.controller.PressPageUp(false, true);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");
            }
            {
                this.controller.Move(new TextPosition(3, 3), false, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.controller.PressPageUp(true, true);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(1, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJ", "KLMN\r\nOPQRST\r\nUVW", "XYZ");

                this.controller.PressPageUp(true, true);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");

                this.controller.PressPageUp(true, true);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");
            }
        }

        [TestMethod]
        public void PressPageDownTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);
            {
                this.controller.Move(new TextPosition(0, 3), false, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressPageDown(false, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressPageDown(false, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.controller.PressPageDown(false, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");
            }
            {
                this.controller.Move(new TextPosition(0, 3), false, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressPageDown(true, false);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQ", "", "RST\r\nUVWXYZ");

                this.controller.PressPageDown(true, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");

                this.controller.PressPageDown(true, false);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "", "XYZ");
            }
            {
                this.controller.Move(new TextPosition(0, 3), false, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressPageDown(false, true);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVWXYZ");

                this.controller.PressPageDown(false, true);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");

                this.controller.PressPageDown(false, true);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");
            }
            {
                this.controller.Move(new TextPosition(0, 3), false, false);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
                AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ");

                this.controller.PressPageDown(true, true);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(2, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQ", "RST\r\nUVWXYZ");

                this.controller.PressPageDown(true, true);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");

                this.controller.PressPageDown(true, true);
                Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
                Assert.AreEqual(new TextPosition(3, 3), this.controller.SelectionCaret);
                AssertController("ABC", "DEFG\r\nHIJKLMN\r\nOPQRST\r\nUVW", "XYZ");
            }
        }

        [TestMethod]
        public void PressDeleteNormalTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 4), false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.controller.PressDelete(false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "T\r\nUVWXYZ");

            this.controller.PressDelete(false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "\r\nUVWXYZ");

            this.controller.PressDelete(false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "UVWXYZ");

            this.controller.Move(new TextPosition(2, 10), false, false);
            Assert.AreEqual(new TextPosition(2, 10), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 10), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRUVWXYZ", "", "");

            this.controller.PressDelete(false, false);
            Assert.AreEqual(new TextPosition(2, 10), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 10), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRUVWXYZ", "", "");
        }

        [TestMethod]
        public void PressDeleteControlTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 4), false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.controller.PressDelete(true, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "\r\nUVWXYZ");

            this.controller.PressDelete(true, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "UVWXYZ");

            this.controller.PressDelete(true, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "WXYZ");

            this.controller.Move(new TextPosition(2, 8), false, false);
            Assert.AreEqual(new TextPosition(2, 8), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 8), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRWXYZ", "", "");

            this.controller.PressDelete(true, false);
            Assert.AreEqual(new TextPosition(2, 8), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 8), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRWXYZ", "", "");
        }

        [TestMethod]
        public void PressDeleteShiftTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 4), false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.controller.PressDelete(false, true);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "UVWXYZ");

            this.controller.Move(new TextPosition(2, 6), false, false);
            Assert.AreEqual(new TextPosition(2, 6), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 6), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nUVWXYZ", "", "");

            this.controller.PressDelete(false, true);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "");
        }

        [TestMethod]
        public void PressDeleteControlShiftTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 4), false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.controller.PressDelete(true, true);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "UVWXYZ");

            this.controller.Move(new TextPosition(2, 6), false, false);
            Assert.AreEqual(new TextPosition(2, 6), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 6), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nUVWXYZ", "", "");

            this.controller.PressDelete(true, true);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "");
        }

        [TestMethod]
        public void PressBackspaceNormalTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 2), false, false);
            Assert.AreEqual(new TextPosition(2, 2), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 2), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOP", "", "QRST\r\nUVWXYZ");

            this.controller.PressBackspace(false, false);
            Assert.AreEqual(new TextPosition(2, 1), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 1), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nO", "", "QRST\r\nUVWXYZ");

            this.controller.PressBackspace(false, false);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "QRST\r\nUVWXYZ");

            this.controller.PressBackspace(false, false);
            Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN", "", "QRST\r\nUVWXYZ");

            this.controller.Move(new TextPosition(0, 0), false, false);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMNQRST\r\nUVWXYZ");

            this.controller.PressBackspace(false, false);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMNQRST\r\nUVWXYZ");
        }

        [TestMethod]
        public void PressBackspaceControlTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 2), false, false);
            Assert.AreEqual(new TextPosition(2, 2), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 2), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOP", "", "QRST\r\nUVWXYZ");

            this.controller.PressBackspace(true, false);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "QRST\r\nUVWXYZ");

            this.controller.PressBackspace(true, false);
            Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(1, 7), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN", "", "QRST\r\nUVWXYZ");

            this.controller.PressBackspace(true, false);
            Assert.AreEqual(new TextPosition(1, 5), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(1, 5), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKL", "", "QRST\r\nUVWXYZ");

            this.controller.Move(new TextPosition(0, 0), false, false);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLQRST\r\nUVWXYZ");

            this.controller.PressBackspace(true, false);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLQRST\r\nUVWXYZ");
        }

        [TestMethod]
        public void PressBackspaceShiftTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 4), false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.controller.PressBackspace(false, true);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "UVWXYZ");

            this.controller.Move(new TextPosition(0, 3), false, false);
            Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
            AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nUVWXYZ");

            this.controller.PressBackspace(false, true);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
            AssertController("", "", "HIJKLMN\r\nUVWXYZ");
        }

        [TestMethod]
        public void PressBackspaceControlShiftTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 4), false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.controller.PressBackspace(false, true);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "UVWXYZ");

            this.controller.Move(new TextPosition(0, 3), false, false);
            Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 3), this.controller.SelectionCaret);
            AssertController("ABC", "", "DEFG\r\nHIJKLMN\r\nUVWXYZ");

            this.controller.PressBackspace(false, true);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
            AssertController("", "", "HIJKLMN\r\nUVWXYZ");
        }

        [TestMethod]
        public void PressEnterNormalTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 4), false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.controller.PressEnter(false, false);
            Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR\r\n", "", "ST\r\nUVWXYZ");

            this.controller.Move(new TextPosition(0, 0), false, false);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ");

            this.controller.PressEnter(false, false);
            Assert.AreEqual(new TextPosition(1, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(1, 0), this.controller.SelectionCaret);
            AssertController("\r\n", "", "ABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ");

            this.controller.Move(new TextPosition(5, 6), false, false);
            Assert.AreEqual(new TextPosition(5, 6), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(5, 6), this.controller.SelectionCaret);
            AssertController("\r\nABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ", "", "");

            this.controller.PressEnter(false, false);
            Assert.AreEqual(new TextPosition(6, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(6, 0), this.controller.SelectionCaret);
            AssertController("\r\nABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ\r\n", "", "");
        }

        [TestMethod]
        public void PressEnterShiftTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 4), false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.controller.PressEnter(false, true);
            Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR\r\n", "", "ST\r\nUVWXYZ");

            this.controller.Move(new TextPosition(0, 0), false, false);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ");

            this.controller.PressEnter(false, true);
            Assert.AreEqual(new TextPosition(1, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(1, 0), this.controller.SelectionCaret);
            AssertController("\r\n", "", "ABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ");

            this.controller.Move(new TextPosition(5, 6), false, false);
            Assert.AreEqual(new TextPosition(5, 6), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(5, 6), this.controller.SelectionCaret);
            AssertController("\r\nABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ", "", "");

            this.controller.PressEnter(false, true);
            Assert.AreEqual(new TextPosition(6, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(6, 0), this.controller.SelectionCaret);
            AssertController("\r\nABCDEFG\r\nHIJKLMN\r\nOPQR\r\nST\r\nUVWXYZ\r\n", "", "");
        }

        [TestMethod]
        public void PressEnterControlTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 4), false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.controller.PressEnter(true, false);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\n", "", "\r\nOPQRST\r\nUVWXYZ");

            this.controller.Move(new TextPosition(0, 0), false, false);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
            AssertController("", "", "ABCDEFG\r\nHIJKLMN\r\n\r\nOPQRST\r\nUVWXYZ");

            this.controller.PressEnter(true, false);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(0, 0), this.controller.SelectionCaret);
            AssertController("", "", "\r\nABCDEFG\r\nHIJKLMN\r\n\r\nOPQRST\r\nUVWXYZ");
        }

        [TestMethod]
        public void PressEnterControlShiftTest()
        {
            this.controller.Input("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", false);

            this.controller.Move(new TextPosition(2, 4), false, false);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(2, 4), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQR", "", "ST\r\nUVWXYZ");

            this.controller.PressEnter(true, true);
            Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(3, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\n", "", "\r\nUVWXYZ");

            this.controller.Move(new TextPosition(4, 6), false, false);
            Assert.AreEqual(new TextPosition(4, 6), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(4, 6), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\n\r\nUVWXYZ", "", "");

            this.controller.PressEnter(true, true);
            Assert.AreEqual(new TextPosition(5, 0), this.controller.SelectionAnchor);
            Assert.AreEqual(new TextPosition(5, 0), this.controller.SelectionCaret);
            AssertController("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\n\r\nUVWXYZ\r\n", "", "");
        }
    }

    class MockProvider : ITextContentProvider, IDisposable
    {
        public TextProvider<object> provider = new TextProvider<object>();

        public int GetLineCount()
        {
            return this.provider.Count;
        }

        public int GetPageLineCount()
        {
            return 2;
        }

        public int GetLineLength(int index)
        {
            return this.provider[index].CharCount;
        }

        public void OnSelectionAreaChanged()
        {
        }

        public bool OnEdit(TextPosition start, TextPosition end, string[] lines)
        {
            this.provider.Edit(start, end, lines);
            return true;
        }

        public void OnFinishEdit()
        {
        }

        public void OnRefreshSuggestion()
        {
        }

        public TextPosition GetLeftWord(TextPosition caret)
        {
            if (caret.col > 0)
            {
                return new TextPosition(caret.row, Math.Max(0, caret.col - 2));
            }
            else if (caret.row > 0)
            {
                return new TextPosition(caret.row - 1, this.provider[caret.row - 1].CharCount);
            }
            else
            {
                return caret;
            }
        }

        public TextPosition GetRightWord(TextPosition caret)
        {
            if (caret.col < this.provider[caret.row].CharCount)
            {
                return new TextPosition(caret.row, Math.Min(this.provider[caret.row].CharCount, caret.col + 2));
            }
            else if (caret.row < this.provider.Count - 1)
            {
                return new TextPosition(caret.row + 1, 0);
            }
            else
            {
                return caret;
            }
        }

        public void Dispose()
        {
            this.provider.Dispose();
        }
    }
}
