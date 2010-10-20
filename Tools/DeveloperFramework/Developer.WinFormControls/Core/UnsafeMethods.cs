using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Developer.WinFormControls.Core
{
    class UnsafeMethods
    {
        [DllImport("kernel32.dll", EntryPoint = "RtlMoveMemory", CharSet = CharSet.Ansi)]
        public extern static long CopyMemory(IntPtr dest, IntPtr source, int size);
    }
}