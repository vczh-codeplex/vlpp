using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using AutoShell.Services;

namespace AutoShell.UI
{
    /// <summary>
    /// 停靠位置
    /// </summary>
    public enum ToolWindowDockType
    {
        /// <summary>
        /// 浮动
        /// </summary>
        Floating,
        /// <summary>
        /// 左侧
        /// </summary>
        Left,
        /// <summary>
        /// 右侧
        /// </summary>
        Right,
        /// <summary>
        /// 上侧
        /// </summary>
        Top,
        /// <summary>
        /// 下侧
        /// </summary>
        Bottom,
        /// <summary>
        /// 文档区
        /// </summary>
        Document
    }

    /// <summary>
    /// 工具窗口对象
    /// </summary>
    public abstract class ToolWindowPane : IDisposable
    {
        private IServiceContainer serviceContainer = null;
        private Guid guid = Guid.Empty;
        private IToolWindowHost toolWindowHost = null;
        private Image icon = null;
        private string caption = string.Empty;
        private ShellGroup toolBarGroup = null;
        private ToolWindowDockType dockType = ToolWindowDockType.Floating;
        private bool enableModifyDockType = true;
        private bool multipleInstances = true;
        private string token = null;

        private ToolStrip toolStrip = null;
        private ToolStripContainer toolStripContainer = null;
        private ShellToolStripController toolStripController = null;
        private ShellGroup dockingMenuGroup = null;

        private ShellItem dockFloating = null;
        private ShellItem dockLeft = null;
        private ShellItem dockRight = null;
        private ShellItem dockTop = null;
        private ShellItem dockBottom = null;
        private ShellItem dockDocument = null;
        private ShellItem dockHide = null;
        private bool hideDockHide = false;

        /// <summary>
        /// 创建顺序
        /// </summary>
        public int CreateOrder { get; internal set; }
        /// <summary>
        /// 工具窗口控件
        /// </summary>
        public abstract Control ToolWindowControl { get; }
        /// <summary>
        /// 缺省尺寸
        /// </summary>
        public abstract Size DefaultSize { get; }
        /// <summary>
        /// 初始化
        /// </summary>
        protected abstract void OnInitialize();
        /// <summary>
        /// 打开
        /// </summary>
        protected abstract void OnOpening();
        /// <summary>
        /// 请求关闭
        /// </summary>
        /// <param name="cancel">true为取消关闭</param>
        protected abstract void OnClosing(ref bool cancel);
        /// <summary>
        /// 关闭
        /// </summary>
        protected abstract void OnClosed();
        /// <summary>
        /// 更新UI状态
        /// </summary>
        protected abstract void OnQueryStatus();

        /// <summary>
        /// 工具窗口销毁事件
        /// </summary>
        public event EventHandler Destroy;

        public ToolWindowPane()
        {
        }

        /// <summary>
        /// 图标
        /// </summary>
        public Image Icon
        {
            get
            {
                return this.icon;
            }
            protected set
            {
                this.icon = value;
                if (this.toolWindowHost != null)
                {
                    this.toolWindowHost.SetIcon(this.icon);
                }
            }
        }

        /// <summary>
        /// 名称
        /// </summary>
        public string Caption
        {
            get
            {
                return this.caption;
            }
            protected set
            {
                this.caption = value;
                if (this.toolWindowHost != null)
                {
                    this.toolWindowHost.SetCaption(this.caption);
                }
            }
        }

        /// <summary>
        /// 工具栏附带的工具条命令组，只可设置一次
        /// </summary>
        public ShellGroup ToolBarGroup
        {
            get
            {
                return this.toolBarGroup;
            }
            protected set
            {
                if (this.toolBarGroup != null)
                {
                    throw new InvalidOperationException(Strings.ExceptionResetToolWindowToolBarGroup);
                }
                else if (value == null)
                {
                    throw new ArgumentNullException(Strings.ExceptionNullToolWindowToolBarGroup, "ToolBarGroup");
                }
                else
                {
                    this.toolBarGroup = value;
                    this.toolStrip = new ToolStrip();
                    this.toolStrip.CanOverflow = true;
                    this.toolStrip.GripStyle = ToolStripGripStyle.Hidden;
                    this.toolStrip.Dock = DockStyle.Fill;
                    this.toolStripContainer.TopToolStripPanel.Controls.Add(this.toolStrip);
                    this.toolStripController = new ShellToolStripController(this.serviceContainer, this.toolStrip, this.toolBarGroup);
                }
            }
        }

        /// <summary>
        /// 停靠位置命令组
        /// </summary>
        public ShellGroup DockingMenuGroup
        {
            get
            {
                return this.dockingMenuGroup;
            }
        }

        /// <summary>
        /// 隐藏“关闭”命令项
        /// </summary>
        public bool HideDockHide
        {
            get
            {
                return this.hideDockHide;
            }
            set
            {
                this.hideDockHide = value;
            }
        }

        /// <summary>
        /// 停靠位置
        /// </summary>
        public ToolWindowDockType DockType
        {
            get
            {
                return this.dockType;
            }
            set
            {
                if (this.dockType != value)
                {
                    this.dockType = value;
                    if (this.toolWindowHost != null)
                    {
                        this.toolWindowHost.DetachPane();
                        this.toolWindowHost = null;
                        OpenInternal(true);
                    }
                }
            }
        }

        /// <summary>
        /// 允许更改停靠位置
        /// </summary>
        public bool EnableModifyDockType
        {
            get
            {
                return this.enableModifyDockType;
            }
            protected set
            {
                this.enableModifyDockType = value;
            }
        }

        /// <summary>
        /// 允许多实例
        /// </summary>
        public bool MultiplyInstances
        {
            get
            {
                return this.multipleInstances;
            }
        }

        /// <summary>
        /// 标记
        /// </summary>
        public string Token
        {
            get
            {
                return this.token;
            }
        }

        /// <summary>
        /// 可见标记
        /// </summary>
        public bool Visible
        {
            get
            {
                return this.toolWindowHost != null;
            }
        }

        /// <summary>
        /// 所属服务容器
        /// </summary>
        public IServiceContainer Services
        {
            get
            {
                return this.serviceContainer;
            }
        }

        /// <summary>
        /// 工具窗口Guid
        /// </summary>
        public Guid ToolWindowGuid
        {
            get
            {
                return this.guid;
            }
        }

        /// <summary>
        /// 打开工具窗口
        /// </summary>
        public void Open()
        {
            OpenInternal(false);
        }

        /// <summary>
        /// 关闭工具窗口
        /// </summary>
        /// <returns></returns>
        public bool Close()
        {
            if (this.toolWindowHost == null)
            {
                return true;
            }
            else
            {
                bool cancel = false;
                OnClosing(ref cancel);
                if (cancel)
                {
                    return false;
                }
                else
                {
                    CloseInternal();
                    return true;
                }
            }
        }

        /// <summary>
        /// 请求更新UI状态
        /// </summary>
        public void QueryStatus()
        {
            if (this.toolStripController != null)
            {
                this.toolStripController.OnQueryStatus();
            }
            OnQueryStatus();
        }

        internal void Initialize(IServiceContainer serviceContainer, Guid guid, bool multipleInstances, string token)
        {
            this.serviceContainer = serviceContainer;
            this.guid = guid;
            this.multipleInstances = multipleInstances;
            this.token = token;

            this.toolStripContainer = new ToolStripContainer();
            this.toolStripContainer.Dock = DockStyle.Fill;
            this.toolStripContainer.TopToolStripPanelVisible = true;
            this.toolStripContainer.LeftToolStripPanelVisible = false;
            this.toolStripContainer.RightToolStripPanelVisible = false;
            this.toolStripContainer.BottomToolStripPanelVisible = false;

            BuildToolWindowMenu();
            OnInitialize();
            this.toolStripContainer.ContentPanel.Controls.Add(this.ToolWindowControl);
            this.ToolWindowControl.Dock = DockStyle.Fill;

            int width = this.ToolWindowControl.MinimumSize.Width;
            int height = this.ToolWindowControl.MinimumSize.Height + this.toolStripContainer.TopToolStripPanel.Height;
            this.toolStripContainer.MinimumSize = new Size(width, height);
        }

        internal Control GetHostControl()
        {
            return this.toolStripContainer;
        }

        private void BuildToolWindowMenu()
        {
            dockingMenuGroup = new ShellGroup(0x0000);
            {
                this.dockFloating = new ShellItem(ShellItemKind.Button, 0x0100);
                this.dockFloating.Text = Strings.MenuTextDockFloating;
                this.dockFloating.Icon = Icons.DockFloating;
                this.dockFloating.QueryStatus += new EventHandler(dockMenu_QueryStatus);
                this.dockFloating.Executed += new EventHandler(dockMenu_Executed);
                dockingMenuGroup.AddEntry(this.dockFloating);
            }
            {
                this.dockLeft = new ShellItem(ShellItemKind.Button, 0x0200);
                this.dockLeft.Text = Strings.MenuTextDockLeft;
                this.dockLeft.Icon = Icons.DockLeft;
                this.dockLeft.QueryStatus += new EventHandler(dockMenu_QueryStatus);
                this.dockLeft.Executed += new EventHandler(dockMenu_Executed);
                dockingMenuGroup.AddEntry(this.dockLeft);
            }
            {
                this.dockRight = new ShellItem(ShellItemKind.Button, 0x0300);
                this.dockRight.Text = Strings.MenuTextDockRight;
                this.dockRight.Icon = Icons.DockRight;
                this.dockRight.QueryStatus += new EventHandler(dockMenu_QueryStatus);
                this.dockRight.Executed += new EventHandler(dockMenu_Executed);
                dockingMenuGroup.AddEntry(this.dockRight);
            }
            {
                this.dockTop = new ShellItem(ShellItemKind.Button, 0x0400);
                this.dockTop.Text = Strings.MenuTextDockTop;
                this.dockTop.Icon = Icons.DockTop;
                this.dockTop.QueryStatus += new EventHandler(dockMenu_QueryStatus);
                this.dockTop.Executed += new EventHandler(dockMenu_Executed);
                dockingMenuGroup.AddEntry(this.dockTop);
            }
            {
                this.dockBottom = new ShellItem(ShellItemKind.Button, 0x0500);
                this.dockBottom.Text = Strings.MenuTextDockBottom;
                this.dockBottom.Icon = Icons.DockBottom;
                this.dockBottom.QueryStatus += new EventHandler(dockMenu_QueryStatus);
                this.dockBottom.Executed += new EventHandler(dockMenu_Executed);
                dockingMenuGroup.AddEntry(this.dockBottom);
            }
            {
                this.dockDocument = new ShellItem(ShellItemKind.Button, 0x0600);
                this.dockDocument.Text = Strings.MenuTextDockDocument;
                this.dockDocument.Icon = Icons.DockDocument;
                this.dockDocument.QueryStatus += new EventHandler(dockMenu_QueryStatus);
                this.dockDocument.Executed += new EventHandler(dockMenu_Executed);
                dockingMenuGroup.AddEntry(this.dockDocument);
            }
            {
                this.dockHide = new ShellItem(ShellItemKind.Button, 0x0700);
                this.dockHide.Text = Strings.MenuTextDockHide;
                this.dockHide.QueryStatus += new EventHandler(dockHide_QueryStatus);
                this.dockHide.Executed += new EventHandler(dockHide_Executed);
                dockingMenuGroup.AddEntry(this.dockHide);
            }
        }

        private void dockHide_Executed(object sender, EventArgs e)
        {
            Close();
        }

        private void dockHide_QueryStatus(object sender, EventArgs e)
        {
            this.dockHide.Text = string.Format(Strings.MenuTextDockHide, this.caption);
            this.dockHide.Visible = !this.hideDockHide;
        }

        private void dockMenu_Executed(object sender, EventArgs e)
        {
            if (sender == this.dockLeft)
            {
                this.DockType = ToolWindowDockType.Left;
            }
            else if (sender == this.dockRight)
            {
                this.DockType = ToolWindowDockType.Right;
            }
            else if (sender == this.dockTop)
            {
                this.DockType = ToolWindowDockType.Top;
            }
            else if (sender == this.dockBottom)
            {
                this.DockType = ToolWindowDockType.Bottom;
            }
            else if (sender == this.dockFloating)
            {
                this.DockType = ToolWindowDockType.Floating;
            }
            else
            {
                this.DockType = ToolWindowDockType.Document;
            }
        }

        private void dockMenu_QueryStatus(object sender, EventArgs e)
        {
            if (sender == this.dockLeft)
            {
                this.dockLeft.Checked = this.dockType == ToolWindowDockType.Left;
                this.dockLeft.Enabled = this.enableModifyDockType || this.dockLeft.Checked;
            }
            else if (sender == this.dockRight)
            {
                this.dockRight.Checked = this.dockType == ToolWindowDockType.Right;
                this.dockRight.Enabled = this.enableModifyDockType || this.dockRight.Checked;
            }
            else if (sender == this.dockTop)
            {
                this.dockTop.Checked = this.dockType == ToolWindowDockType.Top;
                this.dockTop.Enabled = this.enableModifyDockType || this.dockTop.Checked;
            }
            else if (sender == this.dockBottom)
            {
                this.dockBottom.Checked = this.dockType == ToolWindowDockType.Bottom;
                this.dockBottom.Enabled = this.enableModifyDockType || this.dockBottom.Checked;
            }
            else if (sender == this.dockFloating)
            {
                this.dockFloating.Checked = this.dockType == ToolWindowDockType.Floating;
                this.dockFloating.Enabled = this.enableModifyDockType || this.dockFloating.Checked;
            }
            else
            {
                this.dockDocument.Checked = this.dockType == ToolWindowDockType.Document;
                this.dockDocument.Enabled = this.enableModifyDockType || this.dockDocument.Checked;
            }
        }

        private void OpenInternal(bool dockTypeSwitching)
        {
            if (this.toolWindowHost == null)
            {
                this.toolWindowHost = this.serviceContainer.QueryService<IUIShell>().CreateToolWindowHost(this.dockType);
                if (!dockTypeSwitching)
                {
                    OnOpening();
                }
                this.toolWindowHost.SetIcon(this.icon);
                this.toolWindowHost.SetCaption(this.caption);
                this.toolWindowHost.AttachPane(this);
            }
            this.toolWindowHost.Activate();
            this.serviceContainer.QueryService<IUIShell>().QueryUIStatus();
        }

        private void CloseInternal()
        {
            OnClosed();
            this.toolWindowHost.DetachPane();
            this.toolWindowHost = null;
            if (this.multipleInstances)
            {
                if (this.Destroy != null)
                {
                    this.Destroy(this, new EventArgs());
                }
            }
            this.serviceContainer.QueryService<IUIShell>().QueryUIStatus();
        }

        #region IDisposable Members

        public virtual void Dispose()
        {
            if (this.toolWindowHost != null)
            {
                CloseInternal();
            }
            if (this.toolStripController != null)
            {
                this.toolStripController.Dispose();
                this.toolStripContainer.Dispose();
            }

        }

        #endregion
    }
}