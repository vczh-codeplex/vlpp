using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace VlScriptDotNet
{
    public class VlsAssembly
    {
        internal IntPtr assembly;
        internal VlsHost host;

        public VlsBasicFunctionList BasicFunctions { get; private set; }

        public VlsAssembly(VlsHost host, string fileName)
        {
            if (VlScriptHeader.VlsCreateAssemblyFromFile(host.host, fileName, out this.assembly) == VlScriptHeader.VLS_ERR)
            {
                host.RaiseException();
            }
            else
            {
                this.host = host;
                this.BasicFunctions = new VlsBasicFunctionList(this);
            }
        }

        public void Dispose()
        {
            VlScriptHeader.VlsDisposeAssembly(this.assembly);
        }
    }
}
