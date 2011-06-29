using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace SimulatorViewer
{
    static class SimulatorAPI
    {
        /*--------------------------------------------------------------------------------------*/

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateSimulator(bool enableFSAA);

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void DestroySimulator(IntPtr simulator);

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetSimulatorBuffer(IntPtr simulator, int width, int height);

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void ClearSimulatorBuffer(IntPtr simulator);

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr GetSimulatorBitmap(IntPtr simulator);

        /*--------------------------------------------------------------------------------------*/

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateScene1();

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateScene2();

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateScene3();

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateScene4();

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateScene5();

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateScene6();

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateScene7();

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateScene8();

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void RotateScene(IntPtr scene, double xy, double yz, double zx);

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void DestroyScene(IntPtr scene);

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetTriangleCount(IntPtr scene);

        /*--------------------------------------------------------------------------------------*/

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr CreateDirectTrayRenderer(int level);

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void DestroyRenderer(IntPtr scene);

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int DebuggerIntersect(IntPtr simulator, IntPtr scene, IntPtr renderer, int x, int y);

        [DllImport("SimulatorCore.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void RenderScene(IntPtr simulator, IntPtr scene, IntPtr renderer);
    }
}
