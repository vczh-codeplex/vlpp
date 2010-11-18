using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls.Extension;
using Developer.WinFormControls;
using Developer.LanguageProvider;
using Developer.LanguageServices.NativeX.SyntaxTree;

namespace Developer.LanguageServices.NativeX.Extension
{
    public class NativeXTooltipProvider : ITextEditorBoxTooltipExtension
    {
        public ITextEditorControlPanelCallBack Callback { get; private set; }

        protected NativeXEditingObserverProvider Extension { get; private set; }

        public NativeXTooltipProvider(NativeXEditingObserverProvider extension)
        {
            this.Extension = extension;
        }

        public virtual string OnGetSimpleTooltip(TextPosition pos)
        {
            if (this.Extension.AnalyzingResult == null || this.Extension.AnalyzingResult.Unit == null)
            {
                return null;
            }
            NativeXNode node = this.Extension.AnalyzingResult.Unit.FindDeepest<NativeXNode>(pos);
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
                NativeXInstanciatedExpression exp = node as NativeXInstanciatedExpression;
                if (exp != null && exp.Scope != null)
                {
                    hint = (NativeXNode)exp.Scope.Find(exp.ReferencedName);
                    goto SHOW_HINT;
                }
            }
            {
                NativeXInstanceFunctionExpression exp = node as NativeXInstanceFunctionExpression;
                if (exp != null && exp.Scope != null)
                {
                    hint = (NativeXNode)exp.Scope.Find(exp.ConceptName);
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
                NativeXInstanceDeclaration inst = node as NativeXInstanceDeclaration;
                if (inst != null && inst.Scope != null)
                {
                    if (this.Extension.AnalyzingResult.IdTokens.ContainsKey(pos.row))
                    {
                        CodeToken token = this.Extension.AnalyzingResult.IdTokens[pos.row].Where(t => t.Start <= pos && pos <= t.End).FirstOrDefault();
                        if (token.Value == inst.ConceptName)
                        {
                            hint = (NativeXNode)inst.Scope.Find(inst.ConceptName);
                        }
                    }
                }
            }
            {
                NativeXGenericConstraint genecons = node as NativeXGenericConstraint;
                if (genecons != null && genecons.Scope != null)
                {
                    if (this.Extension.AnalyzingResult.IdTokens.ContainsKey(pos.row))
                    {
                        CodeToken token = this.Extension.AnalyzingResult.IdTokens[pos.row].Where(t => t.Start <= pos && pos <= t.End).FirstOrDefault();
                        if (token.Value == genecons.ConceptName)
                        {
                            hint = (NativeXNode)genecons.Scope.Find(genecons.ConceptName);
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

        public virtual void Attach(ITextEditorControlPanelCallBack callback)
        {
            this.Callback = callback;
        }
    }
}
