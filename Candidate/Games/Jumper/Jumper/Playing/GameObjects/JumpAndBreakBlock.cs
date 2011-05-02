using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Jumper.Playing.GameObjects
{
    class JumpAndBreakBlock : BreakableBlock
    {
        public JumpAndBreakBlock(Point position)
            : base(position, Images.JumpAndBreakBlock, Images.JumpAndBreakBlock)
        {
        }

        public override bool ContactPlayer(bool falling)
        {
            if (falling)
            {
                if (!this.IsBreaking)
                {
                    this.World.Player.JumpNormal();
                    this.Break();
                    return true;
                }
            }
            return false;
        }
    }
}
