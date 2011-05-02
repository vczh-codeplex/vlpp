using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace Jumper.Screens
{
    abstract class MenuScreen : GameScreen
    {
        private string title = "";
        private Dictionary<Keys, Action<KeyEventArgs>> menuItemHandlers = new Dictionary<Keys, Action<KeyEventArgs>>();
        private List<string> menuItems = new List<string>();

        protected MenuScreen(string title)
        {
            this.title = title;
        }

        protected void MenuItem(string text, Keys key, Action<KeyEventArgs> handler)
        {
            this.menuItems.Add(text);
            this.menuItemHandlers.Add(key, handler);
        }

        protected override void Paint()
        {
            this.Controller.BufferGraphics.FillRectangle(Brushes.White, 0, 0, this.Controller.BufferSize.Width, this.Controller.BufferSize.Height);
            DrawTextCentered(this.title, "Cooper Black", 40, 40);
            int y = 120;
            foreach (string menuItem in this.menuItems)
            {
                DrawTextCentered(menuItem, "Cooper Black", 24, y);
                y += 40;
            }
            DrawTextRightBottom("http://www.cppblog.com/vczh", "Cooper Black", 10, 32, 32);
        }

        protected override void GameKeyUp(KeyEventArgs e)
        {
            base.GameKeyUp(e);
            if (this.menuItemHandlers.ContainsKey(e.KeyCode))
            {
                this.menuItemHandlers[e.KeyCode](e);
            }
        }
    }
}
