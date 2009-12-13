using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.Services;
using AutoShell.UI;
using System.Windows.Forms;

namespace AutoShell.ServiceImpl
{
    internal class EditorService : IEditorService, IOptionListener
    {
        private static readonly Guid RecentFilesKey = new Guid("{6995CC6D-F153-4020-8B70-44A2F54E0ED9}");
        private const int MaxRecentFiles = 100;

        private IServiceContainer serviceContainer;
        private Dictionary<Guid, FileCategory> categories = new Dictionary<Guid, FileCategory>();
        private Dictionary<Guid, FileType> fileTypes = new Dictionary<Guid, FileType>();
        private Dictionary<Guid, FileEditor> fileEditors = new Dictionary<Guid, FileEditor>();
        private List<ToolWindowPane> editorWindows = new List<ToolWindowPane>();
        private List<string> recentFiles = new List<string>();

        #region IOptionListener Members

        void IOptionListener.OptionUpdated(Guid optionGuid)
        {
            IOptionService optionService = this.serviceContainer.QueryService<IOptionService>();
            this.recentFiles = optionService.CopyOption(EditorService.RecentFilesKey) as List<string>;
        }

        #endregion

        public EditorService(IServiceContainer serviceContainer)
        {
            this.serviceContainer = serviceContainer;

            IOptionService optionService = this.serviceContainer.QueryService<IOptionService>();
            optionService.RegisterOption(EditorService.RecentFilesKey, typeof(List<string>));
            optionService.RegisterOptionListener(EditorService.RecentFilesKey, this);
        }

        private void AddRecentFileItem(string fileName)
        {
            string[] files = this.recentFiles.Where(file => file.ToUpper() == fileName.ToUpper()).ToArray();
            if (files.Length > 0)
            {
                this.recentFiles.Remove(files[0]);
            }
            this.recentFiles.Insert(0, fileName);

            IOptionService optionService = this.serviceContainer.QueryService<IOptionService>();
            optionService.SetOption(EditorService.RecentFilesKey, this.recentFiles);
            optionService.SaveOptions();
        }

        #region IEditorService Members

        public void RegisterFileCategory(Guid categoryGuid, string name)
        {
            RegisterFileCategory(categoryGuid, Guid.Empty, name);
        }

        public void RegisterFileCategory(Guid categoryGuid, Guid parentGuid, string name)
        {
            if (this.categories.ContainsKey(categoryGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionDuplicatedFileCategory, categoryGuid));
            }
            else if (parentGuid != Guid.Empty && !this.categories.ContainsKey(parentGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionFileCategoryNotExists, parentGuid));
            }
            else
            {
                FileCategory category = new FileCategory();
                category.Name = name;
                category.Parent = parentGuid;
                this.categories.Add(categoryGuid, category);
            }
        }

        public void RegisterFileType(Guid typeGuid, FileType fileType)
        {
            if (this.fileTypes.ContainsKey(typeGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionDuplicatedFileType, typeGuid));
            }
            else
            {
                this.fileTypes.Add(typeGuid, fileType);
            }
        }

        public void AssociateFileType(Guid categoryGuid, Guid typeGuid)
        {
            if (!this.categories.ContainsKey(categoryGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionFileCategoryNotExists, categoryGuid));
            }
            else if (!this.fileTypes.ContainsKey(typeGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionFileTypeNotExists, typeGuid));
            }
            else
            {
                List<Guid> fileTypes = this.categories[categoryGuid].FileTypes;
                if (!fileTypes.Contains(typeGuid))
                {
                    fileTypes.Add(typeGuid);
                }
            }
        }

        public void RegisterFileEditor(Guid editorGuid, FileEditor fileEditor)
        {
            if (this.fileEditors.ContainsKey(editorGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionDuplicatedFileEditor, editorGuid));
            }
            else
            {
                this.fileEditors.Add(editorGuid, fileEditor);
            }
        }

        public void AssociateFileEditor(Guid typeGuid, Guid editorGuid)
        {
            if (!this.fileEditors.ContainsKey(editorGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionFileEditorNotExists, editorGuid));
            }
            else if (!this.fileTypes.ContainsKey(typeGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionFileTypeNotExists, typeGuid));
            }
            else
            {
                FileType fileType = this.fileTypes[typeGuid];
                FileEditor fileEditor = this.fileEditors[editorGuid];
                if (!fileType.FileEditors.Contains(editorGuid))
                {
                    fileType.FileEditors.Add(editorGuid);
                }
                if (!fileEditor.FileTypes.Contains(typeGuid))
                {
                    fileEditor.FileTypes.Add(typeGuid);
                }
            }
        }

        public Guid[] GetEditors(string fileExtension)
        {
            return this.fileEditors.Keys
                .Where(guid =>
                    this.fileEditors[guid].FileTypes
                        .Select(type => this.fileTypes[type])
                        .Where(fileType => fileType.FileExtension
                            .Select(ext => ext.ToUpper())
                            .Any(ext => ext == fileExtension.ToUpper())
                            )
                        .Count() > 0
                    )
                .ToArray();
        }

        public Guid[] GetFileTypes(string fileExtension)
        {
            return this.fileTypes.Keys
                .Where(type => this.fileTypes[type].FileExtension
                    .Select(ext => ext.ToUpper())
                    .Any(ext => ext == fileExtension.ToUpper())
                    )
                .ToArray();
        }

        public Guid[] GetFileTypes()
        {
            return this.fileTypes.Keys.ToArray();
        }

        public Guid[] GetFileCategories()
        {
            return this.categories.Keys.ToArray();
        }

        public ToolWindowPane TryOpenEditingFile(string fileName)
        {
            foreach (ToolWindowPane pane in this.editorWindows)
            {
                IEditorWindow editorWindow = pane as IEditorWindow;
                if (editorWindow != null && editorWindow.FileName.ToUpper() == fileName.ToUpper())
                {
                    pane.Open();
                    AddRecentFileItem(fileName);
                    return pane;
                }
            }
            return null;
        }

        public ToolWindowPane OpenFile(string fileName, Guid fileType, Guid editorGuid)
        {
            if (!this.fileTypes.ContainsKey(fileType))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionFileTypeNotExists, fileType));
            }
            else if (!this.fileEditors.ContainsKey(editorGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionFileEditorNotExists, editorGuid));
            }
            else
            {
                FileEditor editor = this.fileEditors[editorGuid];
                IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
                ToolWindowPane pane = uiShell.CreateToolWindow(editor.EditorToolWindow);
                IEditorWindow editorWindow = pane as IEditorWindow;
                if (editor == null)
                {
                    throw new InvalidOperationException(string.Format(Strings.ExceptionEditorWindowDoesNotImplementIEditorWindow, editorGuid));
                }
                editorWindow.Initialize(fileType);
                if (fileName != null)
                {
                    editorWindow.LoadFromFile(fileName);
                    editorWindow.AssociateToFile(fileName);
                    AddRecentFileItem(fileName);
                }
                pane.Open();
                pane.Destroy += new EventHandler(pane_Destroy);
                this.editorWindows.Add(pane);
                return pane;
            }
        }

        public ToolWindowPane NewFile(Guid fileType, Guid editorGuid)
        {
            return OpenFile(null, fileType, editorGuid);
        }

        void pane_Destroy(object sender, EventArgs e)
        {
            this.editorWindows.Remove(sender as ToolWindowPane);
        }

        public ToolWindowPane[] GetEditorWindows()
        {
            return this.editorWindows.ToArray();
        }

        public FileType GetFileType(Guid typeGuid)
        {
            return this.fileTypes[typeGuid];
        }

        public FileEditor GetEditor(Guid editorGuid)
        {
            return this.fileEditors[editorGuid];
        }

        public FileCategory GetCategory(Guid categoryGuid)
        {
            return this.categories[categoryGuid];
        }

        public bool SaveEditingContent(ToolWindowPane pane, bool alwaysPrompt)
        {
            IEditorWindow editorWindow = pane as IEditorWindow;
            if (editorWindow != null && editorWindow.IsFileBased)
            {
                if (string.IsNullOrEmpty(editorWindow.FileName) || alwaysPrompt)
                {
                    Guid typeGuid = Guid.Empty;
                    string fileName = null;
                    if (ShowSaveDialog(new Guid[] { editorWindow.FileType }, out typeGuid, out fileName))
                    {
                        editorWindow.AssociateToFile(fileName);
                    }
                    else
                    {
                        return false;
                    }
                }
                editorWindow.SaveToFile(editorWindow.FileName);
                AddRecentFileItem(editorWindow.FileName);
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool ShowOpenDialog(Guid[] typeGuids, out Guid typeGuid, out string fileName)
        {
            typeGuid = Guid.Empty;
            fileName = null;
            foreach (Guid type in typeGuids)
            {
                if (!this.fileTypes.ContainsKey(type))
                {
                    throw new InvalidOperationException(string.Format(Strings.ExceptionFileTypeNotExists, type));
                }
            }

            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                dialog.AddExtension = true;
                dialog.AutoUpgradeEnabled = true;
                dialog.CheckFileExists = true;
                dialog.CheckPathExists = true;
                dialog.DereferenceLinks = true;
                dialog.Multiselect = false;
                dialog.RestoreDirectory = false;
                dialog.Title = Strings.DialogOpen;
                dialog.ValidateNames = true;

                dialog.Filter = typeGuids
                    .Select(guid => this.fileTypes[guid])
                    .Select(fileType => string.Format(
                        Strings.FileDialogFilter,
                        fileType.FileTypeName,
                        fileType.FileExtension
                            .Select(ext => "*." + ext)
                            .Aggregate((a, b) => a + ";" + b)
                            )
                        )
                    .Aggregate((a, b) => a + "|" + b);

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    typeGuid = typeGuids[dialog.FilterIndex - 1];
                    fileName = dialog.FileName;
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        public bool ShowSaveDialog(Guid[] typeGuids, out Guid typeGuid, out string fileName)
        {
            typeGuid = Guid.Empty;
            fileName = null;
            foreach (Guid type in typeGuids)
            {
                if (!this.fileTypes.ContainsKey(type))
                {
                    throw new InvalidOperationException(string.Format(Strings.ExceptionFileTypeNotExists, type));
                }
            }

            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                dialog.AddExtension = true;
                dialog.AutoUpgradeEnabled = true;
                dialog.CheckPathExists = true;
                dialog.CreatePrompt = false;
                dialog.DereferenceLinks = true;
                dialog.OverwritePrompt = true;
                dialog.RestoreDirectory = false;
                dialog.Title = Strings.DialogSave;
                dialog.ValidateNames = true;

                dialog.Filter = typeGuids
                    .Select(guid => this.fileTypes[guid])
                    .Select(fileType => string.Format(
                        Strings.FileDialogFilter,
                        fileType.FileTypeName,
                        fileType.FileExtension
                            .Select(ext => "*." + ext)
                            .Aggregate((a, b) => a + ";" + b)
                            )
                        )
                    .Aggregate((a, b) => a + "|" + b);

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    typeGuid = typeGuids[dialog.FilterIndex - 1];
                    fileName = dialog.FileName;
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        public string[] GetRecentFiles()
        {
            return this.recentFiles.ToArray();
        }

        #endregion
    }
}
