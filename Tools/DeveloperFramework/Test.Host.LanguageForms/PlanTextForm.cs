using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.ServiceModel;
using Developer.WinFormControls;
using Developer.LanguageProvider;
using Developer.WinFormControls.Extension;

namespace Test.Host.LanguageForms
{
    public partial class PlanTextForm
        : Form
        , ILanguageDefaultColorizerProvider
    {
        protected ServiceHost ServiceHost { get; private set; }

        int ILanguageDefaultColorizerProvider.BlockColorId
        {
            get
            {
                return TextEditorPlanTextColorizer.BlockColorId;
            }
        }

        int ILanguageDefaultColorizerProvider.SnippetColorId
        {
            get
            {
                return TextEditorPlanTextColorizer.SnippetColorId;
            }
        }

        Color ILanguageDefaultColorizerProvider.SnippetBackColor
        {
            get
            {
                return TextEditorPlanTextColorizer.SnippetBackgroundColor;
            }
        }

        public PlanTextForm()
        {
            InitializeComponent();
            textEditorBox.ControlPanel = new TextEditorPlanTextColorizedControlPanel(this);
            textEditorBox.KeyCommands.RegisterCommand(Keys.Q, true, false, AddBlock);
            textEditorBox.KeyCommands.RegisterCommand(Keys.W, true, false, RemoveBlock);
            textEditorBox.KeyCommands.RegisterCommand(Keys.E, true, false, EnterLimitedMode);
            textEditorBox.KeyCommands.RegisterCommand(Keys.R, true, false, LeaveLimitedMode);

            TextEditorService.EditorControl = textEditorBox;
            this.ServiceHost = new ServiceHost(typeof(TextEditorService));
            this.ServiceHost.Open();
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            this.ServiceHost.Close();
        }

        private bool AddBlock(TextEditorBox editor, KeyEventArgs e)
        {
            TextPosition start = editor.SelectionStart;
            TextPosition end = editor.SelectionEnd;
            if (start.row == end.row)
            {
                editor.AddBlock(start.row, start.col, end.col);
            }
            return true;
        }

        private bool RemoveBlock(TextEditorBox editor, KeyEventArgs e)
        {
            TextPosition start = editor.SelectionStart;
            TextPosition end = editor.SelectionEnd;
            if (start.row == end.row)
            {
                editor.RemoveBlock(start.row, start.col, end.col);
            }
            return true;
        }

        private bool EnterLimitedMode(TextEditorBox editor, KeyEventArgs e)
        {
            TextPosition start = editor.SelectionStart;
            TextPosition end = editor.SelectionEnd;
            if (start.row == end.row)
            {
                editor.Controller.EnterLimitedMode(start.row, start.col, end.col);
            }
            return true;
        }

        private bool LeaveLimitedMode(TextEditorBox editor, KeyEventArgs e)
        {
            editor.Controller.LeaveLimitedMode();
            return true;
        }
    }
}
