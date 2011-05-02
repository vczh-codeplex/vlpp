using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;

namespace Jumper.Screens
{
    class StartScreen : MenuScreen
    {
        public StartScreen()
            : base("Jumper")
        {
            this.MenuItem("F2: Play Game", Keys.F2, (e) =>
            {
                this.Controller.OpenScreen(typeof(PlayingScreen));
            });
            this.MenuItem("F3: Read My Blog", Keys.F3, (e) =>
            {
                Process.Start("http://www.cppblog.com/vczh");
            });
            this.MenuItem("F4: Exit", Keys.F4, (e) =>
            {
                this.Controller.GameForm.Close();
            });
        }

        protected override void Paint()
        {
            base.Paint();
            using (Bitmap bitmap = Jumper.Playing.Images.PlayerLeft)
            {
                int x = (this.Controller.BufferSize.Width - bitmap.Width) / 2;
                int y = (this.Controller.BufferSize.Height - bitmap.Height) / 2 + 50;
                this.Controller.BufferGraphics.DrawImage(bitmap, x, y, bitmap.Width, bitmap.Height);
            }
        }
    }
}
