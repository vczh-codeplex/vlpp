using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.Services;
using AutoShell.PublicShell;

namespace AutoShell.ServiceImpl
{
    internal class ServiceContainer : IServiceContainer
    {
        private Dictionary<Type, object> services = new Dictionary<Type, object>();
        private List<Package> packages = new List<Package>();

        public void RecordPackage(Package package)
        {
            this.packages.Add(package);
        }

        #region IServiceContainer Members

        public void RegisterService<T>(T service)
        {
            if (this.services.ContainsKey(typeof(T)))
            {
                throw new InvalidOperationException(string.Format(Strings.ExceptionDuplicatedService, typeof(T).FullName));
            }
            else
            {
                this.services.Add(typeof(T), service);

                IOutputService outputService = QueryService<IOutputService>();
                if (outputService != null)
                {
                    if (typeof(T) == typeof(IOutputService))
                    {
                        outputService.Print(PublicShellCore.LoggerOutputChannel, string.Format(Strings.LogServiceCreated, typeof(IUIShell).FullName));
                    }
                    outputService.Print(PublicShellCore.LoggerOutputChannel, string.Format(Strings.LogServiceCreated, typeof(T).FullName));
                }
            }
        }

        public T QueryService<T>()
        {
            if (this.services.ContainsKey(typeof(T)))
            {
                return (T)this.services[typeof(T)];
            }
            else
            {
                return default(T);
            }
        }

        public Package[] GetInstalledPackages()
        {
            return this.packages.ToArray();
        }

        public Type[] GetInstalledServices()
        {
            return this.services.Keys.ToArray();
        }

        public object GetServiceByType(Type type)
        {
            return this.services[type];
        }

        #endregion
    }
}
