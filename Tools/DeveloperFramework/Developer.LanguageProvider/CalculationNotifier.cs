using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace Developer.LanguageProvider
{
    public abstract class CalculationNotifier<I, O> : IDisposable
    {
        private Thread workingThread = null;
        private Semaphore codeSemaphore = null;
        private object locker = null;

        private I analyzingInput = default(I);
        private int analyzingId = 0;
        private int analyzingCounter = 0;
        private bool analyzingInputAvailable = false;

        protected abstract O Calculate(I input);
        public event CalculationNotifierReceivedHandler<O> Received;

        public CalculationNotifier()
        {
            this.locker = new object();
            this.codeSemaphore = new Semaphore(0, 1);
            this.workingThread = new Thread(Run);
            this.workingThread.Start();
        }

        public int Analyze(I input)
        {
            bool needRelease = false;
            lock (this.locker)
            {
                if (!this.analyzingInputAvailable)
                {
                    needRelease = true;
                }
                this.analyzingInputAvailable = true;
                this.analyzingInput = input;
                this.analyzingId = this.analyzingCounter++;
            }
            if (needRelease)
            {
                this.codeSemaphore.Release(1);
            }
            return this.analyzingId;
        }

        public void Dispose()
        {
            this.workingThread.Abort();
            this.codeSemaphore.Dispose();
        }

        private void Run()
        {
            while (true)
            {
                this.codeSemaphore.WaitOne();
                I input = default(I);
                int id = this.analyzingId;
                lock (this.locker)
                {
                    input = this.analyzingInput;
                    this.analyzingInputAvailable = false;
                }
                O output = Calculate(input);
                if (this.Received != null)
                {
                    this.Received(output, id);
                }
            }
        }
    }

    public delegate void CalculationNotifierReceivedHandler<T>(T result, int id);
}
