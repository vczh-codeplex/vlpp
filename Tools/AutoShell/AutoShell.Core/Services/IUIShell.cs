using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.UI;
using System.Windows.Forms;

namespace AutoShell.Services
{
    /// <summary>
    /// UI服务
    /// </summary>
    public interface IUIShell
    {
        /// <summary>
        /// 注册命令组
        /// </summary>
        /// <param name="guid">命令组Guid</param>
        /// <param name="group">命令组对象</param>
        /// <returns>命令组对象（与group参数相同）</returns>
        ShellGroup RegisterShellGroup(Guid guid, ShellGroup group);
        /// <summary>
        /// 获得命令组对象
        /// </summary>
        /// <param name="guid">命令组Guid</param>
        /// <returns>相应的命令组对象</returns>
        ShellGroup GetShellGroup(Guid guid);
        /// <summary>
        /// 注册工具条控制器
        /// </summary>
        /// <param name="controller">控制器</param>
        void RegisterToolStripController(ShellToolStripController controller);
        /// <summary>
        /// 注销工具条控制器
        /// </summary>
        /// <param name="controller">控制器</param>
        void UnregisterToolStripController(ShellToolStripController controller);
        /// <summary>
        /// 更新所有控制器下命令的UI状态
        /// </summary>
        void QueryUIStatus();
        /// <summary>
        /// 是否已经被初始化
        /// </summary>
        bool IsInitialized { get; }
        /// <summary>
        /// 注册工具条
        /// </summary>
        /// <param name="guid">工具条Guid</param>
        /// <param name="group">命令组</param>
        /// <param name="name">工具条名称</param>
        void RegisterToolBar(Guid guid, ShellGroup group, string name);

        /// <summary>
        /// 注册工具窗口，单实例的窗口如果在插件包构造的时候就被创建，则位置可以在程序关闭后被记住
        /// </summary>
        /// <param name="guid">工具窗口Guid</param>
        /// <param name="toolWindowType">工具窗口类型（必须继承自AutoShell.UI.ToolWindowPane）</param>
        /// <param name="defaultDock">缺省停靠位置</param>
        /// <param name="multipleInstances">true为允许多实例，false则不允许。</param>
        void RegisterToolWindow(Guid guid, Type toolWindowType, ToolWindowDockType defaultDock, bool multipleInstances);
        /// <summary>
        /// 寻找所有已创建的工具窗口
        /// </summary>
        /// <returns></returns>
        ToolWindowPane[] FindToolWindow();
        /// <summary>
        /// 寻找所有已创建的指定类型的工具窗口
        /// </summary>
        /// <param name="guid">工具窗口Guid</param>
        /// <returns>所有由该Guid创建的工具窗口</returns>
        ToolWindowPane[] FindToolWindow(Guid guid);
        /// <summary>
        /// 寻找所有已创建的指定类型和标记的工具窗口
        /// </summary>
        /// <param name="guid">工具窗口Guid</param>
        /// <param name="token">标记</param>
        /// <returns>所有由该Guid创建的有指定标记的工具窗口</returns>
        ToolWindowPane[] FindToolWindow(Guid guid, string token);
        /// <summary>
        /// 创建工具窗口，试图创建已经存在的单实例窗口会返回全局唯一实例
        /// </summary>
        /// <param name="guid">工具窗口Guid</param>
        /// <returns>创建由该Guid指定的工具窗口实例</returns>
        ToolWindowPane CreateToolWindow(Guid guid);
        /// <summary>
        /// 创建带标记的工具窗口，试图创建已经存在的单实例窗口会返回全局唯一实例
        /// </summary>
        /// <param name="guid">工具窗口Guid</param>
        /// <param name="token">标记</param>
        /// <returns>创建由该Guid指定的工具窗口实例并打上标记</returns>
        ToolWindowPane CreateToolWindow(Guid guid, string token);
        IToolWindowHost CreateToolWindowHost(ToolWindowDockType dockType);
        /// <summary>
        /// 获得当前工具窗口
        /// </summary>
        /// <returns>当前工具窗口</returns>
        ToolWindowPane GetActiveToolWindow();
        /// <summary>
        /// 获得当前文档停靠区工具窗口
        /// </summary>
        /// <returns>当前文档停靠区工具窗口</returns>
        ToolWindowPane GetActiveTabbedToolWindow();

        /// <summary>
        /// 显示消息对话框
        /// </summary>
        /// <param name="message">消息内容</param>
        /// <returns>OK</returns>
        DialogResult ShowMessage(string message);
        /// <summary>
        /// 显示带多个按钮的消息对话框
        /// </summary>
        /// <param name="message">消息内容</param>
        /// <param name="buttons">按钮组合</param>
        /// <returns>按下的按钮</returns>
        DialogResult ShowMessage(string message, MessageBoxButtons buttons);
        /// <summary>
        /// 显示带多个按钮和图标的消息对话框
        /// </summary>
        /// <param name="message">消息内容</param>
        /// <param name="buttons">按钮组合</param>
        /// <param name="icons">图标</param>
        /// <returns>按下的按钮</returns>
        DialogResult ShowMessage(string message, MessageBoxButtons buttons, MessageBoxIcon icons);
    }
}