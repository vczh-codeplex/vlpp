using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AutoShell.UI;
using AutoShell.ServiceImpl;
using System.Reflection;
using AutoShell.PublicShell;

namespace AutoShell.Services
{
    public class Shell
    {
        private ServiceContainer serviceContainer;
        private UIShell uiShell;
        private MainForm mainForm;

        public Shell()
        {
            Control.CheckForIllegalCrossThreadCalls = false;
            this.serviceContainer = new ServiceContainer();
            this.uiShell = new UIShell(this.serviceContainer);
            this.serviceContainer.RegisterService<IUIShell>(uiShell);
            this.mainForm = new MainForm(this.serviceContainer);
            this.uiShell.ShellForm = this.mainForm;
            this.mainForm.SetupEnvironment();

            IOutputService outputService = this.serviceContainer.QueryService<IOutputService>();
            outputService.Print(PublicShellCore.LoggerOutputChannel, string.Format(Strings.LogPackageLoaded, typeof(Shell).FullName));
        }

        public void LoadPackage(Assembly packageAssembly)
        {
            IOutputService outputService = this.serviceContainer.QueryService<IOutputService>();
            foreach (Type type in packageAssembly.GetTypes())
            {
                if (typeof(Package).IsAssignableFrom(type))
                {
                    Package package = (Package)type.GetConstructor(new Type[] { }).Invoke(new object[] { });
                    package.Services = this.serviceContainer;
                    package.Initialize();
                    this.serviceContainer.RecordPackage(package);
                    outputService.Print(PublicShellCore.LoggerOutputChannel, string.Format(Strings.LogPackageLoaded, type.FullName));
                }
            }
        }

        public void Initialize()
        {
            this.mainForm.Initialize();
            this.uiShell.Initialized = true;
        }

        public IServiceContainer Services
        {
            get
            {
                return this.serviceContainer;
            }
        }

        public Form Context
        {
            get
            {
                return this.mainForm;
            }
        }
    }
}
