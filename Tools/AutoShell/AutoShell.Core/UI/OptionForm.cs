using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using AutoShell.Services;
using AutoShell.DataModel;
using System.Xml.Linq;
using System.IO;

namespace AutoShell.UI
{
    internal partial class OptionForm : Form, IOptionService
    {
        private class Option
        {
            public Type OptionType { get; set; }
            public object OptionObject { get; set; }
        }

        private class Editor
        {
            public Guid OptionGuid { get; set; }
            public Guid ParentGuid { get; set; }
            public Guid EditorGuid { get; set; }
            public string Name { get; set; }
            public IOptionEditor OptionEditor { get; set; }
        }

        private IServiceContainer serviceContainer;
        private List<IOptionListener> generalOptionListeners = new List<IOptionListener>();
        private Dictionary<Guid, List<IOptionListener>> specifiedOptionListeners = new Dictionary<Guid, List<IOptionListener>>();
        private Dictionary<Guid, Option> options = new Dictionary<Guid, Option>();
        private Dictionary<Guid, Editor> editors = new Dictionary<Guid, Editor>();
        private DataSerializer serializer = new DataSerializer();
        private DataDeserializer deserializer = new DataDeserializer();
        private string fileName = null;

        private Editor currentEditor = null;
        private Dictionary<Guid, Option> tempOptions = new Dictionary<Guid, Option>();

        public OptionForm(IServiceContainer serviceContainer)
        {
            InitializeComponent();
            this.serviceContainer = serviceContainer;
            this.serviceContainer.RegisterService<IOptionService>(this);
        }

        private void CopyOptions(bool save)
        {
            IOptionService optionService = this as IOptionService;
            if (save)
            {
                foreach (Guid key in this.tempOptions.Keys)
                {
                    optionService.SetOption(key, this.tempOptions[key].OptionObject);
                }
                optionService.SaveOptions();
            }
            foreach (Guid key in this.options.Keys)
            {
                Option option = new Option();
                option.OptionType = this.options[key].OptionType;
                option.OptionObject = optionService.CopyOption(key);
                this.tempOptions[key] = option;
            }
        }

        private void BuildEditorTree(TreeNode parent, Editor editor)
        {
            TreeNode node = new TreeNode(editor.Name);
            node.Tag = editor;
            if (parent != null)
            {
                parent.Nodes.Add(node);
            }
            else
            {
                treeViewCategory.Nodes.Add(node);
            }
            foreach (Editor subEditor in this.editors.Values)
            {
                if (subEditor.ParentGuid == editor.EditorGuid)
                {
                    BuildEditorTree(node, subEditor);
                }
            }
        }

        private void OptionForm_Shown(object sender, EventArgs e)
        {
            foreach (Editor editor in this.editors.Values)
            {
                if (editor.ParentGuid == Guid.Empty)
                {
                    BuildEditorTree(null, editor);
                }
            }
            CopyOptions(false);
            treeViewCategory.Nodes[0].Expand();
            treeViewCategory.SelectedNode = treeViewCategory.Nodes[0];
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            if (this.currentEditor != null)
            {
                if (!this.currentEditor.OptionEditor.QueryEndEditOption())
                {
                    return;
                }
                this.currentEditor.OptionEditor.EndEditOption();
            }
            CopyOptions(true);
            Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void treeViewCategory_BeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            if (this.currentEditor != null)
            {
                e.Cancel = !this.currentEditor.OptionEditor.QueryEndEditOption();
            }
        }

        private void treeViewCategory_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (this.currentEditor != null)
            {
                this.currentEditor.OptionEditor.EndEditOption();
                panelEditor.Controls.Remove(this.currentEditor.OptionEditor.GetEditorControl());
            }
            this.currentEditor = e.Node.Tag as Editor;
            this.currentEditor.OptionEditor.BeginEditOption(this.currentEditor.OptionGuid, this.tempOptions[this.currentEditor.OptionGuid].OptionObject);
            Control editorControl = this.currentEditor.OptionEditor.GetEditorControl();
            editorControl.Dock = DockStyle.Fill;
            panelEditor.Controls.Add(editorControl);
        }

        private void OptionForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (this.currentEditor != null)
            {
                panelEditor.Controls.Remove(this.currentEditor.OptionEditor.GetEditorControl());
                this.currentEditor = null;
            }
            treeViewCategory.Nodes.Clear();
        }

        #region IOptionService Members

        public void RegisterOptionListener(IOptionListener listener)
        {
            if (!this.generalOptionListeners.Contains(listener))
            {
                this.generalOptionListeners.Add(listener);
            }
        }

        public void RegisterOptionListener(Guid optionGuid, IOptionListener listener)
        {
            if (!this.options.ContainsKey(optionGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionOptionNotExists, optionGuid));
            }
            if (!this.specifiedOptionListeners.ContainsKey(optionGuid))
            {
                this.specifiedOptionListeners.Add(optionGuid, new List<IOptionListener>());
            }
            this.specifiedOptionListeners[optionGuid].Add(listener);
        }

        public void UnregisterOptionListener(IOptionListener listener)
        {
            if (this.generalOptionListeners.Contains(listener))
            {
                this.generalOptionListeners.Remove(listener);
            }
            foreach (Guid optionGuid in this.specifiedOptionListeners.Keys.ToArray())
            {
                List<IOptionListener> listeners = this.specifiedOptionListeners[optionGuid];
                if (listeners.Contains(listener))
                {
                    listeners.Remove(listener);
                }
                if (listeners.Count == 0)
                {
                    this.specifiedOptionListeners.Remove(optionGuid);
                }
            }
        }

        public void RegisterOption(Guid optionGuid, Type serializableOption)
        {
            if (this.options.ContainsKey(optionGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionDuplicatedOption, optionGuid));
            }
            else
            {
                Option option = new Option();
                option.OptionType = serializableOption;
                option.OptionObject = serializableOption.GetConstructor(new Type[] { }).Invoke(new object[] { });
                this.options.Add(optionGuid, option);
            }
        }

        public void RegisterOptionEditor(Guid optionGuid, Guid editorGuid, string name, IOptionEditor optionEditor)
        {
            RegisterOptionEditor(optionGuid, editorGuid, Guid.Empty, name, optionEditor);
        }

        public void RegisterOptionEditor(Guid optionGuid, Guid editorGuid, Guid parentGuid, string name, IOptionEditor optionEditor)
        {
            if (!this.options.ContainsKey(optionGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionOptionNotExists, optionGuid));
            }
            else if (this.editors.ContainsKey(editorGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionDuplicatedOptionEditor, editorGuid));
            }
            else if (parentGuid != Guid.Empty && !this.editors.ContainsKey(parentGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionOptionEditorNoExists, parentGuid));
            }
            else
            {
                Editor editor = new Editor();
                editor.Name = name;
                editor.OptionEditor = optionEditor;
                editor.OptionGuid = optionGuid;
                editor.EditorGuid = editorGuid;
                editor.ParentGuid = parentGuid;
                this.editors.Add(editorGuid, editor);
            }
        }

        public void LoadOptions(string fileName)
        {
            this.fileName = fileName;
            if (File.Exists(fileName))
            {
                XDocument document = XDocument.Load(fileName);
                foreach (XElement optionElement in document.Root.Elements())
                {
                    Guid guid = new Guid(optionElement.Attribute("Guid").Value);
                    if (this.options.ContainsKey(guid))
                    {
                        SetOption(guid, this.deserializer.Deserialize((XElement)optionElement.FirstNode));
                    }
                }
            }
        }

        public void SaveOptions()
        {
            XElement root = new XElement(
                "AutoShellOptions",
                this.options.Keys.Select(key =>
                    new XElement(
                        "Option",
                        new XAttribute("Guid", key.ToString()),
                        this.serializer.Serialize(this.options[key].OptionObject)
                        )
                    )
                );
            XDocument document = new XDocument();
            document.Add(root);
            document.Save(fileName);
        }

        public object GetOption(Guid optionGuid)
        {
            if (!this.options.ContainsKey(optionGuid))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionOptionNotExists, optionGuid));
            }
            return this.options[optionGuid].OptionObject;
        }

        public object CopyOption(Guid optionGuid)
        {
            return this.deserializer.Deserialize(this.serializer.Serialize(GetOption(optionGuid)));
        }

        public void SetOption(Guid optionGuid, object option)
        {
            this.options[optionGuid].OptionObject = option;
            foreach (IOptionListener listener in this.generalOptionListeners)
            {
                listener.OptionUpdated(optionGuid);
            }
            if (this.specifiedOptionListeners.ContainsKey(optionGuid))
            {
                foreach (IOptionListener listener in this.specifiedOptionListeners[optionGuid])
                {
                    listener.OptionUpdated(optionGuid);
                }
            }
        }

        #endregion
    }
}
