﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.1
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace CodeBoxControlTest.TextEditorServiceReference {
    
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    [System.ServiceModel.ServiceContractAttribute(ConfigurationName="TextEditorServiceReference.ITextEditorService")]
    public interface ITextEditorService {
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/ITextEditorService/GetSelectionAnchor", ReplyAction="http://tempuri.org/ITextEditorService/GetSelectionAnchorResponse")]
        System.Drawing.Point GetSelectionAnchor();
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/ITextEditorService/GetSelectionCaret", ReplyAction="http://tempuri.org/ITextEditorService/GetSelectionCaretResponse")]
        System.Drawing.Point GetSelectionCaret();
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/ITextEditorService/GetSelectionStart", ReplyAction="http://tempuri.org/ITextEditorService/GetSelectionStartResponse")]
        System.Drawing.Point GetSelectionStart();
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/ITextEditorService/GetSelectionEnd", ReplyAction="http://tempuri.org/ITextEditorService/GetSelectionEndResponse")]
        System.Drawing.Point GetSelectionEnd();
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/ITextEditorService/GetLineCount", ReplyAction="http://tempuri.org/ITextEditorService/GetLineCountResponse")]
        int GetLineCount();
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/ITextEditorService/GetLineLength", ReplyAction="http://tempuri.org/ITextEditorService/GetLineLengthResponse")]
        int GetLineLength(int index);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/ITextEditorService/GetString", ReplyAction="http://tempuri.org/ITextEditorService/GetStringResponse")]
        string GetString(System.Drawing.Point p1, System.Drawing.Point p2);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/ITextEditorService/TextPositionToViewPoint", ReplyAction="http://tempuri.org/ITextEditorService/TextPositionToViewPointResponse")]
        System.Drawing.Point TextPositionToViewPoint(System.Drawing.Point p);
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    public interface ITextEditorServiceChannel : CodeBoxControlTest.TextEditorServiceReference.ITextEditorService, System.ServiceModel.IClientChannel {
    }
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    public partial class TextEditorServiceClient : System.ServiceModel.ClientBase<CodeBoxControlTest.TextEditorServiceReference.ITextEditorService>, CodeBoxControlTest.TextEditorServiceReference.ITextEditorService {
        
        public TextEditorServiceClient() {
        }
        
        public TextEditorServiceClient(string endpointConfigurationName) : 
                base(endpointConfigurationName) {
        }
        
        public TextEditorServiceClient(string endpointConfigurationName, string remoteAddress) : 
                base(endpointConfigurationName, remoteAddress) {
        }
        
        public TextEditorServiceClient(string endpointConfigurationName, System.ServiceModel.EndpointAddress remoteAddress) : 
                base(endpointConfigurationName, remoteAddress) {
        }
        
        public TextEditorServiceClient(System.ServiceModel.Channels.Binding binding, System.ServiceModel.EndpointAddress remoteAddress) : 
                base(binding, remoteAddress) {
        }
        
        public System.Drawing.Point GetSelectionAnchor() {
            return base.Channel.GetSelectionAnchor();
        }
        
        public System.Drawing.Point GetSelectionCaret() {
            return base.Channel.GetSelectionCaret();
        }
        
        public System.Drawing.Point GetSelectionStart() {
            return base.Channel.GetSelectionStart();
        }
        
        public System.Drawing.Point GetSelectionEnd() {
            return base.Channel.GetSelectionEnd();
        }
        
        public int GetLineCount() {
            return base.Channel.GetLineCount();
        }
        
        public int GetLineLength(int index) {
            return base.Channel.GetLineLength(index);
        }
        
        public string GetString(System.Drawing.Point p1, System.Drawing.Point p2) {
            return base.Channel.GetString(p1, p2);
        }
        
        public System.Drawing.Point TextPositionToViewPoint(System.Drawing.Point p) {
            return base.Channel.TextPositionToViewPoint(p);
        }
    }
}
