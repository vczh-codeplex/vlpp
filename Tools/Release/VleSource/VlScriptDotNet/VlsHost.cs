using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace VlScriptDotNet
{
    public class VlsHost : IDisposable
    {
        public const int MaxStackSize = VlScriptHeader.VLS_MAX_STACK_SIZE;
        public const int MinStackSize = VlScriptHeader.VLS_MIN_STACK_SIZE;

        public delegate int ForeignFunction(IntPtr result, IntPtr arguments, IntPtr userData);
        public delegate void ConsoleReader(IntPtr wcharBuffer);
        public delegate void ConsoleWriter(string text);
        public delegate void UnitTestPrinter(bool condition, string message);

        internal IntPtr host;
        private List<VlScriptHeader.VlsForeignFunction> registeredForeignFunctions = new List<VlScriptHeader.VlsForeignFunction>();

        public VlsHost(int stackSize)
        {
            if (MinStackSize > stackSize || stackSize > MaxStackSize)
            {
                throw new InvalidOperationException("Stack size out of range.");
            }
            else if (VlScriptHeader.VlsCreateHost(out this.host, stackSize) == VlScriptHeader.VLS_ERR)
            {
                throw new InvalidOperationException("Fail to create host.");
            }
        }

        public void Dispose()
        {
            VlScriptHeader.VlsDisposeHost(this.host);
        }

        public void RaiseException()
        {
            IntPtr error = IntPtr.Zero;
            VlScriptHeader.VlsGetLastHostError(this.host, out error);
            throw new InvalidOperationException(Marshal.PtrToStringUni(error));
        }

        public void InstallCoreNativePlugin()
        {
            if (VlScriptHeader.VlsLoadPlugin_CoreNative(this.host) == VlScriptHeader.VLS_ERR)
            {
                RaiseException();
            }
        }

        public void InstallConsoleNativePlugin(ConsoleReader reader, ConsoleWriter writer)
        {
            if (VlScriptHeader.VlsLoadPlugin_ConsoleNative(this.host,
                new VlScriptHeader.VlsConsoleReader(reader),
                new VlScriptHeader.VlsConsoleWriter(writer)
                ) == VlScriptHeader.VLS_ERR)
            {
                RaiseException();
            }
        }

        public void InstallUnitTestNativePlugin(UnitTestPrinter printer)
        {
            if (VlScriptHeader.VlsLoadPlugin_UnitTestNative(this.host,
                new VlScriptHeader.VlsUnitTestPrinter(printer)
                ) == VlScriptHeader.VLS_ERR)
            {
                RaiseException();
            }
        }

        public void RegisterForeignFunction(string category, string name, ForeignFunction function, IntPtr userData)
        {
            VlScriptHeader.VlsForeignFunction foreignFunction = new VlScriptHeader.VlsForeignFunction(function);
            if (VlScriptHeader.VlsRegisterForeignFunction(host, category, name,
                foreignFunction,
                userData) == VlScriptHeader.VLS_ERR)
            {
                RaiseException();
            }
            else
            {
                this.registeredForeignFunctions.Add(foreignFunction);
            }
        }

        public void InstallAssembly(VlsAssembly assembly)
        {
            if (VlScriptHeader.VlsLoadAssembly(this.host, assembly.assembly) == VlScriptHeader.VLS_ERR)
            {
                RaiseException();
            }
        }
    }
}
