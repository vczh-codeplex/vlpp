using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _TranslateXMLtoCode.Codegen
{
    class Codegen_GacUI_codegen_cpp : Codegen_GacUI_base
    {
        #region GenerateMembers

        protected void GenerateConstructorInitializationList(RgacUDT udt, string internalObjectReferenceInput)
        {
            WriteLine("    :__internal_object_reference({0})", internalObjectReferenceInput);
            foreach (var baseUdt in udt.BaseClasses
                .Where(t => t.Access == GacAccess.Public && this.options.Udts.Contains(t.UDT))
                .Select(t => t.UDT)
                )
            {
                WriteLine("    ,{0}(static_cast<__GacUIInternal<{1}>::InternalObjectType*>((__GacUIInternal<{2}>::InternalObjectType*){3}))",
                    baseUdt.ToString(),
                    baseUdt.ToString(),
                    udt.ToString(),
                    internalObjectReferenceInput
                    );
            }
        }

        protected string GetUnwrappedParameterName(string parameterName)
        {
            return "_unwrapped_" + parameterName;
        }

        protected void GenerateParameterExtraction(string parameterName, RgacType type)
        {
            string originalType = type.OriginalGacType.ToString();
            if (type.Kind == RgacTypeKind.Primitive)
            {
                switch (type.PrimitiveKind)
                {
                    case RgacPrimitiveKind.String:
                    case RgacPrimitiveKind.StringReference:
                    case RgacPrimitiveKind.ConstStringReference:
                        originalType = "vl::ObjectString<wchar_t>";
                        break;
                }
            }
            WriteLine("{0} {1} = {2};",
                originalType,
                GetUnwrappedParameterName(parameterName),
                UnwrapValue(parameterName, type)
                );
        }

        protected void GenerateParameterExtraction(RgacMethod method)
        {
            for (int i = 0; i < method.ParameterNames.Length; i++)
            {
                GenerateParameterExtraction(method.ParameterNames[i], method.ParameterTypes[i]);
            }
        }

        protected void GenerateResultExtraction(string valueCode, RgacType type)
        {
            WriteLine("{0} _wrapped_result = {1};",
                GetType(type),
                WrapValue(valueCode, type)
                );
            WriteLine("return _wrapped_result;");
        }

        protected string GenerateParametersPassing(RgacMethod method)
        {
            return "(" + (method.ParameterNames.Select(GetUnwrappedParameterName).Aggregate("", (a, b) => a == "" ? b : a + ", " + b)) + ")";
        }

        protected void GenerateConstructor(RgacMethod method)
        {
            WriteLine("rptr<{0}> {1}CreateRptr({2})",
                GacUdtTypeName(method.OwnerUDT),
                method.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                method.ParameterTypes
                    .Zip(method.ParameterNames, Tuple.Create)
                    .Select(t => string.Format("{0} {1}", GetType(t.Item1), t.Item2))
                    .Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                );
            Begin("{");
            GenerateParameterExtraction(method);
            WriteLine("{0}* _unwrapped_result = new {1}{2};",
                method.OwnerUDT.AssociatedGacType.ToString(),
                method.OwnerUDT.AssociatedGacType.ToString(),
                GenerateParametersPassing(method)
                );
            WriteLine("return __GacUIInternal<{0}>::BuildRptr(_unwrapped_result);", method.OwnerUDT.ToString());
            End("}");
            WriteLine("");

            if (method.OwnerUDT.Kind == RgacUDTKind.Struct)
            {
                WriteLine("{0} {1}Create({2})",
                    GacUdtTypeName(method.OwnerUDT),
                    method.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                    method.ParameterTypes
                        .Zip(method.ParameterNames, Tuple.Create)
                        .Select(t => string.Format("{0} {1}", GetType(t.Item1), t.Item2))
                        .Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                    );
                Begin("{");
                GenerateParameterExtraction(method);
                WriteLine("{0} _unwrapped_result = {1}{2};",
                    method.OwnerUDT.AssociatedGacType.ToString(),
                    method.OwnerUDT.AssociatedGacType.ToString(),
                    GenerateParametersPassing(method)
                    );
                WriteLine("return __GacUIInternal<{0}>::BuildCopy(&_unwrapped_result);", method.OwnerUDT.ToString());
                End("}");
                WriteLine("");
            }
        }

        protected string GenerateMethodAccess(RgacMethod method, bool isStatic)
        {
            return isStatic
                ? (method.OwnerUDT.AssociatedGacType.ToString() + "::" + method.Name)
                : string.Format("__GacUIInternal<{0}>::GetInternalObject(*this)->{1}", method.OwnerUDT.ToString(), method.Name)
                ;
        }

        protected void GenerateMethod(RgacMethod method, bool isStatic)
        {
            WriteLine("{0} {1}{2}({3})",
                GetType(method.ReturnType),
                method.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                method.Name,
                method.ParameterTypes
                    .Zip(method.ParameterNames, Tuple.Create)
                    .Select(t => string.Format("{0} {1}", GetType(t.Item1), t.Item2))
                    .Aggregate("", (a, b) => a == "" ? b : a + ", " + b)
                );
            Begin("{");
            if (method.OwnerUDT.ToString() == "GuiPolygonElement" && method.Name == "SetPoints")
            {
                WriteLine("vl::collections::Array<vl::presentation::Point> _unwrapped_points(count);");
                WriteLine("for(int i=0;i<count;i++)");
                Begin("{");
                WriteLine("_unwrapped_points[i] = *__GacUIInternal<Point>::GetInternalObject(p[i]);");
                End("}");
                WriteLine("throw 0;");
            }
            else
            {
                GenerateParameterExtraction(method);
                if (method.ReturnType.Kind == RgacTypeKind.Primitive && method.ReturnType.PrimitiveKind == RgacPrimitiveKind.Void)
                {
                    WriteLine("{0}{1};",
                        GenerateMethodAccess(method, isStatic),
                        GenerateParametersPassing(method)
                        );
                }
                else
                {
                    WriteLine("{0} _unwrapped_result = {1}{2};",
                        method.ReturnType.ToString(),
                        GenerateMethodAccess(method, isStatic),
                        GenerateParametersPassing(method)
                        );
                    GenerateResultExtraction("_unwrapped_result", method.ReturnType);
                }
            }
            End("}");
            WriteLine("");
        }

        protected string GenerateVariableAccess(RgacProperty property, bool isStatic)
        {
            return isStatic
                ? (property.OwnerUDT.AssociatedGacType.ToString() + "::" + property.PublicGacFieldAccessor.Name)
                : string.Format("__GacUIInternal<{0}>::GetInternalObject(*this)->{1}", property.OwnerUDT.ToString(), property.PublicGacFieldAccessor.Name)
                ;
        }

        protected void GenerateProperty(RgacProperty property, bool isStatic)
        {
            if (property.Getter != null)
                GenerateMethod(property.Getter, isStatic);
            if (property.Setter != null)
                GenerateMethod(property.Setter, isStatic);
            if (property.PublicGacFieldAccessor != null)
            {
                if (property.IsEventField)
                {
                    WriteLine("{0} {1}on_{2}()",
                        GetType(property.PropertyType),
                        property.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                        property.Name
                        );
                    Begin("{");
                    GenerateResultExtraction(
                        GenerateVariableAccess(property, isStatic),
                        property.PropertyType
                        );
                    End("}");
                    WriteLine("");
                }
                else
                {
                    WriteLine("{0} {1}get_{2}()",
                        GetType(property.PropertyType),
                        property.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                        property.Name
                        );
                    Begin("{");
                    GenerateResultExtraction(
                        GenerateVariableAccess(property, isStatic),
                        property.PropertyType
                        );
                    End("}");
                    WriteLine("");

                    if (property.PublicGacFieldAccessor.Type.Kind != GacTypeKind.Const && !property.IsNotAssignableClassField)
                    {
                        WriteLine("void {0}set_{1}({2} value)",
                            property.OwnerUDT.Name.Aggregate("", (a, b) => a + b + "::"),
                            property.Name,
                            GetType(property.PropertyType)
                            );
                        Begin("{");
                        GenerateParameterExtraction("value", property.PropertyType);
                        WriteLine("{0} = {1};", GenerateVariableAccess(property, isStatic), GetUnwrappedParameterName("value"));
                        End("}");
                        WriteLine("");
                    }
                }
            }
        }

        protected void GenerateMembers(RgacUDT udt)
        {
            {
                WriteLine("{0}{1}(void* __internal_object_reference_input)",
                    udt.Name.Aggregate("", (a, b) => a + b + "::"),
                    udt.Name.Last()
                    );
                GenerateConstructorInitializationList(udt, "__internal_object_reference_input");
                Begin("{");
                End("}");
                WriteLine("");

                WriteLine("{0}~{1}()",
                    udt.Name.Aggregate("", (a, b) => a + b + "::"),
                    udt.Name.Last()
                    );
                Begin("{");
                if (udt.Kind == RgacUDTKind.Struct)
                {
                    WriteLine("delete __GacUIInternal<{0}>::GetInternalObject(*this);", udt.Name.Last());
                }
                End("}");
                WriteLine("");

                WriteLine("void {0}ClearInternalObjectReference()",
                    udt.Name.Aggregate("", (a, b) => a + b + "::")
                    );
                Begin("{");
                WriteLine("__internal_object_reference = 0;");
                foreach (var baseUdt in udt.BaseClasses
                    .Where(t => t.Access == GacAccess.Public && this.options.Udts.Contains(t.UDT))
                    .Select(t => t.UDT)
                    )
                {
                    WriteLine("{0}ClearInternalObjectReference();",
                        baseUdt.Name.Aggregate("", (a, b) => a + b + "::")
                        );
                }
                End("}");
                WriteLine("");
            }
            if (!udt.IsAbstract)
            {
                foreach (var m in udt.Constructors)
                {
                    GenerateConstructor(m);
                }
            }

            foreach (var m in udt.Methods)
            {
                GenerateMethod(m, false);
            }

            foreach (var m in udt.StaticMethods)
            {
                GenerateMethod(m, true);
            }

            foreach (var m in udt.Properties)
            {
                GenerateProperty(m, false);
            }

            foreach (var m in udt.StaticProperties)
            {
                GenerateProperty(m, true);
            }
        }

        #endregion

        protected void GenerateValueExtractor(RgacUDT udt)
        {
            WriteLine("template<>");
            WriteLine("class __GacUIInternal<{0}>", udt.ToString());
            WriteLine("{");
            Begin("public:");
            WriteLine("typedef {0} WrappedObjectType;", udt.ToString());
            WriteLine("typedef {0} InternalObjectType;", udt.AssociatedGacType.Name);
            WriteLine("");

            WriteLine("static InternalObjectType* GetInternalObject(const WrappedObjectType& wrappedObject)");
            Begin("{");
            WriteLine("return (InternalObjectType*)wrappedObject.__internal_object_reference;");
            End("}");
            WriteLine("");

            WriteLine("static InternalObjectType* GetInternalObject(const rptr<WrappedObjectType>& wrappedObject)");
            Begin("{");
            WriteLine("return (InternalObjectType*)wrappedObject->__internal_object_reference;");
            End("}");
            WriteLine("");

            WriteLine("static Ptr<InternalObjectType> GetInternalObject(const sptr<WrappedObjectType>& wrappedObject)");
            Begin("{");
            WriteLine("return _SptrBuilder<WrappedObjectType>::RetrivePtr<InternalObjectType>(wrappedObject);");
            End("}");
            WriteLine("");

            if (udt.Kind == RgacUDTKind.Struct)
            {
                WriteLine("static WrappedObjectType BuildCopy(const InternalObjectType* input)");
                Begin("{");
                WriteLine("return new InternalObjectType(*input);");
                End("}");
                WriteLine("");
            }

            WriteLine("static rptr<WrappedObjectType> BuildRptr(const InternalObjectType* input)");
            Begin("{");
            WriteLine("return _RptrBuilder<WrappedObjectType>::CreateRptr(input);");
            End("}");
            WriteLine("");

            WriteLine("static sptr<WrappedObjectType> BuildSptr(const vl::Ptr<InternalObjectType>& input)");
            Begin("{");
            WriteLine("return _SptrBuilder<WrappedObjectType>::CreateSptr(input);");
            End("}");
            WriteLine("");

            End("};");
            WriteLine("");
        }

        protected void GenerateCppImpl()
        {
            WriteSectionComment("__GacUIInternal<T> Implementations");
            foreach (var udt in GetSortedUdts()
                .Where(t => t.Kind != RgacUDTKind.Enum)
                .ToArray()
                )
            {
                GenerateValueExtractor(udt);
            }

            foreach (var udt in GetSortedUdts()
                .Where(t => t.Kind != RgacUDTKind.Enum)
                .ToArray()
                )
            {
                WriteSectionComment(udt.ToString());
                GenerateMembers(udt);
            }
        }

        protected override void GenerateCodeInternal()
        {
            WriteLine("#include \"GacUICommonInternal.h\"");
            WriteLine("#include \"GacUI.h\"");
            WriteLine("#include \"..\\..\\..\\..\\..\\Candidate\\GUI\\GUI\\GacUI.h\"");
            WriteLine("");
            WriteLine("namespace {0}", Codegen_GacUI_base.GacUINamespace);
            Begin("{");

            WriteLine("template<typename T>");
            WriteLine("class __GacUIInternal{};");

            GenerateCppImpl();

            End("}");
        }

        public Codegen_GacUI_codegen_cpp(CodeGeneratorOptions options)
            : base(options, options.GacuiPath + "GacUI.cpp")
        {
        }
    }
}
