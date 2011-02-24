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
        public TurtleIdeForm()
        {
            InitializeComponent();
            InitializeRibbon(toolStripRibbon);
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
                    RibbonButtonGroup group = new RibbonButtonGroup();
                    group.Name = "Turtle";
                    tab.TabPanel.Groups.Add(group);
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.New;
                        button.Name = "New";
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Open;
                        button.Name = "Open";
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Save;
                        button.Name = "Save";
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.SaveAs;
                        button.Name = "Save As";
                        group.BigItems.Add(button);
                    }
                }
                {
                    RibbonButtonGroup group = new RibbonButtonGroup();
                    group.Name = "Clipboard";
                    tab.TabPanel.Groups.Add(group);
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Cut;
                        button.Name = "Cut";
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Copy;
                        button.Name = "Copy";
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = TurtleIdeRibbon.Paste;
                        button.Name = "Paste";
                        group.BigItems.Add(button);
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
