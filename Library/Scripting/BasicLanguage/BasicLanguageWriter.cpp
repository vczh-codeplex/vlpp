#include "BasicLanguageWriter.h"
#include "..\..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
BasicTypeNode
***********************************************************************/

			BasicTypeNode::ListNode& BasicTypeNode::ListNode::operator<<(const BasicTypeNode& type)
			{
				types->Add(type.GetInternalValue());
				return *this;
			}

			BasicTypeNode::BasicTypeNode(Ptr<BasicType> _type)
				:type(_type)
			{
			}

			Ptr<BasicType> BasicTypeNode::GetInternalValue()const
			{
				return type;
			}

			BasicTypeNode BasicTypeNode::operator*()const
			{
				Ptr<BasicPointerType> result=new BasicPointerType;
				result->elementType=type;
				return result;
			}

			BasicTypeNode BasicTypeNode::operator[](vint size)const
			{
				Ptr<BasicArrayType> result=new BasicArrayType;
				result->elementType=type;
				result->count=e_prim(size).GetInternalValue();
				return result;
			}

			BasicTypeNode BasicTypeNode::operator[](const ListNode& types)const
			{
				Ptr<BasicInstantiatedGenericType> result=new BasicInstantiatedGenericType;
				result->elementType=type;
				CopyFrom(result->argumentTypes.Wrap(), types.types->Wrap());
				return result;
			}

			BasicTypeNode BasicTypeNode::operator()(const ListNode& types)const
			{
				Ptr<BasicFunctionType> result=new BasicFunctionType;
				result->returnType=type;
				CopyFrom(result->parameterTypes.Wrap(), types.types->Wrap());
				return result;
			}

			BasicTypeNode t_int8()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=s8;
				return result;
			}

			BasicTypeNode t_int16()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=s16;
				return result;
			}

			BasicTypeNode t_int32()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=s32;
				return result;
			}

			BasicTypeNode t_int64()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=s64;
				return result;
			}

			BasicTypeNode t_int()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=int_type;
				return result;
			}

			BasicTypeNode t_uint8()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=u8;
				return result;
			}

			BasicTypeNode t_uint16()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=u16;
				return result;
			}

			BasicTypeNode t_uint32()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=u32;
				return result;
			}

			BasicTypeNode t_uint()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=uint_type;
				return result;
			}

			BasicTypeNode t_float()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=f32;
				return result;
			}

			BasicTypeNode t_double()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=f64;
				return result;
			}

			BasicTypeNode t_bool()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=bool_type;
				return result;
			}

			BasicTypeNode t_char()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=char_type;
				return result;
			}

			BasicTypeNode t_wchar()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=wchar_type;
				return result;
			}

			BasicTypeNode t_void()
			{
				Ptr<BasicPrimitiveType> result=new BasicPrimitiveType;
				result->type=void_type;
				return result;
			}

			BasicTypeNode t_type(const WString& type)
			{
				Ptr<BasicReferenceType> result=new BasicReferenceType;
				result->name=type;
				return result;
			}

			BasicTypeNode::ListNode t_types()
			{
				BasicTypeNode::ListNode node;
				node.types=new List<Ptr<BasicType>>;
				return node;
			}

/***********************************************************************
BasicExpressionNode
***********************************************************************/
			
			BasicExpressionNode::ListNode& BasicExpressionNode::ListNode::operator<<(const BasicExpressionNode& expression)
			{
				expressions->Add(expression.GetInternalValue());
				return *this;
			}

			BasicExpressionNode::BasicExpressionNode(Ptr<BasicExpression> _expression)
				:expression(_expression)
			{
			}

			Ptr<BasicExpression> BasicExpressionNode::GetInternalValue()const
			{
				return expression;
			}

			BasicExpressionNode BasicExpressionNode::operator++()const
			{
				Ptr<BasicUnaryExpression> result=new BasicUnaryExpression;
				result->operand=expression;
				result->type=BasicUnaryExpression::PrefixIncrease;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator--()const
			{
				Ptr<BasicUnaryExpression> result=new BasicUnaryExpression;
				result->operand=expression;
				result->type=BasicUnaryExpression::PrefixDecrease;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator++(int)const
			{
				Ptr<BasicUnaryExpression> result=new BasicUnaryExpression;
				result->operand=expression;
				result->type=BasicUnaryExpression::PostfixIncrease;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator--(int)const
			{
				Ptr<BasicUnaryExpression> result=new BasicUnaryExpression;
				result->operand=expression;
				result->type=BasicUnaryExpression::PostfixDecrease;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::Ref()const
			{
				Ptr<BasicUnaryExpression> result=new BasicUnaryExpression;
				result->operand=expression;
				result->type=BasicUnaryExpression::GetAddress;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator*()const
			{
				Ptr<BasicUnaryExpression> result=new BasicUnaryExpression;
				result->operand=expression;
				result->type=BasicUnaryExpression::DereferencePointer;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator-()const
			{
				Ptr<BasicUnaryExpression> result=new BasicUnaryExpression;
				result->operand=expression;
				result->type=BasicUnaryExpression::Negative;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator!()const
			{
				Ptr<BasicUnaryExpression> result=new BasicUnaryExpression;
				result->operand=expression;
				result->type=BasicUnaryExpression::Not;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator~()const
			{
				Ptr<BasicUnaryExpression> result=new BasicUnaryExpression;
				result->operand=expression;
				result->type=BasicUnaryExpression::BitNot;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator+(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Add;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator-(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Sub;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator*(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Mul;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator/(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Div;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator%(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Mod;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator<<(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Shl;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator>>(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Shr;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator&(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::BitAnd;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator|(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::BitOr;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator^(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Xor;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator&&(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::And;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator||(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Or;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator+=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::AddAssign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator-=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::SubAssign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator*=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::MulAssign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator/=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::DivAssign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator%=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::ModAssign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator<<=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::ShlAssign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator>>=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::ShrAssign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator&=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::BitAndAssign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator|=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::BitOrAssign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator^=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::XorAssign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::Assign(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Assign;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator<(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Lt;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator<=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Le;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator>(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Gt;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator>=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Ge;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator==(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Eq;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator!=(const BasicExpressionNode& node)const
			{
				Ptr<BasicBinaryExpression> result=new BasicBinaryExpression;
				result->leftOperand=expression;
				result->rightOperand=node.GetInternalValue();
				result->type=BasicBinaryExpression::Ne;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::Member(const WString& member)const
			{
				Ptr<BasicMemberExpression> result=new BasicMemberExpression;
				result->operand=expression;
				result->member=member;
				result->pointerMember=false;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::PMember(const WString& member)const
			{
				Ptr<BasicMemberExpression> result=new BasicMemberExpression;
				result->operand=expression;
				result->member=member;
				result->pointerMember=true;
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator[](const BasicExpressionNode& subscribe)const
			{
				Ptr<BasicSubscribeExpression> result=new BasicSubscribeExpression;
				result->operand=expression;
				result->subscribe=subscribe.GetInternalValue();
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator[](const BasicTypeNode& castType)const
			{
				Ptr<BasicCastingExpression> result=new BasicCastingExpression;
				result->operand=expression;
				result->type=castType.GetInternalValue();
				return result;
			}

			BasicExpressionNode BasicExpressionNode::operator()(const ListNode& arguments)const
			{
				Ptr<BasicInvokeExpression> result=new BasicInvokeExpression;
				result->function=expression;
				CopyFrom(result->arguments.Wrap(), arguments.expressions->Wrap());
				return result;
			}

			BasicExpressionNode e_prim(unsigned __int8 value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=u8;
				result->argument.s64=0;
				result->argument.u8=value;
				return result;
			}

			BasicExpressionNode e_prim(unsigned __int16 value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=u16;
				result->argument.s64=0;
				result->argument.u16=value;
				return result;
			}

			BasicExpressionNode e_prim(unsigned __int32 value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=u32;
				result->argument.s64=0;
				result->argument.u32=value;
				return result;
			}

			BasicExpressionNode e_prim(unsigned __int64 value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=u64;
				result->argument.s64=0;
				result->argument.u64=value;
				return result;
			}

			BasicExpressionNode e_prim(signed __int8 value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=s8;
				result->argument.s64=0;
				result->argument.s8=value;
				return result;
			}

			BasicExpressionNode e_prim(signed __int16 value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=s16;
				result->argument.s64=0;
				result->argument.s16=value;
				return result;
			}

			BasicExpressionNode e_prim(signed __int32 value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=s32;
				result->argument.s64=0;
				result->argument.s32=value;
				return result;
			}

			BasicExpressionNode e_prim(signed __int64 value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=s64;
				result->argument.s64=0;
				result->argument.s64=value;
				return result;
			}

			BasicExpressionNode e_prim(float value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=f32;
				result->argument.s64=0;
				result->argument.f32=value;
				return result;
			}

			BasicExpressionNode e_prim(double value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=f64;
				result->argument.s64=0;
				result->argument.f64=value;
				return result;
			}

			BasicExpressionNode e_prim(bool value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=bool_type;
				result->argument.s64=0;
				result->argument.bool_value=value;
				return result;
			}

			BasicExpressionNode e_prim(char value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=char_type;
				result->argument.s64=0;
				result->argument.char_value=value;
				return result;
			}

			BasicExpressionNode e_prim(wchar_t value)
			{
				Ptr<BasicNumericExpression> result=new BasicNumericExpression;
				result->implicitIntegerType=false;
				result->type=wchar_type;
				result->argument.s64=0;
				result->argument.wchar_value=value;
				return result;
			}

			BasicExpressionNode e_prim(const WString& value)
			{
				Ptr<BasicUnicodeStringExpression> result=new BasicUnicodeStringExpression;
				result->value=value;
				return result;
			}

			BasicExpressionNode e_prim(const AString& value)
			{
				Ptr<BasicMbcsStringExpression> result=new BasicMbcsStringExpression;
				result->value=value;
				return result;
			}

			BasicExpressionNode e_prim(const wchar_t* value)
			{
				Ptr<BasicUnicodeStringExpression> result=new BasicUnicodeStringExpression;
				result->value=value;
				return result;
			}

			BasicExpressionNode e_prim(const char* value)
			{
				Ptr<BasicMbcsStringExpression> result=new BasicMbcsStringExpression;
				result->value=value;
				return result;
			}

			BasicExpressionNode e_name(const WString& name)
			{
				Ptr<BasicReferenceExpression> result=new BasicReferenceExpression;
				result->name=name;
				return result;
			}

			BasicExpressionNode e_name(const WString& name, const BasicTypeNode::ListNode& types)
			{
				Ptr<BasicInstantiatedExpression> result=new BasicInstantiatedExpression;
				result->reference=new BasicReferenceExpression;
				result->reference->name=name;
				CopyFrom(result->argumentTypes.Wrap(), types.types->Wrap());
				return result;
			}

			BasicExpressionNode e_name(const WString& conceptName, const BasicTypeNode& type, const WString& functionName)
			{
				Ptr<BasicInstanceFunctionExpression> result=new BasicInstanceFunctionExpression;
				result->conceptName=conceptName;
				result->functionName=functionName;
				result->type=type.GetInternalValue();
				return result;
			}

			BasicExpressionNode e_null()
			{
				return new BasicNullExpression();
			}

			BasicExpressionNode e_exception()
			{
				return new BasicExceptionAddressExpression();
			}

			BasicExpressionNode e_result()
			{
				return new BasicFunctionResultExpression;
			}

			BasicExpressionNode::ListNode e_exps()
			{
				BasicExpressionNode::ListNode node;
				node.expressions=new List<Ptr<BasicExpression>>;
				return node;
			}

/***********************************************************************
BasicStatementNode
***********************************************************************/

			BasicStatementNode::BasicStatementNode(Ptr<BasicStatement> _statement)
				:statement(_statement)
			{
			}

			Ptr<BasicStatement> BasicStatementNode::GetInternalValue()const
			{
				return statement;
			}

			BasicStatementNode BasicStatementNode::operator<<(const BasicStatementNode& nextStatement)const
			{
				Ptr<BasicCompositeStatement> result=statement.Cast<BasicCompositeStatement>();
				if(!result)
				{
					result=new BasicCompositeStatement;
					result->statements.Add(statement);
				}
				result->statements.Add(nextStatement.GetInternalValue());
				return result;
			}

			BasicStatementNode s_expr(const BasicExpressionNode& expression)
			{
				Ptr<BasicExpressionStatement> result=new BasicExpressionStatement;
				result->expression=expression.GetInternalValue();
				return result;
			}

			BasicStatementNode s_var(const BasicTypeNode& type, const WString& name)
			{
				Ptr<BasicVariableStatement> result=new BasicVariableStatement;
				result->type=type.GetInternalValue();
				result->name=name;
				return result;
			}

			BasicStatementNode s_var(const BasicTypeNode& type, const WString& name, const BasicExpressionNode& initializer)
			{
				Ptr<BasicVariableStatement> result=new BasicVariableStatement;
				result->type=type.GetInternalValue();
				result->name=name;
				result->initializer=initializer.GetInternalValue();
				return result;
			}

			BasicStatementNode s_if(const BasicExpressionNode& condition, const BasicStatementNode& trueStatement)
			{
				Ptr<BasicIfStatement> result=new BasicIfStatement;
				result->condition=condition.GetInternalValue();
				result->trueStatement=trueStatement.GetInternalValue();
				return result;
			}

			BasicStatementNode s_if(const BasicExpressionNode& condition, const BasicStatementNode& trueStatement, const BasicStatementNode& falseStatement)
			{
				Ptr<BasicIfStatement> result=new BasicIfStatement;
				result->condition=condition.GetInternalValue();
				result->trueStatement=trueStatement.GetInternalValue();
				result->falseStatement=falseStatement.GetInternalValue();
				return result;
			}

			BasicStatementNode s_while(const BasicExpressionNode& condition, const BasicStatementNode& statement)
			{
				Ptr<BasicWhileStatement> result=new BasicWhileStatement;
				result->beginCondition=condition.GetInternalValue();
				result->statement=statement.GetInternalValue();
				return result;
			}

			BasicStatementNode s_do_while(const BasicExpressionNode& condition, const BasicStatementNode& statement)
			{
				Ptr<BasicWhileStatement> result=new BasicWhileStatement;
				result->endCondition=condition.GetInternalValue();
				result->statement=statement.GetInternalValue();
				return result;
			}

			BasicStatementNode s_loop(const BasicStatementNode& statement)
			{
				Ptr<BasicWhileStatement> result=new BasicWhileStatement;
				result->statement=statement.GetInternalValue();
				return result;
			}

			BasicStatementNode s_conditional_loop(const BasicExpressionNode& beginCondition, const BasicExpressionNode& endCondition, const BasicStatementNode& statement)
			{
				Ptr<BasicWhileStatement> result=new BasicWhileStatement;
				result->beginCondition=beginCondition.GetInternalValue();
				result->endCondition=endCondition.GetInternalValue();
				result->statement=statement.GetInternalValue();
				return result;
			}

			BasicStatementNode s_break()
			{
				return new BasicBreakStatement;
			}

			BasicStatementNode s_continue()
			{
				return new BasicContinueStatement;
			}

			BasicStatementNode s_return()
			{
				return new BasicReturnStatement;
			}

			BasicStatementNode s_empty()
			{
				return new BasicEmptyStatement;
			}

			BasicStatementNode s_for(const BasicStatementNode& initializer, const BasicExpressionNode& condition, const BasicStatementNode& sideEffect, const BasicStatementNode& statement)
			{
				Ptr<BasicForStatement> result=new BasicForStatement;
				result->condition=condition.GetInternalValue();
				result->initializer=initializer.GetInternalValue();
				result->sideEffect=sideEffect.GetInternalValue();
				result->statement=statement.GetInternalValue();
				return result;
			}

			BasicStatementNode s_try_catch(const BasicStatementNode& tryStatement, const BasicStatementNode& catchStatement)
			{
				Ptr<BasicTryCatchStatement> result=new BasicTryCatchStatement;
				result->tryStatement=tryStatement.GetInternalValue();
				result->catchStatement=catchStatement.GetInternalValue();
				return result;
			}

			BasicStatementNode s_throw()
			{
				Ptr<BasicThrowStatement> result=new BasicThrowStatement;
				return result;
			}

			BasicStatementNode s_throw(const BasicExpressionNode& expression)
			{
				Ptr<BasicThrowStatement> result=new BasicThrowStatement;
				result->expression=expression.GetInternalValue();
				return result;
			}

/***********************************************************************
BasicFunctionDeclarationNode
***********************************************************************/

			BasicFunctionDeclarationNode::BasicFunctionDeclarationNode(Ptr<BasicFunctionDeclaration> _declaration)
				:declaration(_declaration)
			{
			}

			Ptr<BasicFunctionDeclaration> BasicFunctionDeclarationNode::GetInternalValue()
			{
				return declaration;
			}

			BasicFunctionDeclarationNode& BasicFunctionDeclarationNode::ReturnType(const BasicTypeNode& type)
			{
				declaration->signatureType->returnType=type.GetInternalValue();
				return *this;
			}

			BasicFunctionDeclarationNode& BasicFunctionDeclarationNode::Parameter(const WString& name, const BasicTypeNode& type)
			{
				declaration->signatureType->parameterTypes.Add(type.GetInternalValue());
				declaration->parameterNames.Add(name);
				return *this;
			}

			BasicFunctionDeclarationNode& BasicFunctionDeclarationNode::Linking(const WString& assemblyName, const WString& symbolName)
			{
				declaration->linking.assemblyName=assemblyName;
				declaration->linking.symbolName=symbolName;
				return *this;
			}

			BasicFunctionDeclarationNode& BasicFunctionDeclarationNode::Foreign()
			{
				declaration->foreignFunction=true;
				return *this;
			}

			BasicFunctionDeclarationNode& BasicFunctionDeclarationNode::Statement(const BasicStatementNode& statement)
			{
				declaration->statement=statement.GetInternalValue();
				return *this;
			}

/***********************************************************************
BasicStructureDeclarationNode
***********************************************************************/

			BasicStructureDeclarationNode::BasicStructureDeclarationNode(Ptr<BasicStructureDeclaration> _declaration)
				:declaration(_declaration)
			{
			}

			Ptr<BasicStructureDeclaration> BasicStructureDeclarationNode::GetInternalValue()
			{
				return declaration;
			}
				
			BasicStructureDeclarationNode& BasicStructureDeclarationNode::Member(const WString& name, const BasicTypeNode& type)
			{
				declaration->memberTypes.Add(type.GetInternalValue());
				declaration->memberNames.Add(name);
				return *this;
			}

			BasicStructureDeclarationNode& BasicStructureDeclarationNode::Linking(const WString& assemblyName, const WString& symbolName)
			{
				declaration->linking.assemblyName=assemblyName;
				declaration->linking.symbolName=symbolName;
				return *this;
			}

/***********************************************************************
BasicConceptBaseDeclarationNode
***********************************************************************/

			BasicConceptBaseDeclarationNode::BasicConceptBaseDeclarationNode(Ptr<BasicConceptBaseDeclaration> _declaration)
				:declaration(_declaration)
			{
			}

			Ptr<BasicConceptBaseDeclaration> BasicConceptBaseDeclarationNode::GetInternalValue()
			{
				return declaration;
			}
				
			BasicConceptBaseDeclarationNode& BasicConceptBaseDeclarationNode::Member(const WString& name, const BasicTypeNode& type)
			{
				Ptr<BasicConceptBaseDeclaration::FunctionConcept> functionConcept=new BasicConceptBaseDeclaration::FunctionConcept;
				functionConcept->name=name;
				functionConcept->signatureType=type.GetInternalValue().Cast<BasicFunctionType>();
				declaration->functions.Add(functionConcept);
				return *this;
			}

			BasicConceptBaseDeclarationNode& BasicConceptBaseDeclarationNode::Linking(const WString& assemblyName, const WString& symbolName)
			{
				declaration->linking.assemblyName=assemblyName;
				declaration->linking.symbolName=symbolName;
				return *this;
			}

/***********************************************************************
BasicConceptInstanceDeclarationNode
***********************************************************************/

			BasicConceptInstanceDeclarationNode::BasicConceptInstanceDeclarationNode(Ptr<BasicConceptInstanceDeclaration> _declaration)
				:declaration(_declaration)
			{
			}

			Ptr<BasicConceptInstanceDeclaration> BasicConceptInstanceDeclarationNode::GetInternalValue()
			{
				return declaration;
			}

			BasicConceptInstanceDeclarationNode& BasicConceptInstanceDeclarationNode::Member(const WString& function, const WString& target)
			{
				Ptr<BasicConceptInstanceDeclaration::FunctionInstance> functionInstance=new BasicConceptInstanceDeclaration::FunctionInstance;
				functionInstance->name=function;
				functionInstance->normalFunction=e_name(target).GetInternalValue().Cast<BasicReferenceExpression>();
				declaration->functions.Add(functionInstance);
				return *this;
			}

			BasicConceptInstanceDeclarationNode& BasicConceptInstanceDeclarationNode::Member(const WString& function, const WString& target, const BasicTypeNode::ListNode& types)
			{
				Ptr<BasicConceptInstanceDeclaration::FunctionInstance> functionInstance=new BasicConceptInstanceDeclaration::FunctionInstance;
				functionInstance->name=function;
				functionInstance->genericFunction=e_name(target, types).GetInternalValue().Cast<BasicInstantiatedExpression>();
				declaration->functions.Add(functionInstance);
				return *this;
			}

/***********************************************************************
BasicGenericNode
***********************************************************************/

			void BasicGenericNode::CopyGenericDeclaration(Ptr<BasicDeclaration> declaration)
			{
				CopyFrom(declaration->genericDeclaration.arguments.Wrap(), genericDeclaration->arguments.Wrap());
				CopyFrom(declaration->genericDeclaration.constraints.Wrap(), genericDeclaration->constraints.Wrap());
			}

			BasicGenericNode::BasicGenericNode(Ptr<BasicProgram> _program)
				:program(_program)
				,genericDeclaration(new BasicGeneric)
			{
			}

			BasicGenericNode& BasicGenericNode::GenericArgument(const WString& name)
			{
				genericDeclaration->arguments.Add(name);
				return *this;
			}

			BasicGenericNode& BasicGenericNode::Where(const WString& genericArgument, const WString& conceptName)
			{
				Ptr<BasicGeneric::Constraint> constraint=new BasicGeneric::Constraint;
				constraint->argumentName=genericArgument;
				constraint->conceptName=conceptName;
				genericDeclaration->constraints.Add(constraint);
				return *this;
			}

			void BasicGenericNode::DefineVariable(const WString& name, const BasicTypeNode& type)
			{
				Ptr<BasicVariableDeclaration> declaration=new BasicVariableDeclaration;
				CopyGenericDeclaration(declaration);

				declaration->name=name;
				declaration->type=type.GetInternalValue();
				declaration->constant=false;
				program->declarations.Add(declaration);
			}

			void BasicGenericNode::DefineVariable(const WString& name, const BasicTypeNode& type, const WString& assemblyName, const WString& symbolName)
			{
				Ptr<BasicVariableDeclaration> declaration=new BasicVariableDeclaration;
				CopyGenericDeclaration(declaration);

				declaration->name=name;
				declaration->type=type.GetInternalValue();
				declaration->constant=false;
				declaration->linking.assemblyName=assemblyName;
				declaration->linking.symbolName=symbolName;
				program->declarations.Add(declaration);
			}

			void BasicGenericNode::DefineVariable(const WString& name, const BasicTypeNode& type, const BasicExpressionNode& initializer)
			{
				Ptr<BasicVariableDeclaration> declaration=new BasicVariableDeclaration;
				CopyGenericDeclaration(declaration);

				declaration->name=name;
				declaration->type=type.GetInternalValue();
				declaration->initializer=initializer.GetInternalValue();
				declaration->constant=false;
				program->declarations.Add(declaration);
			}

			void BasicGenericNode::DefineConstant(const WString& name, const BasicTypeNode& type, const BasicExpressionNode& initializer)
			{
				Ptr<BasicVariableDeclaration> declaration=new BasicVariableDeclaration;
				CopyGenericDeclaration(declaration);

				declaration->name=name;
				declaration->type=type.GetInternalValue();
				declaration->initializer=initializer.GetInternalValue();
				declaration->constant=true;
				program->declarations.Add(declaration);
			}

			void BasicGenericNode::DefineRename(const WString& name, const BasicTypeNode& type)
			{
				Ptr<BasicTypeRenameDeclaration> declaration=new BasicTypeRenameDeclaration;
				CopyGenericDeclaration(declaration);

				declaration->name=name;
				declaration->type=type.GetInternalValue();
				program->declarations.Add(declaration);
			}

			BasicFunctionDeclarationNode BasicGenericNode::DefineFunction(const WString& name)
			{
				Ptr<BasicFunctionDeclaration> declaration=new BasicFunctionDeclaration;
				CopyGenericDeclaration(declaration);

				declaration->signatureType=new BasicFunctionType;
				Ptr<BasicPrimitiveType> voidType=new BasicPrimitiveType;
				voidType->type=void_type;
				declaration->signatureType->returnType=voidType;

				declaration->name=name;
				declaration->foreignFunction=false;
				program->declarations.Add(declaration);
				return declaration;
			}

			void BasicGenericNode::DefineStructureForward(const WString& name)
			{
				Ptr<BasicStructureDeclaration> declaration=new BasicStructureDeclaration;
				CopyGenericDeclaration(declaration);

				declaration->name=name;
				declaration->defined=false;
				program->declarations.Add(declaration);
			}

			BasicStructureDeclarationNode BasicGenericNode::DefineStructure(const WString& name)
			{
				Ptr<BasicStructureDeclaration> declaration=new BasicStructureDeclaration;
				CopyGenericDeclaration(declaration);

				declaration->name=name;
				declaration->defined=true;
				program->declarations.Add(declaration);
				return declaration;
			}

			void BasicGenericNode::DefineInstanceForward(const BasicTypeNode& type, const WString& conceptName)
			{
				Ptr<BasicConceptInstanceDeclaration> declaration=new BasicConceptInstanceDeclaration();
				CopyGenericDeclaration(declaration);

				declaration->instanceType=type.GetInternalValue();
				declaration->name=conceptName;
				declaration->defined=false;
				program->declarations.Add(declaration);
			}

			BasicConceptInstanceDeclarationNode BasicGenericNode::DefineInstance(const BasicTypeNode& type, const WString& conceptName)
			{
				Ptr<BasicConceptInstanceDeclaration> declaration=new BasicConceptInstanceDeclaration();
				CopyGenericDeclaration(declaration);

				declaration->instanceType=type.GetInternalValue();
				declaration->name=conceptName;
				declaration->defined=true;
				program->declarations.Add(declaration);
				return declaration;
			}

/***********************************************************************
BasicProgramNode
***********************************************************************/

			BasicProgramNode::BasicProgramNode()
			{
				program=new BasicProgram;
			}

			Ptr<BasicProgram> BasicProgramNode::GetInternalValue()
			{
				return program;
			}

			BasicGenericNode BasicProgramNode::Generic()
			{
				return BasicGenericNode(program);
			}

			void BasicProgramNode::DefineVariable(const WString& name, const BasicTypeNode& type)
			{
				return Generic().DefineVariable(name, type);
			}

			void BasicProgramNode::DefineVariable(const WString& name, const BasicTypeNode& type, const WString& assemblyName, const WString& symbolName)
			{
				return Generic().DefineVariable(name, type, assemblyName, symbolName);
			}

			void BasicProgramNode::DefineVariable(const WString& name, const BasicTypeNode& type, const BasicExpressionNode& initializer)
			{
				return Generic().DefineVariable(name, type, initializer);
			}

			void BasicProgramNode::DefineRename(const WString& name, const BasicTypeNode& type)
			{
				return Generic().DefineRename(name, type);
			}

			BasicFunctionDeclarationNode BasicProgramNode::DefineFunction(const WString& name)
			{
				return Generic().DefineFunction(name);
			}

			void BasicProgramNode::DefineStructureForward(const WString& name)
			{
				return Generic().DefineStructureForward(name);
			}

			BasicStructureDeclarationNode BasicProgramNode::DefineStructure(const WString& name)
			{
				return Generic().DefineStructure(name);
			}

			BasicConceptBaseDeclarationNode BasicProgramNode::DefineConcept(const WString& name, const WString& conceptType)
			{
				Ptr<BasicConceptBaseDeclaration> declaration=new BasicConceptBaseDeclaration;
				declaration->name=name;
				declaration->conceptType=conceptType;
				program->declarations.Add(declaration);
				return BasicConceptBaseDeclarationNode(declaration);
			}

			void BasicProgramNode::DefineInstanceForward(const BasicTypeNode& type, const WString& conceptName)
			{
				return Generic().DefineInstanceForward(type, conceptName);
			}

			BasicConceptInstanceDeclarationNode BasicProgramNode::DefineInstance(const BasicTypeNode& type, const WString& conceptName)
			{
				return Generic().DefineInstance(type, conceptName);
			}
		}
	}
}