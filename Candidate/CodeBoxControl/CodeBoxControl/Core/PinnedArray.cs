using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CodeBoxControl.Core
{
    public class PinnedArray<T> : IDisposable, IEnumerable<T>
        where T : struct
    {
        private T[] buffer;
        private GCHandle gcHandle;
        private bool pinned = false;

        public PinnedArray(int size)
        {
            this.buffer = new T[size];
        }

        public PinnedArray(T[] buffer)
        {
            this.buffer = buffer;
        }

        public void Dispose()
        {
            this.Pinned = false;
        }

        public IEnumerator<T> GetEnumerator()
        {
            return this.buffer.Cast<T>().GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return this.buffer.GetEnumerator();
        }

        public T[] Buffer
        {
            get
            {
                return buffer;
            }
        }

        public bool Pinned
        {
            get
            {
                return this.pinned;
            }
            set
            {
                if (this.pinned != value)
                {
                    this.pinned = value;
                    if (this.pinned)
                    {
                        this.gcHandle = GCHandle.Alloc(this.buffer, GCHandleType.Pinned);
                    }
                    else
                    {
                        this.gcHandle.Free();
                    }
                }
            }
        }

        public IntPtr GetElementAddress(int index)
        {
            if (this.pinned)
            {
                return Marshal.UnsafeAddrOfPinnedArrayElement(this.buffer, index);
            }
            else
            {
                throw new InvalidOperationException("Cannot call Marshal.UnsafeAddrOfPinnedArrayElement on a unpinned array.");
            }
        }

        public void Resize(int size)
        {
            if (size != buffer.Length)
            {
                int minSize = Math.Min(size, this.buffer.Length);
                T[] newBuffer = new T[size];
                Array.Copy(this.buffer, newBuffer, minSize);
                this.buffer = newBuffer;
                if (this.pinned)
                {
                    this.gcHandle.Free();
                    this.gcHandle = GCHandle.Alloc(newBuffer, GCHandleType.Pinned); ;
                }
            }
        }
    }
}
