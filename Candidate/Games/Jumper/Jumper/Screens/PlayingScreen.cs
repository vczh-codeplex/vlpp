using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using Jumper.Playing;

namespace Jumper.Screens
{
    class PlayingScreen : GameScreen
    {
        private Timer timer = null;
        private World world = null;

        public override void AttachController(IGameScreenController controller)
        {
            this.world = new World(controller.BufferSize);
            base.AttachController(controller);
        }

        public override void Dispose()
        {
            base.Dispose();
            this.world.Dispose();
            this.world = null;
        }

        public override void AttachForm(Form form)
        {
            base.AttachForm(form);
            this.timer = new Timer();
            this.timer.Interval = 16;
            this.timer.Tick += new EventHandler(timer_Tick);
            this.timer.Start();
        }

        public override void DetachForm(Form form)
        {
            this.timer.Stop();
            this.timer.Tick -= new EventHandler(timer_Tick);
            this.timer.Dispose();
            this.timer = null;
            base.DetachForm(form);
        }

        protected override void GameKeyDown(KeyEventArgs e)
        {
            base.GameKeyDown(e);
            switch (e.KeyCode)
            {
                case Keys.Left:
                    this.world.Player.PressLeft();
                    break;
                case Keys.Right:
                    this.world.Player.PressRight();
                    break;
            }
        }

        protected override void GameKeyUp(KeyEventArgs e)
        {
            base.GameKeyUp(e);
            switch (e.KeyCode)
            {
                case Keys.Left:
                    this.world.Player.ReleaseLeft();
                    break;
                case Keys.Right:
                    this.world.Player.ReleaseRight();
                    break;
            }
        }

        protected override void Paint()
        {
            if (this.world != null)
            {
                base.Paint();
                this.Controller.BufferGraphics.FillRectangle(Brushes.White, 0, 0, this.Controller.BufferSize.Width, this.Controller.BufferSize.Height);
                this.world.Paint(this.Controller.BufferGraphics);

                string score = this.world.Score.ToString();
                while (score.Length < 5)
                {
                    score = "0" + score;
                }
                DrawTextLeftTop(score, "Cooper Black", 16, 32, 32);
            }
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            try
            {
                this.world.Step();
            }
            catch (WorldCrashException)
            {
                this.Controller.OpenScreen(typeof(GameOverScreen), this.world.Score);
            }
            Paint();
            this.Controller.Flush();
        }
    }
}
