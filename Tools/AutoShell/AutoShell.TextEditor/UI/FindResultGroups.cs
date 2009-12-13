using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.Services;

namespace AutoShell.UI
{
    internal static class FindTargetGroups
    {
        public class CurrentDocumentGroup : IFindTargetGroup
        {
            private IServiceContainer serviceContainer;

            public CurrentDocumentGroup(IServiceContainer serviceContainer)
            {
                this.serviceContainer = serviceContainer;
            }

            #region IFindTargetGroup Members

            public string Name
            {
                get
                {
                    return Strings.FindTargetGroupCurrentWindow;
                }
            }

            public Dictionary<string, IFindTarget> FindTargets
            {
                get
                {
                    Dictionary<String, IFindTarget> targets = new Dictionary<string, IFindTarget>();
                    if (this.Current != null)
                    {
                        targets.Add(this.Current.UniqueName, this.Current);
                    }
                    return targets;
                }
            }

            public IFindTarget Current
            {
                get
                {
                    IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
                    ToolWindowPane pane = uiShell.GetActiveTabbedToolWindow();
                    return pane as IFindTarget;
                }
            }

            public IFindTarget GetNext(IFindTarget current, bool forward)
            {
                return this.Current;
            }

            #endregion
        }

        public class AllOpenDocuments : IFindTargetGroup
        {
            private IServiceContainer serviceContainer;

            public AllOpenDocuments(IServiceContainer serviceContainer)
            {
                this.serviceContainer = serviceContainer;
            }

            #region IFindTargetGroup Members

            public string Name
            {
                get
                {
                    return Strings.FindTargetGroupAllOpenDocuments;
                }
            }

            public Dictionary<string, IFindTarget> FindTargets
            {
                get
                {
                    IEditorService editorService = this.serviceContainer.QueryService<IEditorService>();
                    Dictionary<string, IFindTarget> targets = new Dictionary<string, IFindTarget>();
                    foreach (ToolWindowPane pane in editorService.GetEditorWindows())
                    {
                        IFindTarget target = pane as IFindTarget;
                        if (target != null)
                        {
                            targets.Add(target.UniqueName, target);
                        }
                    }
                    return targets;
                }
            }

            public IFindTarget Current
            {
                get
                {
                    IUIShell uiShell=this.serviceContainer.QueryService<IUIShell>();
                    List<IFindTarget> targets = this.FindTargets.Values.ToList();
                    if (targets.Count > 0)
                    {
                        ToolWindowPane pane = uiShell.GetActiveTabbedToolWindow();
                        IFindTarget target = pane as IFindTarget;
                        if (targets.Contains(target))
                        {
                            return target;
                        }
                        else
                        {
                            return targets[0];
                        }
                    }
                    else
                    {
                        return null;
                    }
                }
            }

            public IFindTarget GetNext(IFindTarget current, bool forward)
            {
                IUIShell uiShell = this.serviceContainer.QueryService<IUIShell>();
                List<IFindTarget> targets = this.FindTargets.Values.ToList();
                if (targets.Count > 0)
                {
                    ToolWindowPane pane = uiShell.GetActiveTabbedToolWindow();
                    IFindTarget target = pane as IFindTarget;
                    int index = targets.IndexOf(target);
                    if (index == -1)
                    {
                        return targets[0];
                    }
                    else
                    {
                        index += forward ? 1 : -1;
                        if (index == -1)
                        {
                            index = targets.Count - 1;
                        }
                        else if(index==targets.Count)
                        {
                            index = 0;
                        }
                        return targets[index];
                    }
                }
                else
                {
                    return null;
                }
            }

            #endregion
        }
    }
}
