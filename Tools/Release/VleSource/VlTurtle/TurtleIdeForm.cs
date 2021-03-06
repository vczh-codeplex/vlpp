﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Developer.RibbonFramework.RibbonElements;

namespace VlTurtle
{
    public partial class TurtleIdeForm : Developer.RibbonFramework.RibbonAeroForm
    {
        private TurtleIdeFormContent content = null;
        private RibbonButton buttonCut;
        private RibbonButton buttonCopy;
        private RibbonButton buttonPaste;
        private RibbonButton buttonUndo;
        private RibbonButton buttonRedo;
        private RibbonButton buttonRun;
        private RibbonButton buttonStop;

        public TurtleIdeForm()
        {
            this.content = new TurtleIdeFormContent();
            InitializeComponent();
            InitializeRibbon(toolStripRibbon, content);
            CreateRibbon();

            this.content.ButtonStateUpdated += new EventHandler(content_ButtonStateUpdated);
            this.content.OperationNew();
        }

        private void content_ButtonStateUpdated(object sender, EventArgs e)
        {
            this.buttonCut.Enabled = this.content.AvailableCut();
            this.buttonCopy.Enabled = this.content.AvailableCopy();
            this.buttonPaste.Enabled = this.content.AvailablePaste();
            this.buttonUndo.Enabled = this.content.AvailableUndo();
            this.buttonRedo.Enabled = this.content.AvailableRedo();
            this.toolRibbonUndo.Enabled = this.content.AvailableUndo();
            this.toolRibbonRedo.Enabled = this.content.AvailableRedo();
            this.buttonRun.Enabled = this.content.AvailableRun();
            this.buttonStop.Enabled = this.content.AvailableStop();
            if (this.Text != this.content.WindowTitle)
            {
                this.Text = this.content.WindowTitle;
                this.Refresh();
            }
            UpdateRibbon(true);
        }

        private void CreateRibbon()
        {
            {
                RibbonButtonTab tab = new RibbonButtonTab();
                tab.Name = "Start";
                tab.DropDown = new RibbonDropDownMenu(this.Ribbon)
                {
                    Menu = CreateStartMenu()
                };
                this.Ribbon.Tabs.Add(tab);
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "Home";
                this.Ribbon.Tabs.Add(tab);
                {
                    RibbonControlGroup group = new RibbonControlGroup();
                    group.Name = "Script";
                    tab.TabPanel.Groups.Add(group);
                    {
                        RibbonControlGroupPanel panel = new RibbonControlGroupPanel();
                        panel.Policy = RibbonControlSizingPolicy.Big;
                        group.Panels.Add(panel);
                        {
                            RibbonButton button = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.New;
                            button.SmallImage = TurtleIdeRibbon.New;
                            button.Name = "New";
                            button.Executed += (s, e) => this.content.OperationNew();
                            panel.ControlItems.Add(button);
                        }
                    }
                    {
                        RibbonControlGroupPanel panel = new RibbonControlGroupPanel();
                        panel.Policy = RibbonControlSizingPolicy.BigSmall;
                        group.Panels.Add(panel);
                        {
                            RibbonButton button = new RibbonButton();
                            button.ButtonStyle = RibbonButtonStyle.SplitButton;
                            button.BigImage = TurtleIdeRibbon.Open;
                            button.SmallImage = TurtleIdeRibbon.Open;
                            button.Name = "Open";
                            button.DropDown = CreateOpenMenu(this.Ribbon);
                            button.Executed += (s, e) => this.content.OperationOpen();
                            panel.ControlItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.Save;
                            button.SmallImage = TurtleIdeRibbon.Save;
                            button.Name = "Save";
                            button.Executed += (s, e) => this.content.OperationSave();
                            panel.ControlItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.SaveAs;
                            button.SmallImage = TurtleIdeRibbon.SaveAs;
                            button.Name = "Save As";
                            button.Executed += (s, e) => this.content.OperationSaveAs();
                            panel.ControlItems.Add(button);
                        }
                    }
                }
                {
                    RibbonControlGroup group = new RibbonControlGroup();
                    group.Name = "Clipboard";
                    tab.TabPanel.Groups.Add(group);
                    {
                        RibbonControlGroupPanel panel = new RibbonControlGroupPanel();
                        panel.Policy = RibbonControlSizingPolicy.BigSmall;
                        group.Panels.Add(panel);
                        {
                            RibbonButton button = this.buttonCut = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.Cut;
                            button.SmallImage = TurtleIdeRibbon.Cut;
                            button.Name = "Cut";
                            button.Executed += (s, e) => this.content.OperationCut();
                            panel.ControlItems.Add(button);
                        }
                        {
                            RibbonButton button = this.buttonCopy = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.Copy;
                            button.SmallImage = TurtleIdeRibbon.Copy;
                            button.Name = "Copy";
                            button.Executed += (s, e) => this.content.OperationCopy();
                            panel.ControlItems.Add(button);
                        }
                        {
                            RibbonButton button = this.buttonPaste = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.Paste;
                            button.SmallImage = TurtleIdeRibbon.Paste;
                            button.Name = "Paste";
                            button.Executed += (s, e) => this.content.OperationPaste();
                            panel.ControlItems.Add(button);
                        }
                    }
                }
                {
                    RibbonButtonGroup group = new RibbonButtonGroup();
                    group.Name = "Edit";
                    tab.TabPanel.Groups.Add(group);
                    {
                        RibbonButton button = this.buttonUndo = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Undo;
                        button.Name = "Undo";
                        button.Executed += (s, e) => this.content.OperationUndo();
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = this.buttonRedo = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Redo;
                        button.Name = "Redo";
                        button.Executed += (s, e) => this.content.OperationRedo();
                        group.BigItems.Add(button);
                    }
                }
                {
                    RibbonButtonGroup group = new RibbonButtonGroup();
                    group.Name = "Debug";
                    tab.TabPanel.Groups.Add(group);
                    {
                        RibbonButton button = this.buttonRun = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Run;
                        button.Name = "Run";
                        button.Executed += (s, e) => this.content.OperationRun();
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = this.buttonStop = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Stop;
                        button.Name = "Stop";
                        button.Executed += (s, e) => this.content.OperationStop();
                        group.BigItems.Add(button);
                    }
                }
            }
            UpdateRibbon(false);
        }

        private RibbonMenu CreateStartMenu()
        {
            RibbonMenu menu = new RibbonMenu();
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = TurtleIdeRibbon.New;
                button.Name = "New Turtle";
                button.Executed += (s, e) => this.content.OperationNew();
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = TurtleIdeRibbon.Open;
                button.Name = "Open Turtle";
                button.Executed += (s, e) => this.content.OperationOpen();
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = TurtleIdeRibbon.Save;
                button.Name = "Save Turtle";
                button.Executed += (s, e) => this.content.OperationSave();
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = TurtleIdeRibbon.SaveAs;
                button.Name = "Save Turtle As";
                button.Executed += (s, e) => this.content.OperationSaveAs();
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.Name = "Exit";
                button.Executed += (s, e) => this.Close();
                menu.MenuItems.Add(button);
            }
            return menu;
        }

        private RibbonDropDownMenu CreateOpenMenu(RibbonContainer container)
        {
            string[] resources = typeof(TurtleIdeForm).Assembly.GetManifestResourceNames()
                .Where(s => s.EndsWith(".turtle.txt"))
                .ToArray();
            RibbonMenu menu = new RibbonMenu();
            foreach (string resource in resources)
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = TurtleIdeRibbon.New;
                button.Name = resource.Substring("VlTurtle.Sample.NativeX.".Length);
                button.Executed += new EventHandler(ribbonOpenSample_Click);
                menu.MenuItems.Add(button);
            }
            return new RibbonDropDownMenu(container)
            {
                Menu = menu
            };
        }

        private void ribbonOpenSample_Click(object sender, EventArgs e)
        {
            string fullName = "VlTurtle.Sample.NativeX." + (sender as RibbonMenuButton).Name;
            this.content.OperationOpenSample(fullName);
        }

        private void toolRibbonSave_Click(object sender, EventArgs e)
        {
            this.content.OperationSave();
        }

        private void toolRibbonUndo_Click(object sender, EventArgs e)
        {
            this.content.OperationUndo();
        }

        private void toolRibbonRedo_Click(object sender, EventArgs e)
        {
            this.content.OperationRedo();
        }
    }
}
