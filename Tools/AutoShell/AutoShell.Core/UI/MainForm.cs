using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AutoShell.Services;
using AutoShell.PublicShell;
using AutoShell.UI.ToolWindow;
using System.Drawing.Drawing2D;
using System.Runtime.InteropServices;
using System.Reflection;
using AutoShell.DataModel;
using AutoShell.UI.Option;

namespace AutoShell.UI
{
    internal partial class MainForm : Form
    {
        private class ToolBarEntry
        {
            public Guid ToolBarGuid { get; set; }
            public ShellGroup ToolBarGroup { get; set; }
            public string Name { get; set; }
            public ToolStrip ToolBar { get; set; }
            public ShellToolStripController Controller { get; set; }
            public ShellItem ToolBarControllerItem { get; set; }
        }

        [DataSerializable]
        private class MainFormOption
        {
            public static readonly Guid OptionKey = new Guid("{E9DC2B49-69F0-4b25-BEDD-C4B715628083}");

            private List<Guid>[] toolWindows = new List<Guid>[6];
            private Dictionary<int, List<Guid>>[] toolBars = new Dictionary<int, List<Guid>>[4];

            public MainFormOption()
            {
                Clear();
            }

            public void Clear()
            {
                for (int i = 0; i < this.toolWindows.Length; i++)
                {
                    this.toolWindows[i] = new List<Guid>();
                }
                for (int i = 0; i < this.toolBars.Length; i++)
                {
                    this.toolBars[i] = new Dictionary<int, List<Guid>>();
                }
            }

            [DataSerializerVisible]
            public List<Guid>[] ToolWindows
            {
                get
                {
                    return this.toolWindows;
                }
                set
                {
                    this.toolWindows = value;
                }
            }

            [DataSerializerVisible]
            public Dictionary<int, List<Guid>>[] ToolBars
            {
                get
                {
                    return this.toolBars;
                }
                set
                {
                    this.toolBars = value;
                }
            }
        }

        private IServiceContainer serviceContainer;
        private ShellToolStripController mainMenuController = null;
        private ToolWindowPane activeToolWindow = null;
        private ToolWindowPane lastActiveDockedToolWindow = null;
        private List<ToolBarEntry> toolBars = new List<ToolBarEntry>();
        private ContextMenuStrip contextMenuToolBars = null;
        private ShellToolStripController contextMenuToolBarsController = null;
        private ShellItem viewToolbarsMenu;
        private OptionForm optionForm = null;

        private IntPtr nextClipboardViewerHandle = IntPtr.Zero;

        public MainForm(IServiceContainer serviceContainer)
        {
            InitializeComponent();
            this.serviceContainer = serviceContainer;

            tabLeft.Alignment = TabAlignment.Bottom;
            tabRight.Alignment = TabAlignment.Bottom;
            tabTop.Alignment = TabAlignment.Bottom;
            tabBottom.Alignment = TabAlignment.Top;
            tabDocument.Alignment = TabAlignment.Top;

            tabLeft.Multiline = false;
            tabRight.Multiline = false;
            tabTop.Multiline = false;
            tabBottom.Multiline = false;
            tabDocument.Multiline = false;

            this.nextClipboardViewerHandle = SetClipboardViewer(this.Handle);

            Assembly entry = Assembly.GetEntryAssembly();
            if (entry != null)
            {
                object[] title = entry.GetCustomAttributes(typeof(AssemblyTitleAttribute), false);
                if (title.Length > 0)
                {
                    this.Text = (title[0] as AssemblyTitleAttribute).Title;
                }
            }
        }

        public void SetupEnvironment()
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            RegisterToolWindows(uiShell);
            RegisterMenuItems(uiShell);
            RegisterCommands(this.serviceContainer.QueryService<ICommandService>());
            RearrangeToolWindowAreas();
            RegisterOptions();
        }

        public void Initialize()
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            foreach (ToolBarEntry entry in this.toolBars)
            {
                entry.ToolBar = new ToolStrip();
                entry.ToolBar.Text = entry.Name;
                entry.ToolBar.Tag = entry;
                entry.Controller = new ShellToolStripController(this.serviceContainer, entry.ToolBar, entry.ToolBarGroup);

                ShellItem itemOpenToolBar = new ShellItem(ShellItemKind.Button, 0x0000);
                itemOpenToolBar.Text = entry.ToolBar.Text;
                itemOpenToolBar.Tag = entry;
                itemOpenToolBar.Executed += new EventHandler(itemOpenToolBar_Executed);
                this.viewToolbarsMenu.SubGroup.AddEntry(itemOpenToolBar);

                entry.ToolBarControllerItem = itemOpenToolBar;
            }

            ShellGroup mainGroup = uiShell.GetShellGroup(PublicShellCore.MainMenuGroup);
            this.mainMenuController = new ShellToolStripController(this.serviceContainer, menuStripMain, mainGroup);
            this.contextMenuToolBars = new ContextMenuStrip();
            this.contextMenuToolBarsController = new ShellToolStripController(this.serviceContainer, this.contextMenuToolBars, this.viewToolbarsMenu.SubGroup);

            toolStripContainer.LeftToolStripPanel.ContextMenuStrip = this.contextMenuToolBars;
            toolStripContainer.RightToolStripPanel.ContextMenuStrip = this.contextMenuToolBars;
            toolStripContainer.TopToolStripPanel.ContextMenuStrip = this.contextMenuToolBars;
            toolStripContainer.BottomToolStripPanel.ContextMenuStrip = this.contextMenuToolBars;

            LoadOptions();
        }

        #region UIShell Helper

        public void RegisterToolBar(Guid guid, ShellGroup group, string name)
        {
            ToolBarEntry entry = new ToolBarEntry();
            entry.ToolBarGuid = guid;
            entry.ToolBarGroup = group;
            entry.Name = name;
            this.toolBars.Add(entry);
        }

        public ToolWindowPane GetActiveToolWindow()
        {
            return this.activeToolWindow;
        }

        public ToolWindowPane GetActiveTabbedToolWindow()
        {
            if (tabDocument.TabPages.Count == 0)
            {
                return null;
            }
            else
            {
                return (tabDocument.SelectedTab as ToolWindowTabPage).AssociatedToolWindow;
            }
        }

        public void SetToolWindowActive(ToolWindowPane pane)
        {
            this.activeToolWindow = pane;
            if (pane.DockType != ToolWindowDockType.Floating)
            {
                this.lastActiveDockedToolWindow = pane;
            }
            this.serviceContainer.QueryService<IUIShell>().QueryUIStatus();
        }

        public void SetToolWindowNotActive(ToolWindowPane pane)
        {
            if (this.lastActiveDockedToolWindow == this.activeToolWindow)
            {
                if (this.activeToolWindow == pane)
                {
                    this.activeToolWindow = null;
                    this.lastActiveDockedToolWindow = null;
                }
            }
            else
            {
                if (this.activeToolWindow == pane)
                {
                    this.activeToolWindow = null;
                    if (pane.DockType == ToolWindowDockType.Floating && this.lastActiveDockedToolWindow != null)
                    {
                        this.lastActiveDockedToolWindow.Open();
                    }
                }
            }
            this.serviceContainer.QueryService<IUIShell>().QueryUIStatus();
        }

        public IToolWindowHost CreateToolWindowHost(ToolWindowDockType dockType)
        {
            switch (dockType)
            {
                case ToolWindowDockType.Floating:
                    return new ToolWindowForm(this);
                case ToolWindowDockType.Left:
                    {
                        ToolWindowTabPage page = new ToolWindowTabPage(this);
                        tabLeft.TabPages.Add(page);
                        page.ImageIndex = 0;
                        RearrangeToolWindowAreas();
                        return page;
                    }
                case ToolWindowDockType.Right:
                    {
                        ToolWindowTabPage page = new ToolWindowTabPage(this);
                        tabRight.TabPages.Add(page);
                        page.ImageIndex = 0;
                        RearrangeToolWindowAreas();
                        return page;
                    }
                case ToolWindowDockType.Top:
                    {
                        ToolWindowTabPage page = new ToolWindowTabPage(this);
                        tabTop.TabPages.Add(page);
                        page.ImageIndex = 0;
                        RearrangeToolWindowAreas();
                        return page;
                    }
                case ToolWindowDockType.Bottom:
                    {
                        ToolWindowTabPage page = new ToolWindowTabPage(this);
                        tabBottom.TabPages.Add(page);
                        page.ImageIndex = 0;
                        RearrangeToolWindowAreas();
                        return page;
                    }
                case ToolWindowDockType.Document:
                    {
                        DocumentWindowTabPage page = new DocumentWindowTabPage(this);
                        tabDocument.TabPages.Add(page);
                        page.ImageIndex = 0;
                        RearrangeToolWindowAreas();
                        return page;
                    }
                default:
                    return null;
            }
        }

        public void RearrangeToolWindowAreas()
        {
            if (tabLeft.TabPages.Count == 0)
            {
                splitLeft.SplitterDistance = splitLeft.Panel1MinSize;
                splitLeft.IsSplitterFixed = true;
            }
            else
            {
                if (splitLeft.SplitterDistance == splitLeft.Panel1MinSize)
                {
                    splitLeft.SplitterDistance = toolStripContainer.ContentPanel.Width / 4;
                }
                splitLeft.IsSplitterFixed = false;
            }

            if (tabRight.TabPages.Count == 0)
            {
                splitRight.SplitterDistance = splitRight.Width - splitRight.SplitterWidth - splitRight.Panel2MinSize;
                splitRight.IsSplitterFixed = true;
            }
            else
            {
                if (splitRight.SplitterDistance == splitRight.Width - splitRight.SplitterWidth - splitRight.Panel2MinSize)
                {
                    splitRight.SplitterDistance = splitRight.Width - toolStripContainer.ContentPanel.Width / 4;
                }
                splitRight.IsSplitterFixed = false;
            }

            if (tabTop.TabPages.Count == 0)
            {
                splitTop.SplitterDistance = splitTop.Panel1MinSize;
                splitTop.IsSplitterFixed = true;
            }
            else
            {
                if (splitTop.SplitterDistance == splitTop.Panel1MinSize)
                {
                    splitTop.SplitterDistance = toolStripContainer.ContentPanel.Height / 4;
                }
                splitTop.IsSplitterFixed = false;
            }

            if (tabBottom.TabPages.Count == 0)
            {
                splitBottom.SplitterDistance = splitBottom.Height - splitBottom.SplitterWidth - splitBottom.Panel2MinSize;
                splitBottom.IsSplitterFixed = true;
            }
            else
            {
                if (splitBottom.SplitterDistance == splitBottom.Height - splitBottom.SplitterWidth - splitBottom.Panel2MinSize)
                {
                    splitBottom.SplitterDistance = splitBottom.Height - toolStripContainer.ContentPanel.Height / 4;
                }
                splitBottom.IsSplitterFixed = false;
            }

            if (tabDocument.TabPages.Count == 0)
            {
                tabDocument.Visible = false;
                this.toolStripContainer.ContentPanel.BackColor = SystemColors.AppWorkspace;
            }
            else
            {
                tabDocument.Visible = true;
                this.toolStripContainer.ContentPanel.BackColor = SystemColors.Window;
            }
        }

        #endregion

        #region Configuration

        private void RegisterToolWindows(IUIShell uiShell)
        {
            uiShell.RegisterToolWindow(PublicShellCore.OutputWindow, typeof(OutputWindowPane), ToolWindowDockType.Bottom, false);
            uiShell.CreateToolWindow(PublicShellCore.OutputWindow);

            uiShell.RegisterToolWindow(PublicShellCore.CommandWindow, typeof(CommandWindowPane), ToolWindowDockType.Bottom, false);
            uiShell.CreateToolWindow(PublicShellCore.CommandWindow);

            this.optionForm = new OptionForm(this.serviceContainer);
        }

        private void RegisterMenuItems(IUIShell uiShell)
        {
            ShellGroup mainGroup = uiShell.RegisterShellGroup(PublicShellCore.MainMenuGroup, new ShellGroup(0x0000));
            {
                ShellItem fileMenu = new ShellItem(ShellItemKind.Button, 0x0000);
                fileMenu.Text = Strings.MenuTextFile;
                mainGroup.AddEntry(fileMenu);
                ShellGroup fileGroup = uiShell.RegisterShellGroup(PublicShellCore.FileMenuGroup, fileMenu.SubGroup);
                {
                    ShellItem fileExitMenu = new ShellItem(ShellItemKind.Button, 0x0100);
                    fileExitMenu.Text = Strings.MenuTextFileExit;
                    fileExitMenu.Executed += new EventHandler(fileExitMenu_Executed);
                    fileGroup.AddEntry(fileExitMenu);
                }
            }
            {
                ShellItem viewMenu = new ShellItem(ShellItemKind.Button, 0x0020);
                viewMenu.Text = Strings.MenuTextView;
                mainGroup.AddEntry(viewMenu);
                ShellGroup viewGroup = uiShell.RegisterShellGroup(PublicShellCore.ViewMenuGroup, viewMenu.SubGroup);
                {
                    ShellGroup viewWindowsGroup = uiShell.RegisterShellGroup(PublicShellCore.ViewWindowsMenuGroup, new ShellGroup(0x0100));
                    viewGroup.AddEntry(viewWindowsGroup);
                    {
                        ShellItem viewCommandMenu = new ShellItem(ShellItemKind.Button, 0x0001);
                        viewCommandMenu.Text = Strings.MenuTextViewCommandWindow;
                        viewCommandMenu.Icon = Icons.CommandWindow;
                        viewCommandMenu.Executed += new EventHandler(viewCommandMenu_Executed);
                        viewWindowsGroup.AddEntry(viewCommandMenu);
                    }
                    {
                        ShellItem viewOutputMenu = new ShellItem(ShellItemKind.Button, 0x0002);
                        viewOutputMenu.Text = Strings.MenuTextViewOutputWindow;
                        viewOutputMenu.Icon = Icons.OutputWindow;
                        viewOutputMenu.Executed += new EventHandler(viewOutputMenu_Executed);
                        viewWindowsGroup.AddEntry(viewOutputMenu);
                    }
                }
                {
                    ShellGroup viewToolbarsGroup = uiShell.RegisterShellGroup(PublicShellCore.ViewToolbarsMenuGroup, new ShellGroup(0x0200));
                    viewGroup.AddEntry(viewToolbarsGroup);
                    {
                        this.viewToolbarsMenu = new ShellItem(ShellItemKind.Button, 0x0001);
                        this.viewToolbarsMenu.Text = Strings.MenuTextViewToolbars;
                        viewToolbarsGroup.AddEntry(this.viewToolbarsMenu);
                    }
                }
            }
            {
                ShellItem toolsMenu = new ShellItem(ShellItemKind.Button, 0x0200);
                toolsMenu.Text = Strings.MenuTextTools;
                mainGroup.AddEntry(toolsMenu);
                ShellGroup toolsGroup = uiShell.RegisterShellGroup(PublicShellCore.ToolsMenuGroup, toolsMenu.SubGroup);
                {
                    ShellItem toolsOptionsMenu = new ShellItem(ShellItemKind.Button, 0x0200);
                    toolsOptionsMenu.Text = Strings.MenuTextToolsOptions;
                    toolsOptionsMenu.Executed += new EventHandler(toolsOptionsMenu_Executed);
                    toolsGroup.AddEntry(toolsOptionsMenu);
                }
            }
            {
                ShellItem windowMenu = new ShellItem(ShellItemKind.Button, 0x0210);
                windowMenu.Text = Strings.MenuTextWindow;
                mainGroup.AddEntry(windowMenu);
                ShellGroup windowGroup = uiShell.RegisterShellGroup(PublicShellCore.WindowMenuGroup, windowMenu.SubGroup);
                {
                    ShellGroup dockingGroup = new ShellGroup(0x0100);
                    windowGroup.AddEntry(dockingGroup);

                    ShellItem windowDockFloating = new ShellItem(ShellItemKind.Button, 0x0010);
                    windowDockFloating.Text = Strings.MenuTextDockFloating;
                    windowDockFloating.Icon = Icons.DockFloating;
                    windowDockFloating.QueryStatus += new EventHandler(windowDockFloating_QueryStatus);
                    windowDockFloating.Executed += new EventHandler(windowDockFloating_Executed);
                    dockingGroup.AddEntry(windowDockFloating);

                    ShellItem windowDockLeft = new ShellItem(ShellItemKind.Button, 0x0020);
                    windowDockLeft.Text = Strings.MenuTextDockLeft;
                    windowDockLeft.Icon = Icons.DockLeft;
                    windowDockLeft.QueryStatus += new EventHandler(windowDockLeft_QueryStatus);
                    windowDockLeft.Executed += new EventHandler(windowDockLeft_Executed);
                    dockingGroup.AddEntry(windowDockLeft);

                    ShellItem windowDockRight = new ShellItem(ShellItemKind.Button, 0x0020);
                    windowDockRight.Text = Strings.MenuTextDockRight;
                    windowDockRight.Icon = Icons.DockRight;
                    windowDockRight.QueryStatus += new EventHandler(windowDockRight_QueryStatus);
                    windowDockRight.Executed += new EventHandler(windowDockRight_Executed);
                    dockingGroup.AddEntry(windowDockRight);

                    ShellItem windowDockTop = new ShellItem(ShellItemKind.Button, 0x0020);
                    windowDockTop.Text = Strings.MenuTextDockTop;
                    windowDockTop.Icon = Icons.DockTop;
                    windowDockTop.QueryStatus += new EventHandler(windowDockTop_QueryStatus);
                    windowDockTop.Executed += new EventHandler(windowDockTop_Executed);
                    dockingGroup.AddEntry(windowDockTop);

                    ShellItem windowDockBottom = new ShellItem(ShellItemKind.Button, 0x0020);
                    windowDockBottom.Text = Strings.MenuTextDockBottom;
                    windowDockBottom.Icon = Icons.DockBottom;
                    windowDockBottom.QueryStatus += new EventHandler(windowDockBottom_QueryStatus);
                    windowDockBottom.Executed += new EventHandler(windowDockBottom_Executed);
                    dockingGroup.AddEntry(windowDockBottom);

                    ShellItem windowDockDocument = new ShellItem(ShellItemKind.Button, 0x0020);
                    windowDockDocument.Text = Strings.MenuTextDockDocument;
                    windowDockDocument.Icon = Icons.DockDocument;
                    windowDockDocument.QueryStatus += new EventHandler(windowDockDocument_QueryStatus);
                    windowDockDocument.Executed += new EventHandler(windowDockDocument_Executed);
                    dockingGroup.AddEntry(windowDockDocument);

                    ShellItem windowDockHide = new ShellItem(ShellItemKind.Button, 0x0020);
                    windowDockHide.Text = Strings.MenuTextDockHide;
                    windowDockHide.QueryStatus += new EventHandler(windowDockHide_QueryStatus);
                    windowDockHide.Executed += new EventHandler(windowDockHide_Executed);
                    dockingGroup.AddEntry(windowDockHide);
                }
            }
            {
                ShellItem helpMenu = new ShellItem(ShellItemKind.Button, 0x0220);
                helpMenu.Text = Strings.MenuTextHelp;
                mainGroup.AddEntry(helpMenu);
                ShellGroup helpGroup = uiShell.RegisterShellGroup(PublicShellCore.HelpMenuGroup, helpMenu.SubGroup);
                {
                    ShellItem helpAboutMenu = new ShellItem(ShellItemKind.Button, 0x0100);
                    helpAboutMenu.QueryStatus += new EventHandler(helpAboutMenu_QueryStatus);
                    helpAboutMenu.Executed += new EventHandler(helpAboutMenu_Executed);
                    helpGroup.AddEntry(helpAboutMenu);
                }
            }
        }

        private void RegisterCommands(ICommandService commandService)
        {
            CommandItem fileExit = new CommandItem();
            fileExit.Name = PublicShellCore.Command_File_Exit;
            fileExit.Description = Strings.CommandDescription_File_Exit;
            fileExit.Execute += (sender, e) => { this.fileExitMenu_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(fileExit);

            CommandItem viewCommand = new CommandItem();
            viewCommand.Name = PublicShellCore.Command_View_CommandWindow;
            viewCommand.Description = Strings.CommandDescription_View_CommandWindow;
            viewCommand.Execute += (sender, e) => { this.viewCommandMenu_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(viewCommand);

            CommandItem viewOutput = new CommandItem();
            viewOutput.Name = PublicShellCore.Command_View_OutputWindow;
            viewOutput.Description = Strings.CommandDescription_View_OutputWindow;
            viewOutput.Execute += (sender, e) => { this.viewOutputMenu_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(viewOutput);

            CommandItem viewOptions = new CommandItem();
            viewOptions.Name = PublicShellCore.Command_Tools_Options;
            viewOptions.Description = Strings.CommandDescription_Tools_Options;
            viewOptions.Execute += (sender, e) => { this.toolsOptionsMenu_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(viewOptions);
        }

        private void RegisterOptions()
        {
            IOptionService optionService = this.serviceContainer.QueryService<IOptionService>();
            optionService.RegisterOption(MainFormOption.OptionKey, typeof(MainFormOption));
            optionService.RegisterOption(EmptyOption.OptionKey, typeof(EmptyOption));
            optionService.RegisterOptionEditor(EmptyOption.OptionKey, PublicShellCore.EnvironmentOptions, Strings.OptionNameEnvironment, new EmptyOptionEditor());
        }

        #endregion

        #region MainForm Option

        private void LoadOptions()
        {
            IOptionService optionService = this.serviceContainer.QueryService<IOptionService>();
            MainFormOption option = optionService.GetOption(MainFormOption.OptionKey) as MainFormOption;

            foreach (ToolBarEntry entry in this.toolBars)
            {
                if (entry.ToolBar.Parent != null)
                {
                    entry.ToolBar.Parent.Controls.Remove(entry.ToolBar);
                }
            }
            ToolStripPanel[] panels = new ToolStripPanel[] { toolStripContainer.TopToolStripPanel, toolStripContainer.BottomToolStripPanel, toolStripContainer.LeftToolStripPanel, toolStripContainer.RightToolStripPanel };
            for (int i = 0; i < panels.Length; i++)
            {
                foreach (KeyValuePair<int, List<Guid>> pair in option.ToolBars[i])
                {
                    foreach (Guid guid in pair.Value)
                    {
                        ToolBarEntry toolBarEntry = this.toolBars.Where(entry => entry.ToolBarGuid == guid).FirstOrDefault();
                        if (toolBarEntry != null)
                        {
                            panels[i].Join(toolBarEntry.ToolBar, pair.Key);
                            toolBarEntry.ToolBarControllerItem.Checked = true;
                        }
                    }
                }
            }

            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowDockType[] docks = new ToolWindowDockType[] { ToolWindowDockType.Top, ToolWindowDockType.Bottom, ToolWindowDockType.Left, ToolWindowDockType.Right, ToolWindowDockType.Document, ToolWindowDockType.Floating };
            for (int i = 0; i < docks.Length; i++)
            {
                foreach (Guid guid in option.ToolWindows[i])
                {
                    try
                    {
                        ToolWindowPane[] panes = uiShell.FindToolWindow(guid);
                        if (panes.Length > 0)
                        {
                            if (!panes[0].MultiplyInstances)
                            {
                                panes[0].DockType = docks[i];
                                panes[0].Open();
                            }
                        }
                    }
                    catch (InvalidOperationException)
                    {
                    }
                }
            }
        }

        private void SaveOptions()
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            IOptionService optionService = this.serviceContainer.QueryService<IOptionService>();
            MainFormOption option = optionService.GetOption(MainFormOption.OptionKey) as MainFormOption;

            option.ToolWindows = new TabControl[] { tabTop, tabBottom, tabLeft, tabRight, tabDocument, null }
                .Select(tabControl => tabControl == null ? null :
                    tabControl.TabPages
                    .Cast<ToolWindowTabPage>()
                    .Select(page => page.AssociatedToolWindow.ToolWindowGuid)
                    .ToList()
                ).ToArray();
            option.ToolWindows[5] =
                uiShell.FindToolWindow()
                .Where(pane => pane.DockType == ToolWindowDockType.Floating && pane.Visible)
                .Select(pane => pane.ToolWindowGuid)
                .ToList();

            option.ToolBars = new ToolStripPanel[] { toolStripContainer.TopToolStripPanel, toolStripContainer.BottomToolStripPanel, toolStripContainer.LeftToolStripPanel, toolStripContainer.RightToolStripPanel }
                .Select(panel =>
                    {
                        KeyValuePair<int, List<Guid>>[] items =
                            panel.Rows
                            .Select((row, index) =>
                                new KeyValuePair<int, List<Guid>>(
                                    index,
                                    row.Controls
                                    .Where(control => control.Tag is ToolBarEntry)
                                    .Select(control => (control.Tag as ToolBarEntry).ToolBarGuid)
                                    .ToList()
                                    )
                            )
                            .ToArray();
                        Dictionary<int, List<Guid>> toolBar = new Dictionary<int, List<Guid>>();
                        foreach (KeyValuePair<int, List<Guid>> item in items)
                        {
                            if (item.Value.Count > 0)
                            {
                                toolBar.Add(item.Key, item.Value);
                            }
                        }
                        return toolBar;
                    }
                ).ToArray();
            optionService.SaveOptions();
        }

        #endregion

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (this.IsHandleCreated && m.HWnd == this.Handle)
            {
                switch (m.Msg)
                {
                    case 0x030D://WM_CHANGECBCHAIN
                        if (this.nextClipboardViewerHandle == m.WParam)
                        {
                            this.nextClipboardViewerHandle = m.LParam;
                        }
                        m.Result = IntPtr.Zero;
                        break;
                    case 0x0308://WM_DRAWCLIPBOARD
                        {
                            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
                            uiShell.QueryUIStatus();
                        }
                        break;
                }
            }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            SaveOptions();
            while (tabDocument.TabPages.Count > 0)
            {
                ToolWindowTabPage page = tabDocument.SelectedTab as ToolWindowTabPage;
                if (!page.AssociatedToolWindow.Close())
                {
                    e.Cancel = true;
                    break;
                }
            }
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            ChangeClipboardChain(this.Handle, this.nextClipboardViewerHandle);
        }

        private void tabToolWindow_Enter(object sender, EventArgs e)
        {
            TabControl tabControl = sender as TabControl;
            if (tabControl.TabPages.Count != 0)
            {
                ToolWindowTabPage page = tabControl.SelectedTab as ToolWindowTabPage;
                if (page != null)
                {
                    page.Focus();
                }
            }
        }

        #region Menu Item Handling

        private void itemOpenToolBar_Executed(object sender, EventArgs e)
        {
            ShellItem item = sender as ShellItem;
            ToolBarEntry entry = item.Tag as ToolBarEntry;
            if (entry.ToolBar.Parent == null)
            {
                ToolStripPanel panel = toolStripContainer.TopToolStripPanel;
                panel.Join(entry.ToolBar, panel.Rows.Length);
                item.Checked = true;
            }
            else
            {
                entry.ToolBar.Parent.Controls.Remove(entry.ToolBar);
                item.Checked = false;
            }
        }

        private void fileExitMenu_Executed(object sender, EventArgs e)
        {
            Close();
        }

        private void viewCommandMenu_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.FindToolWindow(PublicShellCore.CommandWindow)[0];
            pane.Open();
        }

        private void viewOutputMenu_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.FindToolWindow(PublicShellCore.OutputWindow)[0];
            pane.Open();
        }

        private void toolsOptionsMenu_Executed(object sender, EventArgs e)
        {
            this.optionForm.ShowDialog();
        }

        private void helpAboutMenu_QueryStatus(object sender, EventArgs e)
        {
            (sender as ShellItem).Text = string.Format(Strings.MenuTextHelpAbout, this.Text);
        }

        private void helpAboutMenu_Executed(object sender, EventArgs e)
        {
            using (AboutForm form = new AboutForm(this.serviceContainer))
            {
                form.ShowDialog();
            }
        }

        private void tabToolWindow_SelectedIndexChanged(object sender, EventArgs e)
        {
            TabControl tabControl = sender as TabControl;
            if (tabControl.SelectedIndex == -1)
            {
                tabControl.ContextMenuStrip = null;
            }
            else
            {
                ToolWindowTabPage page = tabControl.SelectedTab as ToolWindowTabPage;
                tabControl.ContextMenuStrip = page.DockingMenu;
                page.Focus();
            }
        }

        #endregion

        #region Docking Menu Handling

        private void windowDockHide_QueryStatus(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            ShellItem item = sender as ShellItem;

            item.Enabled = pane != null;
            item.Text = string.Format(Strings.MenuTextDockHide, pane == null ? "" : pane.Caption);
        }

        private void windowDockDocument_QueryStatus(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            ShellItem item = sender as ShellItem;

            item.Enabled = pane != null && (pane.EnableModifyDockType || pane.DockType == ToolWindowDockType.Document);
            item.Checked = pane != null && pane.DockType == ToolWindowDockType.Document;
        }

        private void windowDockBottom_QueryStatus(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            ShellItem item = sender as ShellItem;

            item.Enabled = pane != null && (pane.EnableModifyDockType || pane.DockType == ToolWindowDockType.Bottom);
            item.Checked = pane != null && pane.DockType == ToolWindowDockType.Bottom;
        }

        private void windowDockTop_QueryStatus(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            ShellItem item = sender as ShellItem;

            item.Enabled = pane != null && (pane.EnableModifyDockType || pane.DockType == ToolWindowDockType.Top);
            item.Checked = pane != null && pane.DockType == ToolWindowDockType.Top;
        }

        private void windowDockRight_QueryStatus(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            ShellItem item = sender as ShellItem;

            item.Enabled = pane != null && (pane.EnableModifyDockType || pane.DockType == ToolWindowDockType.Right);
            item.Checked = pane != null && pane.DockType == ToolWindowDockType.Right;
        }

        private void windowDockLeft_QueryStatus(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            ShellItem item = sender as ShellItem;

            item.Enabled = pane != null && (pane.EnableModifyDockType || pane.DockType == ToolWindowDockType.Left);
            item.Checked = pane != null && pane.DockType == ToolWindowDockType.Left;
        }

        private void windowDockFloating_QueryStatus(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            ShellItem item = sender as ShellItem;

            item.Enabled = pane != null && (pane.EnableModifyDockType || pane.DockType == ToolWindowDockType.Floating);
            item.Checked = pane != null && pane.DockType == ToolWindowDockType.Floating;
        }

        private void windowDockHide_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            if (pane != null)
            {
                pane.Close();
            }
        }

        private void windowDockDocument_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            if (pane != null)
            {
                pane.DockType = ToolWindowDockType.Document;
            }
        }

        private void windowDockBottom_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            if (pane != null)
            {
                pane.DockType = ToolWindowDockType.Bottom;
            }
        }

        private void windowDockTop_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            if (pane != null)
            {
                pane.DockType = ToolWindowDockType.Top;
            }
        }

        private void windowDockRight_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            if (pane != null)
            {
                pane.DockType = ToolWindowDockType.Right;
            }
        }

        private void windowDockLeft_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            if (pane != null)
            {
                pane.DockType = ToolWindowDockType.Left;
            }
        }

        private void windowDockFloating_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            if (pane != null)
            {
                pane.DockType = ToolWindowDockType.Floating;
            }
        }

        #endregion

        [DllImport("user32")]
        private extern static IntPtr SetClipboardViewer(IntPtr hWnd);

        [DllImport("user32")]
        private extern static bool ChangeClipboardChain(IntPtr hWndRemove, IntPtr hWndNext);
    }
}