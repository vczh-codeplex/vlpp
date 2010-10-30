using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Developer.RibbonFramework.RibbonElements;

namespace Developer.RibbonFramework
{
    public partial class RibbonHost : RibbonAeroForm
    {
        public RibbonHost()
        {
            InitializeComponent();
            InitializeRibbon(toolStripAero);
            CreateRibbon();
        }

        private void CreateRibbon()
        {
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "开始";
                this.Ribbon.Tabs.Add(tab);
                {
                    RibbonButtonGroup group = new RibbonButtonGroup();
                    group.Name = "剪贴板";
                    tab.Groups.Add(group);
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = ButtonImages.Paste;
                        button.Name = "粘帖1";
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = ButtonImages.Paste;
                        button.Name = "粘帖2";
                        button.Enabled = false;
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.SmallImage = ButtonImages.CutHS;
                        button.Name = "剪切";
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.SmallImage = ButtonImages.CopyHS;
                        button.Name = "复制";
                        button.ButtonStyle = RibbonButtonStyle.SplitButton;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.SmallImage = ButtonImages.Color_linecolor;
                        button.Name = "格式刷";
                        button.ButtonStyle = RibbonButtonStyle.DropDownButton;
                        group.SmallItems.Add(button);
                    }
                }
                {
                    RibbonButtonGroup group = new RibbonButtonGroup();
                    group.Name = "字体";
                    tab.Groups.Add(group);
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = ButtonImages.Paste;
                        button.Name = "粘帖3";
                        button.ButtonStyle = RibbonButtonStyle.DropDownButton;
                        button.DropDown = new RibbonDropDownGroup(this.Ribbon)
                        {
                            Group = CreateDropDownGroup1()
                        };
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.BigImage = ButtonImages.Paste;
                        button.Name = "粘帖4";
                        button.ButtonStyle = RibbonButtonStyle.SplitButton;
                        button.DropDown = new RibbonDropDownGroup(this.Ribbon)
                        {
                            Group = CreateDropDownGroup2()
                        };
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.SmallImage = ButtonImages.Window1;
                        button.Name = "一个窗口";
                        button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                        button.ToggleGroup = 0;
                        button.Toggled = true;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.SmallImage = ButtonImages.Window2;
                        button.Name = "两个窗口";
                        button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                        button.ToggleGroup = 0;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.SmallImage = ButtonImages.Window3;
                        button.Name = "三个窗口";
                        button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                        button.ToggleGroup = 0;
                        button.Enabled = false;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.SmallImage = ButtonImages.Arrow1;
                        button.Name = "第一箭头";
                        button.ButtonStyle = RibbonButtonStyle.ToggleButton;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.SmallImage = ButtonImages.Arrow2;
                        button.Name = "第二箭头";
                        button.ButtonStyle = RibbonButtonStyle.ToggleButton;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.SmallImage = ButtonImages.Arrow3;
                        button.Name = "第三箭头";
                        button.ButtonStyle = RibbonButtonStyle.ToggleButton;
                        group.SmallItems.Add(button);
                    }
                }
                {
                    RibbonToolStripGroup group = new RibbonToolStripGroup();
                    group.Name = "段落";
                    tab.Groups.Add(group);
                    {
                        RibbonToolStrip tool = new RibbonToolStrip();
                        tool.NormalLine = 0;
                        tool.CompactLine = 0;
                        group.ToolStrips.Add(tool);
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.ItemNumber;
                            tool.ToolItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.ItemDot;
                            button.ButtonStyle = RibbonButtonStyle.DropDownButton;
                            button.Enabled = false;
                            tool.ToolItems.Add(button);
                        }
                    }
                    {
                        RibbonToolStrip tool = new RibbonToolStrip();
                        tool.NormalLine = 0;
                        tool.CompactLine = 0;
                        group.ToolStrips.Add(tool);
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.TabIn;
                            tool.ToolItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.TabOut;
                            tool.ToolItems.Add(button);
                        }
                    }
                    {
                        RibbonToolStrip tool = new RibbonToolStrip();
                        tool.NormalLine = 1;
                        tool.CompactLine = 1;
                        group.ToolStrips.Add(tool);
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.AlignLeft;
                            button.Toggled = true;
                            button.ToggleGroup = 0;
                            button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                            tool.ToolItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.AlignCenter;
                            button.ToggleGroup = 0;
                            button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                            tool.ToolItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.AlignRight;
                            button.ToggleGroup = 0;
                            button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                            tool.ToolItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.AlignFull;
                            button.ToggleGroup = 0;
                            button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                            tool.ToolItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.AlignExpand;
                            button.ToggleGroup = 0;
                            button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                            button.Enabled = false;
                            tool.ToolItems.Add(button);
                        }
                    }
                    {
                        RibbonToolStrip tool = new RibbonToolStrip();
                        tool.NormalLine = 1;
                        tool.CompactLine = 1;
                        group.ToolStrips.Add(tool);
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.LinePadding;
                            tool.ToolItems.Add(button);
                        }
                    }
                    {
                        RibbonToolStrip tool = new RibbonToolStrip();
                        tool.NormalLine = 0;
                        tool.CompactLine = 2;
                        group.ToolStrips.Add(tool);
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.TableBorder;
                            button.ButtonStyle = RibbonButtonStyle.DropDownButton;
                            button.DropDown = new RibbonDropDownMenu(this.Ribbon)
                            {
                                Menu = CreateTableMenu()
                            };
                            tool.ToolItems.Add(button);
                        }
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = ToolImages.SortAscending;
                            button.ButtonStyle = RibbonButtonStyle.SplitButton;
                            tool.ToolItems.Add(button);
                        }
                    }
                }
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "插入";
                this.Ribbon.Tabs.Add(tab);
                {
                    RibbonControlGroup group = new RibbonControlGroup();
                    group.Name = "好复杂啊";
                    tab.Groups.Add(group);

                    RibbonControlSizingPolicy[] policies = new RibbonControlSizingPolicy[]{
                        RibbonControlSizingPolicy.Big,
                        RibbonControlSizingPolicy.BigSmall,
                        RibbonControlSizingPolicy.BigCompact,
                        RibbonControlSizingPolicy.Small,
                        RibbonControlSizingPolicy.SmallCompact,
                        RibbonControlSizingPolicy.Compact
                    };
                    Image[] smalls = new Image[] { ControlImages.a1, ControlImages.b1, ControlImages.c1, ControlImages.d1, ControlImages.e1, ControlImages.f1 };
                    Image[] bigs = new Image[] { ControlImages.a2, ControlImages.b2, ControlImages.c2, ControlImages.d2, ControlImages.e2, ControlImages.f2 };
                    RibbonButtonStyle[] styles = new RibbonButtonStyle[] { RibbonButtonStyle.PushButton, RibbonButtonStyle.DropDownButton, RibbonButtonStyle.SplitButton };
                    for (int i = 0; i < 6; i++)
                    {
                        RibbonControlGroupPanel panel = new RibbonControlGroupPanel();
                        panel.Policy = policies[i];
                        group.Panels.Add(panel);
                        for (int j = 0; j < 3; j++)
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = smalls[i];
                            button.BigImage = bigs[i];
                            button.ButtonStyle = styles[j];
                            button.Name = panel.Policy.ToString() + " " + (i + 1).ToString();
                            panel.ControlItems.Add(button);
                        }
                    }
                }
                {
                    RibbonControlGroup group = new RibbonControlGroup();
                    group.Name = "好复杂啊";

                    RibbonControlSizingPolicy[] policies = new RibbonControlSizingPolicy[]{
                        RibbonControlSizingPolicy.Big,
                        RibbonControlSizingPolicy.BigSmall,
                        RibbonControlSizingPolicy.BigCompact,
                        RibbonControlSizingPolicy.Small,
                        RibbonControlSizingPolicy.SmallCompact,
                        RibbonControlSizingPolicy.Compact
                    };
                    Image[] smalls = new Image[] { ControlImages.a1, ControlImages.b1, ControlImages.c1, ControlImages.d1, ControlImages.e1, ControlImages.f1 };
                    Image[] bigs = new Image[] { ControlImages.a2, ControlImages.b2, ControlImages.c2, ControlImages.d2, ControlImages.e2, ControlImages.f2 };
                    RibbonButtonStyle[] styles = new RibbonButtonStyle[] { RibbonButtonStyle.PushButton, RibbonButtonStyle.DropDownButton, RibbonButtonStyle.SplitButton };
                    for (int i = 0; i < 6; i++)
                    {
                        RibbonControlGroupPanel panel = new RibbonControlGroupPanel();
                        panel.Policy = policies[i];
                        group.Panels.Add(panel);
                        for (int j = 0; j < 3; j++)
                        {
                            RibbonButton button = new RibbonButton();
                            button.SmallImage = smalls[i];
                            button.BigImage = bigs[i];
                            button.ButtonStyle = styles[j];
                            button.Name = panel.Policy.ToString() + " " + (i + 1).ToString();
                            panel.ControlItems.Add(button);
                        }
                    }

                    ((tab.Groups[0] as RibbonControlGroup).Panels[0].ControlItems[1] as RibbonButton).DropDown = new RibbonDropDownGroup(this.Ribbon)
                    {
                        Group = group
                    };
                }
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "页面布局";
                this.Ribbon.Tabs.Add(tab);
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "引用";
                this.Ribbon.Tabs.Add(tab);
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "设计";
                this.Ribbon.Tabs.Add(tab);
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "布局";
                this.Ribbon.Tabs.Add(tab);
            }
            {
                RibbonTabGroup group = new RibbonTabGroup();
                group.Name = "表格工具";
                group.FirstIndex = 4;
                group.LastIndex = 5;
                this.Ribbon.TabGroups.Add(group);
            }
            UpdateRibbon(false);
        }

        private RibbonGroup CreateDropDownGroup1()
        {
            RibbonButtonGroup group = new RibbonButtonGroup();
            group.Name = "字体";
            {
                RibbonButton button = new RibbonButton();
                button.BigImage = ButtonImages.Paste;
                button.Name = "粘帖3";
                button.ButtonStyle = RibbonButtonStyle.DropDownButton;
                group.BigItems.Add(button);
            }
            {
                RibbonButton button = new RibbonButton();
                button.BigImage = ButtonImages.Paste;
                button.Name = "粘帖4";
                button.ButtonStyle = RibbonButtonStyle.SplitButton;
                group.BigItems.Add(button);
            }
            {
                RibbonButton button = new RibbonButton();
                button.SmallImage = ButtonImages.Window1;
                button.Name = "一个窗口";
                button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                button.ToggleGroup = 0;
                button.Toggled = true;
                group.SmallItems.Add(button);
            }
            {
                RibbonButton button = new RibbonButton();
                button.SmallImage = ButtonImages.Window2;
                button.Name = "两个窗口";
                button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                button.ToggleGroup = 0;
                group.SmallItems.Add(button);
            }
            {
                RibbonButton button = new RibbonButton();
                button.SmallImage = ButtonImages.Window3;
                button.Name = "三个窗口";
                button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                button.ToggleGroup = 0;
                button.Enabled = false;
                group.SmallItems.Add(button);
            }
            {
                RibbonButton button = new RibbonButton();
                button.SmallImage = ButtonImages.Arrow1;
                button.Name = "第一箭头";
                button.ButtonStyle = RibbonButtonStyle.ToggleButton;
                group.SmallItems.Add(button);
            }
            {
                RibbonButton button = new RibbonButton();
                button.SmallImage = ButtonImages.Arrow2;
                button.Name = "第二箭头";
                button.ButtonStyle = RibbonButtonStyle.ToggleButton;
                group.SmallItems.Add(button);
            }
            {
                RibbonButton button = new RibbonButton();
                button.SmallImage = ButtonImages.Arrow3;
                button.Name = "第三箭头";
                button.ButtonStyle = RibbonButtonStyle.ToggleButton;
                group.SmallItems.Add(button);
            }
            return group;
        }

        private RibbonGroup CreateDropDownGroup2()
        {
            RibbonToolStripGroup group = new RibbonToolStripGroup();
            group.Name = "段落";
            {
                RibbonToolStrip tool = new RibbonToolStrip();
                tool.NormalLine = 0;
                tool.CompactLine = 0;
                group.ToolStrips.Add(tool);
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.ItemNumber;
                    tool.ToolItems.Add(button);
                }
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.ItemDot;
                    button.ButtonStyle = RibbonButtonStyle.DropDownButton;
                    button.Enabled = false;
                    tool.ToolItems.Add(button);
                }
            }
            {
                RibbonToolStrip tool = new RibbonToolStrip();
                tool.NormalLine = 0;
                tool.CompactLine = 0;
                group.ToolStrips.Add(tool);
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.TabIn;
                    tool.ToolItems.Add(button);
                }
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.TabOut;
                    tool.ToolItems.Add(button);
                }
            }
            {
                RibbonToolStrip tool = new RibbonToolStrip();
                tool.NormalLine = 1;
                tool.CompactLine = 1;
                group.ToolStrips.Add(tool);
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.AlignLeft;
                    button.Toggled = true;
                    button.ToggleGroup = 0;
                    button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                    tool.ToolItems.Add(button);
                }
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.AlignCenter;
                    button.ToggleGroup = 0;
                    button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                    tool.ToolItems.Add(button);
                }
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.AlignRight;
                    button.ToggleGroup = 0;
                    button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                    tool.ToolItems.Add(button);
                }
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.AlignFull;
                    button.ToggleGroup = 0;
                    button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                    tool.ToolItems.Add(button);
                }
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.AlignExpand;
                    button.ToggleGroup = 0;
                    button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                    button.Enabled = false;
                    tool.ToolItems.Add(button);
                }
            }
            {
                RibbonToolStrip tool = new RibbonToolStrip();
                tool.NormalLine = 1;
                tool.CompactLine = 1;
                group.ToolStrips.Add(tool);
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.LinePadding;
                    tool.ToolItems.Add(button);
                }
            }
            {
                RibbonToolStrip tool = new RibbonToolStrip();
                tool.NormalLine = 0;
                tool.CompactLine = 2;
                group.ToolStrips.Add(tool);
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.TableBorder;
                    button.ButtonStyle = RibbonButtonStyle.DropDownButton;
                    button.DropDown = new RibbonDropDownMenu(this.Ribbon)
                    {
                        Menu = CreateTableMenu()
                    };
                    tool.ToolItems.Add(button);
                }
                {
                    RibbonButton button = new RibbonButton();
                    button.SmallImage = ToolImages.SortAscending;
                    button.ButtonStyle = RibbonButtonStyle.SplitButton;
                    tool.ToolItems.Add(button);
                }
            }
            return group;
        }

        private RibbonMenu CreateTableMenu()
        {
            RibbonMenu menu = new RibbonMenu();
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.TableNone;
                button.Name = "没有边框";
                button.DropDown = new RibbonDropDownMenu(this.Ribbon)
                {
                    Menu = CreateAlignMenu()
                };
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.TableLeft;
                button.Name = "左边有边框";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.TableRight;
                button.Name = "右边有边框";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.TableBottom;
                button.Name = "下面有边框";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.TableBorder;
                button.Name = "周围都是边框";
                button.Enabled = false;
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.TableAll;
                button.Name = "连里面都是边框";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuSeparator separator = new RibbonMenuSeparator();
                menu.MenuItems.Add(separator);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.SortAscending;
                button.Name = "不知道为什么要排序";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.SortDescending;
                button.Name = "不知道为什么要反着排序";
                button.DropDown = new RibbonDropDownMenu(this.Ribbon)
                {
                    Menu = CreateSortMenu()
                };
                menu.MenuItems.Add(button);
            }
            return menu;
        }

        private RibbonMenu CreateAlignMenu()
        {
            RibbonMenu menu = new RibbonMenu();
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.AlignLeft;
                button.Name = "左对齐";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.AlignCenter;
                button.Name = "中间对齐";
                button.DropDown = new RibbonDropDownMenu(this.Ribbon)
                {
                    Menu = CreateSortMenu()
                };
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.AlignRight;
                button.Name = "右对齐";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.AlignFull;
                button.Name = "全部对齐";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.AlignExpand;
                button.Name = "不知道是什么对齐";
                menu.MenuItems.Add(button);
            }
            return menu;
        }

        private RibbonMenu CreateSortMenu()
        {
            RibbonMenu menu = new RibbonMenu();
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.SortAscending;
                button.Name = "我还是不知道为什么要排序";
                menu.MenuItems.Add(button);
            }
            {
                RibbonMenuButton button = new RibbonMenuButton();
                button.SmallImage = ToolImages.SortDescending;
                button.Name = "我还是不知道为什么要反着排序";
                menu.MenuItems.Add(button);
            }
            return menu;
        }
    }
}
