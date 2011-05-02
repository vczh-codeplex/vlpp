using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Jumper.Playing
{
    abstract class BlockAdditionalObject : SingleImageObject
    {
        private WorldObject blockObject;

        public BlockAdditionalObject(WorldObject blockObject, params Bitmap[] imageToProcess)
            : base(new Point(-1, -1), imageToProcess)
        {
            this.blockObject = blockObject;
            this.blockObject.AdditionalObject = this;
        }

        public override Rectangle Bounds
        {
            get
            {
                Size size = base.Bounds.Size;
                Rectangle block = this.blockObject.Bounds;
                Point position = new Point(block.Left + (block.Width - size.Width) / 2, block.Top + size.Height);
                return new Rectangle(position, size);
            }
        }
    }
}
