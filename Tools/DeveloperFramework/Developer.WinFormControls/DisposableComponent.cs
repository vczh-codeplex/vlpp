using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace Developer.WinFormControls
{
    public class DisposableComponent : IComponent
    {
        public IDisposable disposable { get; private set; }

        public DisposableComponent(IDisposable disposable)
        {
            this.disposable = disposable;
        }

        public event EventHandler Disposed;

        public ISite Site{get;set;}

        public void Dispose()
        {
            this.disposable.Dispose();
            if (this.Disposed != null)
            {
                this.Disposed(this, new EventArgs());
            }
        }
    }
}
