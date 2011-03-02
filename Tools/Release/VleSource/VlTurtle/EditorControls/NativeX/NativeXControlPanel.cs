using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Developer.WinFormControls.Extension;
using Developer.LanguageServices.NativeX.Extension;
using Developer.LanguageServices.NativeX;
using System.Drawing;
using System.ComponentModel;
using Developer.LanguageServices.NativeX.SyntaxTree;
using Developer.LanguageProvider;

namespace VlTurtle.EditorControls.NativeX
{
    class NativeXControlPanel
        : TextEditorBoxExtensibleControlPanel
        , ILanguageDefaultColorizerProvider
        , INativeXPredefinedHeaderReader
        , IDisposable
        , IComponent
    {
        private NativeXEditingObserverProvider editingObserverProvider = null;
        private NativeXContextSensitiveColorizerProvider colorizerProvider = null;
        private NativeXPopupItemProvider popupItemProvider = null;
        private NativeXTooltipProvider tooltipProvider = null;
        private NativeXParameterInfoProvider parameterInfoProvider = null;
        private NativeXWordingProvider wordingProvider = null;
        private LanguageDefaultColorizerExtension languageDefaultColorizer = null;
        private LineNumberProvider lineNumberProvider = null;

        private List<NativeXUnit> predefinedHeaders = new List<NativeXUnit>();

        public NativeXControlPanel()
        {
            this.editingObserverProvider = new NativeXEditingObserverProvider(this);
            this.colorizerProvider = new NativeXContextSensitiveColorizerProvider(this.editingObserverProvider);
            this.popupItemProvider = new NativeXPopupItemProvider(this.editingObserverProvider);
            this.tooltipProvider = new NativeXTooltipProvider(this.editingObserverProvider);
            this.parameterInfoProvider = new NativeXParameterInfoProvider(this.editingObserverProvider);
            this.wordingProvider = new NativeXWordingProvider();
            this.languageDefaultColorizer = new LanguageDefaultColorizerExtension(this);
            this.lineNumberProvider = new LineNumberProvider();

            ExtendBeforeInstall(this.editingObserverProvider);
            ExtendBeforeInstall(this.colorizerProvider);
            ExtendBeforeInstall(this.popupItemProvider);
            ExtendBeforeInstall(this.tooltipProvider);
            ExtendBeforeInstall(this.parameterInfoProvider);
            ExtendBeforeInstall(this.wordingProvider);
            ExtendBeforeInstall(this.languageDefaultColorizer);
            ExtendBeforeInstall(this.lineNumberProvider);
        }

        public IEnumerable<NativeXUnit> PredifinedHeaders
        {
            get
            {
                return this.predefinedHeaders;
            }
        }

        public void AddPredefinedHeader(string code)
        {
            List<CodeToken> tokens = NativeXCodeParser.Tokenize(code.ToCharArray());
            int currentToken = 0;
            bool parseSuccess = false;
            NativeXUnit unit = NativeXCodeParser.ParseUnit(tokens, ref currentToken, ref parseSuccess);
            if (unit != null && parseSuccess)
            {
                unit.BuildScope(null);
                this.predefinedHeaders.Add(unit);
            }
        }

        public void Dispose()
        {
            this.editingObserverProvider.Dispose();
            if (this.Disposed != null)
            {
                this.Disposed(this, new EventArgs());
            }
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

        public event EventHandler Disposed;

        public ISite Site { get; set; }
    }
}
