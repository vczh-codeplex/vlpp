using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace ModelEditor
{
    static class ModelEditorCore
    {
        /***************************************************************
         * Editor Window
        ***************************************************************/

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern IntPtr CreateEditorWindow(IntPtr editorControl, string workingDirectory);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void DestroyEditorWindow(IntPtr window);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void RenderEditorWindow(IntPtr window);

        /***************************************************************
         * Model
        ***************************************************************/

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void DestroyModel(IntPtr window, IntPtr model);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern IntPtr CreateModelCube(IntPtr window);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern IntPtr CreateModelSphere(IntPtr window, int rows, int cols);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern IntPtr CreateModelCylinder(IntPtr window, int rows, int cols);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern IntPtr CreateModelCone(IntPtr window, int rows, int cols);

        /***************************************************************
         * View
        ***************************************************************/

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void ResetView(IntPtr window);

        /***************************************************************
         * Editor Mode
        ***************************************************************/

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void EditorModeSelection(IntPtr window);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void EditorModeFaceSelection(IntPtr window);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void EditorModeVertexSelection(IntPtr window);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void EditorModeTranslation(IntPtr window);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void EditorModeRotation(IntPtr window);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void EditorModeScaling(IntPtr window);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void EditorAxisGlobal(IntPtr window);

        [DllImport("ModelEditorCore.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void EditorAxisLocal(IntPtr window);
    }
}
