using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Jumper.Playing
{
    class Monster : SingleImageObject
    {
        private int min = 0;
        private int max = 0;
        private bool movingLeft = true;

        public Monster(Point position, int worldSize, Random random, Bitmap imageToProcess)
            : base(position, imageToProcess)
        {
            this.min = random.Next(0, worldSize * 4 / 5 - 20);
            this.max = random.Next(min + 20, worldSize - this.Bounds.Width - 1);
        }

        public override void Step()
        {
            base.Step();
            if (this.movingLeft)
            {
                this.position.X = Math.Max(0, this.position.X - 3);
                if (this.position.X <= this.min)
                {
                    this.movingLeft = false;
                }
            }
            else
            {
                this.position.X = Math.Min(max, this.position.X + 3);
                if (this.position.X >= this.max)
                {
                    this.movingLeft = true;
                }
            }
        }

        public override bool ContactPlayer(bool falling)
        {
            if (falling)
            {
                this.World.Player.JumpHigh();
            }
            else
            {
                this.World.Player.Faint();
            }
            return true;
        }
    }
}
