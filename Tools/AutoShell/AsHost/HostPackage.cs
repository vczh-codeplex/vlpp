using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell;
using AutoShell.Services;
using AutoShell.UI;

namespace AsHost
{
    public class HostPackage : Package
    {
        public override void Initialize()
        {
            base.Initialize();

            Guid categoryGuid = Guid.NewGuid();

            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            IEditorService editorService = this.Services.QueryService<IEditorService>();
            editorService.RegisterFileCategory(categoryGuid, "文本文件");
            {
                Guid toolGuid = Guid.NewGuid();
                Guid editorGuid = Guid.NewGuid();
                Guid typeGuid = Guid.NewGuid();
                uiShell.RegisterToolWindow(toolGuid, typeof(TextFileEditor), ToolWindowDockType.Document, true);

                FileType fileType = new FileType();
                fileType.FileExtension = new string[] { "txt" };
                fileType.FileTypeDescription = "Windows 文本文件";
                fileType.FileTypeName = "文本文件";
                fileType.HasDefaultContent = false;
                editorService.RegisterFileType(typeGuid, fileType);

                FileEditor fileEditor = new FileEditor();
                fileEditor.EditorToolWindow = toolGuid;
                fileEditor.FileEditorDescription = "Windows 文本文件编辑器";
                fileEditor.FileEditorName = "AsHost Text Editor";
                editorService.RegisterFileEditor(editorGuid, fileEditor);

                editorService.AssociateFileType(categoryGuid, typeGuid);
                editorService.AssociateFileEditor(typeGuid, editorGuid);
            }
            {
                Guid toolGuid = Guid.NewGuid();
                Guid editorGuid = Guid.NewGuid();
                Guid typeGuid = Guid.NewGuid();
                uiShell.RegisterToolWindow(toolGuid, typeof(CodeFileEditor), ToolWindowDockType.Document, true);

                FileType fileType = new FileType();
                fileType.FileExtension = new string[] { "code" };
                fileType.FileTypeDescription = "Vczh 代码文件";
                fileType.FileTypeName = "代码文件";
                fileType.HasDefaultContent = false;
                editorService.RegisterFileType(typeGuid, fileType);

                FileEditor fileEditor = new FileEditor();
                fileEditor.EditorToolWindow = toolGuid;
                fileEditor.FileEditorDescription = "Vczh 代码文件编辑器";
                fileEditor.FileEditorName = "AsHost Code Editor";
                editorService.RegisterFileEditor(editorGuid, fileEditor);

                editorService.AssociateFileType(categoryGuid, typeGuid);
                editorService.AssociateFileEditor(typeGuid, editorGuid);
            }
        }
    }
}
