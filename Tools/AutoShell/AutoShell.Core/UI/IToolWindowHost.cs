using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace AutoShell.UI
{
    public interface IToolWindowHost
    {
        void SetIcon(Image icon);
        void SetCaption(string caption);
        void AttachPane(ToolWindowPane toolWindowPane);
        void DetachPane();
        void Activate();
    }
}
