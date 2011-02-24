using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Developer.RibbonFramework.RibbonElements;

namespace VlTurtle
{
    public partial class TurtleIdeForm : Developer.RibbonFramework.RibbonAeroForm
    {
        private TurtleIdeFormContent content = null;

        public TurtleIdeForm()
        {
            this.content = new TurtleIdeFormContent();
            InitializeComponent();
            InitializeRibbon(toolStripRibbon, content);
            CreateRibbon();
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
                            panel.ControlItems.Add(button);
                        }
                    }
                    {
                        RibbonControlGroupPanel panel = new RibbonControlGroupPanel();
                        panel.Policy = RibbonControlSizingPolicy.BigSmall;
                        group.Panels.Add(panel);
                        {
                            RibbonButton button = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.Open;
                            button.SmallImage = TurtleIdeRibbon.Open;
                            button.Name = "Open";
                            panel.ControlItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.Save;
                            button.SmallImage = TurtleIdeRibbon.Save;
                            button.Name = "Save";
                            panel.ControlItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.SaveAs;
                            button.SmallImage = TurtleIdeRibbon.SaveAs;
                            button.Name = "Save As";
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
                            RibbonButton button = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.Run;
                            button.SmallImage = TurtleIdeRibbon.Run;
                            button.Name = "Run";
                            panel.ControlItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.Stop;
                            button.SmallImage = TurtleIdeRibbon.Stop;
                            button.Name = "Stop";
                            panel.ControlItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.BigImage = TurtleIdeRibbon.Paste;
                            button.SmallImage = TurtleIdeRibbon.Paste;
                            button.Name = "Paste";
                            panel.ControlItems.Add(button);
                        }
                    }
                }
                {
                    RibbonButtonGroup group = new RibbonButtonGroup();
                    group.Name = "Edit";
                    tab.TabPanel.Groups.Add(group);
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Undo;
                        button.Name = "Undo";
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Redo;
                        button.Name = "Redo";
                        group.BigItems.Add(button);
                    }
                }
                {
                    RibbonButtonGroup group = new RibbonButtonGroup();
                    group.Name = "Debug";
                    tab.TabPanel.Groups.Add(group);
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Run;
                        button.Name = "Run";
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Stop;
                        button.Name = "Stop";
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
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = TurtleIdeRibbon.Open;
                button.Name = "Open Turtle";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = TurtleIdeRibbon.Save;
                button.Name = "Save Turtle";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = TurtleIdeRibbon.SaveAs;
                button.Name = "Save Turtle As";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.Name = "Exit";
                menu.MenuItems.Add(button);
            }
            return menu;
        }
    }
}
