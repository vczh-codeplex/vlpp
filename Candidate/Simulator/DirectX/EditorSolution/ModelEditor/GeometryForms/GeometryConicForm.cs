using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ModelEditor.GeometryForms
{
    public partial class GeometryConicForm : Form
    {
        public GeometryConicType Geometry { get; private set; }
        public int Rows { get; private set; }
        public int Cols { get; private set; }

        public GeometryConicForm(GeometryConicType geometry)
        {
            InitializeComponent();
            comboBoxGeometry.Items.Add(GeometryConicType.Sphere);
            comboBoxGeometry.Items.Add(GeometryConicType.Cylinder);
            comboBoxGeometry.Items.Add(GeometryConicType.Cone);

            this.Geometry = geometry;
            switch (this.Geometry)
            {
                case GeometryConicType.Sphere:
                    comboBoxGeometry.SelectedIndex = 0;
                    numericRows.Value = 10;
                    numericCols.Value = 20;
                    break;
                case GeometryConicType.Cylinder:
                    comboBoxGeometry.SelectedIndex = 1;
                    numericRows.Value = 1;
                    numericCols.Value = 8;
                    break;
                case GeometryConicType.Cone:
                    comboBoxGeometry.SelectedIndex = 2;
                    numericRows.Value = 1;
                    numericCols.Value = 8;
                    break;
            }
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Geometry = (GeometryConicType)comboBoxGeometry.SelectedItem;
            this.Rows = (int)numericRows.Value;
            this.Cols = (int)numericCols.Value;
            Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            Close();
        }
    }

    public enum GeometryConicType
    {
        Sphere,
        Cylinder,
        Cone,
    }
}
