using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace ModelEditor
{
    static class ModelEditorCore
    {
        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateEditorWindow(IntPtr editorControl);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr DestroyEditorWindow(IntPtr window);
    }
}
