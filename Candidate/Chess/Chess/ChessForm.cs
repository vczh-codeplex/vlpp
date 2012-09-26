using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Chess
{
    public partial class ChessForm : Form
    {

        private Bitmap bitmap;
        private Graphics bitmapGraphics;
        private ChessBoard chessBoard = new ChessBoard();

        private bool dragging = false;
        private Point draggingSource;
        private Point draggingOriginalLocation;
        private Point draggingOffset;

        private ChessPlayer? currentPlayer = null;

        public ChessForm()
        {
            bitmap = new Bitmap(1025, 513);
            bitmapGraphics = Graphics.FromImage(bitmap);
            chessBoard.Initialize();
            InitializeComponent();
            panelChess.GetType()
                .GetProperty("DoubleBuffered", BindingFlags.Instance | BindingFlags.NonPublic)
                .SetValue(panelChess, true);
            Redraw();
            ShowPlayer();
        }

        private void Redraw()
        {
            ChessBoardRenderer.Draw(bitmapGraphics, bitmap, chessBoard, dragging, draggingSource, draggingOffset);
            panelChess.Refresh();
        }

        private void ShowPlayer()
        {
            if (currentPlayer == null)
            {
                this.Text = "Chess (Current = NULL)";
            }
            else
            {
                this.Text = string.Format("Chess (Current = {0})", currentPlayer.Value.ToString());
            }
        }

        private void SwitchPlayer()
        {
            switch (currentPlayer.Value)
            {
                case ChessPlayer.红:
                    currentPlayer = ChessPlayer.黑;
                    break;
                case ChessPlayer.黑:
                    currentPlayer = ChessPlayer.红;
                    break;
            }
            ShowPlayer();
        }

        private void panelChess_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(bitmap, 0, 0);
        }

        private void panelChess_MouseDown(object sender, MouseEventArgs e)
        {
            int x = e.X / ChessBoardRenderer.CellSize;
            int y = e.Y / ChessBoardRenderer.CellSize;
            if (0 <= x && x < 8 && 0 <= y && y < 4)
            {
                var chess = chessBoard.chesses[x, y];
                if (chess != null)
                {
                    if (chess.covered)
                    {
                        chess.covered = false;
                        Redraw();

                        if (currentPlayer == null)
                        {
                            currentPlayer = chess.chessPlayer;
                        }
                        SwitchPlayer();
                    }
                    else if (chess.chessPlayer == currentPlayer.Value)
                    {
                        dragging = true;
                        draggingSource = new Point(x, y);
                        draggingOriginalLocation = e.Location;
                        draggingOffset = new Point(0, 0);
                    }
                }
            }
        }

        private void panelChess_MouseUp(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                int sx = draggingSource.X;
                int sy = draggingSource.Y;
                int dx = e.X / ChessBoardRenderer.CellSize;
                int dy = e.Y / ChessBoardRenderer.CellSize;
                if (0 <= dx && dx < 8 && 0 <= dy && dy < 4)
                {
                    if (chessBoard.Move(sx, sy, dx, dy))
                    {
                        SwitchPlayer();
                    }
                }
                dragging = false;
                Redraw();
            }
        }

        private void panelChess_MouseMove(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                draggingOffset = new Point(e.X - draggingOriginalLocation.X, e.Y - draggingOriginalLocation.Y);
                Redraw();
            }
        }
    }

    class ChessCharacterAttribute : Attribute
    {
        public string 红;
        public string 黑;
        public int Count;
        public int Level;

        public ChessCharacterAttribute(string 红, string 黑, int count, int level)
        {
            this.红 = 红;
            this.黑 = 黑;
            this.Count = count;
            this.Level = level;
        }
    }

    enum ChessType
    {
        [ChessCharacter("帅", "将", 1, 7)]
        将,
        [ChessCharacter("士", "士", 2, 6)]
        士,
        [ChessCharacter("相", "象", 2, 5)]
        象,
        [ChessCharacter("车", "车", 2, 4)]
        车,
        [ChessCharacter("马", "马", 2, 3)]
        马,
        [ChessCharacter("炮", "炮", 2, 2)]
        炮,
        [ChessCharacter("兵", "卒", 5, 1)]
        兵,
    }

    enum ChessPlayer
    {
        红, 黑,
    }

    class Chess
    {
        public ChessType chessType;
        public ChessPlayer chessPlayer;
        public bool covered;

        public Chess()
        {
            this.covered = true;
        }
    }

    class ChessBoard
    {
        public Chess[,] chesses = new Chess[8, 4];

        public static ChessCharacterAttribute GetChessProperties(ChessType chessType)
        {
            return (ChessCharacterAttribute)typeof(ChessType)
                .GetField(chessType.ToString(), BindingFlags.Public | BindingFlags.Static)
                .GetCustomAttributes(false)[0];
        }

        public void Initialize()
        {
            Random random = new Random();
            var randomChesses = typeof(ChessType)
                .GetFields(BindingFlags.Public | BindingFlags.Static)
                .SelectMany(f =>
                    {
                        var chessType = (ChessType)f.GetValue(null);
                        int count = GetChessProperties(chessType).Count;
                        return Enumerable.Range(0, count)
                            .Select(x => chessType);
                    })
                .SelectMany(chessType => new Chess[]
                {
                    new Chess{chessType=chessType,chessPlayer= ChessPlayer.红},
                    new Chess{chessType=chessType,chessPlayer= ChessPlayer.黑},
                })
                .OrderBy(c => random.Next())
                .ToArray();
            for (int i = 0; i < chesses.Length; i++)
            {
                chesses[i % 8, i / 8] = randomChesses[i];
            }
        }

        public bool Move(int sx, int sy, int dx, int dy)
        {
            if (Math.Abs(sx - dx) + Math.Abs(sy - dy) == 1)
            {
                Chess sc = chesses[sx, sy];
                Chess dc = chesses[dx, dy];
                if (dc == null)
                {
                    chesses[sx, sy] = null;
                    chesses[dx, dy] = sc;
                    return true;
                }
                else if (dc.chessPlayer != sc.chessPlayer)
                {
                    var sp = GetChessProperties(sc.chessType);
                    var dp = GetChessProperties(dc.chessType);
                    if (sp.Level > dp.Level || sp.Level == 1 && dp.Level == 7)
                    {
                        chesses[sx, sy] = null;
                        chesses[dx, dy] = sc;
                        return true;
                    }
                    else if (sp.Level == dp.Level)
                    {
                        chesses[sx, sy] = null;
                        chesses[dx, dy] = null;
                        return true;
                    }
                }
            }
            return false;
        }
    }

    class ChessBoardRenderer
    {
        public const int CellSize = 128;
        public const int ChessRadius = 56;

        static Font chessFont = new Font(new FontFamily("微软雅黑"), 55);

        private static void DrawBackground(Graphics bitmapGraphics, Bitmap bitmap)
        {
            using (SolidBrush brush = new SolidBrush(Color.FromArgb(239, 228, 176)))
            {
                bitmapGraphics.FillRectangle(brush, bitmapGraphics.ClipBounds);
            }
            using (Pen pen = new Pen(Color.FromArgb(63, 72, 204)))
            {
                for (int x = 0; x < 9; x++)
                {
                    bitmapGraphics.DrawLine(pen, x * CellSize, 0, x * CellSize, bitmap.Height);
                }
                for (int y = 0; y < 5; y++)
                {
                    bitmapGraphics.DrawLine(pen, 0, y * CellSize, bitmap.Width, y * CellSize);
                }
            }
        }

        private static void DrawChessWithLocation(Graphics bitmapGrahpics, Bitmap bitmap, Point location, Chess chess)
        {
            int x = location.X;
            int y = location.Y;
            using (Pen pen = new Pen(Color.FromArgb(0, 64, 128), 3))
            using (SolidBrush brush = new SolidBrush(Color.FromArgb(255, 201, 14)))
            {
                bitmapGrahpics.FillEllipse(brush, x - ChessRadius, y - ChessRadius, ChessRadius * 2, ChessRadius * 2);
                bitmapGrahpics.DrawEllipse(pen, x - ChessRadius, y - ChessRadius, ChessRadius * 2, ChessRadius * 2);
            }
            if (!chess.covered)
            {
                using (SolidBrush brush
                    = chess.chessPlayer == ChessPlayer.红
                    ? new SolidBrush(Color.FromArgb(237, 28, 36))
                    : new SolidBrush(Color.FromArgb(0, 64, 0))
                    )
                {
                    var properties = ChessBoard.GetChessProperties(chess.chessType);
                    var text = chess.chessPlayer == ChessPlayer.红 ? properties.红 : properties.黑;
                    SizeF size = bitmapGrahpics.MeasureString(text, chessFont);
                    int tx = x - (int)size.Width / 2;
                    int ty = y - (int)size.Height / 2;
                    bitmapGrahpics.DrawString(text, chessFont, brush, tx, ty);
                }
            }
        }

        private static Point GetChessLocation(int col, int row)
        {
            int x = col * CellSize + CellSize / 2;
            int y = row * CellSize + CellSize / 2;
            return new Point(x, y);
        }

        private static void DrawChess(Graphics bitmapGrahpics, Bitmap bitmap, int col, int row, Chess chess)
        {
            if (chess != null)
            {
                DrawChessWithLocation(bitmapGrahpics, bitmap, GetChessLocation(col, row), chess);
            }
        }

        public static void Draw(Graphics bitmapGraphics, Bitmap bitmap, ChessBoard chessBoard, bool dragging, Point draggingSource, Point draggingOffset)
        {
            DrawBackground(bitmapGraphics, bitmap);
            for (int x = 0; x < 8; x++)
            {
                for (int y = 0; y < 4; y++)
                {
                    if (!dragging || x != draggingSource.X || y != draggingSource.Y)
                    {
                        DrawChess(bitmapGraphics, bitmap, x, y, chessBoard.chesses[x, y]);
                    }
                }
            }
            if (dragging)
            {
                Chess chess = chessBoard.chesses[draggingSource.X, draggingSource.Y];
                Point location = GetChessLocation(draggingSource.X, draggingSource.Y);
                location.X += draggingOffset.X;
                location.Y += draggingOffset.Y;
                DrawChessWithLocation(bitmapGraphics, bitmap, location, chess);
            }
        }
    }
}
