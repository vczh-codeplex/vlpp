using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.UI;
using AutoShell.Services;
using AutoShell.PublicShell;
using System.Windows.Forms;
using System.IO;
using AutoShell.ServiceImpl;
using AutoShell.UI.ToolWindow;

namespace AutoShell
{
    public class DocumentPackage : Package
    {
        private const int MaxRecentFiles = 10;

        private ShellGroup menuMain = null;

        private ShellGroup menuFile = null;
        private ShellGroup menuFileOpen = null;
        private ShellGroup menuFileSave = null;
        private ShellGroup menuFileClose = null;
        private ShellGroup menuFileRecent = null;
        private ShellGroup menuFileNewFile = null;
        private ShellGroup menuFileOpenFile = null;

        private ShellGroup menuEdit = null;
        private ShellGroup menuEditUndoRedo = null;
        private ShellGroup menuEditClipboard = null;
        private ShellGroup menuEditOperation = null;

        private ShellGroup menuWindows = null;

        private ShellItem itemFileNew = null;
        private ShellItem itemFileNewFile = null;
        private ShellItem itemFileOpen = null;
        private ShellItem itemFileOpenFile = null;
        private ShellItem itemFileSave = null;
        private ShellItem itemFileSaveAs = null;
        private ShellItem itemFileSaveAll = null;
        private ShellItem itemFileClose = null;
        private ShellItem itemFileRecentFiles = null;
        private ShellItem itemFileRecentFilesEmpty = null;
        private List<ShellItem> itemFileRecentFilesFiles = new List<ShellItem>();

        private ShellItem itemEdit = null;
        private ShellItem itemEditUndo = null;
        private ShellItem itemEditRedo = null;
        private ShellItem itemEditCut = null;
        private ShellItem itemEditCopy = null;
        private ShellItem itemEditPaste = null;
        private ShellItem itemEditDelete = null;
        private ShellItem itemEditSelectAll = null;

        private ShellItem itemWindowWindows = null;

        private ShellGroup toolStandard = null;
        private ShellGroup toolStandardFile = null;
        private ShellGroup toolStandardClipboard = null;
        private ShellGroup toolStandardUndoRedo = null;

        private ShellItem buttonStandardNew = null;
        private ShellItem buttonStandardOpen = null;
        private ShellItem buttonStandardSave = null;
        private ShellItem buttonStandardSaveAll = null;
        private ShellItem buttonStandardCut = null;
        private ShellItem buttonStandardCopy = null;
        private ShellItem buttonStandardPaste = null;
        private ShellItem buttonStandardUndo = null;
        private ShellItem buttonStandardRedo = null;

        public override void Initialize()
        {
            base.Initialize();
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            ICommandService commandService = this.Services.QueryService<ICommandService>();

            RegisterMenuItems(uiShell);
            RegisterToolBars(uiShell);
            RegisterCommands(commandService);
            RegisterServices();
        }

        #region Configuration

        private void RegisterMenuItems(IUIShell uiShell)
        {
            this.menuMain = uiShell.GetShellGroup(PublicShellCore.MainMenuGroup);
            this.menuFile = uiShell.GetShellGroup(PublicShellCore.FileMenuGroup);
            this.menuFileOpen = uiShell.RegisterShellGroup(PublicShellDocument.FileOpenMenuGroup, new ShellGroup(0x0010));
            this.menuFileSave = uiShell.RegisterShellGroup(PublicShellDocument.FileSaveMenuGroup, new ShellGroup(0x0020));
            this.menuFileClose = uiShell.RegisterShellGroup(PublicShellDocument.FileCloseMenuGroup, new ShellGroup(0x0030));
            this.menuFileRecent = uiShell.RegisterShellGroup(PublicShellDocument.FileRecentMenuGroup, new ShellGroup(0x0040));
            this.menuFileNewFile = uiShell.RegisterShellGroup(PublicShellDocument.FileNewFileMenuGroup, new ShellGroup(0x0100));
            this.menuFileOpenFile = uiShell.RegisterShellGroup(PublicShellDocument.FileOpenFileMenuGroup, new ShellGroup(0x0100));
            this.menuEdit = uiShell.RegisterShellGroup(PublicShellDocument.EditMenuGroup, new ShellGroup(0x0010));
            this.menuEditUndoRedo = uiShell.RegisterShellGroup(PublicShellDocument.EditUndoRedoMenuGroup, new ShellGroup(0x0010));
            this.menuEditClipboard = uiShell.RegisterShellGroup(PublicShellDocument.EditClipboardMenuGroup, new ShellGroup(0x0020));
            this.menuEditOperation = uiShell.RegisterShellGroup(PublicShellDocument.EditOperationMenUGroup, new ShellGroup(0x0030));
            this.menuWindows = uiShell.GetShellGroup(PublicShellCore.WindowMenuGroup);

            this.menuFile.AddEntry(this.menuFileOpen);
            this.menuFile.AddEntry(this.menuFileSave);
            this.menuFile.AddEntry(this.menuFileClose);
            this.menuFile.AddEntry(this.menuFileRecent);

            this.menuEdit.AddEntry(this.menuEditUndoRedo);
            this.menuEdit.AddEntry(this.menuEditClipboard);
            this.menuEdit.AddEntry(this.menuEditOperation);

            this.itemFileNew = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemFileNew.Text = Strings.MenuTextFileNew;
            this.itemFileNew.Icon = Icons.FileNew;
            this.itemFileNew.SubGroup.AddEntry(this.menuFileNewFile);
            this.menuFileOpen.AddEntry(this.itemFileNew);

            this.itemFileNewFile = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemFileNewFile.Text = Strings.MenuTextFileNewFile;
            this.itemFileNewFile.Icon = Icons.FileNew;
            this.itemFileNewFile.ShortcutKeys = Keys.Control | Keys.N;
            this.itemFileNewFile.Executed += new EventHandler(itemFileNewFile_Executed);
            this.menuFileNewFile.AddEntry(this.itemFileNewFile);

            this.itemFileOpen = new ShellItem(ShellItemKind.Button, 0x0200);
            this.itemFileOpen.Text = Strings.MenuTextFileOpen;
            this.itemFileOpen.Icon = Icons.FileOpen;
            this.itemFileOpen.SubGroup.AddEntry(this.menuFileOpenFile);
            this.menuFileOpen.AddEntry(this.itemFileOpen);

            this.itemFileOpenFile = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemFileOpenFile.Text = Strings.MenuTextFileOpenFile;
            this.itemFileOpenFile.Icon = Icons.FileOpen;
            this.itemFileOpenFile.ShortcutKeys = Keys.Control | Keys.O;
            this.itemFileOpenFile.Executed += new EventHandler(itemFileOpenFile_Executed);
            this.menuFileOpenFile.AddEntry(this.itemFileOpenFile);

            this.itemFileSave = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemFileSave.Text = Strings.MenuTextFileSave;
            this.itemFileSave.Icon = Icons.FileSave;
            this.itemFileSave.ShortcutKeys = Keys.Control | Keys.S;
            this.itemFileSave.QueryStatus += new EventHandler(itemFileSave_QueryStatus);
            this.itemFileSave.Executed += new EventHandler(itemFileSave_Executed);
            this.menuFileSave.AddEntry(this.itemFileSave);

            this.itemFileSaveAs = new ShellItem(ShellItemKind.Button, 0x0200);
            this.itemFileSaveAs.Text = Strings.MenuTextFileSaveAs;
            this.itemFileSaveAs.QueryStatus += new EventHandler(itemFileSaveAs_QueryStatus);
            this.itemFileSaveAs.Executed += new EventHandler(itemFileSaveAs_Executed);
            this.menuFileSave.AddEntry(this.itemFileSaveAs);

            this.itemFileSaveAll = new ShellItem(ShellItemKind.Button, 0x0300);
            this.itemFileSaveAll.Text = Strings.MenuTextFileSaveAll;
            this.itemFileSaveAll.Icon = Icons.FileSaveAll;
            this.itemFileSaveAll.ShortcutKeys = Keys.Control | Keys.Shift | Keys.S;
            this.itemFileSaveAll.Executed += new EventHandler(itemFileSaveAll_Executed);
            this.menuFileSave.AddEntry(this.itemFileSaveAll);

            this.itemFileClose = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemFileClose.Text = Strings.MenuTextFileClose;
            this.itemFileClose.QueryStatus += new EventHandler(itemFileClose_QueryStatus);
            this.itemFileClose.Executed += new EventHandler(itemFileClose_Executed);
            this.menuFileClose.AddEntry(this.itemFileClose);

            this.itemFileRecentFiles = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemFileRecentFiles.Text = Strings.MenuTextFileRecentFiles;
            this.menuFileRecent.AddEntry(this.itemFileRecentFiles);

            this.itemFileRecentFilesEmpty = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemFileRecentFilesEmpty.Text = Strings.MenuTextFileRecentFilesEmpty;
            this.itemFileRecentFilesEmpty.Enabled = false;
            this.itemFileRecentFilesEmpty.QueryStatus += new EventHandler(itemFileRecentFilesEmpty_QueryStatus);
            this.itemFileRecentFiles.SubGroup.AddEntry(this.itemFileRecentFilesEmpty);

            for (int i = 0; i < MaxRecentFiles; i++)
            {
                ShellItem item = new ShellItem(ShellItemKind.Button, 0x0200 + i);
                item.Visible = false;
                item.QueryStatus += new EventHandler(itemFileRecentFilesItem_QueryStatus);
                item.Executed += new EventHandler(itemFileRecentFilesItem_Executed);
                this.itemFileRecentFilesFiles.Add(item);
                this.itemFileRecentFiles.SubGroup.AddEntry(item);
            }

            this.itemEdit = new ShellItem(ShellItemKind.Button, 0x0010);
            this.itemEdit.Text = Strings.MenuTextEdit;
            this.itemEdit.SubGroup.AddEntry(this.menuEdit);
            this.menuMain.AddEntry(this.itemEdit);

            this.itemEditUndo = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemEditUndo.Text = Strings.MenuTextEditUndo;
            this.itemEditUndo.Icon = Icons.EditUndo;
            this.itemEditUndo.ShortcutKeys = Keys.Control | Keys.Z;
            this.itemEditUndo.QueryStatus += new EventHandler(itemEditUndo_QueryStatus);
            this.itemEditUndo.Executed += new EventHandler(itemEditUndo_Executed);
            this.menuEditUndoRedo.AddEntry(this.itemEditUndo);

            this.itemEditRedo = new ShellItem(ShellItemKind.Button, 0x0200);
            this.itemEditRedo.Text = Strings.MenuTextEditRedo;
            this.itemEditRedo.Icon = Icons.EditRedo;
            this.itemEditRedo.ShortcutKeys = Keys.Control | Keys.Y;
            this.itemEditRedo.QueryStatus += new EventHandler(itemEditRedo_QueryStatus);
            this.itemEditRedo.Executed += new EventHandler(itemEditRedo_Executed);
            this.menuEditUndoRedo.AddEntry(this.itemEditRedo);

            this.itemEditCut = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemEditCut.Text = Strings.MenuTextEditCut;
            this.itemEditCut.Icon = Icons.EditCut;
            this.itemEditCut.ShortcutKeys = Keys.Control | Keys.X;
            this.itemEditCut.QueryStatus += new EventHandler(itemEditCut_QueryStatus);
            this.itemEditCut.Executed += new EventHandler(itemEditCut_Executed);
            this.menuEditClipboard.AddEntry(this.itemEditCut);

            this.itemEditCopy = new ShellItem(ShellItemKind.Button, 0x0200);
            this.itemEditCopy.Text = Strings.MenuTextEditCopy;
            this.itemEditCopy.Icon = Icons.EditCopy;
            this.itemEditCopy.ShortcutKeys = Keys.Control | Keys.C;
            this.itemEditCopy.QueryStatus += new EventHandler(itemEditCopy_QueryStatus);
            this.itemEditCopy.Executed += new EventHandler(itemEditCopy_Executed);
            this.menuEditClipboard.AddEntry(this.itemEditCopy);

            this.itemEditPaste = new ShellItem(ShellItemKind.Button, 0x0300);
            this.itemEditPaste.Text = Strings.MenuTextEditPaste;
            this.itemEditPaste.Icon = Icons.EditPaste;
            this.itemEditPaste.ShortcutKeys = Keys.Control | Keys.V;
            this.itemEditPaste.QueryStatus += new EventHandler(itemEditPaste_QueryStatus);
            this.itemEditPaste.Executed += new EventHandler(itemEditPaste_Executed);
            this.menuEditClipboard.AddEntry(this.itemEditPaste);

            this.itemEditDelete = new ShellItem(ShellItemKind.Button, 0x0400);
            this.itemEditDelete.Text = Strings.MenuTextEditDelete;
            this.itemEditDelete.Icon = Icons.EditDelete;
            this.itemEditDelete.ShortcutKeys = Keys.Delete;
            this.itemEditDelete.QueryStatus += new EventHandler(itemEditDelete_QueryStatus);
            this.itemEditDelete.Executed += new EventHandler(itemEditDelete_Executed);
            this.menuEditClipboard.AddEntry(this.itemEditDelete);

            this.itemEditSelectAll = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemEditSelectAll.Text = Strings.MenuTextEditSelectAll;
            this.itemEditSelectAll.ShortcutKeys = Keys.Control | Keys.A;
            this.itemEditSelectAll.QueryStatus += new EventHandler(itemEditSelectAll_QueryStatus);
            this.itemEditSelectAll.Executed += new EventHandler(itemEditSelectAll_Executed);
            this.menuEditOperation.AddEntry(this.itemEditSelectAll);

            this.itemWindowWindows = new ShellItem(ShellItemKind.Button, 0x0200);
            this.itemWindowWindows.Text = Strings.MenuTextWindowWindows;
            this.itemWindowWindows.Executed += new EventHandler(itemWindowWindows_Executed);
            this.menuWindows.AddEntry(this.itemWindowWindows);
        }

        private void RegisterToolBars(IUIShell uiShell)
        {
            this.toolStandard = uiShell.RegisterShellGroup(PublicShellDocument.StandardToolBarGroup, new ShellGroup(0x0000));
            this.toolStandardFile = uiShell.RegisterShellGroup(PublicShellDocument.StandardFileToolBarGroup, new ShellGroup(0x0100));
            this.toolStandardClipboard = uiShell.RegisterShellGroup(PublicShellDocument.StandardClipboardToolBarGroup, new ShellGroup(0x0200));
            this.toolStandardUndoRedo = uiShell.RegisterShellGroup(PublicShellDocument.StandardUndoRedoToolBarGroup, new ShellGroup(0x0300));

            this.toolStandard.AddEntry(this.toolStandardFile);
            this.toolStandard.AddEntry(this.toolStandardClipboard);
            this.toolStandard.AddEntry(this.toolStandardUndoRedo);

            this.buttonStandardNew = new ShellItem(ShellItemKind.TrackButton, 0x0010);
            this.buttonStandardNew.Icon = Icons.FileNew;
            this.buttonStandardNew.ToolTipText = Strings.ToolTipStandardNew;
            this.buttonStandardNew.SubGroup.AddEntry(this.menuFileNewFile);
            this.toolStandardFile.AddEntry(this.buttonStandardNew);

            this.buttonStandardOpen = new ShellItem(ShellItemKind.TrackButton, 0x0020);
            this.buttonStandardOpen.Icon = Icons.FileOpen;
            this.buttonStandardOpen.ToolTipText = Strings.ToolTipStandardOpen;
            this.buttonStandardOpen.SubGroup.AddEntry(this.menuFileOpenFile);
            this.toolStandardFile.AddEntry(this.buttonStandardOpen);

            this.buttonStandardSave = new ShellItem(ShellItemKind.Button, 0x0030);
            this.buttonStandardSave.Icon = Icons.FileSave;
            this.buttonStandardSave.ToolTipText = Strings.ToolTipStandardSave;
            this.buttonStandardSave.QueryStatus += new EventHandler(buttonStandardSave_QueryStatus);
            this.buttonStandardSave.Executed += new EventHandler(itemFileSave_Executed);
            this.toolStandardFile.AddEntry(this.buttonStandardSave);

            this.buttonStandardSaveAll = new ShellItem(ShellItemKind.Button, 0x0040);
            this.buttonStandardSaveAll.Icon = Icons.FileSaveAll;
            this.buttonStandardSaveAll.ToolTipText = Strings.ToolTipStandardSaveAll;
            this.buttonStandardSaveAll.Executed += new EventHandler(itemFileSaveAll_Executed);
            this.toolStandardFile.AddEntry(this.buttonStandardSaveAll);

            this.buttonStandardCut = new ShellItem(ShellItemKind.Button, 0x0010);
            this.buttonStandardCut.Icon = Icons.EditCut;
            this.buttonStandardCut.ToolTipText = Strings.ToolTipStandardCut;
            this.buttonStandardCut.QueryStatus += new EventHandler(buttonStandardCut_QueryStatus);
            this.buttonStandardCut.Executed += new EventHandler(itemEditCut_Executed);
            this.toolStandardClipboard.AddEntry(this.buttonStandardCut);

            this.buttonStandardCopy = new ShellItem(ShellItemKind.Button, 0x0020);
            this.buttonStandardCopy.Icon = Icons.EditCopy;
            this.buttonStandardCopy.ToolTipText = Strings.ToolTipStandardCopy;
            this.buttonStandardCopy.QueryStatus += new EventHandler(buttonStandardCopy_QueryStatus);
            this.buttonStandardCopy.Executed += new EventHandler(itemEditCopy_Executed);
            this.toolStandardClipboard.AddEntry(this.buttonStandardCopy);

            this.buttonStandardPaste = new ShellItem(ShellItemKind.Button, 0x0030);
            this.buttonStandardPaste.Icon = Icons.EditPaste;
            this.buttonStandardPaste.ToolTipText = Strings.ToolTipStandardPaste;
            this.buttonStandardPaste.QueryStatus += new EventHandler(buttonStandardPaste_QueryStatus);
            this.buttonStandardPaste.Executed += new EventHandler(itemEditPaste_Executed);
            this.toolStandardClipboard.AddEntry(this.buttonStandardPaste);

            this.buttonStandardUndo = new ShellItem(ShellItemKind.Button, 0x0010);
            this.buttonStandardUndo.Icon = Icons.EditUndo;
            this.buttonStandardUndo.ToolTipText = Strings.ToolTipStandardUndo;
            this.buttonStandardUndo.QueryStatus += new EventHandler(buttonStandardUndo_QueryStatus);
            this.buttonStandardUndo.Executed += new EventHandler(itemEditUndo_Executed);
            this.toolStandardUndoRedo.AddEntry(this.buttonStandardUndo);

            this.buttonStandardRedo = new ShellItem(ShellItemKind.Button, 0x0020);
            this.buttonStandardRedo.Icon = Icons.EditRedo;
            this.buttonStandardRedo.ToolTipText = Strings.ToolTipStandardRedo;
            this.buttonStandardRedo.QueryStatus += new EventHandler(buttonStandardRedo_QueryStatus);
            this.buttonStandardRedo.Executed += new EventHandler(itemEditRedo_Executed);
            this.toolStandardUndoRedo.AddEntry(this.buttonStandardRedo);

            uiShell.RegisterToolBar(PublicShellDocument.StandardToolBar, this.toolStandard, Strings.ToolBarStandard);
        }

        private void RegisterCommands(ICommandService commandService)
        {
            CommandItem commandFileNewFile = new CommandItem();
            commandFileNewFile.Name = PublicShellDocument.Command_File_New_File;
            commandFileNewFile.Description = Strings.CommandDescription_File_New_File;
            commandFileNewFile.Execute += (sender, e) => { itemFileNewFile_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandFileNewFile);

            CommandItem commandFileOpenFile = new CommandItem();
            commandFileOpenFile.Name = PublicShellDocument.Command_File_Open_File;
            commandFileOpenFile.Description = Strings.CommandDescription_File_Open_File;
            commandFileOpenFile.Execute += (sender, e) => { itemFileOpenFile_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandFileOpenFile);

            CommandItem commandFileSave = new CommandItem();
            commandFileSave.Name = PublicShellDocument.Command_File_Save;
            commandFileSave.Description = Strings.CommandDescription_File_Save;
            commandFileSave.Execute += (sender, e) => { itemFileSave_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandFileSave);

            CommandItem commandFileSaveAs = new CommandItem();
            commandFileSaveAs.Name = PublicShellDocument.Command_File_SaveAs;
            commandFileSaveAs.Description = Strings.CommandDescription_File_SaveAs;
            commandFileSaveAs.Execute += (sender, e) => { itemFileSaveAs_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandFileSaveAs);

            CommandItem commandFileSaveAll = new CommandItem();
            commandFileSaveAll.Name = PublicShellDocument.Command_File_SaveAll;
            commandFileSaveAll.Description = Strings.CommandDescription_File_SaveAll;
            commandFileSaveAll.Execute += (sender, e) => { itemFileSaveAll_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandFileSaveAll);

            CommandItem commandEditUndo = new CommandItem();
            commandEditUndo.Name = PublicShellDocument.Command_Edit_Undo;
            commandEditUndo.Description = Strings.CommandDescription_Edit_Undo;
            commandEditUndo.Execute += (sender, e) => { itemEditUndo_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandEditUndo);

            CommandItem commandEditRedo = new CommandItem();
            commandEditRedo.Name = PublicShellDocument.Command_Edit_Redo;
            commandEditRedo.Description = Strings.CommandDescription_Edit_Redo;
            commandEditRedo.Execute += (sender, e) => { itemEditRedo_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandEditRedo);

            CommandItem commandEditCut = new CommandItem();
            commandEditCut.Name = PublicShellDocument.Command_Edit_Cut;
            commandEditCut.Description = Strings.CommandDescription_Edit_Cut;
            commandEditCut.Execute += (sender, e) => { itemEditCut_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandEditCut);

            CommandItem commandEditCopy = new CommandItem();
            commandEditCopy.Name = PublicShellDocument.Command_Edit_Copy;
            commandEditCopy.Description = Strings.CommandDescription_Edit_Copy;
            commandEditCopy.Execute += (sender, e) => { itemEditCopy_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandEditCopy);

            CommandItem commandEditPaste = new CommandItem();
            commandEditPaste.Name = PublicShellDocument.Command_Edit_Paste;
            commandEditPaste.Description = Strings.CommandDescription_Edit_Paste;
            commandEditPaste.Execute += (sender, e) => { itemEditPaste_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandEditPaste);

            CommandItem commandEditDelete = new CommandItem();
            commandEditDelete.Name = PublicShellDocument.Command_Edit_Delete;
            commandEditDelete.Description = Strings.CommandDescription_Edit_Delete;
            commandEditDelete.Execute += (sender, e) => { itemEditDelete_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandEditDelete);

            CommandItem commandEditSelectAll = new CommandItem();
            commandEditSelectAll.Name = PublicShellDocument.Command_Edit_SelectAll;
            commandEditSelectAll.Description = Strings.CommandDescription_Edit_SelectAll;
            commandEditSelectAll.Execute += (sender, e) => { itemEditSelectAll_Executed(null, new EventArgs()); };
            commandService.RegisterCommand(commandEditSelectAll);
        }

        private void RegisterServices()
        {
            this.Services.RegisterService<IEditorService>(new EditorService(this.Services));
        }

        #endregion

        private IEditorWindow GetCurrentEditorWindow()
        {
            return DocumentHelper.CurrentEditorWindow(this.Services);
        }

        private string GetToolWindowFileName(ToolWindowPane pane)
        {
            IEditorWindow editor = pane as IEditorWindow;
            if (pane == null)
            {
                return "";
            }
            else if (editor == null || !editor.IsFileBased)
            {
                return pane.Caption;
            }
            else
            {
                if (string.IsNullOrEmpty(editor.FileName))
                {
                    return pane.Caption;
                }
                else
                {
                    return Path.GetFileName(editor.FileName);
                }
            }
        }

        #region MenuItem QueryStatus

        private void itemEditSelectAll_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.itemEditSelectAll.Enabled = editor != null && editor.CanSelectAll;
        }

        private void itemEditDelete_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.itemEditDelete.Enabled = editor != null && editor.CanClearSelection;
        }

        private void itemEditPaste_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.itemEditPaste.Enabled = editor != null && editor.CanPaste;
        }

        private void itemEditCopy_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.itemEditCopy.Enabled = editor != null && editor.CanCopy;
        }

        private void itemEditCut_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.itemEditCut.Enabled = editor != null && editor.CanCut;
        }

        private void itemEditRedo_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.itemEditRedo.Enabled = editor != null && editor.CanRedo;
        }

        private void itemEditUndo_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.itemEditUndo.Enabled = editor != null && editor.CanUndo;
        }

        private void itemFileRecentFilesItem_QueryStatus(object sender, EventArgs e)
        {
            int index = this.itemFileRecentFilesFiles.IndexOf((ShellItem)sender);
            string[] recentFiles = this.Services.QueryService<IEditorService>().GetRecentFiles();
            if (index < recentFiles.Length)
            {
                this.itemFileRecentFilesFiles[index].Visible = true;
                this.itemFileRecentFilesFiles[index].Text = recentFiles[index];
            }
            else
            {
                this.itemFileRecentFilesFiles[index].Visible = false;
            }
        }

        private void itemFileRecentFilesEmpty_QueryStatus(object sender, EventArgs e)
        {
            IEditorService editorService = this.Services.QueryService<IEditorService>();
            this.itemFileRecentFilesEmpty.Visible = editorService.GetRecentFiles().Length == 0;
        }

        private void itemFileClose_QueryStatus(object sender, EventArgs e)
        {
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            bool enabled = pane != null && pane.DockType == ToolWindowDockType.Document;
            this.itemFileClose.Text = string.Format(Strings.MenuTextFileClose, enabled ? GetToolWindowFileName(pane) : "");
            this.itemFileClose.Enabled = enabled;
        }

        private void itemFileSaveAs_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.itemFileSaveAs.Enabled = editor != null && editor.IsFileBased;
        }

        private void itemFileSave_QueryStatus(object sender, EventArgs e)
        {
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            IEditorWindow editor = pane as IEditorWindow;
            bool enabled = editor != null && pane.DockType == ToolWindowDockType.Document;
            this.itemFileSave.Text = string.Format(Strings.MenuTextFileSave, enabled ? GetToolWindowFileName(pane) : "");
            this.itemFileSave.Enabled = editor != null && editor.IsFileBased;
        }

        #endregion

        #region ToolBar QueryStatus

        private void buttonStandardRedo_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.buttonStandardRedo.Enabled = editor != null && editor.CanRedo;
        }

        private void buttonStandardUndo_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.buttonStandardUndo.Enabled = editor != null && editor.CanUndo;
        }

        private void buttonStandardPaste_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.buttonStandardPaste.Enabled = editor != null && editor.CanPaste;
        }

        private void buttonStandardCopy_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.buttonStandardCopy.Enabled = editor != null && editor.CanCopy;
        }

        private void buttonStandardCut_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = GetCurrentEditorWindow();
            this.buttonStandardCut.Enabled = editor != null && editor.CanCut;
        }

        private void buttonStandardSave_QueryStatus(object sender, EventArgs e)
        {
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            IEditorWindow editor = pane as IEditorWindow;
            this.buttonStandardSave.ToolTipText = string.Format(Strings.ToolTipStandardSave, GetToolWindowFileName(pane));
            this.buttonStandardSave.Enabled = editor != null && editor.IsFileBased;
        }

        #endregion

        #region MenuItem Execute

        private void itemWindowWindows_Executed(object sender, EventArgs e)
        {
            using (EditingWindowsForm form = new EditingWindowsForm(this.Services))
            {
                form.ShowDialog();
            }
        }

        private void itemEditSelectAll_Executed(object sender, EventArgs e)
        {
            IEditorWindow editorWindow = GetCurrentEditorWindow();
            if (editorWindow != null)
            {
                editorWindow.SelectAll();
            }
        }

        private void itemEditDelete_Executed(object sender, EventArgs e)
        {
            IEditorWindow editorWindow = GetCurrentEditorWindow();
            if (editorWindow != null)
            {
                editorWindow.ClearSelection();
            }
        }

        private void itemEditPaste_Executed(object sender, EventArgs e)
        {
            IEditorWindow editorWindow = GetCurrentEditorWindow();
            if (editorWindow != null)
            {
                editorWindow.Paste();
            }
        }

        private void itemEditCopy_Executed(object sender, EventArgs e)
        {
            IEditorWindow editorWindow = GetCurrentEditorWindow();
            if (editorWindow != null)
            {
                editorWindow.Copy();
            }
        }

        private void itemEditCut_Executed(object sender, EventArgs e)
        {
            IEditorWindow editorWindow = GetCurrentEditorWindow();
            if (editorWindow != null)
            {
                editorWindow.Cut();
            }
        }

        private void itemEditRedo_Executed(object sender, EventArgs e)
        {
            IEditorWindow editorWindow = GetCurrentEditorWindow();
            if (editorWindow != null)
            {
                editorWindow.Redo();
            }
        }

        private void itemEditUndo_Executed(object sender, EventArgs e)
        {
            IEditorWindow editorWindow = GetCurrentEditorWindow();
            if (editorWindow != null)
            {
                editorWindow.Undo();
            }
        }

        private void itemFileRecentFilesItem_Executed(object sender, EventArgs e)
        {
            IEditorService editorService = this.Services.QueryService<IEditorService>();
            int index = this.itemFileRecentFilesFiles.IndexOf((ShellItem)sender);
            string[] recentFiles = editorService.GetRecentFiles();
            string fileName = recentFiles[index];
            if (editorService.TryOpenEditingFile(fileName) == null)
            {
                string ext = Path.GetExtension(fileName);
                if (!string.IsNullOrEmpty(ext))
                {
                    ext = ext.Substring(1);
                }
                Guid[] typeGuids = editorService.GetFileTypes(ext);
                foreach (Guid typeGuid in typeGuids)
                {
                    FileType fileType = editorService.GetFileType(typeGuid);
                    if (fileType.FileEditors.Count > 0)
                    {
                        editorService.OpenFile(fileName, typeGuid, fileType.FileEditors[0]);
                        return;
                    }
                }
                IUIShell uiShell = this.Services.QueryService<IUIShell>();
                uiShell.ShowMessage(string.Format(Strings.MessageBoxNoAssociatedEditor, fileName));
            }
        }

        private void itemFileClose_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveTabbedToolWindow();
            if (pane != null)
            {
                pane.Close();
            }
        }

        private void itemFileSaveAll_Executed(object sender, EventArgs e)
        {
            IEditorService editorService = this.Services.QueryService<IEditorService>();
            foreach (ToolWindowPane pane in editorService.GetEditorWindows())
            {
                editorService.SaveEditingContent(pane, false);
            }
        }

        private void itemFileSaveAs_Executed(object sender, EventArgs e)
        {
            IEditorWindow editorWindow = GetCurrentEditorWindow();
            if (editorWindow != null)
            {
                IEditorService editorService = this.Services.QueryService<IEditorService>();
                editorService.SaveEditingContent(editorWindow as ToolWindowPane, true);
            }
        }

        private void itemFileSave_Executed(object sender, EventArgs e)
        {
            IEditorWindow editorWindow = GetCurrentEditorWindow();
            if (editorWindow != null)
            {
                IEditorService editorService = this.Services.QueryService<IEditorService>();
                editorService.SaveEditingContent(editorWindow as ToolWindowPane, false);
            }
        }

        private void itemFileOpenFile_Executed(object sender, EventArgs e)
        {
            IEditorService editorService = this.Services.QueryService<IEditorService>();
            Guid fileType = Guid.Empty;
            string fileName = null;
            if (editorService.ShowOpenDialog(editorService.GetFileTypes(), out fileType, out fileName))
            {
                if (editorService.TryOpenEditingFile(fileName) == null)
                {
                    string ext = Path.GetExtension(fileName);
                    if (!string.IsNullOrEmpty(ext))
                    {
                        ext = ext.Substring(1);
                    }
                    Guid[] editors = editorService.GetEditors(ext);
                    if (editors.Length == 0)
                    {
                        IUIShell uiShell = this.Services.QueryService<IUIShell>();
                        uiShell.ShowMessage(string.Format(Strings.MessageBoxNoAssociatedEditor, fileName));
                    }
                    else
                    {
                        editorService.OpenFile(fileName, fileType, editors[0]);
                    }
                }
            }
        }

        private void itemFileNewFile_Executed(object sender, EventArgs e)
        {
            using (NewFileForm form = new NewFileForm(this.Services))
            {
                if (form.ShowDialog() == DialogResult.OK)
                {
                    IEditorService editorService = this.Services.QueryService<IEditorService>();
                    FileType fileType = editorService.GetFileType(form.FileType);
                    if (fileType.FileEditors.Count == 0)
                    {
                        IUIShell uiShell = this.Services.QueryService<IUIShell>();
                        uiShell.ShowMessage(string.Format(Strings.MessageBoxNoAssociatedEditor, fileType.FileTypeName));
                    }
                    else
                    {
                        editorService.NewFile(form.FileType, fileType.FileEditors[0]);
                    }
                }
            }
        }

        #endregion
    }

    public static class DocumentHelper
    {
        public static IEditorWindow CurrentEditorWindow(IServiceContainer serviceContainer)
        {
            IUIShell uiShell = serviceContainer.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.GetActiveToolWindow();
            if (pane == null)
            {
                return null;
            }
            else if (pane is IEditorWindow)
            {
                return pane as IEditorWindow;
            }
            else
            {
                pane = uiShell.GetActiveTabbedToolWindow();
                if (pane == null)
                {
                    return null;
                }
                else
                {
                    return pane as IEditorWindow;
                }
            }
        }
    }
}
