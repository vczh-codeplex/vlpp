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
                TextPosition pos = this.EditingObserverExtension.ConvertToEditingPosition(newEnd);
                string inputText = this.Callback.TextEditorBox.TextProvider.GetString(start, newEnd);
                switch (inputText)
                {
                    case "(":
                    case ",":
                        {
                            if (editingNode.FindDeepest<NativeXInvokeExpression>(pos) != null)
                            {
                                this.Callback.TextEditorBox.OpenQuickInfoTooltip();
                            }
                        }
                        break;
                    case ")":
                        {
                            NativeXNode node = editingNode.FindDeepest<NativeXNode>(pos);
                            if (node == null || node.FindParent<NativeXInvokeExpression>() == null)
                            {
                                this.Callback.TextEditorBox.CloseQuickInfoTooltip();
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

        public XDocument OnGetQuickInfoTooltip()
        {
            return null;
        }

        public void Attach(ITextEditorControlPanelCallBack callback)
        {
            this.Callback = callback;
        }
    }
}
