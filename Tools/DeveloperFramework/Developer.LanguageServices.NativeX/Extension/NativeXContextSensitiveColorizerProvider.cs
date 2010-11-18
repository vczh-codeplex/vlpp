using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls.Extension;
using Developer.WinFormControls;
using Developer.WinFormControls.Core;
using Developer.LanguageProvider;
using Developer.LanguageServices.NativeX.SyntaxTree;

namespace Developer.LanguageServices.NativeX.Extension
{
    public class NativeXContextSensitiveColorizerProvider : ITextEditorBoxColorizerExtension
    {
        public ITextEditorControlPanelCallBack Callback { get; private set; }

        protected NativeXEditingObserverProvider EditingObserverExtension { get; private set; }

        public NativeXContextSensitiveColorizerProvider(NativeXEditingObserverProvider editingObserverExtension)
        {
            this.EditingObserverExtension = editingObserverExtension;
        }

        public virtual bool NeedColorLineForDisplay(int lineIndex)
        {
            return this.EditingObserverExtension.AnalyzingResult != null && this.EditingObserverExtension.AnalyzingResult.Unit != null && this.EditingObserverExtension.AnalyzingResult.IdTokens.ContainsKey(lineIndex);
        }

        public virtual void ColorLineForDisplay(int lineIndex, int[] colors)
        {
            if (this.EditingObserverExtension.AnalyzingResult == null || this.EditingObserverExtension.AnalyzingResult.Unit == null)
            {
                return;
            }
            TextLine<TextEditorBox.LineInfo> line = this.Callback.TextEditorBox.TextProvider[lineIndex];
            foreach (var token in this.EditingObserverExtension.AnalyzingResult.IdTokens[lineIndex])
            {
                bool needColor = false;
                {
                    var type = this.EditingObserverExtension.AnalyzingResult.Unit.FindDeepest<NativeXReferenceType>(token.Start);
                    if (type != null && type.ReferencedName == token.Value)
                    {
                        CodeScope scope = type.Scope;
                        if (scope != null)
                        {
                            CodeNode node = type.Scope.Find(type.ReferencedName);
                            if (node is NativeXTypeRenameDeclaration || node is NativeXStructureDeclaration)
                            {
                                needColor = true;
                            }
                        }
                    }
                }
                {
                    var inst = this.EditingObserverExtension.AnalyzingResult.Unit.FindDeepest<NativeXInstanceFunctionExpression>(token.Start);
                    if (inst != null && inst.ConceptName == token.Value)
                    {
                        CodeScope scope = inst.Scope;
                        if (scope != null)
                        {
                            CodeNode node = inst.Scope.Find(inst.ConceptName);
                            if (node is NativeXConceptDeclaration)
                            {
                                needColor = true;
                            }
                        }
                    }
                }
                {
                    var genecons = this.EditingObserverExtension.AnalyzingResult.Unit.FindDeepest<NativeXGenericConstraint>(token.Start);
                    if (genecons != null && genecons.ConceptName == token.Value)
                    {
                        CodeScope scope = genecons.Scope;
                        if (scope != null)
                        {
                            CodeNode node = genecons.Scope.Find(genecons.ConceptName);
                            if (node is NativeXConceptDeclaration)
                            {
                                needColor = true;
                            }
                        }
                    }
                }
                {
                    var inst = this.EditingObserverExtension.AnalyzingResult.Unit.FindDeepest<NativeXInstanceDeclaration>(token.Start);
                    if (inst != null && inst.ConceptName == token.Value)
                    {
                        CodeScope scope = inst.Scope;
                        if (scope != null)
                        {
                            CodeNode node = inst.Scope.Find(inst.ConceptName);
                            if (node is NativeXConceptDeclaration)
                            {
                                needColor = true;
                            }
                        }
                    }
                }
                {
                    var decl = this.EditingObserverExtension.AnalyzingResult.Unit.FindDeepest<NativeXDeclaration>(token.Start);
                    if (decl != null && decl.Name == token.Value)
                    {
                        if (decl is NativeXStructureDeclaration || decl is NativeXTypeRenameDeclaration || decl is NativeXConceptDeclaration)
                        {
                            needColor = true;
                        }
                    }
                }
                if (needColor)
                {
                    int start = Math.Max(0, Math.Min(token.Start.col, line.CharCount - 1));
                    int end = Math.Min(token.End.col, line.CharCount);
                    for (int i = start; i < end; i++)
                    {
                        colors[i] = NativeXColorizer.TypeColorId;
                    }
                }
            }
        }

        public virtual void Attach(WinFormControls.ITextEditorControlPanelCallBack callback)
        {
            this.Callback = callback;
        }
    }
}
