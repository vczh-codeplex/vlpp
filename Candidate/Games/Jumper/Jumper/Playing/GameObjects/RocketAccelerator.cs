using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Jumper.Playing.GameObjects
{
    class RocketAccelerator : BlockAdditionalObject
    {
        public RocketAccelerator(WorldObject blockObject)
            : base(blockObject, Images.Rocket)
        {
        }

        public override bool ContactPlayer(bool falling)
        {
            this.World.Player.FlyWithRocket();
            return true;
        }
    }
}
