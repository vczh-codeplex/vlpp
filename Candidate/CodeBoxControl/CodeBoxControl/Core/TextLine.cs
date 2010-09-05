using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodeBoxControl.Core
{
    public class TextLine<T> : IDisposable
        where T : new()
    {
        private const int BufferBlock = 128;

        private PinnedArray<char> charArray = null;
        private int[] colorArray = null;
        private int availableCharCount = 0;
        private T tag;

        public T Tag
        {
            get
            {
                if (this.tag == null)
                {
                    this.tag = new T();
                }
                return this.tag;
            }
        }

        public TextLine()
        {
            this.charArray = new PinnedArray<char>(BufferBlock);
            this.colorArray = new int[BufferBlock];
        }

        public void Dispose()
        {
            this.charArray.Dispose();
        }

        public int CharCount
        {
            get
            {
                return this.availableCharCount;
            }
        }

        public string Text
        {
            get
            {
                return GetString(0, this.availableCharCount);
            }
        }

        public char[] CharArray
        {
            get
            {
                return this.charArray.Buffer;
            }
        }

        public int[] ColorArray
        {
            get
            {
                return this.colorArray;
            }
        }

        public string GetString(int start, int count)
        {
            char[] buffer = new char[count];
            Array.Copy(this.charArray.Buffer, start, buffer, 0, count);
            return new string(buffer);
        }

        private int GetBlockSize(int size)
        {
            return (size + BufferBlock - 1) / BufferBlock * BufferBlock;
        }

        #region Edit Functions

        public void Edit(int start, int count, PinnedArray<char> text, int textStart, int textLength)
        {
            int newSize = this.availableCharCount + textLength - count;
            int newBlock = GetBlockSize(newSize);
            if (newBlock > this.charArray.Buffer.Length)
            {
                this.charArray.Resize(newBlock);
            }

            this.charArray.Pinned = true;
            text.Pinned = true;
            UnsafeMethods.CopyMemory(this.charArray.GetElementAddress(start + textLength), this.charArray.GetElementAddress(start + count), (this.availableCharCount - (start + count)) * sizeof(char));
            UnsafeMethods.CopyMemory(this.charArray.GetElementAddress(start), text.GetElementAddress(textStart), textLength * sizeof(char));
            this.charArray.Pinned = false;
            text.Pinned = false;
            this.availableCharCount = newSize;

            if (this.charArray.Buffer.Length - newBlock > BufferBlock)
            {
                this.charArray.Resize(newBlock);
            }
            if (this.colorArray.Length != this.charArray.Buffer.Length)
            {
                this.colorArray = new int[this.charArray.Buffer.Length];
            }
        }

        public void Edit(int start, int count, TextLine<T> text, int textStart, int textLength)
        {
            Edit(start, count, text.charArray, textStart, textLength);
        }

        public void Edit(int start, int count, char[] text, int textStart, int textLength)
        {
            using (PinnedArray<char> array = new PinnedArray<char>(text))
            {
                Edit(start, count, array, textStart, textLength);
            }
        }

        public void Edit(int start, int count, string text, int textStart, int textLength)
        {
            Edit(start, count, text.ToCharArray(), textStart, textLength);
        }

        public void Edit(int start, int count, PinnedArray<char> text)
        {
            Edit(start, count, text, 0, text.Buffer.Length);
        }

        public void Edit(int start, int count, TextLine<T> text)
        {
            Edit(start, count, text, 0, text.CharCount);
        }

        public void Edit(int start, int count, char[] text)
        {
            Edit(start, count, text, 0, text.Length);
        }

        public void Edit(int start, int count, string text)
        {
            Edit(start, count, text, 0, text.Length);
        }

        #endregion
    }
}
