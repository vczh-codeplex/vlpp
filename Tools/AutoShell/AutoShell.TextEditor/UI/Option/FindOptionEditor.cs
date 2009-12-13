using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AutoShell.Services;

namespace AutoShell.UI.Option
{
    internal partial class FindOptionEditor : UserControl, IOptionEditor
    {
        private IServiceContainer serviceContainer = null;
        private FindOption option = null;

        public FindOptionEditor()
        {
            InitializeComponent();
        }

        #region IOptionEditor Members

        void IOptionEditor.Initialize(IServiceContainer serviceContainer)
        {
            this.serviceContainer = serviceContainer;
        }

        void IOptionEditor.BeginEditOption(Guid optionGuid, object option)
        {
            this.option = option as FindOption;
            checkBoxMaintain.Checked = this.option.EditorWindowMaintainFindResult;
            checkBoxClose.Checked = this.option.CloseWindowAfterSearching;
        }

        bool IOptionEditor.QueryEndEditOption()
        {
            return true;
        }

        void IOptionEditor.EndEditOption()
        {
            this.option.EditorWindowMaintainFindResult = checkBoxMaintain.Checked;
            this.option.CloseWindowAfterSearching = checkBoxClose.Checked;
        }

        Control IOptionEditor.GetEditorControl()
        {
            return this;
        }

        #endregion
    }
}
