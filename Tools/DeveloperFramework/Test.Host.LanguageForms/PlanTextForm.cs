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

namespace Test.Host.LanguageForms
{
    public partial class PlanTextForm : Form
    {
        protected ServiceHost ServiceHost { get; private set; }

        public PlanTextForm()
        {
            InitializeComponent();
            textEditorBox.KeyCommands.RegisterCommand(Keys.Q, true, false, AddBlock);
            textEditorBox.KeyCommands.RegisterCommand(Keys.W, true, false, RemoveBlock);

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
            TextPosition start = textEditorBox.SelectionStart;
            TextPosition end = textEditorBox.SelectionEnd;
            if (start.row == end.row)
            {
                textEditorBox.AddBlock(start.row, start.col, end.col);
            }
            return true;
        }

        private bool RemoveBlock(TextEditorBox editor, KeyEventArgs e)
        {
            TextPosition start = textEditorBox.SelectionStart;
            TextPosition end = textEditorBox.SelectionEnd;
            if (start.row == end.row)
            {
                textEditorBox.RemoveBlock(start.row, start.col, end.col);
            }
            return true;
        }
    }
}
