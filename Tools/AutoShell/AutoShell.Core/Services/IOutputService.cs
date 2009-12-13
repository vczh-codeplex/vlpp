using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.Services
{
    /// <summary>
    /// 输出窗口服务
    /// </summary>
    public interface IOutputService
    {
        /// <summary>
        /// 注册新的输出频道
        /// </summary>
        /// <param name="guid">频道Guid</param>
        /// <param name="name">频道名称</param>
        void RegisterChannel(Guid guid, string name);
        /// <summary>
        /// 选中输出频道
        /// </summary>
        /// <param name="guid">频道Guid</param>
        void OpenChannel(Guid guid);
        /// <summary>
        /// 向频道打印消息
        /// </summary>
        /// <param name="guid">频道Guid</param>
        /// <param name="message">消息</param>
        void Print(Guid guid, string message);
        /// <summary>
        /// 清楚频道中的消息
        /// </summary>
        /// <param name="guid">频道Guid</param>
        void Clear(Guid guid);
    }
}
