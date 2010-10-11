﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CodeBoxControl;
using System.IO;
using System.ServiceModel;
using CodeBoxControl.Core;

namespace CodeForm
{
    public partial class CSharpForm : Form
    {
        private ServiceHost serviceHost = null;

        public CSharpForm()
        {
            InitializeComponent();
            textEditorBox.Colorizer = new CSharpColorizer();
            textEditorBox.ControlPanel = new CSharpControlPanel();
            TextEditorService.EditorControl = textEditorBox;
            this.serviceHost = new ServiceHost(typeof(TextEditorService));
            this.serviceHost.Open();

            textEditorBox.KeyCommands.RegisterCommand(Keys.Q, true, false, AddBlock);
            textEditorBox.KeyCommands.RegisterCommand(Keys.W, true, false, RemoveBlock);
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

        private void CodeForm_Shown(object sender, EventArgs e)
        {
            if (File.Exists("CSharpCode.txt"))
            {
                using (StreamReader reader = new StreamReader("CSharpCode.txt"))
                {
                    textEditorBox.Text = reader.ReadToEnd();
                }
            }
        }

        private void CodeForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.serviceHost.Close();
        }
    }
}
