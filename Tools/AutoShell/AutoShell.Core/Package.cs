using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AutoShell.Services;

namespace AutoShell
{
    public class Package
    {
        private IServiceContainer serviceContainer;

        /// <summary>
        /// 全局服务容器
        /// </summary>
        public IServiceContainer Services
        {
            get
            {
                return this.serviceContainer;
            }
            internal set
            {
                this.serviceContainer = value;
            }
        }

        public virtual void Initialize()
        {
        }
    }
}
