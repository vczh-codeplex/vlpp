using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;
using Gate.BoardComponents;

namespace Gate
{
    class BoardEditorToolBox : ListView
    {
        private List<IBoardEditorTool> predefinedTools = new List<IBoardEditorTool>();
        private SortedList<string, IBoardEditorTool> customTools = new SortedList<string, IBoardEditorTool>();
        private ImageList toolIcons;

        private void CreateListViewItem(IBoardEditorTool tool, int position)
        {
            this.toolIcons.Images.Add(tool.Name, tool.Icon);
            ListViewItem item = new ListViewItem();
            item.Text = tool.Name;
            item.ImageKey = tool.Name;
            item.Tag = tool;
            this.Items.Insert(position, item);
        }

        private void DestroyListViewItem(IBoardEditorTool tool, int position)
        {
            if (this.SelectedTool == tool)
            {
                SelectTool(0);
            }
            this.Items.RemoveAt(position);
            this.toolIcons.Images.RemoveByKey(tool.Name);
        }

        private void SelectTool(int position)
        {
            ListViewItem item = this.Items[position];
            if (!item.Selected)
            {
                item.Selected = true;
            }
        }

        private void AddPredefinedTool(IBoardEditorTool tool)
        {
            this.predefinedTools.Add(tool);
            CreateListViewItem(tool, this.predefinedTools.Count - 1);
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                this.SmallImageList = null;
                this.toolIcons.Dispose();
            }
            base.Dispose(disposing);
        }

        public event EventHandler SelectedToolChanged;

        public BoardEditorToolBox()
        {
            this.Columns.Add(new ColumnHeader()
            {
                Text = "Name",
                Width = 180,
            });

            this.FullRowSelect = true;
            this.MultiSelect = false;
            this.HideSelection = false;

            this.toolIcons = new ImageList();
            this.toolIcons.ImageSize = new Size(16, 16);
            this.toolIcons.ColorDepth = ColorDepth.Depth32Bit;
            this.SmallImageList = this.toolIcons;

            this.SelectedIndexChanged += new EventHandler(BoardEditorToolBox_SelectedIndexChanged);
        }

        #region Event Handlers

        private void BoardEditorToolBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.SelectedToolChanged != null)
            {
                this.SelectedToolChanged(this, new EventArgs());
            }
        }

        #endregion

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public IBoardEditorTool SelectedTool
        {
            get
            {
                if (this.SelectedItems.Count == 0)
                {
                    return null;
                }
                else
                {
                    return (IBoardEditorTool)this.SelectedItems[0].Tag;
                }
            }
            set
            {
                int index = this.predefinedTools.IndexOf(value);
                if (index != -1)
                {
                    SelectTool(index);
                    return;
                }

                index = this.customTools.IndexOfValue(value);
                if (index != -1)
                {
                    SelectTool(this.predefinedTools.Count + index);
                    return;
                }
            }
        }

        public void LoadPredefinedTools()
        {
            if (this.predefinedTools.Count == 0)
            {
                AddPredefinedTool(new Tools.BoardEditorDragViewTool());
                AddPredefinedTool(new Tools.BoardEditorSelectObjectTool());
                AddPredefinedTool(new Tools.BoardEditorWireTool());

                AddPredefinedTool(new Tools.BoardEditorAddGateTool(PrimitiveGates.And));
                AddPredefinedTool(new Tools.BoardEditorAddGateTool(PrimitiveGates.Or));
                AddPredefinedTool(new Tools.BoardEditorAddGateTool(PrimitiveGates.Not));
                AddPredefinedTool(new Tools.BoardEditorAddGateTool(PrimitiveGates.Xor));
                AddPredefinedTool(new Tools.BoardEditorAddGateTool(PrimitiveGates.Xnor));
                AddPredefinedTool(new Tools.BoardEditorAddGateTool(PrimitiveGates.Nand));
                AddPredefinedTool(new Tools.BoardEditorAddGateTool(PrimitiveGates.Nor));
                AddPredefinedTool(new Tools.BoardEditorAddInputTool(true));
                AddPredefinedTool(new Tools.BoardEditorAddInputTool(false));
                AddPredefinedTool(new Tools.BoardEditorAddCubeGeneratorTool());

                this.SelectedIndices.Add(0);
            }
        }

        public void AddTool(IBoardEditorTool tool)
        {
            this.customTools.Add(tool.Name, tool);
            int index = this.customTools.IndexOfKey(tool.Name);
            CreateListViewItem(tool, this.predefinedTools.Count + index);
        }
    }
}
