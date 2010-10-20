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
        private I analyzingInput = default(I);
        private bool analyzingInputAvailable = false;
        private Semaphore codeSemaphore = null;
        private object locker = null;

        public CalculationNotifier()
        {
            this.locker = new object();
            this.codeSemaphore = new Semaphore(0, 1);
            this.workingThread = new Thread(Run);
            this.workingThread.Start();
        }

        public void Analyze(I input)
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
            }
            if (needRelease)
            {
                this.codeSemaphore.Release(1);
            }
        }

        public void Dispose()
        {
            this.workingThread.Abort();
            this.codeSemaphore.Dispose();
        }

        protected abstract O Calculate(I input);
        protected abstract void Receive(O output);

        private void Run()
        {
            while (true)
            {
                this.codeSemaphore.WaitOne();
                I input = default(I);
                lock (this.locker)
                {
                    input = this.analyzingInput;
                    this.analyzingInputAvailable = false;
                }
                O output = Calculate(input);
                Receive(output);
            }
        }
    }
}
