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
    class NativeXControlPanel
        : TextEditorBoxExtensibleControlPanel
        , ILanguageDefaultColorizerProvider
        , IDisposable
    {
        class LanguageFormNativeXProvider : NativeXEditingObserverProvider.NativeXProvider
        {
            private NativeXForm form = null;
            //private int counter = 0;

            class EmptyHeaders : INativeXPredefinedHeaderReader
            {

                IEnumerable<NativeXUnit> INativeXPredefinedHeaderReader.PredifinedHeaders
                {
                    get { return new NativeXUnit[] { }; }
                }
            }

            public LanguageFormNativeXProvider(NativeXForm form)
                : base(new EmptyHeaders())
            {
                this.form = form;
            }

            public override void TemporaryResultUpdated()
            {
                //this.form.ContextText = this.Extension.EditingNodeCode.Text
                //    + "\r\n********************************\r\nSYNTAX TREE\r\n********************************\r\n"
                //    + (this.Extension.EditingNode == null ? "<NULL>" : this.Extension.EditingNode.ToString());
            }

            public override void AnalyzerResultReceived(NativeXAnalyzingResult result)
            {
                //string treeText = result.Unit == null ? "<NULL>" : result.Unit.ToString();
                //string statusText = (++this.counter).ToString();

                //this.Extension.Callback.TextEditorBox.Invoke(new MethodInvoker(() =>
                //{
                //    this.form.TreeText = treeText;
                //    this.form.StatusText = statusText;
                //}));
            }
        }

        private NativeXEditingObserverProvider editingObserverProvider = null;
        private NativeXContextSensitiveColorizerProvider colorizerProvider = null;
        private NativeXPopupItemProvider popupItemProvider = null;
        private NativeXTooltipProvider tooltipProvider = null;
        private NativeXParameterInfoProvider parameterInfoProvider = null;
        private NativeXWordingProvider wordingProvider = null;
        private LanguageDefaultColorizerExtension languageDefaultColorizer = null;

        public NativeXControlPanel(NativeXForm form)
        {
            this.editingObserverProvider = new NativeXEditingObserverProvider(new LanguageFormNativeXProvider(form));
            this.colorizerProvider = new NativeXContextSensitiveColorizerProvider(this.editingObserverProvider);
            this.popupItemProvider = new NativeXPopupItemProvider(this.editingObserverProvider);
            this.tooltipProvider = new NativeXTooltipProvider(this.editingObserverProvider);
            this.parameterInfoProvider = new NativeXParameterInfoProvider(this.editingObserverProvider);
            this.wordingProvider = new NativeXWordingProvider();
            this.languageDefaultColorizer = new LanguageDefaultColorizerExtension(this);

            ExtendBeforeInstall(this.editingObserverProvider);
            ExtendBeforeInstall(this.colorizerProvider);
            ExtendBeforeInstall(this.popupItemProvider);
            ExtendBeforeInstall(this.tooltipProvider);
            ExtendBeforeInstall(this.parameterInfoProvider);
            ExtendBeforeInstall(this.wordingProvider);
            ExtendBeforeInstall(this.languageDefaultColorizer);
        }

        public void Dispose()
        {
            this.editingObserverProvider.Dispose();
        }

        int ILanguageDefaultColorizerProvider.BlockColorId
        {
            get
            {
                return NativeXColorizer.BlockColorId;
            }
        }

        int ILanguageDefaultColorizerProvider.SnippetColorId
        {
            get
            {
                return NativeXColorizer.SnippetColorId;
            }
        }

        Color ILanguageDefaultColorizerProvider.SnippetBackColor
        {
            get
            {
                return NativeXColorizer.SnippetBackgroundColor;
            }
        }
    }
}
