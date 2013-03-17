using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DelayMonad
{
    public interface IDelay
    {
        TimeSpan ExecutionDelay { get; }
        void Execute();
    }

    public class Delay<T> : IDelay
    {
        public TimeSpan ExecutionDelay { get; private set; }
        public Action<Action<T>> Continuation { get; private set; }

        public Delay(TimeSpan executionDelay, Action<Action<T>> continuation)
        {
            this.ExecutionDelay = executionDelay;
            this.Continuation = continuation;
        }

        public void Execute()
        {
            this.Continuation(t => { });
        }
    }

    static class Delay
    {
        public static Delay<TResult> Select<TSource, TResult>(this Delay<TSource> source, Func<TSource, TResult> selector)
        {
            return new Delay<TResult>(
                source.ExecutionDelay,
                callback => source.Continuation(result => callback(selector(result)))
                );
        }

        public static Delay<TResult> SelectMany<TSource, TResult>(this Delay<TSource> source, Func<TSource, Delay<TResult>> selector)
        {
            return new Delay<TResult>(
                source.ExecutionDelay,
                callback => source.Continuation(result =>
                    {
                        var delay = selector(result);
                        Run(
                            new Delay<object>(
                                delay.ExecutionDelay,
                                o => delay.Continuation(callback)
                                )
                            );
                    })
                );
        }

        public static Delay<TResult> SelectMany<TSource, TCollection, TResult>(this Delay<TSource> source, Func<TSource, Delay<TCollection>> collectionSelector, Func<TSource, TCollection, TResult> resultSelector)
        {
            return source.SelectMany(t => collectionSelector(t).Select(u => resultSelector(t, u)));
        }

        public static Delay<T> Create<T>(TimeSpan executionDelay, Func<T> function)
        {
            return new Delay<T>(executionDelay, callback => callback(function()));
        }

        public static object Do(Action action)
        {
            action();
            return null;
        }

        public static void Run<T>(Delay<T> delay)
        {
            MainForm.AddDelay(delay);
        }
    }
}
