using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.Services
{
    /// <summary>
    /// 查找服务
    /// </summary>
    public interface IFindService
    {
        /// <summary>
        /// 注册查找范围
        /// </summary>
        /// <param name="group"></param>
        void RegisterGroup(IFindTargetGroup group);
        /// <summary>
        /// 注销查找范围
        /// </summary>
        /// <param name="group"></param>
        void UnregisterGroup(IFindTargetGroup group);
        /// <summary>
        /// 获得所有注册的查找范围
        /// </summary>
        /// <returns>查找范围数组</returns>
        IFindTargetGroup[] GetGroups();
        /// <summary>
        /// 更新查找结果项目内容
        /// </summary>
        /// <param name="results"></param>
        void UpdateFindResults(FindResult[] results);
        /// <summary>
        /// 设置查找结果项目
        /// </summary>
        /// <param name="results"></param>
        void SetFindResults(FindResult[] results);
    }

    /// <summary>
    /// 查找范围
    /// </summary>
    public interface IFindTargetGroup
    {
        /// <summary>
        /// 范围名称
        /// </summary>
        string Name { get; }
        /// <summary>
        /// 关联的所有查找目标
        /// </summary>
        Dictionary<string, IFindTarget> FindTargets { get; }
        /// <summary>
        /// 当前激活的查找目标
        /// </summary>
        IFindTarget Current { get; }
        /// <summary>
        /// 获得下一个查找目标
        /// </summary>
        /// <param name="current">当前查找目标</param>
        /// <param name="forward">true==向前，false==向后</param>
        /// <returns>下一个查找目标</returns>
        IFindTarget GetNext(IFindTarget current, bool forward);
    }

    /// <summary>
    /// 查找目标
    /// </summary>
    public interface IFindTarget
    {
        /// <summary>
        /// 全局唯一名称
        /// </summary>
        string UniqueName { get; }
        /// <summary>
        /// 显示名称
        /// </summary>
        string Name { get; }
        /// <summary>
        /// 文字内容
        /// </summary>
        string Text { get; }
        /// <summary>
        /// 是否可以被替换
        /// </summary>
        bool CanReplace { get; }
        /// <summary>
        /// 是否正在被一个编辑器编辑
        /// </summary>
        bool IsOpened { get; }
        /// <summary>
        /// 当前位置
        /// </summary>
        int Position { get; }

        /// <summary>
        /// 激活并选中一个查找结果项目
        /// </summary>
        /// <param name="result">查找结果项目</param>
        void Focus(FindResult result);
        /// <summary>
        /// 将查找结果项目替换成另一个字符串并激活选中
        /// </summary>
        /// <param name="result">查找项目</param>
        /// <param name="text">新字符串</param>
        void Replace(FindResult result, string text);
        /// <summary>
        /// 关联一系列查找项目，并要求在内容被编辑的时候更新查找项目的信息
        /// </summary>
        /// <param name="results">查找项目数组</param>
        void SetResultAndTrack(FindResult[] results);
        /// <summary>
        /// 同时替换所有关联的查找项目的内容
        /// </summary>
        /// <param name="texts">新字符串数组，数组长度跟关联的查找项目长度一致</param>
        void ReplaceTrackedResult(string[] texts);
    }

    /// <summary>
    /// 查找结果项目
    /// </summary>
    public class FindResult
    {
        /// <summary>
        /// 关联的查找目标
        /// </summary>
        public IFindTarget FindTarget { get; set; }
        /// <summary>
        /// 开始位置
        /// </summary>
        public int Start { get; set; }
        /// <summary>
        /// 长度
        /// </summary>
        public int Length { get; set; }
        /// <summary>
        /// 该行的内容（可以不被更新）
        /// </summary>
        public string Line { get; set; }
    }
}
