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
using Developer.WinFormControls.Extension;
using Developer.LanguageServices.NativeX.Extension;

namespace Test.Host.LanguageForms.NativeX
{
    class NativeXControlPanel : TextEditorBoxExtensibleControlPanel, IDisposable
    {
        class LanguageFormNativeXProvider : NativeXEditingObserverProvider.NativeXProvider
        {
            private NativeXForm form = null;
            private int counter = 0;

            public LanguageFormNativeXProvider(NativeXForm form)
            {
                this.form = form;
            }

            public override void TemporaryResultUpdated()
            {
                this.form.ContextText = this.Extension.EditingNodeCode.Text
                    + "\r\n********************************\r\nSYNTAX TREE\r\n********************************\r\n"
                    + (this.Extension.EditingNode == null ? "<NULL>" : this.Extension.EditingNode.ToString());
            }

            public override void AnalyzerResultReceived(NativeXAnalyzingResult result)
            {
                string treeText = result.Unit == null ? "<NULL>" : result.Unit.ToString();
                string statusText = (++this.counter).ToString();

                this.Extension.Callback.TextEditorBox.Invoke(new MethodInvoker(() =>
                {
                    this.form.TreeText = treeText;
                    this.form.StatusText = statusText;
                }));
            }
        }

        private LanguagePopupListExtension popupListExtension = null;
        private NativeXEditingObserverProvider editingObserverProvider = null;
        private NativeXContextSensitiveColorizerProvider colorizerProvider = null;
        private NativeXPopupItemProvider popupItemProvider = null;
        private NativeXTooltipProvider tooltipProvider = null;

        public NativeXControlPanel(NativeXForm form)
        {
            this.popupListExtension = new LanguagePopupListExtension();
            this.editingObserverProvider = new NativeXEditingObserverProvider(new LanguageFormNativeXProvider(form));
            this.colorizerProvider = new NativeXContextSensitiveColorizerProvider(this.editingObserverProvider);
            this.popupItemProvider = new NativeXPopupItemProvider(this.editingObserverProvider, this.popupListExtension);
            this.tooltipProvider = new NativeXTooltipProvider(this.editingObserverProvider);

            ExtendBeforeInstall(this.popupListExtension);
            ExtendBeforeInstall(this.editingObserverProvider);
            ExtendBeforeInstall(this.colorizerProvider);
            ExtendBeforeInstall(this.popupItemProvider);
            ExtendBeforeInstall(this.tooltipProvider);
        }

        public void Dispose()
        {
            this.editingObserverProvider.Dispose();
        }
    }
}
