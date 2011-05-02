using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Jumper.Playing
{
    abstract class BreakableBlock : SingleImageObject
    {
        private Bitmap leftBlock;
        private Bitmap rightBlock;
        private int breakingTime = 0;
        private int velocity = 0;

        public bool IsBreaking { get; private set; }

        public BreakableBlock(Point position, Bitmap brokenImageToProcess, params Bitmap[] imageToProcess)
            : base(position, imageToProcess)
        {
            using (Bitmap left = new Bitmap(brokenImageToProcess.Width / 2, brokenImageToProcess.Height))
            using (Bitmap right = new Bitmap(brokenImageToProcess.Width - brokenImageToProcess.Width / 2, brokenImageToProcess.Height))
            using (Graphics gleft = Graphics.FromImage(left))
            using (Graphics gright = Graphics.FromImage(right))
            {
                RectangleF ld = new RectangleF(0, 0, left.Width, left.Height);
                RectangleF ls = ld;
                RectangleF rd = new RectangleF(0, 0, right.Width, right.Height);
                RectangleF rs = rd;
                rs.X += ls.Width;

                gleft.DrawImage(brokenImageToProcess, ld, ls, GraphicsUnit.Pixel);
                gright.DrawImage(brokenImageToProcess, rd, rs, GraphicsUnit.Pixel);
                this.leftBlock = Initialize(left);
                this.rightBlock = Initialize(right);
            }
        }

        public override void Dispose()
        {
            this.leftBlock.Dispose();
            this.rightBlock.Dispose();
            base.Dispose();
        }

        public override void Step()
        {
            base.Step();
            if (this.IsBreaking)
            {
                this.breakingTime++;
                this.position.Y += this.velocity;
                this.velocity -= 2;
            }
        }

        public override void Paint(Graphics graphics)
        {
            if (this.IsBreaking)
            {
                Matrix originalTransform = graphics.Transform;
                Point rp = GetRenderingPosition(this.World);
                double t = Math.PI * this.breakingTime / 45;
                {
                    RectangleF bounds = new Rectangle(rp, this.leftBlock.Size);
                    PointF p1 = rp;
                    PointF p2 = new PointF(p1.X + this.leftBlock.Width * (float)Math.Cos(t), p1.Y + this.leftBlock.Width * (float)Math.Sin(t));
                    PointF p3 = new PointF(p1.X - this.leftBlock.Height * (float)Math.Sin(t), p1.Y + this.leftBlock.Height * (float)Math.Cos(t));
                    Matrix transform = new Matrix(bounds, new PointF[] { p1, p2, p3 });
                    graphics.Transform = transform;
                    graphics.DrawImage(this.leftBlock, rp);
                }
                {
                    RectangleF bounds = new Rectangle(rp, this.rightBlock.Size);
                    PointF basep = new PointF(rp.X + this.Bounds.Width, rp.Y);
                    PointF p1 = new PointF(basep.X - this.rightBlock.Width * (float)Math.Cos(t), basep.Y + this.rightBlock.Width * (float)Math.Sin(t));
                    PointF p2 = new PointF(p1.X + this.rightBlock.Width * (float)Math.Cos(t), p1.Y - this.rightBlock.Width * (float)Math.Sin(t));
                    PointF p3 = new PointF(p1.X + this.rightBlock.Height * (float)Math.Sin(t), p1.Y + this.rightBlock.Height * (float)Math.Cos(t));
                    Matrix transform = new Matrix(bounds, new PointF[] { p1, p2, p3 });
                    graphics.Transform = transform;
                    graphics.DrawImage(this.rightBlock, rp);
                }
                graphics.Transform = originalTransform;
            }
            else
            {
                base.Paint(graphics);
            }
        }

        #region API

        public void Break()
        {
            this.IsBreaking = true;
        }

        #endregion
    }
}
