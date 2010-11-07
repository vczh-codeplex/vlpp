using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.WinFormControls.Core
{
    public class TextLine<T> : IDisposable
        where T : new()
    {
        private const int BufferBlock = 128;

        private PinnedArray<char> charArray = null;
        private int[] colorArray = null;
        private int[] offsetArray = null;
        private int availableCharCount = 0;
        private List<Tuple<int, int>> blocks = new List<Tuple<int, int>>();
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
            this.offsetArray = new int[BufferBlock];
        }

        public void Dispose()
        {
            this.charArray.Dispose();
        }

        #region Infomation

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

        public int[] OffsetArray
        {
            get
            {
                return this.offsetArray;
            }
        }

        public string GetString(int start, int count)
        {
            char[] buffer = new char[count];
            Array.Copy(this.charArray.Buffer, start, buffer, 0, count);
            return new string(buffer);
        }

        #endregion

        #region Edit Functions

        public void Edit(int start, int count, PinnedArray<char> text, int textStart, int textLength)
        {
            int blockOffset = textLength - count;
            for (int i = this.blocks.Count - 1; i >= 0; i--)
            {
                Tuple<int, int> block = this.blocks[i];
                if (start < block.Item2 && start + count > block.Item1)
                {
                    this.blocks.RemoveAt(i);
                }
                else if (start + count <= block.Item1)
                {
                    this.blocks[i] = Tuple.Create(block.Item1 + blockOffset, block.Item2 + blockOffset);
                }
            }

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
                this.offsetArray = new int[this.charArray.Buffer.Length];
            }
            else
            {
                for (int i = start; i < this.charArray.Buffer.Length; i++)
                {
                    this.offsetArray[i] = 0;
                }
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

        #region Block

        public int GetLeftBlock(int index)
        {
            foreach (Tuple<int, int> block in this.blocks)
            {
                if (block.Item2 == index)
                {
                    return block.Item1;
                }
            }
            return index;
        }

        public int GetRightBlock(int index)
        {
            foreach (Tuple<int, int> block in this.blocks)
            {
                if (block.Item1 == index)
                {
                    return block.Item2;
                }
            }
            return index;
        }

        public Tuple<int, int> GetBlock(int index)
        {
            foreach (Tuple<int, int> block in this.blocks)
            {
                if (block.Item1 < index && index < block.Item2)
                {
                    return block;
                }
            }
            return Tuple.Create(index, index);
        }

        public bool AddBlock(int start, int end)
        {
            if (0 <= start && start < end && end <= this.availableCharCount)
            {
                foreach (Tuple<int, int> block in this.blocks)
                {
                    if (start < block.Item2 && end > block.Item1)
                    {
                        return false;
                    }
                }
                this.blocks.Add(Tuple.Create(start, end));
                return true;
            }
            return false;
        }

        public bool RemoveBlock(int start, int end)
        {
            int index = this.blocks.IndexOf(Tuple.Create(start, end));
            if (index >= 0) this.blocks.RemoveAt(index);
            return index >= 0;
        }

        public int BlockCount
        {
            get
            {
                return this.blocks.Count;
            }
        }

        public IEnumerable<Tuple<int, int>> Blocks
        {
            get
            {
                return this.blocks;
            }
        }

        public Tuple<int, int> GetBlockByIndex(int index)
        {
            return this.blocks[index];
        }

        #endregion

        #region Implementation

        private int GetBlockSize(int size)
        {
            return (size + BufferBlock - 1) / BufferBlock * BufferBlock;
        }

        #endregion
    }
}
