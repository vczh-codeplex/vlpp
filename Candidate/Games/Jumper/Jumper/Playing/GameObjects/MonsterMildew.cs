﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Jumper.Playing.GameObjects
{
    class MonsterMildew : Monster
    {
        public MonsterMildew(Point position, int worldSize, Random random)
            : base(position, worldSize, random, Images.MonsterB)
        {
        }
    }
}
