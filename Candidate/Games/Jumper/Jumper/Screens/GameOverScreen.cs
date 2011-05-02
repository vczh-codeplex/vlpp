using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Configuration;

namespace Jumper.Screens
{
    class GameOverScreen : MenuScreen
    {
        private int score = 0;

        public GameOverScreen(int score)
            : base("Game Over")
        {
            this.MenuItem("F2: Replay", Keys.F2, (e) =>
            {
                this.Controller.OpenScreen(typeof(PlayingScreen));
            });
            this.MenuItem("F3: Back", Keys.F3, (e) =>
            {
                this.Controller.OpenScreen(typeof(StartScreen));
            });

            this.score = score;
        }

        public override void AttachController(IGameScreenController controller)
        {
            GameSettings settings = controller.Settings;
            if (this.score > settings.HigestScore)
            {
                settings.HigestScore = this.score;
            }
            base.AttachController(controller);
        }

        protected override void Paint()
        {
            base.Paint();
            this.DrawTextCentered("Your Score: " + this.score.ToString(), "Cooper Black", 24, 300);
            this.DrawTextCentered("Higest Score: " + this.Controller.Settings.HigestScore.ToString(), "Cooper Black", 24, 360);
        }
    }
}
