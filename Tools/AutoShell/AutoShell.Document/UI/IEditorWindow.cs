using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.UI
{
    /// <summary>
    /// 编辑器窗口
    /// </summary>
    public interface IEditorWindow
    {
        /// <summary>
        /// 正在编辑的文件类型
        /// </summary>
        Guid FileType { get; }
        /// <summary>
        /// 文件名，还不拥有文件名的时候返回string.Empty
        /// </summary>
        string FileName { get; }
        /// <summary>
        /// 是否基于文件编辑
        /// </summary>
        bool IsFileBased { get; }
        /// <summary>
        /// 是否有未保存的更改
        /// </summary>
        bool IsModified { get; }
        /// <summary>
        /// 是否可以撤销
        /// </summary>
        bool CanUndo { get; }
        /// <summary>
        /// 是否可以重做
        /// </summary>
        bool CanRedo { get; }
        /// <summary>
        /// 是否可以剪切
        /// </summary>
        bool CanCut { get; }
        /// <summary>
        /// 是否可以复制
        /// </summary>
        bool CanCopy { get; }
        /// <summary>
        /// 是否可以粘贴
        /// </summary>
        bool CanPaste { get; }
        /// <summary>
        /// 是否可以清楚选中的内容
        /// </summary>
        bool CanClearSelection { get; }
        /// <summary>
        /// 是否可以选中所有内容
        /// </summary>
        bool CanSelectAll { get; }

        /// <summary>
        /// 根据一个给定的文件类型进行初始化
        /// </summary>
        /// <param name="fileType">类型Guid</param>
        void Initialize(Guid fileType);
        /// <summary>
        /// 打开文件（不关联文件名）
        /// </summary>
        /// <param name="fileName">文件名</param>
        void LoadFromFile(string fileName);
        /// <summary>
        /// 保存文件（不关联文件名）
        /// </summary>
        /// <param name="fileName">文件名</param>
        void SaveToFile(string fileName);
        /// <summary>
        /// 将编辑器关联到一个文件名上
        /// </summary>
        /// <param name="fileName">文件名</param>
        void AssociateToFile(string fileName);
        /// <summary>
        /// 撤销
        /// </summary>
        void Undo();
        /// <summary>
        /// 重做
        /// </summary>
        void Redo();
        /// <summary>
        /// 剪切
        /// </summary>
        void Cut();
        /// <summary>
        /// 复制
        /// </summary>
        void Copy();
        /// <summary>
        /// 粘帖
        /// </summary>
        void Paste();
        /// <summary>
        /// 清除选中的内容
        /// </summary>
        void ClearSelection();
        /// <summary>
        /// 选中所有内容
        /// </summary>
        void SelectAll();
    }
}