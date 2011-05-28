#include "NativeXErrorMessageTranslator.h"
#include "NativeXCodeGenerator.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{

/***********************************************************************
NativeXMessageTranslator
***********************************************************************/

			class NativeXErrorMessageTranslator : public BasicErrorMessageTranslator
			{
			protected:
				BasicAnalyzer*			analyzer;

				BasicTypeRecord* GetExpressionType(BasicExpression* expression)
				{
					return analyzer->GetEnv()->GetExpressionType(expression);
				}

				WString ToString(BasicType* type)
				{
					BP argument(
						analyzer->GetEnv(),
						analyzer->GetEnv()->GlobalScope(),
						analyzer->GetTypeManager(),
						analyzer->GetTypeInfoManager(),
						*(List<Ptr<BasicLanguageCodeException>>*)0,
						*(SortedList<WString>*)0
						);
					return ToString(BasicLanguage_GetTypeRecord(type, argument, true));
				}

				WString ToString(BasicTypeRecord* type)
				{
					switch(type->GetType())
					{
					case BasicTypeRecord::Primitive:
						return PrimitiveTypeToString(type->PrimitiveType());
					case BasicTypeRecord::Pointer:
						return ToString(type->ElementType())+L"*";
					case BasicTypeRecord::Array:
						return ToString(type->ElementType())+L"["+itow(type->ElementCount())+L"]";
					case BasicTypeRecord::Function:
						{
							WString result=L"function "+ToString(type->ReturnType())+L"(";
							for(vint i=0;i<type->ParameterCount();i++)
							{
								if(i)result+=L", ";
								result+=ToString(type->ParameterType(i));
							}
							result+=L")";
							return result;
						}
					case BasicTypeRecord::Structure:
						{
							BasicGenericStructureProxyTypeRecord* proxy=dynamic_cast<BasicGenericStructureProxyTypeRecord*>(type);
							if(proxy)
							{
								BasicTypeRecord* genericType=proxy->UninstanciatedStructureType()->ElementType();
								WString result=genericType->Declaration()->name;
								for(vint i=0;i<genericType->ParameterCount();i++)
								{
									result+=(i?L", ":L"<");
									result+=ToString(genericType->ParameterType(i));
								}
								result+=L">";
								return result;
							}
							else
							{
								return type->Declaration()->name;
							}
						}
						break;
					case BasicTypeRecord::GenericArgument:
						return type->ArgumentName();
					case BasicTypeRecord::Generic:
						return type->Declaration()->name;
					default:
						return L"[UNKNOWN-NATIVEX-TYPE]";
					}
				}

				WString OpToString(BasicUnaryExpression* expression)
				{
					return UnaryOperatorToString(expression->type);
				}

				WString OpToString(BasicBinaryExpression* expression)
				{
					return BinaryOperatorToString(expression->type);
				}

				WString BreakStatementToString()
				{
					return L"break";
				}

				WString ContinueStatementToString()
				{
					return L"continue";
				}

				BasicTypeRecord* GetIntegerType()
				{
					return analyzer->GetTypeManager()->GetPrimitiveType(int_type);
				}

				BasicTypeRecord* GetBooleanType()
				{
					return analyzer->GetTypeManager()->GetPrimitiveType(bool_type);
				}

			public:
				NativeXErrorMessageTranslator(BasicAnalyzer* _analyzer)
					:analyzer(_analyzer)
				{
				}
			};

			Ptr<BasicErrorMessageTranslator> CreateNativeXErrorMessageTranslator(BasicAnalyzer* analyzer)
			{
				return new NativeXErrorMessageTranslator(analyzer);
			}
		}
	}
}