using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Jumper.Playing.GameObjects
{
    class MovingBlock : SingleImageObject
    {
        private bool movingLeft = true;

        public MovingBlock(Point position)
            : base(position, Images.MovingBlock)
        {
        }

        public override void Step()
        {
            base.Step();
            if (this.movingLeft)
            {
                this.position.X = Math.Max(0, this.position.X - 2);
                if (this.position.X <= 0)
                {
                    this.movingLeft = false;
                }
            }
            else
            {
                int max = this.World.Size.Width - this.Bounds.Width;
                this.position.X = Math.Min(max, this.position.X + 2);
                if (this.position.X >= max)
                {
                    this.movingLeft = true;
                }
            }
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
