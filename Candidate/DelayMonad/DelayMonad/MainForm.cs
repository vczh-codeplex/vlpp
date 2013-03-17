using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DelayMonad
{
    public partial class MainForm : Form
    {
        private static SortedList<DateTime, IDelay> delays = new SortedList<DateTime, IDelay>();

        public static void AddDelay(IDelay delay)
        {
            delays.Add(DateTime.Now + delay.ExecutionDelay, delay);
        }

        public MainForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            button1.Enabled = false;
            Delay.Run(
                from a in Delay.Create(TimeSpan.FromSeconds(3), () =>
                {
                    textBox1.ReadOnly = true;
                    textBox3.Text = "textBox1.Text readed";
                    return textBox1.Text;
                })
                from b in Delay.Create(TimeSpan.FromSeconds(3), () =>
                {
                    textBox2.ReadOnly = true;
                    textBox3.Text = "textBox2.Text readed";
                    return textBox2.Text;
                })
                from _ in Delay.Create(TimeSpan.FromSeconds(3), () =>
                {
                    try
                    {
                        textBox3.Text = (int.Parse(a) + int.Parse(b)).ToString();
                    }
                    catch (Exception ex)
                    {
                        textBox3.Text = ex.Message;
                    }
                    textBox1.ReadOnly = false;
                    textBox2.ReadOnly = false;
                    button1.Enabled = true;
                    return null as object;
                })
                select _
                );
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            var now = DateTime.Now;
            var ds =
                (
                    from d in delays
                    where now > d.Key
                    select d
                ).ToArray();
            foreach (var d in ds)
            {
                delays.Remove(d.Key);
            }
            foreach (var d in ds)
            {
                d.Value.Execute();
            }
        }
    }
}
