using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.UI
{
    public interface IDocumentTabbedWindow
    {
        void OnPrepareShellItems(ShellGroup documentMenuGroup, ShellGroup toolWindowMenuGroup);
    }
}
