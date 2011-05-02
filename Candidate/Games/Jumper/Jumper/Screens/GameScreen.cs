using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Jumper.Screens
{
    abstract class GameScreen : IDisposable
    {
        protected IGameScreenController Controller { get; private set; }

        public virtual void AttachController(IGameScreenController controller)
        {
            this.Controller = controller;
            AttachForm(this.Controller.GameForm);
            Paint();
            this.Controller.Flush();
        }

        public virtual void Dispose()
        {
            DetachForm(this.Controller.GameForm);
        }

        public virtual void AttachForm(Form form)
        {
            form.KeyDown += new KeyEventHandler(form_KeyDown);
            form.KeyPress += new KeyPressEventHandler(form_KeyPress);
            form.KeyUp += new KeyEventHandler(form_KeyUp);
        }

        public virtual void DetachForm(Form form)
        {
            form.KeyDown -= new KeyEventHandler(form_KeyDown);
            form.KeyPress -= new KeyPressEventHandler(form_KeyPress);
            form.KeyUp -= new KeyEventHandler(form_KeyUp);
        }

        #region Callbacks

        protected virtual void GameKeyDown(KeyEventArgs e)
        {
        }

        protected virtual void GameKeyPress(KeyPressEventArgs e)
        {
        }

        protected virtual void GameKeyUp(KeyEventArgs e)
        {
        }

        protected virtual void Paint()
        {
        }

        #endregion

        #region API

        protected void DrawTextCentered(string text, string fontName, int size, int ty)
        {
            using (Font font = new Font(fontName, size))
            {
                SizeF textSize = this.Controller.BufferGraphics.MeasureString(text, font);
                int x = (int)((this.Controller.BufferSize.Width - textSize.Width) / 2);
                int y = ty;
                this.Controller.BufferGraphics.DrawString(text, font, Brushes.Black, x, y);
            }
        }

        protected void DrawTextLeftTop(string text, string fontName, int size, int tx, int ty)
        {
            using (Font font = new Font(fontName, size))
            {
                SizeF textSize = this.Controller.BufferGraphics.MeasureString(text, font);
                int x = tx;
                int y = ty;
                this.Controller.BufferGraphics.DrawString(text, font, Brushes.Black, x, y);
            }
        }

        protected void DrawTextLeftBottom(string text, string fontName, int size, int tx, int ty)
        {
            using (Font font = new Font(fontName, size))
            {
                SizeF textSize = this.Controller.BufferGraphics.MeasureString(text, font);
                int x = tx;
                int y = (int)(this.Controller.BufferSize.Height - textSize.Height - ty);
                this.Controller.BufferGraphics.DrawString(text, font, Brushes.Black, x, y);
            }
        }

        protected void DrawTextRightTop(string text, string fontName, int size, int tx, int ty)
        {
            using (Font font = new Font(fontName, size))
            {
                SizeF textSize = this.Controller.BufferGraphics.MeasureString(text, font);
                int x = (int)(this.Controller.BufferSize.Width - textSize.Width - tx);
                int y = ty;
                this.Controller.BufferGraphics.DrawString(text, font, Brushes.Black, x, y);
            }
        }

        protected void DrawTextRightBottom(string text, string fontName, int size, int tx, int ty)
        {
            using (Font font = new Font(fontName, size))
            {
                SizeF textSize = this.Controller.BufferGraphics.MeasureString(text, font);
                int x = (int)(this.Controller.BufferSize.Width - textSize.Width - tx);
                int y = (int)(this.Controller.BufferSize.Height - textSize.Height - ty);
                this.Controller.BufferGraphics.DrawString(text, font, Brushes.Black, x, y);
            }
        }

        #endregion

        #region Event Handlers

        private void form_KeyDown(object sender, KeyEventArgs e)
        {
            GameKeyDown(e);
        }

        private void form_KeyPress(object sender, KeyPressEventArgs e)
        {
            GameKeyPress(e);
        }

        private void form_KeyUp(object sender, KeyEventArgs e)
        {
            GameKeyUp(e);
        }

        #endregion
    }

    interface IGameScreenController
    {
        Graphics BufferGraphics { get; }
        Size BufferSize { get; }
        Form GameForm { get; }
        GameSettings Settings { get; }
        void Flush();
        void OpenScreen(Type screenType, params object[] arguments);
    }
}
