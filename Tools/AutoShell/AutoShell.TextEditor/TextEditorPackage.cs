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
using AutoShell.UI.Option;

namespace AutoShell
{
    public class TextEditorPackage : Package
    {
        private ShellGroup menuEditOperation = null;
        private ShellGroup menuEditFind = null;
        private ShellGroup menuViewWindows = null;

        private ShellItem itemEditFind = null;
        private ShellItem itemEditFindQuickFind = null;
        private ShellItem itemEditFindQuickReplace = null;
        private ShellItem itemEditFindFindInFiles = null;
        private ShellItem itemEditFindReplaceInFiles = null;
        private ShellItem itemEditGoTo = null;

        private ShellItem itemViewFindResultWindow = null;

        public override void Initialize()
        {
            base.Initialize();
            IUIShell uiShell = this.Services.QueryService<IUIShell>();

            RegisterMenuItems(uiShell);
            RegisterToolWindows(uiShell);
            RegisterOptions();

            IFindService findService = this.Services.QueryService<IFindService>();
            findService.RegisterGroup(new FindTargetGroups.CurrentDocumentGroup(this.Services));
            findService.RegisterGroup(new FindTargetGroups.AllOpenDocuments(this.Services));
        }

        #region Configuration

        private void RegisterMenuItems(IUIShell uiShell)
        {
            this.menuEditOperation = uiShell.GetShellGroup(PublicShellDocument.EditOperationMenUGroup);
            this.menuEditFind = uiShell.RegisterShellGroup(PublicShellTextEditor.EditFindMenuGroup, new ShellGroup(0x0000));
            this.menuViewWindows = uiShell.GetShellGroup(PublicShellCore.ViewWindowsMenuGroup);

            this.itemEditFind = new ShellItem(ShellItemKind.Button, 0x0200);
            this.itemEditFind.Text = Strings.MenuTextEditFind;
            this.itemEditFind.Icon = Icons.EditQuickFind;
            this.itemEditFind.SubGroup.AddEntry(this.menuEditFind);
            this.menuEditOperation.AddEntry(this.itemEditFind);

            this.itemEditGoTo = new ShellItem(ShellItemKind.Button, 0x0300);
            this.itemEditGoTo.Text = Strings.MenuTextEditGoTo;
            this.itemEditGoTo.ShortcutKeys = Keys.Control | Keys.G;
            this.itemEditGoTo.QueryStatus += new EventHandler(itemEditGoTo_QueryStatus);
            this.itemEditGoTo.Executed += new EventHandler(itemEditGoTo_Executed);
            this.menuEditOperation.AddEntry(this.itemEditGoTo);

            this.itemEditFindQuickFind = new ShellItem(ShellItemKind.Button, 0x0100);
            this.itemEditFindQuickFind.Text = Strings.MenuTextEditFindQuickFind;
            this.itemEditFindQuickFind.Icon = Icons.EditQuickFind;
            this.itemEditFindQuickFind.ShortcutKeys = Keys.Control | Keys.F;
            this.itemEditFindQuickFind.Executed += new EventHandler(itemEditFindQuickFind_Executed);
            this.menuEditFind.AddEntry(this.itemEditFindQuickFind);

            this.itemEditFindQuickReplace = new ShellItem(ShellItemKind.Button, 0x0200);
            this.itemEditFindQuickReplace.Text = Strings.MenuTextEditFindQuickReplace;
            this.itemEditFindQuickReplace.Icon = Icons.EditQuickReplace;
            this.itemEditFindQuickReplace.ShortcutKeys = Keys.Control | Keys.H;
            this.itemEditFindQuickReplace.Executed += new EventHandler(itemEditFindQuickReplace_Executed);
            this.menuEditFind.AddEntry(this.itemEditFindQuickReplace);

            this.itemEditFindFindInFiles = new ShellItem(ShellItemKind.Button, 0x0300);
            this.itemEditFindFindInFiles.Text = Strings.MenuTextEditFindFindInFiles;
            this.itemEditFindFindInFiles.Icon = Icons.EditFindInFiles;
            this.itemEditFindFindInFiles.ShortcutKeys = Keys.Control | Keys.Shift | Keys.F;
            this.itemEditFindFindInFiles.Executed += new EventHandler(itemEditFindFindInFiles_Executed);
            this.menuEditFind.AddEntry(this.itemEditFindFindInFiles);

            this.itemEditFindReplaceInFiles = new ShellItem(ShellItemKind.Button, 0x0400);
            this.itemEditFindReplaceInFiles.Text = Strings.MenuTextEditFindReplaceInFiles;
            this.itemEditFindReplaceInFiles.Icon = Icons.EditReplaceInFiles;
            this.itemEditFindReplaceInFiles.ShortcutKeys = Keys.Control | Keys.Shift | Keys.H;
            this.itemEditFindReplaceInFiles.Executed += new EventHandler(itemEditFindReplaceInFiles_Executed);
            this.menuEditFind.AddEntry(this.itemEditFindReplaceInFiles);

            this.itemViewFindResultWindow = new ShellItem(ShellItemKind.Button, 0x0003);
            this.itemViewFindResultWindow.Text = Strings.MenuTextViewFindResultWindow;
            this.itemViewFindResultWindow.Icon = Icons.FindResultWindow;
            this.itemViewFindResultWindow.Executed += new EventHandler(itemViewFindResultWindow_Executed);
            this.menuViewWindows.AddEntry(this.itemViewFindResultWindow);
        }

        private void RegisterToolWindows(IUIShell uiShell)
        {
            uiShell.RegisterToolWindow(PublicShellTextEditor.FindWindow, typeof(FindWindowPane), ToolWindowDockType.Floating, false);
            uiShell.CreateToolWindow(PublicShellTextEditor.FindWindow);

            uiShell.RegisterToolWindow(PublicShellTextEditor.FindResultWindow, typeof(FindResultWindowPane), ToolWindowDockType.Bottom, false);
            uiShell.CreateToolWindow(PublicShellTextEditor.FindResultWindow);
        }

        private void RegisterOptions()
        {
            IOptionService optionService = this.Services.QueryService<IOptionService>();
            optionService.RegisterOption(FindOption.OptionKey, typeof(FindOption));
            optionService.RegisterOptionEditor(FindOption.OptionKey, PublicShellTextEditor.FindOptions, PublicShellCore.EnvironmentOptions, Strings.OptionNameFind, new FindOptionEditor());
        }

        #endregion

        #region MenuItem QueryStatus

        private void itemEditGoTo_QueryStatus(object sender, EventArgs e)
        {
            IEditorWindow editor = DocumentHelper.CurrentEditorWindow(this.Services);
            IGotoableWindow gotoWindow = editor as IGotoableWindow;
            this.itemEditGoTo.Enabled = gotoWindow != null && gotoWindow.CanGoto;
        }

        #endregion

        #region MenuItem Execute

        private void itemViewFindResultWindow_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            ToolWindowPane pane = uiShell.FindToolWindow(PublicShellTextEditor.FindResultWindow)[0];
            pane.Open();
        }

        private void itemEditGoTo_Executed(object sender, EventArgs e)
        {
            IEditorWindow editorWindow = DocumentHelper.CurrentEditorWindow(this.Services);
            if (editorWindow != null && editorWindow is IGotoableWindow)
            {
                IGotoableWindow gotoableWindow = editorWindow as IGotoableWindow;
                using (GotoForm form = new GotoForm(gotoableWindow.MaxGotoLines))
                {
                    form.LineNumber = gotoableWindow.CurrentLineNumber;
                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        gotoableWindow.Goto(form.LineNumber);
                    }
                }
            }
        }

        private void itemEditFindReplaceInFiles_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            FindWindowPane pane = uiShell.FindToolWindow(PublicShellTextEditor.FindWindow)[0] as FindWindowPane;
            pane.SetActionKind(FindWindowPane.ActionKind.ReplaceInFiles);
            pane.Open();
        }

        private void itemEditFindFindInFiles_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            FindWindowPane pane = uiShell.FindToolWindow(PublicShellTextEditor.FindWindow)[0] as FindWindowPane;
            pane.SetActionKind(FindWindowPane.ActionKind.FindInFiles);
            pane.Open();
        }

        private void itemEditFindQuickReplace_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            FindWindowPane pane = uiShell.FindToolWindow(PublicShellTextEditor.FindWindow)[0] as FindWindowPane;
            pane.SetActionKind(FindWindowPane.ActionKind.QuickReplace);
            pane.Open();
        }

        private void itemEditFindQuickFind_Executed(object sender, EventArgs e)
        {
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            FindWindowPane pane = uiShell.FindToolWindow(PublicShellTextEditor.FindWindow)[0] as FindWindowPane;
            pane.SetActionKind(FindWindowPane.ActionKind.QuickFind);
            pane.Open();
        }

        #endregion
    }
}
