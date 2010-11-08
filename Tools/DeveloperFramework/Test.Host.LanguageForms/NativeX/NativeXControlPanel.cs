using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls;
using System.Windows.Forms;
using Developer.WinFormControls.Core;
using Developer.LanguageProvider;
using System.Drawing;
using Developer.LanguageServices.NativeX.SyntaxTree;
using Developer.LanguageServices.NativeX;

namespace Test.Host.LanguageForms.NativeX
{
    class NativeXControlPanel : LanguageControlPanel<NativeXAnalyzingResult, NativeXDeclaration, NativeXEditingDeclarations>
    {
        private NativeXForm form = null;
        private int counter = 0;

        public NativeXControlPanel(NativeXForm form)
        {
            this.form = form;
        }

        #region LanguageControlPanel Members

        protected override CalculationNotifier<string, NativeXAnalyzingResult> CreateAnalyzer()
        {
            return new NativeXCodeAnalyzer();
        }

        protected override NativeXEditingDeclarations ParseEditingNodeContainer(string code)
        {
            List<CodeToken> tokens = NativeXCodeParser.Tokenize(code.ToCharArray());
            int currentToken = 0;
            bool parseSuccess = false;
            return NativeXCodeParser.ParseEditingDeclarations(tokens, ref currentToken, ref parseSuccess);
        }

        protected override NativeXDeclaration GetEditingNode(NativeXAnalyzingResult result, TextPosition pos)
        {
            if (result != null && result.Unit != null && result.Unit.Declarations != null)
            {
                if (pos >= result.Unit.End && result.Unit.Declarations.Count > 0)
                {
                    return result.Unit.Declarations.Last();
                }
                else
                {
                    return result.Unit.FindDeepest<NativeXDeclaration>(pos);
                }
            }
            else
            {
                return null;
            }
        }

        protected override void ResultUpdated()
        {
            TemporaryResultUpdated();
            this.Callback.TextEditorBox.RedrawContent(true, false);
        }

        protected override void TemporaryResultUpdated()
        {
            this.form.ContextText = this.EditingNodeCode.Text
                + "\r\n********************************\r\nSYNTAX TREE\r\n********************************\r\n"
                + (this.EditingNode == null ? "<NULL>" : this.EditingNode.ToString());
        }

        protected override void AnalyzerResultReceived(NativeXAnalyzingResult result)
        {
            string treeText = result.Unit == null ? "<NULL>" : result.Unit.ToString();
            string statusText = (++this.counter).ToString();

            this.Callback.TextEditorBox.Invoke(new MethodInvoker(() =>
            {
                this.form.TreeText = treeText;
                this.form.StatusText = statusText;
            }));
        }

        #endregion

        #region Popups

        private void PopupStructureMembers(NativeXAbstractStructureType structureType)
        {
            this.Callback.TextEditorBox.PopupItems(
                NativeXPopupItemProvider.PopupStructureMembers(structureType),
                forceClosingPrevious: true);
        }

        private void PopupInstanceFunctions(NativeXInstanceFunctionExpression function)
        {
            if (function.ConceptName != null && function.Scope != null)
            {
                this.Callback.TextEditorBox.PopupItems(
                    NativeXPopupItemProvider.PopupInstanceFunctions(function.ConceptName, function.Scope),
                    forceClosingPrevious: true);
            }
        }

        private void PopupConcepts(CodeScope scope)
        {
            this.Callback.TextEditorBox.PopupItems(
                NativeXPopupItemProvider.PopupConcepts(scope),
                forceClosingPrevious: true);
        }

        private void PopupGenericParameters(string reference, CodeScope scope)
        {
            this.Callback.TextEditorBox.PopupItems(
                NativeXPopupItemProvider.PopupGenericParameters(scope),
                searchingKey: reference);
        }

        private void PopupInstanceFunctions(string reference, string conceptName, CodeScope scope)
        {
            this.Callback.TextEditorBox.PopupItems(
                NativeXPopupItemProvider.PopupInstanceFunctions(conceptName, scope),
                searchingKey: reference,
                forceClosingPrevious: true);
        }

        private void PopupExpressions(string reference, CodeScope scope)
        {
            this.Callback.TextEditorBox.PopupItems(
                NativeXPopupItemProvider.PopupExpressions(scope),
                searchingKey: reference);
        }

        private void PopupExpressionsAndTypes(string reference, CodeScope scope)
        {
            this.Callback.TextEditorBox.PopupItems(
                NativeXPopupItemProvider.PopupExpressions(scope).Concat(NativeXPopupItemProvider.PopupTypes(scope)),
                searchingKey: reference);
        }

        private void PopupExpressionsAndStatements(string reference, CodeScope scope)
        {
            this.Callback.TextEditorBox.PopupItems(
                NativeXPopupItemProvider.PopupExpressions(scope).Concat(NativeXPopupItemProvider.PopupStatementKeywords()),
                searchingKey: reference);
        }

        private void PopupTypes(string reference, CodeScope scope)
        {
            this.Callback.TextEditorBox.PopupItems(
                NativeXPopupItemProvider.PopupTypes(scope),
                searchingKey: reference);
        }

        private void PopupDeclarationKeywords(string reference)
        {
            this.Callback.TextEditorBox.PopupItems(
                NativeXPopupItemProvider.PopupDeclarationKeywords(),
                searchingKey: reference);
        }

        #endregion

        #region ITextEditorControlPanel Members

        public override void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
            base.OnAfterEdit(start, oldEnd, newEnd);
            if (!this.PreventPopupList)
            {
                string inputText = this.Callback.TextEditorBox.TextProvider.GetString(start, newEnd);
                if (this.EditingNode == null)
                {
                    if (inputText.Length == 1)
                    {
                        if ('a' <= inputText[0] && inputText[0] <= 'z' || 'A' <= inputText[0] && inputText[0] <= 'Z')
                        {
                            PopupDeclarationKeywords(inputText);
                        }
                    }
                }
                else
                {
                    switch (inputText)
                    {
                        case ".":
                            {
                                var node = this.EditingNode.FindDeepest<NativeXMemberExpression>(ConvertToEditingPosition(newEnd));
                                if (node != null)
                                {
                                    PopupStructureMembers(node.GetStructureType());
                                    break;
                                }
                            }
                            break;
                        case ">":
                            {
                                var node = this.EditingNode.FindDeepest<NativeXPointerMemberExpression>(ConvertToEditingPosition(newEnd));
                                if (node != null)
                                {
                                    PopupStructureMembers(node.GetStructureType());
                                    break;
                                }
                            }
                            break;
                        case ":":
                            {
                                TextPosition end = ConvertToEditingPosition(newEnd);
                                if (end.col >= 2)
                                {
                                    if (this.EditingNodeCode.GetString(new TextPosition(end.row, end.col - 2), new TextPosition(end.row, end.col - 1)) == ":")
                                    {
                                        var node = this.EditingNode.FindDeepest<NativeXInstanceFunctionExpression>(end);
                                        if (node != null && node.Scope != null)
                                        {
                                            PopupInstanceFunctions(node);
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        {
                                            var node = this.EditingNode.FindDeepest<NativeXGenericConstraint>(end);
                                            if (node != null && node.Scope != null)
                                            {
                                                PopupConcepts(node.Scope);
                                                break;
                                            }
                                        }
                                        {
                                            var node = this.EditingNode.FindDeepest<NativeXInstanceDeclaration>(end);
                                            if (node != null && node.Scope != null)
                                            {
                                                PopupConcepts(node.Scope);
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        default:
                            {
                                var node = this.EditingNode.FindDeepest<NativeXReferenceExpression>(ConvertToEditingPosition(newEnd));
                                if (node != null && node.Scope != null && node.ReferencedName != null && node.ReferencedName == inputText)
                                {
                                    if (node.FindParent<NativeXNameExpressionPair>() != null)
                                    {
                                        PopupExpressions(node.ReferencedName, node.Scope);
                                        break;
                                    }

                                    var exprstat = node.FindParent<NativeXExpressionStatement>();
                                    if (exprstat != null && exprstat.Expression == node)
                                    {
                                        PopupExpressionsAndStatements(node.ReferencedName, node.Scope);
                                        break;
                                    }

                                    PopupExpressionsAndTypes(node.ReferencedName, node.Scope);
                                    break;
                                }
                            }
                            {
                                var node = this.EditingNode.FindDeepest<NativeXReferenceType>(ConvertToEditingPosition(newEnd));
                                if (node != null && node.Scope != null && node.ReferencedName != null && node.ReferencedName == inputText)
                                {
                                    PopupTypes(node.ReferencedName, node.Scope);
                                    break;
                                }
                            }
                            {
                                var node = this.EditingNode.FindDeepest<NativeXNameExpressionPair>(ConvertToEditingPosition(newEnd));
                                if (node != null && node.Scope != null && node.Name != null && node.Name == inputText)
                                {
                                    var instdecl = node.FindParent<NativeXInstanceDeclaration>();
                                    if (instdecl != null && instdecl.ConceptName != null)
                                    {
                                        PopupInstanceFunctions(node.Name, instdecl.ConceptName, node.Scope);
                                        break;
                                    }
                                }
                            }
                            {
                                var node = this.EditingNode.FindDeepest<NativeXGenericConstraint>(ConvertToEditingPosition(newEnd));
                                if (node != null && node.Scope != null && node.ParameterName != null && node.ParameterName == inputText)
                                {
                                    PopupGenericParameters(node.ParameterName, node.Scope);
                                    break;
                                }
                            }
                            {
                                var node = this.EditingNode.FindDeepest<NativeXNode>(ConvertToEditingPosition(newEnd));
                                if (inputText.Length == 1 && node.GetType() == typeof(NativeXDeclaration))
                                {
                                    if ('a' <= inputText[0] && inputText[0] <= 'z' || 'A' <= inputText[0] && inputText[0] <= 'Z')
                                    {
                                        PopupDeclarationKeywords(inputText);
                                        break;
                                    }
                                }
                            }
                            break;
                    }
                }
            }
        }

        public override bool NeedColorLineForDisplay(int lineIndex)
        {
            return this.AnalyzingResult != null && this.AnalyzingResult.Unit != null && this.AnalyzingResult.IdTokens.ContainsKey(lineIndex);
        }

        public override void ColorLineForDisplay(int lineIndex, int[] colors)
        {
            TextLine<TextEditorBox.LineInfo> line = this.Callback.TextEditorBox.TextProvider[lineIndex];
            foreach (var token in this.AnalyzingResult.IdTokens[lineIndex])
            {
                bool needColor = false;
                {
                    var type = this.AnalyzingResult.Unit.FindDeepest<NativeXReferenceType>(token.Start);
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
                    var inst = this.AnalyzingResult.Unit.FindDeepest<NativeXInstanceFunctionExpression>(token.Start);
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
                    var genecons = this.AnalyzingResult.Unit.FindDeepest<NativeXGenericConstraint>(token.Start);
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
                    var inst = this.AnalyzingResult.Unit.FindDeepest<NativeXInstanceDeclaration>(token.Start);
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

        #endregion
    }
}
