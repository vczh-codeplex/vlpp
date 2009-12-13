using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.UI;
using AutoShell.Services;
using System.Windows.Forms;

namespace AutoShell.ServiceImpl
{
    internal class UIShell : IUIShell
    {
        private class ToolWindowEntry
        {
            public Guid Guid { get; set; }
            public Type ToolWindowType { get; set; }
            public ToolWindowDockType DefaultDock { get; set; }
            public bool MultipleInstances { get; set; }
            public List<ToolWindowPane> Instances { get; private set; }

            public ToolWindowEntry()
            {
                this.Instances = new List<ToolWindowPane>();
            }
        }

        private IServiceContainer serviceContainer;
        private Dictionary<Guid, ShellGroup> groups = new Dictionary<Guid, ShellGroup>();
        private List<ShellToolStripController> toolStripControllers = new List<ShellToolStripController>();
        private Dictionary<Guid, ToolWindowEntry> toolWindowEntries = new Dictionary<Guid, ToolWindowEntry>();
        private bool isInitialized = false;
        private MainForm mainForm = null;
        private int usedCreateOrder = 0;

        public UIShell(IServiceContainer serviceContainer)
        {
            this.serviceContainer = serviceContainer;
        }

        public bool Initialized
        {
            get
            {
                return this.isInitialized;
            }
            set
            {
                this.isInitialized = value;
            }
        }

        public MainForm ShellForm
        {
            get
            {
                return this.mainForm;
            }
            set
            {
                this.mainForm = value;
            }
        }

        #region IUIShell Members

        public ShellGroup RegisterShellGroup(Guid guid, ShellGroup group)
        {
            if (this.groups.ContainsKey(guid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionDuplicatedGroup, guid));
            }
            else
            {
                this.groups.Add(guid, group);
            }
            return group;
        }

        public ShellGroup GetShellGroup(Guid guid)
        {
            if (this.groups.ContainsKey(guid))
            {
                return this.groups[guid];
            }
            else
            {
                return null;
            }
        }

        public void RegisterToolStripController(ShellToolStripController controller)
        {
            if (!this.toolStripControllers.Contains(controller))
            {
                this.toolStripControllers.Add(controller);
                controller.OnQueryStatus();
            }
        }

        public void UnregisterToolStripController(ShellToolStripController controller)
        {
            if (this.toolStripControllers.Contains(controller))
            {
                this.toolStripControllers.Remove(controller);
            }
        }

        public void QueryUIStatus()
        {
            foreach (ShellToolStripController controller in this.toolStripControllers)
            {
                controller.OnQueryStatus();
            }
            foreach (ToolWindowEntry entry in this.toolWindowEntries.Values)
            {
                foreach (ToolWindowPane pane in entry.Instances)
                {
                    pane.QueryStatus();
                }
            }
        }

        public bool IsInitialized
        {
            get
            {
                return this.isInitialized;
            }
        }

        public void RegisterToolBar(Guid guid, ShellGroup group, string name)
        {
            this.mainForm.RegisterToolBar(guid, group, name);
        }

        public void RegisterToolWindow(Guid guid, Type toolWindowType, ToolWindowDockType defaultDock, bool multipleInstances)
        {
            if (this.toolWindowEntries.ContainsKey(guid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionDuplicatedToolWindiw, guid));
            }
            else
            {
                ToolWindowEntry entry = new ToolWindowEntry();
                entry.Guid = guid;
                entry.ToolWindowType = toolWindowType;
                entry.DefaultDock = defaultDock;
                entry.MultipleInstances = multipleInstances;
                this.toolWindowEntries.Add(guid, entry);
            }
        }

        public ToolWindowPane[] FindToolWindow()
        {
            List<ToolWindowPane> panes = new List<ToolWindowPane>();
            foreach (ToolWindowEntry entry in this.toolWindowEntries.Values)
            {
                panes.AddRange(entry.Instances);
            }
            return panes.ToArray();
        }

        public ToolWindowPane[] FindToolWindow(Guid guid)
        {
            return FindToolWindow(guid, null);
        }

        public ToolWindowPane[] FindToolWindow(Guid guid, string token)
        {
            if (this.toolWindowEntries.ContainsKey(guid))
            {
                if (token == null)
                {
                    return this.toolWindowEntries[guid].Instances.ToArray();
                }
                else
                {
                    List<ToolWindowPane> requiredPanes = new List<ToolWindowPane>();
                    foreach (ToolWindowPane pane in this.toolWindowEntries[guid].Instances)
                    {
                        if (pane.Token == token)
                        {
                            requiredPanes.Add(pane);
                        }
                    }
                    return requiredPanes.ToArray();
                }
            }
            else
            {
                throw new InvalidOperationException(Strings.ExceptionToolWindowNotExists);
            }
        }

        public ToolWindowPane CreateToolWindow(Guid guid)
        {
            return CreateToolWindow(guid, null);
        }

        public ToolWindowPane CreateToolWindow(Guid guid, string token)
        {
            if (this.toolWindowEntries.ContainsKey(guid))
            {
                ToolWindowEntry entry = this.toolWindowEntries[guid];
                if (!entry.MultipleInstances && entry.Instances.Count > 0)
                {
                    return entry.Instances[0];
                }
                else
                {
                    ToolWindowPane pane = (ToolWindowPane)entry.ToolWindowType.GetConstructor(new Type[] { }).Invoke(new object[] { });
                    pane.CreateOrder = this.usedCreateOrder++;
                    pane.Initialize(this.serviceContainer, guid, entry.MultipleInstances, token);
                    pane.DockType = entry.DefaultDock;
                    if (!entry.MultipleInstances)
                    {
                        pane.Destroy += new EventHandler(pane_Destroy);
                    }
                    entry.Instances.Add(pane);
                    return pane;
                }
            }
            else
            {
                throw new InvalidOperationException(Strings.ExceptionToolWindowNotExists);
            }
        }

        private void pane_Destroy(object sender, EventArgs e)
        {
            ToolWindowPane pane = sender as ToolWindowPane;
            this.toolWindowEntries[pane.ToolWindowGuid].Instances.Remove(pane);
        }

        public IToolWindowHost CreateToolWindowHost(ToolWindowDockType dockType)
        {
            return this.mainForm.CreateToolWindowHost(dockType);
        }

        public ToolWindowPane GetActiveToolWindow()
        {
            return this.mainForm.GetActiveToolWindow();
        }

        public ToolWindowPane GetActiveTabbedToolWindow()
        {
            return this.mainForm.GetActiveTabbedToolWindow();
        }

        public DialogResult ShowMessage(string message)
        {
            return MessageBox.Show(message, this.mainForm.Text);
        }

        public DialogResult ShowMessage(string message, MessageBoxButtons buttons)
        {
            return MessageBox.Show(message, this.mainForm.Text, buttons);
        }

        public DialogResult ShowMessage(string message, MessageBoxButtons buttons, MessageBoxIcon icons)
        {
            return MessageBox.Show(message, this.mainForm.Text, buttons, icons);
        }

        #endregion
    }
}