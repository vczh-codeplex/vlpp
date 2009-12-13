using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AutoShell.UI
{
    public abstract class CodeEditorWindowPane : TextBasedEditorWindowPane
    {
        private CodeBox codeBox = null;

        public CodeEditorWindowPane()
        {
        }

        protected abstract ICodeFormatter CodeFormatter { get; }

        public override TextBoxBase EditorControl
        {
            get
            {
                return this.codeBox;
            }
        }

        internal bool Formatting
        {
            get
            {
                return !this.EnableObserving;
            }
            set
            {
                this.EnableObserving = !value;
            }
        }

        protected override void OnInitialize()
        {
            this.codeBox = new CodeBox(this, this.CodeFormatter);
            base.OnInitialize();
        }
    }
}
