using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Jumper.Playing.GameObjects
{
    class Player : WorldObject
    {
        enum Direction
        {
            Left,
            Right,
        }

        enum FlyingTool
        {
            None,
            Hat,
            Rocket,
        }

        private Bitmap imagePlayerLeft = null;
        private Bitmap imagePlayerRight = null;
        private Bitmap imagePlayerLeftFaint = null;
        private Bitmap imagePlayerRightFaint = null;
        private Bitmap imagePlayerLeftWithHat = null;
        private Bitmap imagePlayerRightWithHat = null;
        private Bitmap imagePlayerLeftWithHat2 = null;
        private Bitmap imagePlayerRightWithHat2 = null;
        private Bitmap imagePlayerLeftWithRocket = null;
        private Bitmap imagePlayerRightWithRocket = null;
        private Bitmap imagePlayerLeftWithRocket2 = null;
        private Bitmap imagePlayerRightWithRocket2 = null;

        private World world = null;
        private Direction currentDirection = Direction.Left;
        private FlyingTool flyingTool = FlyingTool.None;
        private bool pressingDirectionKey = false;
        private Point position = new Point(0, 0);
        private Point velocity = new Point(0, 0);
        private int flyingTime = 0;
        private int shieldingTime = 0;
        private bool fainting = false;

        public Player()
        {
            this.imagePlayerLeft = Initialize(Images.PlayerLeft);
            this.imagePlayerRight = Initialize(Images.PlayerRight);
            this.imagePlayerLeftFaint = Initialize(Images.PlayerLeftFaint);
            this.imagePlayerRightFaint = Initialize(Images.PlayerRightFaint);
            this.imagePlayerLeftWithHat = Initialize(Images.PlayerLeftWithHat);
            this.imagePlayerRightWithHat = Initialize(Images.PlayerRightWithHat);
            this.imagePlayerLeftWithHat2 = Initialize(Images.PlayerLeftWithHat2);
            this.imagePlayerRightWithHat2 = Initialize(Images.PlayerRightWithHat2);
            this.imagePlayerLeftWithRocket = Initialize(Images.PlayerLeftWithRocket);
            this.imagePlayerRightWithRocket = Initialize(Images.PlayerRightWithRocket);
            this.imagePlayerLeftWithRocket2 = Initialize(Images.PlayerLeftWithRocket2);
            this.imagePlayerRightWithRocket2 = Initialize(Images.PlayerRightWithRocket2);
        }

        public override void Dispose()
        {
            this.imagePlayerLeft.Dispose();
            this.imagePlayerRight.Dispose();
            this.imagePlayerLeftFaint.Dispose();
            this.imagePlayerRightFaint.Dispose();
            this.imagePlayerLeftWithHat.Dispose();
            this.imagePlayerRightWithHat.Dispose();
            this.imagePlayerLeftWithHat2.Dispose();
            this.imagePlayerRightWithHat2.Dispose();
            this.imagePlayerLeftWithRocket.Dispose();
            this.imagePlayerRightWithRocket.Dispose();
            this.imagePlayerLeftWithRocket2.Dispose();
            this.imagePlayerRightWithRocket2.Dispose();
        }

        public int MaxHeight { get; private set; }

        public override Rectangle Bounds
        {
            get
            {
                return new Rectangle(this.position, this.imagePlayerLeft.Size);
            }
        }

        public override void AttachWorld(World world)
        {
            this.world = world;
            this.position = new Point((world.Size.Width - this.Bounds.Width) / 2, this.Bounds.Height + 32);
            JumpNormal();
        }

        public override void Step()
        {
            // log previous data
            int oldTop = this.MaxHeight;
            int oldY = this.position.Y;

            // move player
            this.position.X += this.velocity.X;
            this.position.Y += this.velocity.Y;
            if (this.shieldingTime > 0)
            {
                this.shieldingTime -= 1;
            }
            if (!this.fainting && this.flyingTime > 0)
            {
                this.flyingTime -= 1;
                if (this.flyingTime == 0)
                {
                    this.flyingTool = FlyingTool.None;
                }
            }
            else
            {
                this.velocity.Y -= 1;
            }
            if (!this.pressingDirectionKey)
            {
                if (this.velocity.X < 0)
                {
                    this.velocity.X += 2;
                }
                else if (this.velocity.X > 0)
                {
                    this.velocity.X -= 2;
                }
            }

            // correct X value
            if (this.position.X < -this.Bounds.Width)
            {
                this.position.X = this.world.Size.Width + this.position.X + this.Bounds.Width;
            }
            else if (this.position.X >= this.world.Size.Width)
            {
                this.position.X = this.position.X - this.Bounds.Width - this.world.Size.Width;
            }

            // game over if player falls out of the screen
            this.MaxHeight = Math.Max(this.MaxHeight, this.position.Y);
            if (this.position.Y <= this.world.WorldTop - this.world.Size.Height)
            {
                throw new WorldCrashException();
            }
            if (oldTop != this.MaxHeight)
            {
                this.world.CreateWorldObjects(oldTop, this.world.WorldTop);
            }

            // fall and collide
            const int blank = 13;

            int y1 = oldY - this.Bounds.Height;
            int y2 = this.position.Y - this.Bounds.Height;
            WorldObject collidedObject = null;
            if (y2 < y1)
            {
                Rectangle walkingArea = new Rectangle(this.position.X + blank, y2, this.Bounds.Width - blank * 2, y1 - y2 + 1);
                collidedObject = this.world.Objects
                    .Where(o => o != this && o.Bounds.Top <= y1 && o.Bounds.IntersectsWith(walkingArea))
                    .OrderBy(o => o.Bounds.Top)
                    .LastOrDefault();
                if (collidedObject != null && collidedObject.ContactPlayer(true))
                {
                    collidedObject.ContactPlayer(true);
                }
            }

            // collide
            Rectangle playerBounds = this.Bounds;
            playerBounds.X += blank;
            playerBounds.Width -= blank * 2;
            foreach (var obj in this.world.Objects)
            {
                if (obj != this && obj.Bounds.IntersectsWith(playerBounds))
                {
                    obj.ContactPlayer(false);
                }
            }
        }

        private Bitmap GetImage()
        {
            switch (this.currentDirection)
            {
                case Direction.Left:
                    if (this.fainting)
                    {
                        return this.imagePlayerLeftFaint;
                    }
                    else
                    {
                        switch (this.flyingTool)
                        {
                            case FlyingTool.None:
                                return this.imagePlayerLeft;
                            case FlyingTool.Hat:
                                return (this.flyingTime % 2 == 0) ? this.imagePlayerLeftWithHat : this.imagePlayerLeftWithHat2;
                            case FlyingTool.Rocket:
                                return (this.flyingTime % 2 == 0) ? this.imagePlayerLeftWithRocket : this.imagePlayerLeftWithRocket2;
                        }
                    }
                    break;
                case Direction.Right:
                    if (this.fainting)
                    {
                        return this.imagePlayerRightFaint;
                    }
                    else
                    {
                        switch (this.flyingTool)
                        {
                            case FlyingTool.None:
                                return this.imagePlayerRight;
                            case FlyingTool.Hat:
                                return (this.flyingTime % 2 == 0) ? this.imagePlayerRightWithHat : this.imagePlayerRightWithHat2;
                            case FlyingTool.Rocket:
                                return (this.flyingTime % 2 == 0) ? this.imagePlayerRightWithRocket : this.imagePlayerRightWithRocket2;
                        }
                    }
                    break;
            }
            return null;
        }

        public override void Paint(Graphics graphics)
        {
            Bitmap image = GetImage();
            graphics.DrawImage(image, this.GetRenderingPosition(this.world));
            if (this.shieldingTime > 0)
            {
                using (Pen pen = new Pen(Color.FromArgb(34, 177, 76), 3))
                using (SolidBrush brush = new SolidBrush(Color.FromArgb((byte)Math.Min(128, 128 * this.shieldingTime / 100), 181, 230, 29)))
                {
                    Rectangle r = new Rectangle(GetRenderingPosition(this.world) - new Size(10, 10), image.Size + new Size(20, 20));
                    graphics.FillEllipse(brush, r);
                    graphics.DrawEllipse(pen, r);
                }
            }
        }

        public override bool ContactPlayer(bool falling)
        {
            throw new NotSupportedException();
        }

        #region State Transfering Handlers

        public void JumpNormal()
        {
            if (!this.fainting && this.velocity.Y <= 0)
            {
                this.velocity = new Point(this.velocity.X, 25);
            }
        }

        public void JumpHigh()
        {
            if (!this.fainting && this.velocity.Y <= 0)
            {
                this.velocity = new Point(this.velocity.X, 40);
            }
        }

        public void FlyWithHat()
        {
            if (!this.fainting && this.flyingTool == FlyingTool.None)
            {
                this.velocity = new Point(this.velocity.X, 40);
                this.flyingTime = 80;
                this.flyingTool = FlyingTool.Hat;
                this.shieldingTime = 120;
            }
        }

        public void FlyWithRocket()
        {
            if (!this.fainting && this.flyingTool == FlyingTool.None)
            {
                this.velocity = new Point(this.velocity.X, 60);
                this.flyingTime = 120;
                this.flyingTool = FlyingTool.Rocket;
                this.shieldingTime = 160;
            }
        }

        public void Faint()
        {
            if (this.flyingTool == FlyingTool.None && this.shieldingTime == 0)
            {
                this.fainting = true;
            }
        }

        public void InstallShield()
        {
            this.shieldingTime = Math.Max(this.shieldingTime, 100);
        }

        #endregion

        #region Input Handlers

        public void PressLeft()
        {
            if (!this.fainting)
            {
                this.pressingDirectionKey = true;
                this.currentDirection = Direction.Left;
                this.velocity.X = -10;
            }
        }

        public void ReleaseLeft()
        {
            if (!this.fainting)
            {
                this.pressingDirectionKey = false;
            }
        }

        public void PressRight()
        {
            if (!this.fainting)
            {
                this.pressingDirectionKey = true;
                this.currentDirection = Direction.Right;
                this.velocity.X = 10;
            }
        }

        public void ReleaseRight()
        {
            if (!this.fainting)
            {
                this.pressingDirectionKey = false;
            }
        }

        #endregion
    }
}
