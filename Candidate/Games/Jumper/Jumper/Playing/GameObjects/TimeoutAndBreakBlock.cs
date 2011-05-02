using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Jumper.Playing.GameObjects
{
    class TimeoutAndBreakBlock : BreakableBlock
    {
        private int lifetime = 0;

        public TimeoutAndBreakBlock(Point position)
            : base(position, Images.TimeoutAndBreakBlockC
            , Images.TimeoutAndBreakBlockA
            , Images.TimeoutAndBreakBlockB
            , Images.TimeoutAndBreakBlockC)
        {
        }

        public override void Step()
        {
            base.Step();
            if (!this.IsBreaking)
            {
                const int unitLifetime = 40;
                this.lifetime++;
                if (lifetime == unitLifetime)
                {
                    this.ImageIndex = 1;
                }
                else if (lifetime == unitLifetime * 2)
                {
                    this.ImageIndex = 2;
                }
                else if (lifetime == unitLifetime * 3)
                {
                    Break();
                }
            }
        }

        public override bool ContactPlayer(bool falling)
        {
            if (falling && !this.IsBreaking)
            {
                this.World.Player.JumpNormal();
                Break();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
