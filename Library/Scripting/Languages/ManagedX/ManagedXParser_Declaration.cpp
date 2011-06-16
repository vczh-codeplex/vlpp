#include "ManagedXParser_Common.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
/***********************************************************************
Basic Declaration Fragments (Generic)
***********************************************************************/

			Ptr<ManagedGenericInfo::Argument> ToGenericArgument(const ParsingPair<
						ParsingPair<ManagedGenericInfo::ArgumentConversion, RegexToken>,
						ParsingList<ParsingList<Ptr<ManagedType>>>
					>& input)
			{
				Ptr<ManagedGenericInfo::Argument> argument=new ManagedGenericInfo::Argument;
				argument->newConstraint=false;
				argument->conversion=input.First().First();
				argument->name=WString(input.First().Second().reading, input.First().Second().length);
				if(input.Second().Head())
				{
					Ptr<ParsingList<Ptr<ManagedType>>::Node> type=input.Second().Head()->Value().Head();
					while(type)
					{
						if(type->Value())
						{
							argument->typeConstraints.Add(type->Value());
						}
						else
						{
							argument->newConstraint=true;
						}
						type=type->Next();
					}
				}
				return argument;
			}

			Ptr<ManagedGenericInfo> ToGenericInfo(const ParsingList<Ptr<ManagedGenericInfo::Argument>>& input)
			{
				Ptr<ManagedGenericInfo> info=new ManagedGenericInfo;
				Ptr<ParsingList<Ptr<ManagedGenericInfo::Argument>>::Node> current=input.Head();
				while(current)
				{
					info->arguments.Add(current->Value());
					current=current->Next();
				}
				return info;
			}
		}
	}

	namespace combinator
	{
		namespace x
		{
			using namespace scripting::managedlanguage;

			template<>
			struct ReferenceFiller<Ptr<ManagedGenericInfo>>
			{
				static void Fill(const ReferenceContainer<ManagedGenericInfo>& reference, const Ptr<ManagedGenericInfo>& value)
				{
					if(reference.reference)
					{
						CopyFrom(reference.reference->arguments.Wrap(), value->arguments.Wrap());
					}
				}
			};
		}
	}

	namespace scripting
	{
		namespace language_managedx
		{

/***********************************************************************
Basic Declaration Fragments (Attribute)
***********************************************************************/

			Ptr<ManagedNewObjectExpression> ToAttributeItem1(const x::tp<
					RegexToken,
					Ptr<ManagedType>
				>::ResultType& input)
			{
				Ptr<ManagedNewObjectExpression> exp=CreateNode<ManagedNewObjectExpression>(input.First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(exp->objectType)
					, input);
				return exp;
			}

			Ptr<ManagedNewObjectExpression> ToAttributeItem2(const x::tp<
					RegexToken,
					Ptr<ManagedType>,
					x::rep<Ptr<ManagedPropertySetter>>
				>::ResultType& input)
			{
				Ptr<ManagedNewObjectExpression> exp=CreateNode<ManagedNewObjectExpression>(input.First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(exp->objectType)
					.ref(exp->properties)
					, input);
				return exp;
			}

			Ptr<ManagedNewObjectExpression> ToAttributeItem3(const x::tp<
					RegexToken,
					Ptr<ManagedType>,
					x::rep<Ptr<ManagedArgument>>,
					x::rep<Ptr<ManagedPropertySetter>>
				>::ResultType& input)
			{
				Ptr<ManagedNewObjectExpression> exp=CreateNode<ManagedNewObjectExpression>(input.First().First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(exp->objectType)
					.ref(exp->arguments)
					.ref(exp->properties)
					, input);
				return exp;
			}

			Ptr<ManagedAttributeInfo> ToAttributeInfo(const ParsingList<Ptr<ManagedNewObjectExpression>>& input)
			{
				Ptr<ManagedAttributeInfo> info=new ManagedAttributeInfo;
				Ptr<ParsingList<Ptr<ManagedNewObjectExpression>>::Node> current=input.Head();
				while(current)
				{
					info->attributes.Add(current->Value());
					current=current->Next();
				}
				return info;
			}
		}
	}

	namespace combinator
	{
		namespace x
		{
			using namespace scripting::managedlanguage;

			template<>
			struct ReferenceFiller<Ptr<ManagedAttributeInfo>>
			{
				static void Fill(const ReferenceContainer<ManagedAttributeInfo>& reference, const Ptr<ManagedAttributeInfo>& value)
				{
					if(reference.reference)
					{
						CopyFrom(reference.reference->attributes.Wrap(), value->attributes.Wrap());
					}
				}
			};
		}
	}

	namespace scripting
	{
		namespace language_managedx
		{

/***********************************************************************
Basic Declaration Fragments (Parameter)
***********************************************************************/

			Ptr<ManagedParameter> ToParameter(const x::tp<
					Ptr<ManagedAttributeInfo>,
					ManagedParameter::ParameterType,
					Ptr<ManagedType>,
					RegexToken,
					ParsingList<Ptr<ManagedExpression>>
				>::ResultType& input)
			{
				Ptr<ManagedParameter> parameter=CreateNode<ManagedParameter>(input.First().Second());
				x::Fill(
					x::ref(parameter->attributeInfo)
					.ref(parameter->parameterType)
					.ref(parameter->type)
					.ref(parameter->name)
					.ref(parameter->defaultValue)
					, input);
				parameter->name=ConvertID(parameter->name);
				return parameter;
			}

/***********************************************************************
Basic Declaration Members
***********************************************************************/

			Ptr<ManagedMember> ToTypeMember(const Ptr<ManagedDeclaration>& input)
			{
				Ptr<ManagedTypeMember> member=CreateNode<ManagedTypeMember>(input);
				member->declaration=input;
				return member;
			}

			Ptr<ManagedMember> ToFieldMember(const x::tp<
					Ptr<ManagedAttributeInfo>,
					declatt::Accessor,
					declatt::MemberType,
					declatt::DataType,
					Ptr<ManagedType>,
					RegexToken,
					x::opt<Ptr<ManagedExpression>>
				>::ResultType& input)
			{
				Ptr<ManagedField> member=CreateNode<ManagedField>(input.First().Second());
				x::Fill(
					x::ref(member->attributeInfo)
					.ref(member->accessor)
					.ref(member->memberType)
					.ref(member->dataType)
					.ref(member->type)
					.ref(member->name)
					.ref(member->initializer)
					, input);
				member->name=ConvertID(member->name);
				return member;
			}

			Ptr<ManagedMember> ToMethodMember(const x::tp<
					Ptr<ManagedAttributeInfo>,
					ParsingList<Ptr<ManagedGenericInfo>>,
					declatt::Accessor,
					declatt::MemberType,
					declatt::Inheritation,
					bool,
					Ptr<ManagedType>,
					x::opt<Ptr<ManagedType>>,
					RegexToken,
					x::rep<Ptr<ManagedParameter>>,
					Ptr<ManagedStatement>
				>::ResultType& input)
			{
				Ptr<ManagedMethod> member=CreateNode<ManagedMethod>(input.First().First().Second());
				x::Fill(
					x::ref(member->attributeInfo)
					.ref(member->genericInfo)
					.ref(member->accessor)
					.ref(member->memberType)
					.ref(member->inheritation)
					.ref(member->externalMethod)
					.ref(member->returnType)
					.ref(member->implementedInterfaceType)
					.ref(member->name)
					.ref(member->parameters)
					.ref(member->body)
					, input);
				member->name=ConvertID(member->name);
				return member;
			}

			Ptr<ManagedMember> ToOperatorMethodMember(const x::tp<
					Ptr<ManagedAttributeInfo>,
					ParsingList<Ptr<ManagedGenericInfo>>,
					declatt::Accessor,
					declatt::MemberType,
					declatt::Inheritation,
					bool,
					Ptr<ManagedType>,
					x::opt<Ptr<ManagedType>>,
					ParsingPair<RegexToken, WString>,
					x::rep<Ptr<ManagedParameter>>,
					Ptr<ManagedStatement>
				>::ResultType& input)
			{
				Ptr<ManagedMethod> member=CreateNode<ManagedMethod>(input.First().First().Second().First());
				x::Fill(
					x::ref(member->attributeInfo)
					.ref(member->genericInfo)
					.ref(member->accessor)
					.ref(member->memberType)
					.ref(member->inheritation)
					.ref(member->externalMethod)
					.ref(member->returnType)
					.ref(member->implementedInterfaceType)
					.skip<ParsingPair<RegexToken, WString>>()
					.ref(member->parameters)
					.ref(member->body)
					, input);
				member->name=input.First().First().Second().Second();
				return member;
			}

			Ptr<ManagedMember> ToConstructorMember(const x::tp<
					Ptr<ManagedAttributeInfo>,
					declatt::Accessor,
					bool,
					RegexToken,
					x::rep<Ptr<ManagedParameter>>,
					x::opt<x::rep<Ptr<ManagedArgument>>>,
					Ptr<ManagedStatement>
				>::ResultType& input)
			{
				Ptr<ManagedConstructor> member=CreateNode<ManagedConstructor>(input.First().First().First().Second());
				x::Fill(
					x::ref(member->attributeInfo)
					.ref(member->accessor)
					.ref(member->implicit)
					.skip<RegexToken>()
					.ref(member->parameters)
					.ref(member->baseArguments)
					.ref(member->body)
					, input);
				return member;
			}

/***********************************************************************
Extended Declaration Members
***********************************************************************/

			Ptr<ManagedMember> ToPropertyMember(const x::tp<
				Ptr<ManagedAttributeInfo>,
				declatt::Accessor,
				declatt::MemberType,
				declatt::Inheritation,
				Ptr<ManagedType>,
				x::opt<Ptr<ManagedType>>,
				RegexToken,
				x::tp<
					x::opt<Ptr<ManagedStatement>>,
					x::opt<x::tp<x::opt<declatt::Accessor>, x::opt<RegexToken>, Ptr<ManagedStatement>>>
					>
				>::ResultType& input)
			{
				Ptr<ManagedProperty> member=CreateNode<ManagedProperty>(input.First().Second());
				x::Fill(
					x::ref(member->attributeInfo)
					.ref(member->accessor)
					.ref(member->memberType)
					.ref(member->inheritation)
					.ref(member->type)
					.ref(member->implementedInterfaceType)
					.ref(member->name)
					, input.First());
				member->setterAccessor=member->accessor;
				x::Fill(
					x::ref(member->getter)
					.ref(
						x::ref(member->setterAccessor=member->accessor)
						.ref(member->setterParameter)
						.ref(member->setter)
					)
					, input.Second());
				member->name=ConvertID(member->name);
				return member;
			}

			Ptr<ManagedMember> ToConverterOperatorMember(const x::tp<
					Ptr<ManagedAttributeInfo>,
					ParsingList<Ptr<ManagedGenericInfo>>,
					declatt::Accessor,
					declatt::MemberType,
					declatt::Inheritation,
					bool,
					Ptr<ManagedType>,
					Ptr<ManagedStatement>
				>::ResultType& input)
			{
				Ptr<ManagedConverterOperator> member=CreateNode<ManagedConverterOperator>(input.First().Second());
				x::Fill(
					x::ref(member->attributeInfo)
					.ref(member->genericInfo)
					.ref(member->accessor)
					.ref(member->memberType)
					.ref(member->inheritation)
					.ref(member->implicit)
					.ref(member->targetType)
					.ref(member->body)
					, input);
				return member;
			}

/***********************************************************************
Basic Declarations
***********************************************************************/

			Ptr<ManagedDeclaration> ToTypeDecl(const x::tp<
					Ptr<ManagedAttributeInfo>,
					ParsingList<Ptr<ManagedGenericInfo>>,
					declatt::Accessor,
					declatt::Inheritation,
					ManagedTypeDeclaration::DeclarationType,
					RegexToken,
					x::rep<Ptr<ManagedType>>,
					x::rep<Ptr<ManagedMember>>
				>::ResultType& input)
			{
				Ptr<ManagedTypeDeclaration> decl=CreateNode<ManagedTypeDeclaration>(input.First().First().Second());
				x::Fill(
					x::ref(decl->attributeInfo)
					.ref(decl->genericInfo)
					.ref(decl->accessor)
					.ref(decl->inheritation)
					.ref(decl->declarationType)
					.ref(decl->name)
					.ref(decl->baseTypes)
					.ref(decl->members)
					, input);
				return decl;
			}

			Ptr<ManagedDeclaration> ToNamespaceDecl(const x::tp<
				RegexToken,
				x::rep<RegexToken>,
				x::rep<Ptr<ManagedDeclaration>>
				>::ResultType& input)
			{
				Ptr<ManagedNamespaceDeclaration> decl=CreateNode<ManagedNamespaceDeclaration>(input.First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(decl->namespaceFragments)
					.ref(decl->declarations)
					, input);
				for(vint i=0;i<decl->namespaceFragments.Count();i++)
				{
					decl->namespaceFragments[i]=ConvertID(decl->namespaceFragments[i]);
				}
				return decl;
			}

/***********************************************************************
Extended Declarations
***********************************************************************/

			Ptr<ManagedEnumItem> ToEnumItem(const x::tp<
				Ptr<ManagedAttributeInfo>,
				RegexToken,
				x::opt<Ptr<ManagedExpression>>
				>::ResultType& input)
			{
				Ptr<ManagedEnumItem> item=CreateNode<ManagedEnumItem>(input.First().Second());
				x::Fill(
					x::ref(item->attributeInfo)
					.ref(item->name)
					.ref(item->value)
					, input);
				item->name=ConvertID(item->name);
				return item;
			}

			Ptr<ManagedDeclaration> ToEnum(const x::tp<
				Ptr<ManagedAttributeInfo>,
				declatt::Accessor,
				RegexToken,
				x::opt<RegexToken>,
				RegexToken,
				x::rep<Ptr<ManagedEnumItem>>
				>::ResultType& input)
			{
				Ptr<ManagedEnumerationDeclaration> decl=CreateNode<ManagedEnumerationDeclaration>(input.First().First().First().Second());
				x::Fill(
					x::ref(decl->attributeInfo)
					.ref(decl->accessor)
					.skip<RegexToken>()
					.ref(decl->composable)
					.ref(decl->name)
					.ref(decl->items)
					, input);
				decl->name=ConvertID(decl->name);
				return decl;
			}

			Ptr<ManagedDeclaration> ToTypeRenameDecl(const x::tp<
					ParsingList<Ptr<ManagedGenericInfo>>, 
					declatt::Accessor,
					RegexToken,
					Ptr<ManagedType>
				>::ResultType& input)
			{
				Ptr<ManagedTypeRenameDeclaration> decl=CreateNode<ManagedTypeRenameDeclaration>(input.First().Second());
				x::Fill(
					x::ref(decl->genericInfo)
					.ref(decl->accessor)
					.ref(decl->name)
					.ref(decl->type)
					, input);
				decl->name=ConvertID(decl->name);
				return decl;
			}

			Ptr<ManagedDeclaration> ToUsingNamespaceDecl(const x::tp<
				RegexToken,
				x::rep<RegexToken>
				>::ResultType& input)
			{
				Ptr<ManagedUsingNamespaceDeclaration> decl=CreateNode<ManagedUsingNamespaceDeclaration>(input.First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(decl->namespaceFragments)
					, input);
				for(vint i=0;i<decl->namespaceFragments.Count();i++)
				{
					decl->namespaceFragments[i]=ConvertID(decl->namespaceFragments[i]);
				}
				return decl;
			}

/***********************************************************************
Unit
***********************************************************************/

			Ptr<ManagedXUnit> ToUnit(const ParsingList<Ptr<ManagedDeclaration>>& input)
			{
				Ptr<ManagedProgram> program=new ManagedProgram;
				Ptr<ParsingList<Ptr<ManagedDeclaration>>::Node> current=input.Head();
				while(current)
				{
					program->declarations.Add(current->Value());
					current=current->Next();
				}
				
				Ptr<ManagedXUnit> unit=new ManagedXUnit;
				unit->program=program;
				unit->codeIndex=-1;
				return unit;
			}

/***********************************************************************
ManagedXParserImpl
***********************************************************************/

			void ManagedXParserImpl::LoadDeclarations()
			{
				/*--------DECLARATION FRAGMENTS--------*/

				genericTypeConstraint	= type | let(NEW + OPEN_EXP_BRACE(NeedOpenExpBrace) + CLOSE_EXP_BRACE(NeedCloseExpBrace), Ptr<ManagedType>());

				genericArgument			= ((genericArgconv + ID(NeedId)) + opt(COLON(NeedColon) >> (
											plist(genericTypeConstraint + *(COMMA >> genericTypeConstraint))
										)))[ToGenericArgument];

				genericInfo				= (GENERIC + LT(NeedLt) >> (
											plist(genericArgument + *(COMMA >> genericArgument))
										) << GT(NeedGt))[ToGenericInfo];

				attributeItem1			= ((OPEN_ARRAY_BRACE + type) << CLOSE_ARRAY_BRACE(NeedCloseArrayBrace))[ToAttributeItem1];

				attributeItem2			= ((
											OPEN_ARRAY_BRACE +
											type +
											(OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(propertySetter + *(COMMA >> propertySetter))) << CLOSE_EXP_BRACE(NeedCloseExpBrace))
											) << CLOSE_ARRAY_BRACE(NeedCloseExpBrace)
											)[ToAttributeItem2];

				attributeItem3			= ((
											OPEN_ARRAY_BRACE +
											type +
											(OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(argument + *(COMMA >> argument))) +
											(*(COMMA >> propertySetter) << CLOSE_EXP_BRACE(NeedCloseExpBrace))
											) << CLOSE_ARRAY_BRACE(NeedCloseArrayBrace)
											)[ToAttributeItem3];

				attributeInfo			= (*(attributeItem1 | attributeItem2 | attributeItem3))[ToAttributeInfo];

				parameter				= (attributeInfo + functionArgconv + type+ ID(NeedId) + opt(EQ >> expression))[ToParameter];

				/*--------DECLARATION MEMBERS--------*/

				member					= ((attributeInfo + accessor + memberType + dataType + type + ID(NeedId) + opt(EQ >> expression)) << SEMICOLON(NeedSemicolon))[ToFieldMember]
										| (attributeInfo + accessor + memberType + inheritation + type + opt(type << COLON(NeedColon) << COLON(NeedColon)) + ID(NeedId) + 
											(OPEN_DECL_BRACE(NeedOpenDeclBrace) >> (
												opt(GET >> statement) +
												opt(opt(setterAccessor) + (SET >> opt(OPEN_EXP_BRACE(NeedOpenExpBrace) >> ID(NeedId) << CLOSE_EXP_BRACE(NeedCloseExpBrace))) + statement)
											) << CLOSE_DECL_BRACE(NeedCloseDeclBrace))
										  )[ToPropertyMember]
										| (attributeInfo + opt(genericInfo) + accessor + memberType + inheritation + implicitExplicit + (AS(NeedAs) >> type)
											+ (OPEN_EXP_BRACE(NeedOpenExpBrace) >> CLOSE_EXP_BRACE(NeedCloseExpBrace) >> statement)
										  )[ToConverterOperatorMember]
										| (attributeInfo + opt(genericInfo) + accessor + memberType + inheritation + internalExternal +
											type + 
											opt(type << COLON(NeedColon) << COLON(NeedColon)) + ID(NeedId) +
											(OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(parameter + *(COMMA >> parameter))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)) +
											statement
										  )[ToMethodMember]
										| (attributeInfo + opt(genericInfo) + accessor + memberType + inheritation + internalExternal +
											type + 
											opt(type << COLON(NeedColon) << COLON(NeedColon)) + (OPERATOR + operatorNode(NeedOperator)) +
											(OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(parameter + *(COMMA >> parameter))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)) +
											statement
										  )[ToOperatorMethodMember]
										| (attributeInfo + accessor + implicitExplicit + CONSTRUCTOR + 
											(OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(parameter + *(COMMA >> parameter))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)) +
											opt(COLON >> BASE(NeedBase) >>(OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(argument + *(COMMA >> argument))) << CLOSE_EXP_BRACE(NeedCloseExpBrace))) +
											statement
										  )[ToConstructorMember]
										| declaration[ToTypeMember]
										;

				/*--------DECLARATIONS--------*/

				enumItem				= (attributeInfo + ID(NeedId) + opt(EQ >> expression))[ToEnumItem];

				declaration				= ((USING + plist(ID(NeedId) + *(DOT >> ID(NeedId))) << SEMICOLON(NeedSemicolon)))[ToUsingNamespaceDecl]
										| ((opt(genericInfo) + accessor + (USING >> ID(NeedId)) + (EQ(NeedEq) >> type)) << SEMICOLON(NeedSemicolon))[ToTypeRenameDecl]
										| (NAMESPACE + plist(ID(NeedId) + *(DOT >> ID(NeedId))) + (OPEN_DECL_BRACE(NeedOpenDeclBrace) >> *declaration << CLOSE_DECL_BRACE(NeedCloseDeclBrace)))[ToNamespaceDecl]
										| (
											attributeInfo +
											opt(genericInfo) +
											accessor +
											inheritation +
											(let(CLASS, ManagedTypeDeclaration::Class) | let(STRUCT, ManagedTypeDeclaration::Structure) | let(INTERFACE, ManagedTypeDeclaration::Interface)) +
											ID(NeedId) +
											plist(opt(COLON >> (type + *(COMMA >> type)))) +
											(OPEN_DECL_BRACE(NeedOpenDeclBrace) >> *member << CLOSE_DECL_BRACE(NeedCloseDeclBrace))
										  )[ToTypeDecl]
										| (attributeInfo + accessor + ENUM + opt(SWITCH) + ID + (
											OPEN_DECL_BRACE(NeedOpenDeclBrace) >> plist(opt(enumItem + *(COMMA >> enumItem))) << CLOSE_DECL_BRACE(NeedCloseDeclBrace)
											))[ToEnum]
										;
				unit					= (*declaration)[ToUnit];
			}
		}
	}
}