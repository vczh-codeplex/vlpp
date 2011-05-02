using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Jumper.Playing.GameObjects
{
    class Shield : BlockAdditionalObject
    {
        public Shield(WorldObject blockObject)
            : base(blockObject, Images.Shield)
        {
        }

        public override bool ContactPlayer(bool falling)
        {
            if (falling)
            {
                this.World.Player.JumpNormal();
            }
            this.World.Player.InstallShield();
            return true;
        }
    }
}
