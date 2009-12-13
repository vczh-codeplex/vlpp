using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using AutoShell.Services;
using System.Text.RegularExpressions;
using AutoShell.PublicShell;
using AutoShell.UI.Option;

namespace AutoShell.UI.ToolWindow
{
    internal class FindWindowPane : ToolWindowPane, IFindService
    {
        public enum ActionKind
        {
            QuickFind,
            FindInFiles,
            QuickReplace,
            ReplaceInFiles
        }

        private FindWindowControl control = null;
        private ShellItem itemFind = null;
        private ShellItem itemQuickFind = null;
        private ShellItem itemFindInFiles = null;
        private ShellItem itemQuickReplace = null;
        private ShellItem itemReplaceInFiles = null;
        private ActionKind actionKind = ActionKind.QuickFind;

        private List<IFindTargetGroup> groups = new List<IFindTargetGroup>();
        private bool finding = false;

        public FindWindowPane()
        {
            this.control = new FindWindowControl();
            this.control.Find += new EventHandler(control_Find);
        }

        public override Control ToolWindowControl
        {
            get
            {
                return this.control;
            }
        }

        public override Size DefaultSize
        {
            get
            {
                return this.control.MinimumSize;
            }
        }

        public void SetActionKind(ActionKind kind)
        {
            ShellItem activatedItem = null;
            switch (kind)
            {
                case ActionKind.QuickFind:
                    activatedItem = this.itemQuickFind;
                    this.control.EnableBackward = true;
                    this.control.EnableReplace = false;
                    break;
                case ActionKind.FindInFiles:
                    activatedItem = this.itemFindInFiles;
                    this.control.EnableBackward = false;
                    this.control.EnableReplace = false;
                    break;
                case ActionKind.QuickReplace:
                    activatedItem = this.itemQuickReplace;
                    this.control.EnableBackward = true;
                    this.control.EnableReplace = true;
                    break;
                case ActionKind.ReplaceInFiles:
                    activatedItem = this.itemReplaceInFiles;
                    this.control.EnableBackward = false;
                    this.control.EnableReplace = true;
                    break;
            }
            this.actionKind = kind;
            this.itemFind.SetActivatedItem(activatedItem);
            this.Caption = string.Format(Strings.ToolWindowFind, activatedItem.Text);
        }

        protected override void OnInitialize()
        {
            this.Caption = Strings.ToolWindowFind;
            this.Icon = Icons.FindResultWindow;
            this.EnableModifyDockType = false;

            ShellGroup toolBarGroup = new ShellGroup(0x0000);

            this.itemFind = new ShellItem(ShellItemKind.TrackButton, 0x0000);
            this.itemFind.Text = Strings.ToolItemQuickFind;
            toolBarGroup.AddEntry(this.itemFind);

            this.itemQuickFind = new ShellItem(ShellItemKind.Button, 0x0000);
            this.itemQuickFind.Text = Strings.ToolItemQuickFind;
            this.itemQuickFind.Icon = Icons.EditQuickFind;
            this.itemQuickFind.Executed += new EventHandler(itemQuickFind_Executed);
            itemFind.SubGroup.AddEntry(this.itemQuickFind);

            this.itemFindInFiles = new ShellItem(ShellItemKind.Button, 0x0000);
            this.itemFindInFiles.Text = Strings.ToolItemFindInFiles;
            this.itemFindInFiles.Icon = Icons.EditFindInFiles;
            this.itemFindInFiles.Executed += new EventHandler(itemFindInFiles_Executed);
            itemFind.SubGroup.AddEntry(this.itemFindInFiles);

            this.itemQuickReplace = new ShellItem(ShellItemKind.Button, 0x0000);
            this.itemQuickReplace.Text = Strings.ToolItemQuickReplace;
            this.itemQuickReplace.Icon = Icons.EditQuickReplace;
            this.itemQuickReplace.Executed += new EventHandler(itemQuickReplace_Executed);
            itemFind.SubGroup.AddEntry(this.itemQuickReplace);

            this.itemReplaceInFiles = new ShellItem(ShellItemKind.Button, 0x0000);
            this.itemReplaceInFiles.Text = Strings.ToolItemReplaceInFiles;
            this.itemReplaceInFiles.Icon = Icons.EditReplaceInFiles;
            this.itemReplaceInFiles.Executed += new EventHandler(itemReplaceInFiles_Executed);
            itemFind.SubGroup.AddEntry(this.itemReplaceInFiles);

            this.ToolBarGroup = toolBarGroup;
            SetActionKind(ActionKind.QuickFind);

            this.Services.RegisterService<IFindService>(this);
        }

        private void itemReplaceInFiles_Executed(object sender, EventArgs e)
        {
            SetActionKind(ActionKind.ReplaceInFiles);
        }

        private void itemQuickReplace_Executed(object sender, EventArgs e)
        {
            SetActionKind(ActionKind.QuickReplace);
        }

        private void itemFindInFiles_Executed(object sender, EventArgs e)
        {
            SetActionKind(ActionKind.FindInFiles);
        }

        private void itemQuickFind_Executed(object sender, EventArgs e)
        {
            SetActionKind(ActionKind.QuickFind);
        }

        protected override void OnOpening()
        {
        }

        protected override void OnClosing(ref bool cancel)
        {
        }

        protected override void OnClosed()
        {
        }

        protected override void OnQueryStatus()
        {
        }

        private void UpdateFindResultWindow(List<FindResult> results)
        {
            IUIShell uiShell = this.Services.QueryService<IUIShell>();
            FindResultWindowPane pane = uiShell.FindToolWindow(PublicShellTextEditor.FindResultWindow)[0] as FindResultWindowPane;
            pane.SetResults(results.ToArray());
        }

        private void control_Find(object sender, EventArgs e)
        {
            try
            {
                this.control.UpdateRegex();
            }
            catch (ArgumentException ex)
            {
                this.Services.QueryService<IUIShell>().ShowMessage(ex.Message, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            RegexPattern pattern = this.control.ReplacementPattern;
            bool forward = this.control.Forward;
            IFindTargetGroup group = this.control.Group;

            switch (this.actionKind)
            {
                case ActionKind.QuickFind:
                case ActionKind.QuickReplace:
                    {
                        Regex regex = forward ? this.control.ForwardRegex : this.control.BackwordRegex;
                        IFindTarget firstTarget = group.Current;
                        IFindTarget currentTarget = group.Current;
                        int firstCount = 0;
                        bool found = false;

                        while (firstCount < 2 && currentTarget != null)
                        {
                            string text = currentTarget.Text;
                            int start = forward ? 0 : text.Length;
                            if (currentTarget == firstTarget && firstCount == 0)
                            {
                                start = currentTarget.Position;
                            }

                            if (this.actionKind == ActionKind.QuickFind || currentTarget.CanReplace)
                            {
                                Match match = regex.Match(text, start);
                                if (match.Success)
                                {
                                    found = true;
                                    FindResult result = new FindResult()
                                    {
                                        FindTarget = currentTarget,
                                        Start = match.Index,
                                        Length = match.Length
                                    };
                                    if (this.actionKind == ActionKind.QuickFind)
                                    {
                                        currentTarget.Focus(result);
                                    }
                                    else
                                    {
                                        currentTarget.Replace(result, pattern.Generate(match));
                                    }
                                    break;
                                }
                            }

                            if (currentTarget == firstTarget)
                            {
                                firstCount++;
                            }
                            currentTarget = group.GetNext(currentTarget, forward);
                        }
                        if (!found)
                        {
                            this.Services.QueryService<IUIShell>().ShowMessage(string.Format(Strings.FindNotValue, this.control.FindText));
                        }
                    }
                    break;
                case ActionKind.FindInFiles:
                case ActionKind.ReplaceInFiles:
                    {
                        IOptionService optionService = this.Services.QueryService<IOptionService>();
                        FindOption findOption = optionService.GetOption(FindOption.OptionKey) as FindOption;

                        this.finding = true;
                        List<FindResult> results = new List<FindResult>();
                        foreach (IFindTarget target in group.FindTargets.Values)
                        {
                            if (this.actionKind == ActionKind.FindInFiles || target.CanReplace)
                            {
                                Match[] matches = this.control.ForwardRegex.Matches(target.Text).Cast<Match>().ToArray();
                                FindResult[] targetResults = matches
                                    .Select(match => new FindResult()
                                    {
                                        FindTarget = target,
                                        Start = match.Index,
                                        Length = match.Length
                                    })
                                    .ToArray();
                                results.AddRange(targetResults);
                                target.SetResultAndTrack(targetResults);
                                if (this.actionKind == ActionKind.ReplaceInFiles)
                                {
                                    target.ReplaceTrackedResult(matches
                                        .Select(match => pattern.Generate(match))
                                        .ToArray()
                                        );
                                }
                                if (!findOption.EditorWindowMaintainFindResult)
                                {
                                    target.SetResultAndTrack(new FindResult[] { });
                                }
                            }
                        }
                        UpdateFindResultWindow(results);
                        if (results.Count == 0)
                        {
                            this.Services.QueryService<IUIShell>().ShowMessage(string.Format(Strings.FindNotValue, this.control.FindText));
                        }
                        else
                        {
                            if (findOption.CloseWindowAfterSearching)
                            {
                                Close();
                            }
                        }
                        this.finding = false;
                    }
                    break;
            }
        }

        #region IFindService Members

        void IFindService.RegisterGroup(IFindTargetGroup group)
        {
            this.groups.Add(group);
            this.control.UpdateGroups(this.groups);
        }

        void IFindService.UnregisterGroup(IFindTargetGroup group)
        {
            this.groups.Remove(group);
            this.control.UpdateGroups(this.groups);
        }

        IFindTargetGroup[] IFindService.GetGroups()
        {
            return this.groups.ToArray();
        }

        void IFindService.UpdateFindResults(FindResult[] results)
        {
            if (!this.finding)
            {
                IUIShell uiShell = this.Services.QueryService<IUIShell>();
                FindResultWindowPane pane = uiShell.FindToolWindow(PublicShellTextEditor.FindResultWindow)[0] as FindResultWindowPane;
                pane.UpdateResults(results);
            }
        }

        void IFindService.SetFindResults(FindResult[] results)
        {
            UpdateFindResultWindow(results.ToList());
        }

        #endregion
    }
}
