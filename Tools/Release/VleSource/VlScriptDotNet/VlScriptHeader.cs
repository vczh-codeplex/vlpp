using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace VlScriptDotNet
{
    static class VlScriptHeader
    {
        public const int VLS_OK = 1;
        public const int VLS_ERR = 0;

        public const int VLS_MIN_STACK_SIZE = 1024;
        public const int VLS_MAX_STACK_SIZE = 1048576;
        public const int VLS_MIN_DATA_SIZE = 1;
        public const int VLS_MAX_DATA_SIZE = 256;
        public const int VLS_DEFAULT_STACK_SIZE = 65536;

        public delegate int VlsForeignFunction(IntPtr result, IntPtr arguments, IntPtr userData);
        public delegate void VlsConsoleReader(IntPtr wcharBuffer);
        public delegate void VlsConsoleWriter(string text);
        public delegate void VlsUnitTestPrinter(bool condition, string message);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsCreateHost(out IntPtr host, int stackSize);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsDisposeHost(IntPtr host);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsCreateState(IntPtr host, out IntPtr state);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsDisposeState(IntPtr state);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsCreateAssemblyFromFile(IntPtr host, string fileName, out IntPtr assembly);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsDisposeAssembly(IntPtr assembly);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsGetLastError(IntPtr host, ref IntPtr error, out int length);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsGetBasicFunctionCount(IntPtr assembly, out int count);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsGetBasicFunctionName(IntPtr assembly, int index, out IntPtr name);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsGetBasicFunctionEntry(IntPtr assembly, int index, out int entry);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsRegisterForeignFunction(IntPtr host, string category, string name, VlsForeignFunction function, IntPtr userData);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsLoadPlugin_CoreNative(IntPtr host);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsLoadPlugin_ConsoleNative(IntPtr host, VlsConsoleReader reader, VlsConsoleWriter writer);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsLoadPlugin_UnitTestNative(IntPtr host, VlsUnitTestPrinter printer);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsLoadAssembly(IntPtr host, IntPtr assembly);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsInitAssembly(IntPtr state, IntPtr assembly);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsPush(IntPtr state, IntPtr data, int size);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsSetFunction(IntPtr state, IntPtr assembly, int entry, IntPtr resultAddress);

        [DllImport("VlScript.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int VlsExecute(IntPtr state);
    }
}
