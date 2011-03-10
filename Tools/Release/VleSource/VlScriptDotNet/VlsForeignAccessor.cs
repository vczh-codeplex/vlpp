using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace VlScriptDotNet
{
    public struct VlsForeignAccessor
    {
        private IntPtr arguments;
        private IntPtr result;
        private int size;

        public IntPtr Arguments { get { return this.arguments; } }
        public IntPtr Result { get { return this.result; } }
        public int Size { get { return this.size; } }

        public VlsForeignAccessor(IntPtr result, IntPtr arguments)
        {
            this.arguments = arguments;
            this.result = result;
            this.size = 0;
        }

        public T Get<T>()
            where T : struct
        {
            IntPtr reading = this.arguments + this.size;
            this.size += Marshal.SizeOf(typeof(T));
            return (T)Marshal.PtrToStructure(reading, typeof(T));
        }

        public string GetAStr()
        {
            return Marshal.PtrToStringAnsi(Get<IntPtr>());
        }

        public string GetWStr()
        {
            return Marshal.PtrToStringUni(Get<IntPtr>());
        }

        public void Set<T>(T value)
            where T : struct
        {
            Marshal.StructureToPtr(value, this.result, false);
        }
    }
}
