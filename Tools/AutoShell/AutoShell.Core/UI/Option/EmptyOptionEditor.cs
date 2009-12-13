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
    public partial class EmptyOptionEditor : UserControl, IOptionEditor
    {
        public EmptyOptionEditor()
        {
            InitializeComponent();
        }

        #region IOptionEditor Members

        void IOptionEditor.Initialize(IServiceContainer serviceContainer)
        {
        }

        void IOptionEditor.BeginEditOption(Guid optionGuid, object option)
        {
        }

        bool IOptionEditor.QueryEndEditOption()
        {
            return true;
        }

        void IOptionEditor.EndEditOption()
        {
        }

        Control IOptionEditor.GetEditorControl()
        {
            return this;
        }

        #endregion
    }
}
