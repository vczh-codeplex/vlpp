using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Jumper.Playing.GameObjects;

namespace Jumper.Playing
{
    class World : IDisposable
    {
        private Random random = new Random();
        private List<WorldObject> objects = new List<WorldObject>();
        private int previousCreateObjectTop = 0;

        public Size Size { get; private set; }
        public Player Player { get; private set; }

        public World(Size size)
        {
            this.Size = size;
            AddObject(this.Player = new Player());
            CreateWorldObjects(0, this.Size.Height);
        }

        public int Score
        {
            get
            {
                return this.Player.MaxHeight / 3;
            }
        }

        public int WorldTop
        {
            get
            {
                return Math.Max(this.Size.Height, this.Player.MaxHeight + this.Size.Height / 3);
            }
        }

        public IEnumerable<WorldObject> Objects
        {
            get
            {
                return this.objects;
            }
        }

        public void AddObject(WorldObject obj)
        {
            this.objects.Add(obj);
            obj.AttachWorld(this);
        }

        public void Step()
        {
            for (int i = this.objects.Count - 1; i >= 0; i--)
            {
                WorldObject obj = this.objects[i];
                obj.Step();
                if (obj.Bounds.Top <= this.WorldTop - this.Size.Height && (!this.objects.Contains(obj.AdditionalObject) || obj.AdditionalObject.Bounds.Top <= this.WorldTop - this.Size.Height))
                {
                    if (obj != this.Player)
                    {
                        this.objects.RemoveAt(i);
                        obj.Dispose();
                    }
                }
            }
        }

        public void Paint(Graphics graphics)
        {
            for (int i = this.objects.Count - 1; i >= 0; i--)
            {
                WorldObject obj = this.objects[i];
                obj.Paint(graphics);
            }
        }

        public void Dispose()
        {
            foreach (var obj in this.objects)
            {
                obj.Dispose();
            }
        }

        private bool lastOneIsBrokenBlock = false;

        public void CreateWorldObjects(int oldTop, int newTop)
        {
            int createUnit = Math.Min(128, 64 + 40 * this.Score / 5000);
            while (this.previousCreateObjectTop <= newTop)
            {
                int x = this.random.Next(this.Size.Width - 100);
                int y = this.previousCreateObjectTop + createUnit;

                this.previousCreateObjectTop = y;
                WorldObject block = null;
                int blockId = this.random.Next(Math.Min(500, this.Score / 75), 1000);
                bool acceptAccelerator = false;

                if (0 <= blockId && blockId < 500)
                {
                    block = new StaticBlock(new Point(x, y));
                    acceptAccelerator = true;
                }
                else if (500 <= blockId && blockId < 700)
                {
                    block = new MovingBlock(new Point(x, y));
                    acceptAccelerator = true;
                }
                else if (700 <= blockId && blockId < 800)
                {
                    block = new JumpAndBreakBlock(new Point(x, y));
                }
                else if (800 <= blockId && blockId < 900)
                {
                    block = new TimeoutAndBreakBlock(new Point(x, y));
                }
                else if (this.lastOneIsBrokenBlock)
                {
                    block = new JumpAndBreakBlock(new Point(x, y));
                }
                else
                {
                    block = new BrokenBlock(new Point(x, y));
                }
                this.lastOneIsBrokenBlock = block is BrokenBlock || block is TimeoutAndBreakBlock;
                AddObject(block);

                if (acceptAccelerator)
                {
                    WorldObject accelerator = null;
                    int acceleratorId = this.random.Next(1000);

                    if (0 <= acceleratorId && acceleratorId < 100)
                    {
                        accelerator = new JumperAccelerator(block);
                    }
                    else if (100 <= acceleratorId && acceleratorId < 120)
                    {
                        accelerator = new FlyingHatAccelerator(block);
                    }
                    else if (120 <= acceleratorId && acceleratorId < 130)
                    {
                        accelerator = new RocketAccelerator(block);
                    }
                    else if (130 <= acceleratorId && acceleratorId < 200)
                    {
                        if (this.Score >= 5000)
                        {
                            accelerator = new Shield(block);
                            int monsterId = random.Next(100);
                            if (0 <= monsterId && monsterId < 50)
                            {
                                AddObject(new MonsterBat(new Point(x, y + 500), this.Size.Width, this.random));
                            }
                            else
                            {
                                AddObject(new MonsterMildew(new Point(x, y + 500), this.Size.Width, this.random));
                            }
                        }
                    }

                    if (accelerator != null)
                    {
                        AddObject(accelerator);
                    }
                }
            }
        }
    }
}
