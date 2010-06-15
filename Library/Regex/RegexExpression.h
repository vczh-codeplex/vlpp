/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Regex::RegexExpression

Classes:
	Expression						�����ʽ����				|
	CharSetExpression				���ַ������ʽ				| a, [a-b], [^a-b0_9], \.rnt\/()+*?{}[]<>^$!=SsDdLlWw, [\rnt-[]\/^$]
	LoopExpression					��ѭ�����ʽ				| a{3}, a{3,}, a{1,3}, a+, a*, a?, LOOP?
	SequenceExpression				��˳����ʽ				| ab
	AlternateExpression				��ѡ����ʽ				| a|b
	BeginExpression					�����Ǵ����ַ�����ʼ���ʽ	| ^
	EndExpression					�����Ǵ����ַ���ĩβ���ʽ	| $
	CaptureExpression				�����Ǵ���������ʽ		| (<name>expr), (?expr)
	MatchExpression					�����Ǵ���ƥ����ʽ		| (<$name>), (<$name;i>), (<$i>)
	PositiveExpression				�����Ǵ�������Ԥ����ʽ	| (=expr)
	NegativeExpression				�����Ǵ�������Ԥ����ʽ	| (!expr)
	UsingExpression					�����ñ��ʽ				| (<#name1>expr)...(<&name1>)...

	RegexExpression					��������ʽ

Functions:
	ParseRegexExpression			�����ַ�������ΪRegexExpression��������﷨�����������쳣
***********************************************************************/

#ifndef VCZH_REGEX_REGEXEXPRESSION
#define VCZH_REGEX_REGEXEXPRESSION

#include "RegexData.h"
#include "RegexAutomaton.h"

namespace vl
{
	namespace regex_internal
	{
		class IRegexExpressionAlgorithm;

/***********************************************************************
������ʽ���ʽ��
***********************************************************************/

		class Expression : public Object, private NotCopyable
		{
		public:
			typedef Ptr<Expression>								Ref;
			typedef Dictionary<WString, Expression::Ref>		Map;

			virtual void				Apply(IRegexExpressionAlgorithm& algorithm)=0;
			bool						IsEqual(Expression* expression);
			bool						HasNoExtension();
			bool						CanTreatAsPure();
			void						NormalizeCharSet(CharRange::List& subsets);
			void						CollectCharSet(CharRange::List& subsets);
			void						ApplyCharSet(CharRange::List& subsets);
			Automaton::Ref				GenerateEpsilonNfa();
		};

		class CharSetExpression : public Expression
		{
		public:
			CharRange::List				ranges;
			bool						reverse;

			bool						AddRangeWithConflict(CharRange range);
			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class LoopExpression : public Expression
		{
		public:
			Expression::Ref				expression;		//��ѭ�����ʽ
			vint							min;			//����
			vint							max;			//���ޣ�-1��������
			bool						preferLong;		//��ƥ������

			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class SequenceExpression : public Expression
		{
		public:
			Expression::Ref				left;			//����ʽ
			Expression::Ref				right;			//�ұ��ʽ

			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class AlternateExpression : public Expression
		{
		public:
			Expression::Ref				left;			//����ʽ
			Expression::Ref				right;			//�ұ��ʽ

			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class BeginExpression: public Expression
		{
		public:

			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class EndExpression : public Expression
		{
		public:

			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class CaptureExpression : public Expression
		{
		public:
			WString						name;			//���������մ���ȱʡ����
			Expression::Ref				expression;		//��������ʽ

			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class MatchExpression : public Expression
		{
		public:
			WString						name;			//���������մ���ȱʡ����
			vint							index;			//������ţ�-1����ǿղ����������

			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class PositiveExpression : public Expression
		{
		public:
			Expression::Ref				expression;		//����ƥ����ʽ

			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class NegativeExpression : public Expression
		{
		public:
			Expression::Ref				expression;		//����ƥ����ʽ

			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class UsingExpression : public Expression
		{
		public:
			WString						name;			//������

			void						Apply(IRegexExpressionAlgorithm& algorithm);
		};

		class RegexExpression : public Object, private NotCopyable
		{
		public:
			typedef Ptr<RegexExpression>						Ref;

			Expression::Map				definitions;	//�����ӱ��ʽ
			Expression::Ref				expression;		//�����ʽ

			Expression::Ref				Merge();
		};

/***********************************************************************
�㷨����
***********************************************************************/

		class IRegexExpressionAlgorithm : public Interface
		{
		public:
			virtual void				Visit(CharSetExpression* expression)=0;
			virtual void				Visit(LoopExpression* expression)=0;
			virtual void				Visit(SequenceExpression* expression)=0;
			virtual void				Visit(AlternateExpression* expression)=0;
			virtual void				Visit(BeginExpression* expression)=0;
			virtual void				Visit(EndExpression* expression)=0;
			virtual void				Visit(CaptureExpression* expression)=0;
			virtual void				Visit(MatchExpression* expression)=0;
			virtual void				Visit(PositiveExpression* expression)=0;
			virtual void				Visit(NegativeExpression* expression)=0;
			virtual void				Visit(UsingExpression* expression)=0;
		};

		template<typename ReturnType, typename ParameterType=void*>
		class RegexExpressionAlgorithm : public Object, public IRegexExpressionAlgorithm
		{
		private:
			ReturnType					returnValue;
			void*						parameterValue;
		public:

			ReturnType Invoke(Expression* expression, ParameterType parameter)
			{
				parameterValue=(void*)&parameter;
				expression->Apply(*this);
				return returnValue;
			}

			ReturnType Invoke(Expression::Ref expression, ParameterType parameter)
			{
				parameterValue=(void*)&parameter;
				expression->Apply(*this);
				return returnValue;
			}

			virtual ReturnType			Apply(CharSetExpression* expression, ParameterType parameter)=0;
			virtual ReturnType			Apply(LoopExpression* expression, ParameterType parameter)=0;
			virtual ReturnType			Apply(SequenceExpression* expression, ParameterType parameter)=0;
			virtual ReturnType			Apply(AlternateExpression* expression, ParameterType parameter)=0;
			virtual ReturnType			Apply(BeginExpression* expression, ParameterType parameter)=0;
			virtual ReturnType			Apply(EndExpression* expression, ParameterType parameter)=0;
			virtual ReturnType			Apply(CaptureExpression* expression, ParameterType parameter)=0;
			virtual ReturnType			Apply(MatchExpression* expression, ParameterType parameter)=0;
			virtual ReturnType			Apply(PositiveExpression* expression, ParameterType parameter)=0;
			virtual ReturnType			Apply(NegativeExpression* expression, ParameterType parameter)=0;
			virtual ReturnType			Apply(UsingExpression* expression, ParameterType parameter)=0;
		public:
			void Visit(CharSetExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(LoopExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(SequenceExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(AlternateExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(BeginExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(EndExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(CaptureExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(MatchExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(PositiveExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(NegativeExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(UsingExpression* expression)
			{
				returnValue=Apply(expression, *((ParameterType*)parameterValue));
			}
		};

		template<typename ParameterType>
		class RegexExpressionAlgorithm<void, ParameterType> : public Object, public IRegexExpressionAlgorithm
		{
		private:
			void*						parameterValue;
		public:

			void Invoke(Expression* expression, ParameterType parameter)
			{
				parameterValue=(void*)&parameter;
				expression->Apply(*this);
			}

			void Invoke(Expression::Ref expression, ParameterType parameter)
			{
				parameterValue=(void*)&parameter;
				expression->Apply(*this);
			}

			virtual void				Apply(CharSetExpression* expression, ParameterType parameter)=0;
			virtual void				Apply(LoopExpression* expression, ParameterType parameter)=0;
			virtual void				Apply(SequenceExpression* expression, ParameterType parameter)=0;
			virtual void				Apply(AlternateExpression* expression, ParameterType parameter)=0;
			virtual void				Apply(BeginExpression* expression, ParameterType parameter)=0;
			virtual void				Apply(EndExpression* expression, ParameterType parameter)=0;
			virtual void				Apply(CaptureExpression* expression, ParameterType parameter)=0;
			virtual void				Apply(MatchExpression* expression, ParameterType parameter)=0;
			virtual void				Apply(PositiveExpression* expression, ParameterType parameter)=0;
			virtual void				Apply(NegativeExpression* expression, ParameterType parameter)=0;
			virtual void				Apply(UsingExpression* expression, ParameterType parameter)=0;
		public:
			void Visit(CharSetExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(LoopExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(SequenceExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(AlternateExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(BeginExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(EndExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(CaptureExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(MatchExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(PositiveExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(NegativeExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}

			void Visit(UsingExpression* expression)
			{
				Apply(expression, *((ParameterType*)parameterValue));
			}
		};

/***********************************************************************
��������
***********************************************************************/

		extern Ptr<LoopExpression>		ParseLoop(const wchar_t*& input);
		extern Ptr<Expression>			ParseCharSet(const wchar_t*& input);
		extern Ptr<Expression>			ParseFunction(const wchar_t*& input);
		extern Ptr<Expression>			ParseUnit(const wchar_t*& input);
		extern Ptr<Expression>			ParseJoin(const wchar_t*& input);
		extern Ptr<Expression>			ParseAlt(const wchar_t*& input);
		extern Ptr<Expression>			ParseExpression(const wchar_t*& input);
		extern RegexExpression::Ref		ParseRegexExpression(const WString& code);
	}
}

#endif