using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.UI;
using System.Drawing;

namespace AutoShell.Services
{
    /// <summary>
    /// 文件类型
    /// </summary>
    public class FileType
    {
        public delegate void WriteDefaultContentHandler(string fileName);

        /// <summary>
        /// 类型名称
        /// </summary>
        public string FileTypeName { get; set; }
        /// <summary>
        /// 类型描述
        /// </summary>
        public string FileTypeDescription { get; set; }
        /// <summary>
        /// 支持的所有扩展名，不需要“.”
        /// </summary>
        public string[] FileExtension { get; set; }
        /// <summary>
        /// 是否具有缺省内容
        /// </summary>
        public bool HasDefaultContent { get; set; }
        /// <summary>
        /// 类型图标
        /// </summary>
        public Image Icon { get; set; }
        /// <summary>
        /// 关联的编辑器
        /// </summary>
        public List<Guid> FileEditors { get; private set; }
        /// <summary>
        /// 存在文件名的时候制造缺省内容
        /// </summary>
        public event WriteDefaultContentHandler MakeContentWithFileName;
        /// <summary>
        /// 不存在文件名的时候制造缺省类型
        /// </summary>
        public event WriteDefaultContentHandler MakeContentWithoutFileName;

        public FileType()
        {
            this.FileEditors = new List<Guid>();
        }

        public void OnMakeContentWithFileName(string fileName)
        {
            if (this.MakeContentWithFileName != null)
            {
                this.MakeContentWithFileName(fileName);
            }
        }

        public void OnMakeContentWithoutFileName(string fileName)
        {
            if (this.MakeContentWithoutFileName != null)
            {
                this.MakeContentWithoutFileName(fileName);
            }
        }
    }

    /// <summary>
    /// 文件编辑器
    /// </summary>
    public class FileEditor
    {
        /// <summary>
        /// 编辑器名称
        /// </summary>
        public string FileEditorName { get; set; }
        /// <summary>
        /// 编辑器描述
        /// </summary>
        public string FileEditorDescription { get; set; }
        /// <summary>
        /// 与编辑器关联的工具窗口
        /// </summary>
        public Guid EditorToolWindow { get; set; }
        /// <summary>
        /// 关联的文件类型
        /// </summary>
        public List<Guid> FileTypes { get; private set; }

        public FileEditor()
        {
            this.FileTypes = new List<Guid>();
        }
    }

    /// <summary>
    /// 文件类型组
    /// </summary>
    public class FileCategory
    {
        /// <summary>
        /// 组名
        /// </summary>
        public string Name { get; set; }
        /// <summary>
        /// 父组
        /// </summary>
        public Guid Parent { get; set; }
        /// <summary>
        /// 包含的文件类型
        /// </summary>
        public List<Guid> FileTypes { get; private set; }

        public FileCategory()
        {
            this.FileTypes = new List<Guid>();
        }
    }

    /// <summary>
    /// 编辑器服务
    /// </summary>
    public interface IEditorService
    {
        /// <summary>
        /// 注册文件类型组
        /// </summary>
        /// <param name="categoryGuid">组Guid</param>
        /// <param name="name">组名</param>
        void RegisterFileCategory(Guid categoryGuid, string name);
        /// <summary>
        /// 注册文件类型组
        /// </summary>
        /// <param name="categoryGuid">组Guid</param>
        /// <param name="parentGuid">父组</param>
        /// <param name="name">组名</param>
        void RegisterFileCategory(Guid categoryGuid, Guid parentGuid, string name);
        /// <summary>
        /// 注册文件类型
        /// </summary>
        /// <param name="typeGuid">类型Guid</param>
        /// <param name="fileType">文件类型对象</param>
        void RegisterFileType(Guid typeGuid, FileType fileType);
        /// <summary>
        /// 将文件类型关联到文件类型组
        /// </summary>
        /// <param name="categoryGuid">组Guid</param>
        /// <param name="typeGuid">类型Guid</param>
        void AssociateFileType(Guid categoryGuid, Guid typeGuid);
        /// <summary>
        /// 注册文件编辑器
        /// </summary>
        /// <param name="editorGuid">编辑器Guid</param>
        /// <param name="fileEditor">编辑器对象</param>
        void RegisterFileEditor(Guid editorGuid, FileEditor fileEditor);
        /// <summary>
        /// 将文件编辑器关联到文件类型
        /// </summary>
        /// <param name="typeGuid">类型Guid</param>
        /// <param name="editorGuid">编辑器Guid</param>
        void AssociateFileEditor(Guid typeGuid, Guid editorGuid);
        /// <summary>
        /// 根据扩展名获得可用的所有编辑器Guid
        /// </summary>
        /// <param name="fileExtension">扩展名</param>
        /// <returns>符合条件的编辑器Guid</returns>
        Guid[] GetEditors(string fileExtension);
        /// <summary>
        /// 根据扩展名获得关联的所有文件类型
        /// </summary>
        /// <param name="fileExtension">扩展名</param>
        /// <returns>符合条件的类型Guid</returns>
        Guid[] GetFileTypes(string fileExtension);
        /// <summary>
        /// 获得注册的所有文件类型
        /// </summary>
        /// <returns>类型Guid数组</returns>
        Guid[] GetFileTypes();
        /// <summary>
        /// 获得注册的所有文件类型组
        /// </summary>
        /// <returns>类型组Guid</returns>
        Guid[] GetFileCategories();
        /// <summary>
        /// 尝试打开一个已经打开的文件
        /// </summary>
        /// <param name="fileName">文件名</param>
        /// <returns>编辑器的工具窗口，如果该文件没有正在被编辑那么返回null</returns>
        ToolWindowPane TryOpenEditingFile(string fileName);
        /// <summary>
        /// 打开文件
        /// </summary>
        /// <param name="fileName">文件名</param>
        /// <param name="fileType">类型Guid</param>
        /// <param name="editorGuid">编辑器Guid</param>
        /// <returns>编辑器的工具窗口</returns>
        ToolWindowPane OpenFile(string fileName, Guid fileType, Guid editorGuid);
        /// <summary>
        /// 新建文件
        /// </summary>
        /// <param name="fileType">类型Guid</param>
        /// <param name="editorGuid">编辑器Guid</param>
        /// <returns>编辑器的工具窗口</returns>
        ToolWindowPane NewFile(Guid fileType, Guid editorGuid);
        /// <summary>
        /// 获得正在使用的所有编辑器工具窗口
        /// </summary>
        /// <returns>工具窗口数组</returns>
        ToolWindowPane[] GetEditorWindows();
        /// <summary>
        /// 查找文件类型
        /// </summary>
        /// <param name="typeGuid">类型Guid</param>
        /// <returns>文件类型对象</returns>
        FileType GetFileType(Guid typeGuid);
        /// <summary>
        /// 查找编辑器
        /// </summary>
        /// <param name="editorGuid">编辑器Guid</param>
        /// <returns>文件编辑器对象</returns>
        FileEditor GetEditor(Guid editorGuid);
        /// <summary>
        /// 查找文件类型组
        /// </summary>
        /// <param name="categoryGuid">组Guid</param>
        /// <returns>文件类型组对象</returns>
        FileCategory GetCategory(Guid categoryGuid);
        /// <summary>
        /// 保存正在编辑的内容
        /// </summary>
        /// <param name="pane">编辑器工具窗口</param>
        /// <param name="alwaysPrompt">是否强制弹出保存对话框</param>
        /// <returns>true==成功，false==失败</returns>
        bool SaveEditingContent(ToolWindowPane pane, bool alwaysPrompt);
        /// <summary>
        /// 显示打开对话框
        /// </summary>
        /// <param name="typeGuids">允许的所有类型Guid</param>
        /// <param name="typeGuid">选中用的类型Guid</param>
        /// <param name="fileName">文件名</param>
        /// <returns>true==成功，false==取消</returns>
        bool ShowOpenDialog(Guid[] typeGuids, out Guid typeGuid, out string fileName);
        /// <summary>
        /// 显示保存对话框
        /// </summary>
        /// <param name="typeGuids">允许的所有类型Guid</param>
        /// <param name="typeGuid">选中的类型Guid</param>
        /// <param name="fileName">文件名</param>
        /// <returns>true==成功，false==取消</returns>
        bool ShowSaveDialog(Guid[] typeGuids, out Guid typeGuid, out string fileName);
        /// <summary>
        /// 获得最近打开的文件列表
        /// </summary>
        /// <returns>文件名数组</returns>
        string[] GetRecentFiles();
    }
}
