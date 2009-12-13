using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AutoShell.Services
{
    /// <summary>
    /// 服务容器
    /// </summary>
    public interface IServiceContainer
    {
        /// <summary>
        /// 注册服务
        /// </summary>
        /// <typeparam name="T">服务类型</typeparam>
        /// <param name="service">服务对象</param>
        void RegisterService<T>(T service);
        /// <summary>
        /// 请求服务对象
        /// </summary>
        /// <typeparam name="T">服务类型</typeparam>
        /// <returns>服务对象</returns>
        T QueryService<T>();
        /// <summary>
        /// 获取所有已注册的插件包
        /// </summary>
        /// <returns>已注册的插件包数组</returns>
        Package[] GetInstalledPackages();
        /// <summary>
        /// 获取所有已注册的服务类型
        /// </summary>
        /// <returns>已注册的服务类型数组</returns>
        Type[] GetInstalledServices();
        /// <summary>
        /// 请求服务对象
        /// </summary>
        /// <param name="type">服务类型</param>
        /// <returns>服务对象</returns>
        object GetServiceByType(Type type);
    }
}
