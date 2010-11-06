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
    class NativeXControlPanel : LanguageControlPanel<NativeXAnalyzingResult, NativeXStatement, NativeXEditingStatement>
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

        protected override NativeXEditingStatement ParseEditingNodeContainer(string code)
        {
            List<CodeToken> tokens = NativeXCodeParser.Tokenize(code.ToCharArray());
            int currentToken = 0;
            bool parseSuccess = false;
            return NativeXCodeParser.ParseEditingStatement(tokens, ref currentToken, ref parseSuccess);
        }

        protected override NativeXStatement GetEditingNode(NativeXAnalyzingResult result, TextPosition pos)
        {
            if (result != null && result.Unit != null)
            {
                return result.Unit.FindDeepest<NativeXStatement>(pos);
            }
            else
            {
                return null;
            }
        }

        protected override void ResultUpdated()
        {
            TemporaryResultUpdated();
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

        #region ITextEditorControlPanel Members

        private void PopupStructureMembers(NativeXAbstractStructureType structureType)
        {
            if (structureType != null)
            {
                List<string> members = structureType.Members.Keys.ToList();
                string message = members.Aggregate("", (a, b) => a + "\r\n" + b);
                MessageBox.Show(message, this.form.Text);
            }
        }

        public override void OnAfterEdit(TextPosition start, TextPosition oldEnd, TextPosition newEnd)
        {
            base.OnAfterEdit(start, oldEnd, newEnd);
            if (this.EditingNode != null)
            {
                string inputText = this.Callback.TextEditorBox.TextProvider.GetString(start, newEnd);
                switch (inputText)
                {
                    case ".":
                        {
                            var node = this.EditingNode.FindDeepest<NativeXMemberExpression>(ConvertToEditingPosition(newEnd));
                            if (node != null)
                            {
                                PopupStructureMembers(node.GetStructureType());
                            }
                        }
                        break;
                    case ">":
                        {
                            var node = this.EditingNode.FindDeepest<NativeXPointerMemberExpression>(ConvertToEditingPosition(newEnd));
                            if (node != null)
                            {
                                PopupStructureMembers(node.GetStructureType());
                            }
                        }
                        break;
                }
            }
        }

        #endregion
    }
}
