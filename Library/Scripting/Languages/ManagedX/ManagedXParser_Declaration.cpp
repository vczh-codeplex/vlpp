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

			void CopyGenericInfo(ManagedGenericInfo& dst, Ptr<ManagedGenericInfo> src)
			{
				CopyFrom(dst.arguments.Wrap(), src->arguments.Wrap());
			}

/***********************************************************************
Basic Declaration Fragments (Attribute)
***********************************************************************/

			Ptr<ManagedNewObjectExpression> ToAttributeItem1(const ParsingPair<
				RegexToken,
				Ptr<ManagedType>>& input)
			{
				Ptr<ManagedNewObjectExpression> exp=CreateNode<ManagedNewObjectExpression>(input.First());
				exp->objectType=input.Second();
				return exp;
			}

			Ptr<ManagedNewObjectExpression> ToAttributeItem2(const ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedType>>,
				ParsingList<Ptr<ManagedPropertySetter>>>& input)
			{
				Ptr<ManagedNewObjectExpression> exp=CreateNode<ManagedNewObjectExpression>(input.First().First());
				exp->objectType=input.First().Second();
				{
					Ptr<ParsingList<Ptr<ManagedPropertySetter>>::Node> current=input.Second().Head();
					while(current)
					{
						exp->properties.Add(current->Value());
						current=current->Next();
					}
				}
				return exp;
			}

			Ptr<ManagedNewObjectExpression> ToAttributeItem3(const ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedType>>,
				ParsingList<Ptr<ManagedArgument>>>,
				ParsingList<Ptr<ManagedPropertySetter>>>& input)
			{
				Ptr<ManagedNewObjectExpression> exp=CreateNode<ManagedNewObjectExpression>(input.First().First().First());
				exp->objectType=input.First().First().Second();
				{
					Ptr<ParsingList<Ptr<ManagedArgument>>::Node> current=input.First().Second().Head();
					while(current)
					{
						exp->arguments.Add(current->Value());
						current=current->Next();
					}
				}
				{
					Ptr<ParsingList<Ptr<ManagedPropertySetter>>::Node> current=input.Second().Head();
					while(current)
					{
						exp->properties.Add(current->Value());
						current=current->Next();
					}
				}
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

			void CopyAttributeInfo(ManagedAttributeInfo& dst, Ptr<ManagedAttributeInfo> src)
			{
				CopyFrom(dst.attributes.Wrap(), src->attributes.Wrap());
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

			Ptr<ManagedMember> ToFieldMember(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				Ptr<ManagedAttributeInfo>,
				declatt::Accessor>,
				declatt::MemberType>,
				declatt::DataType>,
				Ptr<ManagedType>>,
				RegexToken>,
				ParsingList<Ptr<ManagedExpression>>>& input)
			{
				Ptr<ManagedField> member=CreateNode<ManagedField>(input.First().Second());
				CopyAttributeInfo(member->attributeInfo, input.First().First().First().First().First().First());
				member->accessor=input.First().First().First().First().First().Second();
				member->memberType=input.First().First().First().First().Second();
				member->dataType=input.First().First().First().Second();
				member->type=input.First().First().Second();
				member->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));
				if(input.Second().Head())
				{
					member->initializer=input.Second().Head()->Value();
				}
				return member;
			}

/***********************************************************************
Extended Declaration Members
***********************************************************************/

			Ptr<ManagedMember> ToPropertyMember(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				Ptr<ManagedAttributeInfo>,
				declatt::Accessor>,
				declatt::MemberType>,
				declatt::Inheritation>,
				Ptr<ManagedType>>,
				ParsingList<Ptr<ManagedType>>>,
				RegexToken>,
				ParsingPair<
					ParsingList<Ptr<ManagedStatement>>,
					ParsingList<ParsingPair<ParsingList<declatt::Accessor>, Ptr<ManagedStatement>>>
					>>& input)
			{
				Ptr<ManagedProperty> member=CreateNode<ManagedProperty>(input.First().Second());
				CopyAttributeInfo(member->attributeInfo, input.First().First().First().First().First().First().First());
				member->accessor=input.First().First().First().First().First().First().Second();
				member->memberType=input.First().First().First().First().First().Second();
				member->inheritation=input.First().First().First().First().Second();
				member->type=input.First().First().First().Second();
				if(input.First().First().Second().Head())
				{
					member->implementedInterfaceType=input.First().First().Second().Head()->Value();
				}
				member->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));

				member->setterAccessor=member->accessor;
				if(input.Second().First().Head())
				{
					member->getter=input.Second().First().Head()->Value();
				}
				if(input.Second().Second().Head())
				{
					if(input.Second().Second().Head()->Value().First().Head())
					{
						member->setterAccessor=input.Second().Second().Head()->Value().First().Head()->Value();
					}
					member->setter=input.Second().Second().Head()->Value().Second();
				}
				return member;
			}

			Ptr<ManagedMember> ToConverterOperatorMember(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				Ptr<ManagedAttributeInfo>,
				ParsingList<Ptr<ManagedGenericInfo>>>,
				declatt::Accessor>,
				declatt::MemberType>,
				declatt::Inheritation>,
				bool>,
				Ptr<ManagedType>>,
				Ptr<ManagedStatement>>& input)
			{
				Ptr<ManagedConverterOperator> member=CreateNode<ManagedConverterOperator>(input.First().Second());
				CopyAttributeInfo(member->attributeInfo, input.First().First().First().First().First().First().First());
				if(input.First().First().First().First().First().First().Second().Head())
				{
					CopyGenericInfo(member->genericInfo, input.First().First().First().First().First().First().Second().Head()->Value());
				}
				member->accessor=input.First().First().First().First().First().Second();
				member->memberType=input.First().First().First().First().Second();
				member->inheritation=input.First().First().First().Second();
				member->implicit=input.First().First().Second();
				member->targetType=input.First().Second();
				member->body=input.Second();
				return member;
			}

/***********************************************************************
Basic Declarations
***********************************************************************/

			Ptr<ManagedDeclaration> ToTypeDecl(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				Ptr<ManagedAttributeInfo>,
				ParsingList<Ptr<ManagedGenericInfo>>>,
				declatt::Accessor>,
				declatt::Inheritation>,
				ManagedTypeDeclaration::DeclarationType>,
				RegexToken>,
				ParsingList<Ptr<ManagedType>>>,
				ParsingList<Ptr<ManagedMember>>>& input)
			{
				Ptr<ManagedTypeDeclaration> decl=CreateNode<ManagedTypeDeclaration>(input.First().First().Second());
				CopyAttributeInfo(decl->attributeInfo, input.First().First().First().First().First().First().First());
				if(input.First().First().First().First().First().First().Second().Head())
				{
					CopyGenericInfo(decl->genericInfo, input.First().First().First().First().First().First().Second().Head()->Value());
				}
				decl->accessor=input.First().First().First().First().First().Second();
				decl->inheritation=input.First().First().First().First().Second();
				decl->declarationType=input.First().First().First().Second();
				decl->name=ConvertID(WString(input.First().First().Second().reading, input.First().First().Second().length));
				{
					Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.First().Second().Head();
					while(current)
					{
						decl->baseTypes.Add(current->Value());
						current=current->Next();
					}
				}
				{
					Ptr<ParsingList<Ptr<ManagedMember>>::Node> current=input.Second().Head();
					while(current)
					{
						decl->members.Add(current->Value());
						current=current->Next();
					}
				}
				return decl;
			}

			Ptr<ManagedDeclaration> ToNamespaceDecl(const ParsingPair<ParsingPair<RegexToken, ParsingList<RegexToken>>, ParsingList<Ptr<ManagedDeclaration>>>& input)
			{
				Ptr<ManagedNamespaceDeclaration> decl=CreateNode<ManagedNamespaceDeclaration>(input.First().First());
				{
					Ptr<ParsingList<RegexToken>::Node> current=input.First().Second().Head();
					while(current)
					{
						decl->namespaceFragments.Add(WString(current->Value().reading, current->Value().length));
						current=current->Next();
					}
				}
				{
					Ptr<ParsingList<Ptr<ManagedDeclaration>>::Node> current=input.Second().Head();
					while(current)
					{
						decl->declarations.Add(current->Value());
						current=current->Next();
					}
				}
				return decl;
			}

/***********************************************************************
Extended Declarations
***********************************************************************/

			Ptr<ManagedEnumItem> ToEnumItem(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedExpression>>>& input)
			{
				Ptr<ManagedEnumItem> item=CreateNode<ManagedEnumItem>(input.First());
				item->name=ConvertID(WString(input.First().reading, input.First().length));
				if(input.Second().Head())
				{
					item->value=input.Second().Head()->Value();
				}
				return item;
			}

			Ptr<ManagedDeclaration> ToEnum(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				Ptr<ManagedAttributeInfo>,
				declatt::Accessor>,
				RegexToken>,
				ParsingList<RegexToken>>,
				RegexToken>,
				ParsingList<Ptr<ManagedEnumItem>>>& input)
			{
				Ptr<ManagedEnumerationDeclaration> decl=CreateNode<ManagedEnumerationDeclaration>(input.First().First().First().Second());
				CopyAttributeInfo(decl->attributeInfo, input.First().First().First().First().First());
				decl->accessor=input.First().First().First().First().Second();
				decl->composable=input.First().First().Second().Head();
				decl->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));

				Ptr<ParsingList<Ptr<ManagedEnumItem>>::Node> current=input.Second().Head();
				while(current)
				{
					decl->items.Add(current->Value());
					current=current->Next();
				}
				return decl;
			}

			Ptr<ManagedDeclaration> ToTypeRenameDecl(const ParsingPair<ParsingPair<ParsingPair<
				ParsingList<Ptr<ManagedGenericInfo>>, 
				declatt::Accessor>, 
				RegexToken>, 
				Ptr<ManagedType>>& input)
			{
				Ptr<ManagedTypeRenameDeclaration> decl=CreateNode<ManagedTypeRenameDeclaration>(input.First().Second());
				if(input.First().First().First().Head())
				{
					Ptr<ManagedGenericInfo> genericInfo=input.First().First().First().Head()->Value();
					CopyGenericInfo(decl->genericInfo, genericInfo);
				}
				decl->accessor=input.First().First().Second();
				decl->name=WString(input.First().Second().reading, input.First().Second().length);
				decl->type=input.Second();
				return decl;
			}

			Ptr<ManagedDeclaration> ToUsingNamespaceDecl(const ParsingPair<RegexToken, ParsingList<RegexToken>>& input)
			{
				Ptr<ManagedUsingNamespaceDeclaration> decl=CreateNode<ManagedUsingNamespaceDeclaration>(input.First());
				Ptr<ParsingList<RegexToken>::Node> current=input.Second().Head();
				while(current)
				{
					decl->namespaceFragments.Add(WString(current->Value().reading, current->Value().length));
					current=current->Next();
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
											OPEN_DECL_BRACE(NeedOpenDeclBrace) >> plist(genericTypeConstraint + *(COMMA >> genericTypeConstraint)) << CLOSE_DECL_BRACE(NeedCloseDeclBrace)
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

				/*--------DECLARATION MEMBERS--------*/

				member					= ((attributeInfo + accessor + memberType + dataType + type + ID(NeedId) + opt(EQ >> expression)) << SEMICOLON(NeedSemicolon))[ToFieldMember]
										| (attributeInfo + accessor + memberType + inheritation + type + opt(type << COLON(NeedColon) << COLON(NeedColon)) + ID(NeedId) + 
											(OPEN_DECL_BRACE(NeedOpenDeclBrace) >> (
												opt(GET >> statement) +
												opt(opt(setterAccessor) + (SET >> statement))
											) << CLOSE_DECL_BRACE(NeedCloseDeclBrace))
										  )[ToPropertyMember]
										| (attributeInfo + opt(genericInfo) + accessor + memberType + inheritation + implicitExplicit + (AS(NeedAs) >> type)
											+ (OPEN_EXP_BRACE(NeedOpenExpBrace) >> CLOSE_EXP_BRACE(NeedCloseExpBrace) >> statement)
										  )[ToConverterOperatorMember]
										| declaration[ToTypeMember]
										;

				/*--------DECLARATIONS--------*/

				enumItem				= (ID(NeedId) + opt(EQ >> expression))[ToEnumItem];

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