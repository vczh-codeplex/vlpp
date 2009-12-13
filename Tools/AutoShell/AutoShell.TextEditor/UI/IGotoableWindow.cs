using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.UI
{
    /// <summary>
    /// 可跳转编辑器
    /// </summary>
    public interface IGotoableWindow
    {
        /// <summary>
        /// 是否存在跳转内容
        /// </summary>
        bool CanGoto { get; }
        /// <summary>
        /// 可跳转的行数数量
        /// </summary>
        int MaxGotoLines { get; }
        /// <summary>
        /// 当前选中的行
        /// </summary>
        int CurrentLineNumber { get; }
        /// <summary>
        /// 跳转到指定行
        /// </summary>
        /// <param name="lineNumber"></param>
        void Goto(int lineNumber);
    }
}
