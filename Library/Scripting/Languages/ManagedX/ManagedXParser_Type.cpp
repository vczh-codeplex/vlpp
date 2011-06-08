#include "ManagedXParser_Common.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{

/***********************************************************************
Basic Types
***********************************************************************/

			Ptr<ManagedType> ToKeywordType(const RegexToken& input)
			{
				WString name;
				WString keyword(input.reading, input.length);
				if(keyword==L"sbyte")			name = L"SInt8";
				else if(keyword==L"byte")		name = L"UInt8";
				else if(keyword==L"short")		name = L"SInt16";
				else if(keyword==L"word")		name = L"UInt16";
				else if(keyword==L"int")		name = L"SInt32";
				else if(keyword==L"uint")		name = L"UInt32";
				else if(keyword==L"long")		name = L"SInt64";
				else if(keyword==L"ulong")		name = L"UInt64";
				else if(keyword==L"char")		name = L"Char";
				else if(keyword==L"string")		name = L"String";
				else if(keyword==L"float")		name = L"Single";
				else if(keyword==L"double")		name = L"Double";
				else if(keyword==L"bool")		name = L"Bool";
				else if(keyword==L"object")		name = L"Object";
				else if(keyword==L"void")		name = L"Void";
#ifdef VCZH_64
				else if(keyword==L"intptr")		name = L"SInt64";
				else if(keyword==L"uintptr")	name = L"UInt64";
#else
				else if(keyword==L"intptr")		name = L"SInt32";
				else if(keyword==L"uintptr")	name = L"UInt32";
#endif

				Ptr<ManagedMemberType> system=CreateNode<ManagedMemberType>(input);
				system->member=L"System";

				Ptr<ManagedMemberType> type=CreateNode<ManagedMemberType>(input);
				type->operand=system;
				type->member=name;
				return type;
			}

			Ptr<ManagedType> ToReferenceType(const RegexToken& input)
			{
				Ptr<ManagedReferencedType> type=CreateNode<ManagedReferencedType>(input);
				type->name=ConvertID(WString(input.reading, input.length));
				return type;
			}

			Ptr<ManagedType> ToGlobalType(const RegexToken& input)
			{
				Ptr<ManagedMemberType> type=CreateNode<ManagedMemberType>(input);
				type->member=WString(input.reading, input.length);
				return type;
			}

			Ptr<ManagedType> ToLrecType(const Ptr<ManagedType>& elementType, const Ptr<typelrec::TypeLrecBase>& decoratorType)
			{
				return decoratorType->Set(elementType);
			}

			Ptr<typelrec::TypeLrecBase> ToMemberTypeLrec(const RegexToken& input)
			{
				Ptr<ManagedMemberType> type=CreateNode<ManagedMemberType>(input);
				type->member=ConvertID(WString(input.reading, input.length));
				return typelrec::ToTypeLrec(type);
			}

			Ptr<typelrec::TypeLrecBase> ToInstantiatedGenericTypeLrec(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedType>>>& input)
			{
				Ptr<ManagedInstantiatedGenericType> type=CreateNode<ManagedInstantiatedGenericType>(input.First());
				Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.Second().Head();
				while(current)
				{
					type->argumentTypes.Add(current->Value());
					current=current->Next();
				}
				return typelrec::ToTypeLrec(type);
			}

/***********************************************************************
Extended Types
***********************************************************************/

			Ptr<typelrec::TypeLrecBase> ToArrayType(const ParsingPair<RegexToken, ParsingList<RegexToken>>& input)
			{
				Ptr<ManagedArrayType> type=CreateNode<ManagedArrayType>(input.First());
				Ptr<ParsingList<RegexToken>::Node> current=input.Second().Head();
				int count=1;
				while(current)
				{
					count++;
					current=current->Next();
				}
				type->dimensionCount=count;
				return typelrec::ToTypeLrec(type);
			}

			Ptr<ManagedType> ToFunctionType(const ParsingPair<ParsingPair<RegexToken, Ptr<ManagedType>>, ParsingList<Ptr<ManagedType>>>& input)
			{
				Ptr<ManagedFunctionType> type=CreateNode<ManagedFunctionType>(input.First().First());
				type->returnType=input.First().Second();
				Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.Second().Head();
				while(current)
				{
					type->parameterTypes.Add(current->Value());
					current=current->Next();
				}
				return type;
			}

			Ptr<ManagedType> ToEventType(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedType>>>& input)
			{
				Ptr<ManagedEventType> type=CreateNode<ManagedEventType>(input.First());
				Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.Second().Head();
				while(current)
				{
					type->parameterTypes.Add(current->Value());
					current=current->Next();
				}
				return type;
			}

			Ptr<ManagedType> ToAutoReferType(const RegexToken& input)
			{
				return CreateNode<ManagedAutoReferType>(input);
			}

			Ptr<ManagedType> ToDynamicType(const RegexToken& input)
			{
				return CreateNode<ManagedDynamicType>(input);
			}

/***********************************************************************
ManagedXParserImpl
***********************************************************************/

			void ManagedXParserImpl::LoadTypes()
			{
				/*--------TYPES--------*/

				primitiveType			= TYPEKEYWORD[ToKeywordType]
										| ID(NeedId)[ToReferenceType]
										| VAR[ToAutoReferType]
										| DYNAMIC[ToDynamicType]
										| (FUNCTION + type + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(type + *(COMMA >> type))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)))[ToFunctionType]
										| (EVENT + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(type + *(COMMA >> type))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)))[ToEventType]
										| ((GLOBAL + COLON(NeedColon) + COLON(NeedColon)) >> ID(NeedId))[ToGlobalType]
										;

				type					= lrec(primitiveType + *( (DOT >> ID)[ToMemberTypeLrec]
																| ((LT + plist(type + *(COMMA >> type))) << GT(NeedGt))[ToInstantiatedGenericTypeLrec]
																| ((OPEN_ARRAY_BRACE + *COMMA) << CLOSE_ARRAY_BRACE(NeedCloseArrayBrace))[ToArrayType]
																), ToLrecType);
			}
		}
	}
}