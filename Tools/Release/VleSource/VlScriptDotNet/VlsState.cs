using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace VlScriptDotNet
{
    public class VlsState : IDisposable
    {
        internal IntPtr state;
        internal VlsHost host;

        public VlsState(VlsHost host)
        {
            if (VlScriptHeader.VlsCreateState(host.host, out this.state) == VlScriptHeader.VLS_ERR)
            {
                host.RaiseException();
            }
            else
            {
                this.host = host;
            }
        }

        public void Dispose()
        {
            VlScriptHeader.VlsDisposeState(this.state);
        }

        public void InitializeInstalledAssembly(VlsAssembly assembly)
        {
            if (VlScriptHeader.VlsInitAssembly(this.state, assembly.assembly) == VlScriptHeader.VLS_ERR)
            {
                this.host.RaiseException();
            }
        }

        public void PushParameter(IntPtr data, int size)
        {
            if (VlScriptHeader.VlsPush(this.state, data, size) == VlScriptHeader.VLS_ERR)
            {
                this.host.RaiseException();
            }
        }

        public void PrepareToCall(VlsBasicFunction function, IntPtr resultAddress)
        {
            if (VlScriptHeader.VlsSetFunction(this.state, function.Assembly.assembly, function.Entry, resultAddress) == VlScriptHeader.VLS_ERR)
            {
                this.host.RaiseException();
            }
        }

        public void Call()
        {
            if (VlScriptHeader.VlsExecute(this.state) == VlScriptHeader.VLS_ERR)
            {
                this.host.RaiseException();
            }
        }
    }
}
