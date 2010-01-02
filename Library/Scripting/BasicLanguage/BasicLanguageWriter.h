/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEWRITER
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEWRITER

#include "BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{

/***********************************************************************
Type
***********************************************************************/

			class BasicTypeNode : public Object
			{
			protected:
				Ptr<BasicType>							type;
			public:
				class ListNode
				{
				public:
					Ptr<collections::List<Ptr<BasicType>>>			types;

					ListNode&							operator<<(const BasicTypeNode& type);
				};

				BasicTypeNode(Ptr<BasicType> _type);
				Ptr<BasicType>							GetInternalValue()const;

				BasicTypeNode							operator*()const;
				BasicTypeNode							operator[](int size)const;
				BasicTypeNode							operator()(const ListNode& types)const;
			};

			extern BasicTypeNode						t_int8();
			extern BasicTypeNode						t_int16();
			extern BasicTypeNode						t_int32();
			extern BasicTypeNode						t_int64();
			extern BasicTypeNode						t_uint8();
			extern BasicTypeNode						t_uint16();
			extern BasicTypeNode						t_uint32();
			extern BasicTypeNode						t_uint64();
			extern BasicTypeNode						t_float();
			extern BasicTypeNode						t_double();
			extern BasicTypeNode						t_bool();
			extern BasicTypeNode						t_void();
			extern BasicTypeNode						t_type(const WString& type);
			extern BasicTypeNode::ListNode				t_types();

/***********************************************************************
Expression
***********************************************************************/

			class BasicExpressionNode : public Object
			{
			protected:
				Ptr<BasicExpression>					expression;
			public:
				class ListNode
				{
				public:
					Ptr<collections::List<Ptr<BasicExpression>>>	expressions;

					ListNode&							operator<<(const BasicExpressionNode& expression);
				};

				BasicExpressionNode(Ptr<BasicExpression> _expression);
				Ptr<BasicExpression>					GetInternalValue()const;

				BasicExpressionNode						operator++()const;
				BasicExpressionNode						operator--()const;
				BasicExpressionNode						operator++(int)const;
				BasicExpressionNode						operator--(int)const;
				BasicExpressionNode						Ref()const;
				BasicExpressionNode						operator*()const;
				BasicExpressionNode						operator-()const;
				BasicExpressionNode						operator!()const;

				BasicExpressionNode						operator+(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator-(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator*(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator/(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator%(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator<<(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator>>(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator&(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator|(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator^(const BasicExpressionNode& node)const;

				BasicExpressionNode						operator+=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator-=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator*=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator/=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator%=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator<<=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator>>=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator&=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator|=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator^=(const BasicExpressionNode& node)const;
				BasicExpressionNode						Assign(const BasicExpressionNode& node)const;

				BasicExpressionNode						operator<(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator<=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator>(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator>=(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator==(const BasicExpressionNode& node)const;
				BasicExpressionNode						operator!=(const BasicExpressionNode& node)const;

				BasicExpressionNode						Member(const WString& member)const;
				BasicExpressionNode						PMember(const WString& member)const;
				BasicExpressionNode						operator[](const BasicExpressionNode& subscribe)const;
				BasicExpressionNode						operator[](const BasicTypeNode& castType)const;
				BasicExpressionNode						operator()(const ListNode& arguments)const;
			};

			extern BasicExpressionNode					e_prim(unsigned __int8 value);
			extern BasicExpressionNode					e_prim(unsigned __int16 value);
			extern BasicExpressionNode					e_prim(unsigned __int32 value);
			extern BasicExpressionNode					e_prim(unsigned __int64 value);
			extern BasicExpressionNode					e_prim(signed __int8 value);
			extern BasicExpressionNode					e_prim(signed __int16 value);
			extern BasicExpressionNode					e_prim(signed __int32 value);
			extern BasicExpressionNode					e_prim(signed __int64 value);
			extern BasicExpressionNode					e_prim(float value);
			extern BasicExpressionNode					e_prim(double value);
			extern BasicExpressionNode					e_prim(bool value);
			extern BasicExpressionNode					e_prim(char value);
			extern BasicExpressionNode					e_prim(wchar_t value);
			extern BasicExpressionNode					e_prim(const WString& value);
			extern BasicExpressionNode					e_prim(const AString& value);
			extern BasicExpressionNode					e_name(const WString& name);
			extern BasicExpressionNode					e_result();
			extern BasicExpressionNode::ListNode		e_exps();

/***********************************************************************
Statement
***********************************************************************/

			class BasicStatementNode : public Object
			{
			protected:
				Ptr<BasicStatement>						statement;
			public:
				BasicStatementNode(Ptr<BasicStatement> _statement);
				Ptr<BasicStatement>						GetInternalValue()const;

				BasicStatementNode						operator<<(const BasicStatementNode& nextStatement)const;
			};

			extern BasicStatementNode					s_expr(const BasicExpressionNode& expression);
			extern BasicStatementNode					s_var(const BasicTypeNode& type, const WString& name);
			extern BasicStatementNode					s_var(const BasicTypeNode& type, const WString& name, const BasicExpressionNode& initializer);
			extern BasicStatementNode					s_if(const BasicExpressionNode& condition, const BasicStatementNode& trueStatement);
			extern BasicStatementNode					s_if(const BasicExpressionNode& condition, const BasicStatementNode& trueStatement, const BasicStatementNode& falseStatement);
			extern BasicStatementNode					s_while(const BasicExpressionNode& condition, const BasicStatementNode& statement);
			extern BasicStatementNode					s_do_while(const BasicExpressionNode& condition, const BasicStatementNode& statement);
			extern BasicStatementNode					s_break();
			extern BasicStatementNode					s_continue();
			extern BasicStatementNode					s_return();
			extern BasicStatementNode					s_empty();
			extern BasicStatementNode					s_for(const BasicStatementNode& initializer, const BasicExpressionNode& condition, const BasicStatementNode& sideEffect, const BasicStatementNode& statement);
		}
	}
}

#endif