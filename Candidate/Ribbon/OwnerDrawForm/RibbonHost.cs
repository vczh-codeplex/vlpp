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
                        button.Image = ButtonImages.Paste;
                        button.Name = "粘帖";
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.Image = ButtonImages.Paste;
                        button.Name = "我还要粘帖";
                        group.BigItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.Image = ButtonImages.CutHS;
                        button.Name = "剪切";
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.Image = ButtonImages.CopyHS;
                        button.Name = "复制";
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.Image = ButtonImages.Color_linecolor;
                        button.Name = "格式刷";
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.Image = ButtonImages.Window1;
                        button.Name = "一个窗口";
                        button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                        button.ToggleGroup = 0;
                        button.Toggled = true;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.Image = ButtonImages.Window2;
                        button.Name = "两个窗口";
                        button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                        button.ToggleGroup = 0;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.Image = ButtonImages.Window3;
                        button.Name = "三个窗口";
                        button.ButtonStyle = RibbonButtonStyle.GroupedToggleButton;
                        button.ToggleGroup = 0;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.Image = ButtonImages.Arrow1;
                        button.Name = "第一箭头";
                        button.ButtonStyle = RibbonButtonStyle.ToggleButton;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.Image = ButtonImages.Arrow2;
                        button.Name = "第二箭头";
                        button.ButtonStyle = RibbonButtonStyle.ToggleButton;
                        group.SmallItems.Add(button);
                    }
                    {
                        RibbonButton button = new RibbonButton();
                        button.Image = ButtonImages.Arrow3;
                        button.Name = "第三箭头";
                        button.ButtonStyle = RibbonButtonStyle.ToggleButton;
                        group.SmallItems.Add(button);
                    }
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "字体";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "段落";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "样式";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "编辑";
                    tab.Groups.Add(group);
                }
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "插入";
                this.Ribbon.Tabs.Add(tab);
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "页";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "表格";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "插图";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "链接";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "页眉和页脚";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "文本";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "符号";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "特殊符号";
                    tab.Groups.Add(group);
                }
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "页面布局";
                this.Ribbon.Tabs.Add(tab);
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "主题";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "页面设置";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "稿纸";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "页面背景";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "段落";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "排列";
                    tab.Groups.Add(group);
                }
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "引用";
                this.Ribbon.Tabs.Add(tab);
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "目录";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "脚注";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "引文与书目";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "题注";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "索引";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "引文目录";
                    tab.Groups.Add(group);
                }
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "设计";
                this.Ribbon.Tabs.Add(tab);
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "表格样式选项";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "表样式";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "绘图边框";
                    tab.Groups.Add(group);
                }
            }
            {
                RibbonTab tab = new RibbonTab();
                tab.Name = "布局";
                this.Ribbon.Tabs.Add(tab);
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "表";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "行和列";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "合并";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "单元格大小";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "对齐方式";
                    tab.Groups.Add(group);
                }
                {
                    RibbonGroup group = new RibbonGroup();
                    group.Name = "数据";
                    tab.Groups.Add(group);
                }
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
    }
}
