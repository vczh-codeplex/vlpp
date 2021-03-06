﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Threading;

namespace NodeServiceGuard
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            bool creatednew = false;
            using (Semaphore semaphore = new Semaphore(0, 1, "NodeServerGuardedService", out creatednew))
            {
                if (creatednew)
                {
                    MainForm mainForm = new MainForm();
                    Application.Run(mainForm);
                }
                else
                {
                    MessageBox.Show("Node Service Guard already has a running instance.", "Node Service Guard v1.0");
                }
            }
        }
    }
}
