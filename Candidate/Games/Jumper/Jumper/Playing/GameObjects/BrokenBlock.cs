using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Jumper.Playing.GameObjects
{
    class BrokenBlock : BreakableBlock
    {
        public BrokenBlock(Point position)
            : base(position, Images.BrokenBlock, Images.BrokenBlock)
        {
        }

        public override bool ContactPlayer(bool falling)
        {
            if (falling)
            {
                this.Break();
            }
            return false;
        }
    }
}
