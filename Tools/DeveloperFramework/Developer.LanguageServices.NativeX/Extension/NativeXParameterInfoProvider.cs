﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls.Extension;
using System.Xml.Linq;
using Developer.WinFormControls;
using Developer.LanguageProvider;
using Developer.LanguageServices.NativeX.SyntaxTree;

namespace Developer.LanguageServices.NativeX.Extension
{
    public class NativeXParameterInfoProvider
        : ITextEditorBoxEditingObserverExtension
        , ITextEditorBoxTooltipExtension
    {
        protected NativeXEditingObserverProvider EditingObserverExtension { get; private set; }

        public ITextEditorControlPanelCallBack Callback { get; private set; }

        public NativeXParameterInfoProvider(NativeXEditingObserverProvider editingObserverExtension)
        {
            this.EditingObserverExtension = editingObserverExtension;
        }

        public void OnBeforeEdit(TextPosition start, TextPosition end, ref string[] lines)
        {
        }

        public void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
            if (this.Callback.TextEditorBox.PressingChar
                && this.EditingObserverExtension.EditingNode != null
                && this.EditingObserverExtension.EditingNode.Scope != null
                )
            {
                var editingNode = this.EditingObserverExtension.EditingNode;
                TextPosition pos = this.EditingObserverExtension.CodePositionToEditing(newEnd);
                string inputText = this.Callback.TextEditorBox.TextProvider.GetString(start, newEnd);
                switch (inputText)
                {
                    case "(":
                    case ",":
                        {
                            if (editingNode.FindDeepest<NativeXInvokeExpression>(pos) != null)
                            {
                                this.Callback.TextEditorBox.UIExtensions.OpenQuickInfoTooltip();
                            }
                        }
                        break;
                    case ")":
                        {
                            NativeXNode node = editingNode.FindDeepest<NativeXNode>(pos);
                            if (node == null || node.FindParent<NativeXInvokeExpression>() == null)
                            {
                                this.Callback.TextEditorBox.UIExtensions.CloseQuickInfoTooltip();
                            }
                        }
                        break;
                }
            }
        }

        public string OnGetSimpleTooltip(TextPosition pos)
        {
            return null;
        }

        public Tuple<XDocument, TextPosition> OnGetQuickInfoTooltip()
        {
            if (this.EditingObserverExtension.EditingNode != null
                && this.EditingObserverExtension.EditingNode.Scope != null
                )
            {
                var editingNode = this.EditingObserverExtension.EditingNode;
                TextPosition pos = this.EditingObserverExtension.NodePositionToEditing(this.Callback.TextEditorBox.SelectionCaret);
                NativeXNode node = editingNode.FindDeepest<NativeXNode>(pos);
                if (node != null)
                {
                    NativeXInvokeExpression invoke = (node as NativeXInvokeExpression) ?? node.FindParent<NativeXInvokeExpression>();
                    while (invoke != null && invoke.Function != null)
                    {
                        TextPosition start = invoke.Function.End;
                        TextPosition end = this.EditingObserverExtension.CodePositionToEditing(this.Callback.TextEditorBox.SelectionCaret);
                        if (!this.EditingObserverExtension.IsTemporaryEditingNode)
                        {
                            start = this.EditingObserverExtension.CodePositionToEditing(start);
                        }
                        if (start < end)
                        {
                            string inside = this.EditingObserverExtension.EditingNodeCode.GetString(start, end);
                            int openCount = inside.Where(c => c == '(').Count();
                            int closeCount = inside.Where(c => c == ')').Count();
                            if (openCount > closeCount)
                            {
                                break;
                            }
                        }
                        invoke = invoke.FindParent<NativeXInvokeExpression>();
                    }
                    if (invoke != null && invoke.Function != null)
                    {
                        NativeXExpression function = invoke.Function;
                        int index = 0;
                        if (invoke.Arguments != null)
                        {
                            for (int i = 0; i < invoke.Arguments.Count; i++)
                            {
                                NativeXNode argument = invoke.Arguments[i];
                                if (argument != null && argument.Start <= pos && pos <= argument.End)
                                {
                                    index = i;
                                    break;
                                }
                                else
                                {
                                    index = Math.Min(index + 1, invoke.Arguments.Count - 1);
                                }
                            }
                        }
                        Tuple<string, string, string> info = function.GetFormattedFunctionQuickInfo(index);
                        if (!string.IsNullOrEmpty(info.Item1 + info.Item2 + info.Item3))
                        {
                            var content = new RichContent.Content(
                                new RichContent.Text(info.Item1),
                                new RichContent.Bold(new RichContent.Text(info.Item2)),
                                new RichContent.Text(info.Item3)
                                );
                            return Tuple.Create<XDocument, TextPosition>(content, this.EditingObserverExtension.NodePositionToGlobal(invoke.Start));
                        }
                    }
                }
            }
            return null;
        }

        public void Attach(ITextEditorControlPanelCallBack callback)
        {
            this.Callback = callback;
        }
    }
}
