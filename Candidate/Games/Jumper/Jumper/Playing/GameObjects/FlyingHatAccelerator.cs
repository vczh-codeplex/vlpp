using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Jumper.Playing.GameObjects
{
    class FlyingHatAccelerator : BlockAdditionalObject
    {
        public FlyingHatAccelerator(WorldObject blockObject)
            : base(blockObject, Images.FlyingHat)
        {
        }

        public override bool ContactPlayer(bool falling)
        {
            this.World.Player.FlyWithHat();
            return true;
        }
    }
}
