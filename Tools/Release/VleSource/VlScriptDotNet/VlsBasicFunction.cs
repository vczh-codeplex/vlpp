using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace VlScriptDotNet
{
    public class VlsBasicFunction
    {
        public string Name { get; internal set; }
        public int Entry { get; internal set; }
        public VlsAssembly Assembly { get; internal set; }
    }

    public class VlsBasicFunctionList : IEnumerable<VlsBasicFunction>
    {
        internal VlsAssembly assembly;

        internal VlsBasicFunctionList(VlsAssembly assembly)
        {
            this.assembly = assembly;
        }

        public int Count
        {
            get
            {
                int count = 0;
                if (VlScriptHeader.VlsGetBasicFunctionCount(this.assembly.assembly, out count) == VlScriptHeader.VLS_ERR)
                {
                    this.assembly.host.RaiseException();
                }
                return count;
            }
        }

        public VlsBasicFunction this[int index]
        {
            get
            {
                IntPtr name = IntPtr.Zero;
                int entry = 0;
                if (VlScriptHeader.VlsGetBasicFunctionName(this.assembly.assembly, index, out name) == VlScriptHeader.VLS_ERR)
                {
                    this.assembly.host.RaiseException();
                }
                if (VlScriptHeader.VlsGetBasicFunctionEntry(this.assembly.assembly, index, out entry) == VlScriptHeader.VLS_ERR)
                {
                    this.assembly.host.RaiseException();
                }
                return new VlsBasicFunction()
                {
                    Name = Marshal.PtrToStringUni(name),
                    Entry = entry,
                    Assembly = this.assembly
                };
            }
        }

        IEnumerator<VlsBasicFunction> IEnumerable<VlsBasicFunction>.GetEnumerator()
        {
            int count = this.Count;
            for (int i = 0; i < count; i++)
            {
                yield return this[i];
            }
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return (this as IEnumerable<VlsBasicFunction>).GetEnumerator();
        }
    }
}
