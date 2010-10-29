﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing.Drawing2D;
using Developer.RibbonFramework.RibbonElements;

namespace Developer.RibbonFramework
{
    public partial class RibbonAeroForm : ToolStripAeroForm, IRibbonInputCallback
    {
        private RibbonContainer container = null;

        public RibbonAeroForm()
        {
            InitializeComponent();
        }

        void IRibbonInputCallback.CaptureMouse(bool capture)
        {
            this.AeroContainer.Capture = true;
        }

        Point IRibbonInputCallback.GetLocationInScreen(RibbonContainer ribbonContainer)
        {
            return this.AeroContainer.PointToScreen(new Point(0, 0));
        }

        public RibbonContainer Ribbon
        {
            get
            {
                return this.container;
            }
        }

        public void UpdateRibbon(bool refresh)
        {
            Rectangle ribbonBounds = new Rectangle(0, 0, this.AeroClientBounds.Width, 140);
            int tabOffset = this.AeroIconSize.Width;
            if (this.AeroIconBounds.Bottom < this.AeroClientBounds.Top)
            {
                tabOffset = 0;
            }
            if (ribbonBounds != this.container.RibbonBounds)
            {
                this.container.Update(this.AeroGraphics, ribbonBounds, tabOffset);
            }
            if (refresh)
            {
                RenderAeroBitmap();
                this.Refresh();
            }
        }

        protected void InitializeRibbon(ToolStrip toolStripAero)
        {
            this.InitializeAero(toolStripAero);
            this.AeroContainer.MouseUp += new MouseEventHandler(AeroContainer_MouseUp);
            this.AeroContainer.MouseMove += new MouseEventHandler(AeroContainer_MouseMove);
            this.AeroContainer.MouseDown += new MouseEventHandler(AeroContainer_MouseDown);
            this.AeroContainer.MouseLeave += new EventHandler(AeroContainer_MouseLeave);
            this.container = new RibbonContainer(this);
        }

        private MouseEventArgs CopyForRibbon(MouseEventArgs e)
        {
            return new MouseEventArgs(e.Button, e.Clicks, e.X, e.Y, e.Delta);
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            this.container.Dispose();
            this.container = null;
        }

        protected override void OnDrawBackground(Graphics g, Rectangle bounds)
        {
            base.OnDrawBackground(g, bounds);
            if (this.container != null)
            {
                UpdateRibbon(false);
                this.container.RenderTabPanel(g, bounds);
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            if (this.container != null)
            {
                Rectangle panelBounds = this.AeroClientBounds;
                panelBounds.Height = this.container.RibbonBounds.Height;
                this.container.RenderTabGroups(e.Graphics, panelBounds, this.IsAeroFullBorderControlEnabled);
            }
        }

        private void AeroContainer_MouseDown(object sender, MouseEventArgs e)
        {
            if (this.container.OnMouseDown(CopyForRibbon(e)))
            {
                RenderAeroBitmap();
                this.AeroContainer.Refresh();
            }
        }

        private void AeroContainer_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.container.OnMouseMove(CopyForRibbon(e)))
            {
                RenderAeroBitmap();
                this.AeroContainer.Refresh();
            }
        }

        private void AeroContainer_MouseUp(object sender, MouseEventArgs e)
        {
            if (this.container.OnMouseUp(CopyForRibbon(e)))
            {
                RenderAeroBitmap();
                this.AeroContainer.Refresh();
            }
        }

        private void AeroContainer_MouseLeave(object sender, EventArgs e)
        {
            if (this.container.OnMouseLeave(e))
            {
                RenderAeroBitmap();
                this.AeroContainer.Refresh();
            }
        }
    }
}

/*
 * RibbonContainer (Tabs, TabGroups)
 * RibbonTab (Text)
 * RibbonTabGroup (Text, Tabs)
 * RibbonGroup (Text, DetailedButton)
 *      RibbonButtonGroup (BigButtons, SmallButtons)
 *      RibbonToolGroup
 * RibbonDropDown
 * 
 * RibbonItem (Image, Text, ShowInGallery)
 *      RibbonButton (Items, ToggleGroup=-1)
 *          #Normal
 *          #Toggle
 *          #DropDown
 *          #Split (Tab Only)
 *      RibbonGallery (UnitSize)
 *          #Tiled
 *          #List
 *      RibbonSpliter
 *      RibbonGroupHeader
*/