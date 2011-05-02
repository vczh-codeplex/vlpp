using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Jumper.Playing.GameObjects
{
    class StaticBlock : SingleImageObject
    {
        public StaticBlock(Point position)
            : base(position, Images.StaticBlock)
        {
        }

        public override bool ContactPlayer(bool falling)
        {
            if (falling)
            {
                this.World.Player.JumpNormal();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
