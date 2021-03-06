﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls.Extension;
using Developer.WinFormControls;
using Developer.LanguageProvider;
using Developer.LanguageServices.NativeX.SyntaxTree;
using System.Xml.Linq;

namespace Developer.LanguageServices.NativeX.Extension
{
    public class NativeXTooltipProvider : ITextEditorBoxTooltipExtension
    {
        public ITextEditorControlPanelCallBack Callback { get; private set; }

        protected NativeXEditingObserverProvider EditingObserverExtension { get; private set; }

        public NativeXTooltipProvider(NativeXEditingObserverProvider editingObserverExtension)
        {
            this.EditingObserverExtension = editingObserverExtension;
        }

        public virtual string OnGetSimpleTooltip(TextPosition pos)
        {
            if (this.EditingObserverExtension.AnalyzingResult == null || this.EditingObserverExtension.AnalyzingResult.Unit == null)
            {
                return null;
            }
            NativeXNode node = this.EditingObserverExtension.AnalyzingResult.Unit.FindDeepest<NativeXNode>(pos);
            NativeXNode hint = null;
            {
                NativeXReferenceExpression exp = node as NativeXReferenceExpression;
                if (exp != null && exp.Scope != null)
                {
                    hint = (NativeXNode)exp.Scope.Find(exp.ReferencedName);
                    goto SHOW_HINT;
                }
            }
            {
                NativeXReferenceType type = node as NativeXReferenceType;
                if (type != null && type.Scope != null)
                {
                    hint = (NativeXNode)type.Scope.Find(type.ReferencedName);
                    goto SHOW_HINT;
                }
            }
            {
                NativeXConceptReference concept = node as NativeXConceptReference;
                if (concept != null && concept.Scope != null)
                {
                    hint = (NativeXNode)concept.Scope.Find(concept.ReferenceName);
                    goto SHOW_HINT;
                }
            }
            {
                NativeXInstanciatedExpression exp = node as NativeXInstanciatedExpression;
                if (exp != null && exp.Scope != null)
                {
                    hint = (NativeXNode)exp.Scope.Find(exp.ReferencedName);
                    goto SHOW_HINT;
                }
            }
            {
                NativeXInstanceFunctionExpression exp = node as NativeXInstanceFunctionExpression;
                if (exp != null && exp.Scope != null && exp.ConceptName != null)
                {
                    hint = (NativeXNode)exp.Scope.Find(exp.ConceptName.ReferenceName);
                    goto SHOW_HINT;
                }
            }
            {
                NativeXInstanceDeclaration inst = node as NativeXInstanceDeclaration;
                if (inst != null && inst.Scope != null && inst.ConceptName != null)
                {
                    if (this.EditingObserverExtension.AnalyzingResult.IdTokens.ContainsKey(pos.row))
                    {
                        CodeToken token = this.EditingObserverExtension.AnalyzingResult.IdTokens[pos.row].Where(t => t.Start <= pos && pos <= t.End).FirstOrDefault();
                        if (token.Value == inst.ConceptName.ReferenceName)
                        {
                            hint = (NativeXNode)inst.Scope.Find(inst.ConceptName.ReferenceName);
                        }
                    }
                }
            }
            {
                NativeXGenericConstraint genecons = node as NativeXGenericConstraint;
                if (genecons != null && genecons.Scope != null && genecons.ConceptName != null)
                {
                    if (this.EditingObserverExtension.AnalyzingResult.IdTokens.ContainsKey(pos.row))
                    {
                        CodeToken token = this.EditingObserverExtension.AnalyzingResult.IdTokens[pos.row].Where(t => t.Start <= pos && pos <= t.End).FirstOrDefault();
                        if (token.Value == genecons.ConceptName.ReferenceName)
                        {
                            hint = (NativeXNode)genecons.Scope.Find(genecons.ConceptName.ReferenceName);
                        }
                    }
                }
            }
        SHOW_HINT:
            if (hint != null)
            {
                NativeXFunctionDeclaration function = hint as NativeXFunctionDeclaration;
                TextPosition start, end;
                if (function == null)
                {
                    start = hint.Start;
                    end = hint.End;
                }
                else
                {
                    start = hint.Start;
                    end = function.Statement == null ? function.End : function.Statement.Start;
                }
                if (start < end)
                {
                    start = this.Callback.TextEditorBox.Controller.Normalize(start);
                    end = this.Callback.TextEditorBox.Controller.Normalize(end);
                    string text = this.Callback.TextEditorBox.TextProvider.GetString(start, end);
                    if (!string.IsNullOrWhiteSpace(text))
                    {
                        return text;
                    }
                }
            }
            return null;
        }

        public Tuple<XDocument, TextPosition> OnGetQuickInfoTooltip()
        {
            return null;
        }

        public virtual void Attach(ITextEditorControlPanelCallBack callback)
        {
            this.Callback = callback;
        }
    }
}
