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
                TextPosition tokenPos = new TextPosition(token.Start.row, (token.Start.col + token.End.col) / 2);
                {
                    var type = this.EditingObserverExtension.AnalyzingResult.Unit.FindDeepest<NativeXReferenceType>(tokenPos);
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
                    var conref = this.EditingObserverExtension.AnalyzingResult.Unit.FindDeepest<NativeXConceptReference>(tokenPos);
                    if (conref != null && conref.ReferenceName == token.Value)
                    {
                        CodeScope scope = conref.Scope;
                        if (scope != null)
                        {
                            CodeNode node = conref.Scope.Find(conref.ReferenceName);
                            if (node is NativeXConceptDeclaration)
                            {
                                needColor = true;
                            }
                        }
                    }
                }
                {
                    var decl = this.EditingObserverExtension.AnalyzingResult.Unit.FindDeepest<NativeXNode>(tokenPos) as NativeXDeclaration;
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
