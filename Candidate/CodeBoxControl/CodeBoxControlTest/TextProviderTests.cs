using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using CodeBoxControl.Core;
using System.Runtime.InteropServices;

namespace CodeBoxControlTest
{
    [TestClass]
    public class TextProviderTests
    {
        [TestMethod]
        public void PinnedArrayTest()
        {
            using (PinnedArray<int> array = new PinnedArray<int>(10))
            {
                array.Pinned = true;
                Assert.AreEqual(10, array.Buffer.Length);
                for (int i = 0; i < 10; i++)
                {
                    array.Buffer[i] = i;
                }

                array.Resize(20);
                Assert.AreEqual(20, array.Buffer.Length);
                Assert.IsTrue(array.Buffer.Take(10).SequenceEqual(Enumerable.Range(0, 10)));

                array.Resize(5);
                Assert.AreEqual(5, array.Buffer.Length);
                Assert.IsTrue(array.Buffer.SequenceEqual(Enumerable.Range(0, 5)));

                int[] array2 = new int[5];
                Marshal.Copy(array.GetElementAddress(0), array2, 0, 5);
                Assert.IsTrue(array.Buffer.SequenceEqual(array2));

                int[] array3 = Enumerable.Range(5, 5).ToArray();
                Marshal.Copy(array3, 0, array.GetElementAddress(0), 5);
                Assert.IsTrue(array.Buffer.SequenceEqual(array3));
            }
        }

        private void AssertTextLine<T>(string text, TextLine<T> line)
            where T : new()
        {
            Assert.AreEqual(text.Length, line.CharCount);
            Assert.AreEqual(text, line.Text);
            for (int i = 0; i < text.Length; i++)
            {
                for (int j = i; j < text.Length; j++)
                {
                    Assert.AreEqual(text.Substring(i, j - i), line.GetString(i, j - i));
                }
            }
        }

        [TestMethod]
        public void TextLineTest()
        {
            using (TextLine<object> line = new TextLine<object>())
            {
                AssertTextLine("", line);

                line.Edit(0, 0, " is ");
                AssertTextLine(" is ", line);

                line.Edit(0, 0, "Vczh");
                AssertTextLine("Vczh is ", line);

                line.Edit(8, 0, "genius!");
                AssertTextLine("Vczh is genius!", line);

                line.Edit(5, 2, "IS");
                AssertTextLine("Vczh IS genius!", line);

                line.Edit(5, 2, "is a");
                AssertTextLine("Vczh is a genius!", line);

                line.Edit(10, 6, "天才");
                AssertTextLine("Vczh is a 天才!", line);
            }
        }

        [TestMethod]
        public void TestLineBlocks()
        {
            using (TextLine<object> line = new TextLine<object>())
            {
                line.Edit(0, 0, "Vczh is a genius!");
                Assert.AreEqual(0, line.BlockCount);

                Assert.IsFalse(line.AddBlock(3, 3));
                Assert.AreEqual(0, line.BlockCount);

                Assert.IsFalse(line.AddBlock(-1, 5));
                Assert.AreEqual(0, line.BlockCount);

                Assert.IsFalse(line.AddBlock(8, 20));
                Assert.AreEqual(0, line.BlockCount);

                Assert.IsTrue(line.AddBlock(5, 9));
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));

                Assert.IsTrue(line.AddBlock(1, 4));
                Assert.AreEqual(2, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));
                Assert.AreEqual(Tuple.Create(1, 4), line.GetBlockByIndex(1));

                Assert.IsTrue(line.AddBlock(9, 10));
                Assert.AreEqual(3, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));
                Assert.AreEqual(Tuple.Create(1, 4), line.GetBlockByIndex(1));
                Assert.AreEqual(Tuple.Create(9, 10), line.GetBlockByIndex(2));

                Assert.IsFalse(line.AddBlock(7, 13));
                Assert.IsFalse(line.AddBlock(0, 3));
                Assert.IsFalse(line.RemoveBlock(0, 0));
                Assert.IsTrue(line.RemoveBlock(1, 4));
                Assert.IsTrue(line.RemoveBlock(9, 10));
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));

                for (int i = 0; i < 17; i++)
                {
                    Assert.AreEqual(i == 9 ? 5 : i, line.GetLeftBlock(i));
                    Assert.AreEqual(i == 5 ? 9 : i, line.GetRightBlock(i));
                    if (5 < i && i < 9)
                    {
                        Assert.AreEqual(Tuple.Create(5, 9), line.GetBlock(i));
                    }
                    else
                    {
                        Assert.AreEqual(Tuple.Create(i, i), line.GetBlock(i));
                    }
                }

                line.Edit(10, 0, "X");
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));

                line.Edit(9, 1, "ABCDEFG");
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));

                line.Edit(3, 1, "XYZ");
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(7, 11), line.GetBlockByIndex(0));

                line.Edit(6, 1, "");
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(6, 10), line.GetBlockByIndex(0));

                line.Edit(0, line.CharCount, "Vczh is a genius!");
                Assert.AreEqual(0, line.BlockCount);
                Assert.IsTrue(line.AddBlock(5, 9));
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));
                line.Edit(4, 2, "");
                Assert.AreEqual(0, line.BlockCount);

                line.Edit(0, line.CharCount, "Vczh is a genius!");
                Assert.AreEqual(0, line.BlockCount);
                Assert.IsTrue(line.AddBlock(5, 9));
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));
                line.Edit(7, 4, "123");
                Assert.AreEqual(0, line.BlockCount);

                line.Edit(0, line.CharCount, "Vczh is a genius!");
                Assert.AreEqual(0, line.BlockCount);
                Assert.IsTrue(line.AddBlock(5, 9));
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));
                line.Edit(5, 4, "..");
                Assert.AreEqual(0, line.BlockCount);

                line.Edit(0, line.CharCount, "Vczh is a genius!");
                Assert.AreEqual(0, line.BlockCount);
                Assert.IsTrue(line.AddBlock(5, 9));
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));
                line.Edit(4, 6, "..");
                Assert.AreEqual(0, line.BlockCount);

                line.Edit(0, line.CharCount, "Vczh is a genius!");
                Assert.AreEqual(0, line.BlockCount);
                Assert.IsTrue(line.AddBlock(5, 9));
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));
                line.Edit(4, 5, "..");
                Assert.AreEqual(0, line.BlockCount);

                line.Edit(0, line.CharCount, "Vczh is a genius!");
                Assert.AreEqual(0, line.BlockCount);
                Assert.IsTrue(line.AddBlock(5, 9));
                Assert.AreEqual(1, line.BlockCount);
                Assert.AreEqual(Tuple.Create(5, 9), line.GetBlockByIndex(0));
                line.Edit(5, 5, "..");
                Assert.AreEqual(0, line.BlockCount);
            }
        }

        private void AssertTextProvider<T>(string text, TextProvider<T> provider)
            where T : new()
        {
            string[] lines = text.Split(new string[] { "\r\n" }, StringSplitOptions.None);
            Assert.AreEqual(lines.Length, provider.Count);
            for (int i = 0; i < lines.Length; i++)
            {
                AssertTextLine(lines[i], provider[i]);
            }
            Assert.AreEqual(text, provider.Text);
        }

        [TestMethod]
        public void TextProviderTest()
        {
            using (TextProvider<object> text = new TextProvider<object>())
            {
                AssertTextProvider("", text);

                Assert.AreEqual(new TextPosition(0, 15), text.Edit(new TextPosition(0, 0), new TextPosition(0, 0), "Vczh is genius!"));
                AssertTextProvider("Vczh is genius!", text);

                Assert.AreEqual(new TextPosition(1, 15), text.Edit(new TextPosition(0, 15), new TextPosition(0, 15), "", "Vczh is clever!"));
                AssertTextProvider("Vczh is genius!\r\nVczh is clever!", text);

                Assert.AreEqual(new TextPosition(0, 6), text.Edit(new TextPosition(0, 6), new TextPosition(1, 6)));
                AssertTextProvider("Vczh is clever!", text);
            }
            using (TextProvider<object> text = new TextProvider<object>())
            {
                Assert.AreEqual(new TextPosition(3, 6), text.Edit(new TextPosition(0, 0), new TextPosition(0, 0), "ABCDEFG", "HIJKLMN", "OPQRST", "UVWXYZ"));
                AssertTextProvider("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", text);

                Assert.AreEqual(new TextPosition(0, 7), text.Edit(new TextPosition(0, 4), new TextPosition(2, 3), "..."));
                AssertTextProvider("ABCD...RST\r\nUVWXYZ", text);

                Assert.AreEqual(new TextPosition(2, 3), text.Edit(new TextPosition(0, 4), new TextPosition(0, 7), "EFG", "HIJKLMN", "OPQ"));
                AssertTextProvider("ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ", text);

                Assert.AreEqual(new TextPosition(1, 3), text.Edit(new TextPosition(0, 4), new TextPosition(2, 3), "...", "???"));
                AssertTextProvider("ABCD...\r\n???RST\r\nUVWXYZ", text);
            }
        }
    }
}
