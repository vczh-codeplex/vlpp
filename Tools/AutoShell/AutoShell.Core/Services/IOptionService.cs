using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AutoShell.Services
{
    /// <summary>
    /// 配置服务
    /// </summary>
    public interface IOptionService
    {
        /// <summary>
        /// 注册全局配置更改监听器
        /// </summary>
        /// <param name="listener">监听器</param>
        void RegisterOptionListener(IOptionListener listener);
        /// <summary>
        /// 注册具体配置更改监听器
        /// </summary>
        /// <param name="optionGuid">配置Guid</param>
        /// <param name="listener">监听器</param>
        void RegisterOptionListener(Guid optionGuid, IOptionListener listener);
        /// <summary>
        /// 注销配置监听器
        /// </summary>
        /// <param name="listener">监听器</param>
        void UnregisterOptionListener(IOptionListener listener);
        /// <summary>
        /// 注册配置
        /// </summary>
        /// <param name="optionGuid">配置Guid</param>
        /// <param name="serializableOption">配置类型（需打AutoShell.DataModel.DataSerializableAttribute）</param>
        void RegisterOption(Guid optionGuid, Type serializableOption);
        /// <summary>
        /// 注册配置编辑器
        /// </summary>
        /// <param name="optionGuid">配置Guid</param>
        /// <param name="editorGuid">编辑器Guid</param>
        /// <param name="name">项目名称</param>
        /// <param name="optionEditor">编辑器对象</param>
        void RegisterOptionEditor(Guid optionGuid, Guid editorGuid, string name, IOptionEditor optionEditor);
        /// <summary>
        /// 注册配置子编辑器
        /// </summary>
        /// <param name="optionGuid">配置Guid</param>
        /// <param name="editorGuid">编辑器Guid</param>
        /// <param name="parentGuid">父编辑器Guid</param>
        /// <param name="name">项目名称</param>
        /// <param name="optionEditor">编辑器对象</param>
        void RegisterOptionEditor(Guid optionGuid, Guid editorGuid, Guid parentGuid, string name, IOptionEditor optionEditor);
        /// <summary>
        /// 从文件加载配置并保留文件名
        /// </summary>
        /// <param name="fileName">配置文件名</param>
        void LoadOptions(string fileName);
        /// <summary>
        /// 将配置保留到最近加载的配置文件中
        /// </summary>
        void SaveOptions();
        /// <summary>
        /// 获得配置对象
        /// </summary>
        /// <param name="optionGuid">配置Guid</param>
        /// <returns>配置对象</returns>
        object GetOption(Guid optionGuid);
        /// <summary>
        /// 复制配置对象
        /// </summary>
        /// <param name="optionGuid">配置Guid</param>
        /// <returns>配置对象</returns>
        object CopyOption(Guid optionGuid);
        /// <summary>
        /// 更改配置对象
        /// </summary>
        /// <param name="optionGuid">配置Guid</param>
        /// <param name="option">配置对象</param>
        void SetOption(Guid optionGuid, object option);
    }

    /// <summary>
    /// 配置监听器
    /// </summary>
    public interface IOptionEditor
    {
        /// <summary>
        /// 初始化配置监听器
        /// </summary>
        /// <param name="serviceContainer">全局服务容器</param>
        void Initialize(IServiceContainer serviceContainer);
        /// <summary>
        /// 开始编辑配置
        /// </summary>
        /// <param name="optionGuid">配置Guid</param>
        /// <param name="option">配置对象</param>
        void BeginEditOption(Guid optionGuid, object option);
        /// <summary>
        /// 请求结束编辑配置
        /// </summary>
        /// <returns>返回true代表配置编辑器可以关闭，false为不可以关闭或切换到另一个编辑器</returns>
        bool QueryEndEditOption();
        /// <summary>
        /// 结束编辑配置
        /// </summary>
        void EndEditOption();
        /// <summary>
        /// 获取配置编辑器控件
        /// </summary>
        /// <returns>控件实例</returns>
        Control GetEditorControl();
    }

    /// <summary>
    /// 配置监听器
    /// </summary>
    public interface IOptionListener
    {
        /// <summary>
        /// 通知所关心的配置已经被更改
        /// </summary>
        /// <param name="optionGuid">配置Guid</param>
        void OptionUpdated(Guid optionGuid);
    }
}