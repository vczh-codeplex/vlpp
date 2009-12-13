using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.Services
{
    /// <summary>
    /// 命令回调参数
    /// </summary>
    public class CommandEventArgs : EventArgs
    {
        /// <summary>
        /// 命令输出信息
        /// </summary>
        public string Message { get; set; }
        /// <summary>
        /// 参数内容
        /// </summary>
        public string[] Parameters { get; private set; }

        /// <summary>
        /// 构造命令回调参数
        /// </summary>
        /// <param name="parameters">参数内容</param>
        public CommandEventArgs(string[] parameters)
        {
            this.Message = string.Empty;
            this.Parameters = parameters;
        }
    }

    /// <summary>
    /// 命令回调函数句柄
    /// </summary>
    /// <param name="sender">命令发送者</param>
    /// <param name="e">命令回调参数</param>
    public delegate void CommandHandler(object sender, CommandEventArgs e);

    /// <summary>
    /// 命令对象
    /// </summary>
    public class CommandItem
    {
        /// <summary>
        /// 命令回调事件
        /// </summary>
        public event CommandHandler Execute;

        /// <summary>
        /// 命令名
        /// </summary>
        public string Name { get; set; }
        /// <summary>
        /// 命令简要描述
        /// </summary>
        public string Description { get; set; }
        /// <summary>
        /// 所属服务容器
        /// </summary>
        public IServiceContainer Services { get; internal set; }

        /// <summary>
        /// 构造命令对象
        /// </summary>
        public CommandItem()
        {
            this.Name = string.Empty;
            this.Description = string.Empty;
        }

        /// <summary>
        /// 调用命令
        /// </summary>
        /// <param name="parameters">参数内容</param>
        /// <returns>输出信息</returns>
        public string OnExecute(string[] parameters)
        {
            if (this.Execute != null)
            {
                CommandEventArgs e = new CommandEventArgs(parameters);
                this.Execute(this, e);
                return e.Message;
            }
            else
            {
                return string.Empty;
            }
        }
    }

    /// <summary>
    /// 命令服务
    /// </summary>
    public interface ICommandService
    {
        /// <summary>
        /// 注册命令
        /// </summary>
        /// <param name="command">命令对象</param>
        void RegisterCommand(CommandItem command);
        /// <summary>
        /// 执行命令
        /// </summary>
        /// <param name="name">命令名</param>
        /// <param name="parameters">参数内容</param>
        /// <returns>输出信息</returns>
        string ExecuteCommand(string name, params string[] parameters);
        /// <summary>
        /// 获得所有已注册命令
        /// </summary>
        /// <returns>所有已注册命令的数组</returns>
        CommandItem[] GetCommands();
    }
}