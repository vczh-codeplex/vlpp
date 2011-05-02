using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Jumper.Playing.GameObjects
{
    class JumperAccelerator : BlockAdditionalObject
    {
        public JumperAccelerator(WorldObject blockObject)
            : base(blockObject, Images.JumperCollapsed, Images.JumperExpanded)
        {
        }

        public override bool ContactPlayer(bool falling)
        {
            if (falling)
            {
                this.World.Player.JumpHigh();
                this.ImageIndex = 1;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
