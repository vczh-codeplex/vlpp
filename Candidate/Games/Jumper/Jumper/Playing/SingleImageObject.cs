using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Jumper.Playing
{
    abstract class SingleImageObject : WorldObject
    {
        private Bitmap[] images = null;
        protected Point position = new Point(0, 0);

        protected int ImageIndex { get; set; }
        protected World World { get; private set; }

        public SingleImageObject(Point position, params Bitmap[] imageToProcess)
        {
            this.images = imageToProcess.Select(Initialize).ToArray();
            if (position.X != -1 && position.Y != -1)
            {
                this.position = new Point(position.X, position.Y + this.Bounds.Height);
            }
        }

        public override Rectangle Bounds
        {
            get
            {
                return new Rectangle(this.position, this.images[this.ImageIndex].Size);
            }
        }

        public override void AttachWorld(World world)
        {
            this.World = world;
        }

        public override void Step()
        {
        }

        public override void Paint(Graphics graphics)
        {
            graphics.DrawImage(this.images[this.ImageIndex], GetRenderingPosition(this.World));
        }

        public override void Dispose()
        {
            foreach (Bitmap bitmap in this.images)
            {
                bitmap.Dispose();
            }
        }
    }
}
