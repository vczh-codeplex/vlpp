using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Developer.RibbonFramework.RibbonElements
{
    public class RibbonResourceManager : IDisposable
    {
        private Dictionary<Type, List<IDisposable>> usingObjects = new Dictionary<Type, List<IDisposable>>();
        private Dictionary<Type, List<IDisposable>> freeObjects = new Dictionary<Type, List<IDisposable>>();

        private List<IDisposable> Get(Dictionary<Type, List<IDisposable>> objects, Type type)
        {
            List<IDisposable> result = null;
            if (!objects.TryGetValue(type, out result))
            {
                result = new List<IDisposable>();
                objects.Add(type, result);
            }
            return result;
        }

        public void Dispose()
        {
            foreach (var obj in this.usingObjects.Values.SelectMany(v => v))
            {
                obj.Dispose();
            }
            foreach (var obj in this.freeObjects.Values.SelectMany(v => v))
            {
                obj.Dispose();
            }
        }

        public T Allocate<T>(Type type)
            where T : IDisposable
        {
            T result = default(T);
            List<IDisposable> frees = Get(this.freeObjects, type);
            if (frees.Count == 0)
            {
                result = (T)type.GetConstructor(new Type[] { }).Invoke(new object[] { });
            }
            else
            {
                result = (T)frees[0];
                frees.RemoveAt(0);
            }
            Get(this.usingObjects, type).Add(result);
            return result;
        }

        public void Free(IDisposable obj)
        {
            Type type = obj.GetType();
            if (Get(this.usingObjects, type).Remove(obj))
            {
                Get(this.freeObjects, type).Add(obj);
            }
        }
    }
}
