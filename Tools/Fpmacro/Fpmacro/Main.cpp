#include "..\..\..\Library\Console.h"
#include "..\..\..\Library\Pointer.h"
#include "..\..\..\Library\Stream\FileStream.h"
#include "..\..\..\Library\Stream\Accessor.h"
#include "..\..\..\Library\Stream\CharFormat.h"
#include "..\..\..\Library\Collections\List.h"
#include "..\..\..\Library\Collections\Dictionary.h"
#include "..\..\..\Library\Collections\Operation.h"
#include "..\..\..\Library\Combinator\StringCombinator.h"

#include <windows.h>

using namespace vl;
using namespace vl::console;
using namespace vl::stream;
using namespace vl::collections;
using namespace vl::combinator;

/***********************************************************************
Running Environment
***********************************************************************/

class RunningObject : public Object
{
public:
	typedef Ptr<RunningObject>			Ref;

	class Environment : public Object
	{
	public:
		typedef Ptr<Environment>							Ref;
		typedef Dictionary<WString, RunningObject::Ref>		ObjectMap;

		Ref								previous;
		ObjectMap						objects;
	};

protected:
	virtual RunningObject::Ref			RunInternal(RunningObject::Ref self, Environment::Ref environment)=0;
	virtual RunningObject::Ref			InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)=0;

public:

	static RunningObject::Ref Run(RunningObject::Ref object, Environment::Ref environment)
	{
		return object->RunInternal(object, environment);
	}

	static RunningObject::Ref Invoke(RunningObject::Ref object, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		return object->InvokeInternal(object, environment, arguments);
	}
};

class ReferenceObject : public RunningObject
{
public:
	WString							name;

protected:
	RunningObject::Ref RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		while(environment)
		{
			int index=environment->objects.Keys().IndexOf(name);
			if(index!=-1)
			{
				RunningObject::Ref result=Run(environment->objects.Values()[index], environment);
				return result;
			}
			else
			{
				environment=environment->previous;
			}
		}
		throw Exception(L"找不到"+name+L"。");
	}

	RunningObject::Ref InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		throw Exception(L"对象引用"+name+L"不是一个函数，或该引用未被求值。");
	}
};

class ExecutableObject : public RunningObject
{
public:
	List<WString>					parameters;
	RunningObject::Ref				runningObject;
	Environment::Ref				predefinedEnvironment;
	WString							name;

protected:
	RunningObject::Ref RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		return self;
	}

	RunningObject::Ref InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		if(parameters.Count()!=arguments.Count())
		{
			throw Exception(L"参数数量不一致。");
		}
		Environment::Ref newEnvironment=new Environment;
		newEnvironment->previous=environment;
		for(int i=0;i<parameters.Count();i++)
		{
			newEnvironment->objects.Add(parameters[i], Run(arguments.Get(i), environment));
		}
		CopyFrom(newEnvironment->objects.Wrap(), predefinedEnvironment->objects.Wrap(), true);
		return Run(runningObject, newEnvironment);
	}
};

class TextObject : public RunningObject
{
public:
	WString							text;

protected:
	RunningObject::Ref RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		return self;
	}

	RunningObject::Ref InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		throw Exception(L"字符串表达式不是一个函数，该字符串表达式内容为：\""+text+L"\"。");
	}
};

class ConcatObject : public RunningObject
{
public:
	List<RunningObject::Ref>		objects;

protected:
	RunningObject::Ref RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		WString text;
		for(int i=0;i<objects.Count();i++)
		{
			RunningObject::Ref object=Run(objects[i], environment);
			TextObject* textObject=dynamic_cast<TextObject*>(object.Obj());
			if(textObject)
			{
				text+=textObject->text;
			}
			else
			{
				throw Exception(L"连接表达式链接到错误的对象，已经连接的字符串为：\""+text+L"\"。");
			}
		}
		TextObject* object=new TextObject;
		object->text=text;
		return object;
	}

	RunningObject::Ref InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		throw Exception(L"连接表达式不是一个函数。");
	}
};

class ArrayObject : public RunningObject
{
public:
	List<RunningObject::Ref>		objects;

protected:
	RunningObject::Ref RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		return self;
	}

	RunningObject::Ref InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		throw Exception(L"数组表达式不是一个函数。");
	}
};

class InvokeObject : public RunningObject
{
public:
	RunningObject::Ref				target;
	List<RunningObject::Ref>		arguments;

protected:
	RunningObject::Ref RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		return Invoke(Run(target, environment), environment, arguments);
	}

	RunningObject::Ref InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		return Invoke(Run(self, environment), environment, arguments);
	}
};

class PredefinedObject : public RunningObject
{
public:
	enum Type
	{
		Add,
		Sub,
		Mul,
		Div,
		Mod,
		LE,
		LT,
		GE,
		GT,
		EQ,
		NE,
		If,
		Loop,
		LoopSep,
		Head,
		Tail,
		Length,
		Get,
		Unknown
	};

	Type							type;

protected:
	RunningObject::Ref RunInternal(RunningObject::Ref self, Environment::Ref environment)
	{
		return self;
	}

	int GetInt(RunningObject::Ref object, Environment::Ref environment)
	{
		object=Run(object, environment);
		TextObject* textObject=dynamic_cast<TextObject*>(object.Obj());
		if(textObject)
		{
			return wtoi(textObject->text);
		}
		else
		{
			throw Exception(L"将对象转换为数字发生错误。");
		}
	}

	WString GetStr(RunningObject::Ref object, Environment::Ref environment)
	{
		object=Run(object, environment);
		TextObject* textObject=dynamic_cast<TextObject*>(object.Obj());
		if(textObject)
		{
			return textObject->text;
		}
		else
		{
			throw Exception(L"将对象转换为字符串发生错误。");
		}
	}

	RunningObject::Ref GetObj(int number)
	{
		TextObject* textObject=new TextObject;
		textObject->text=itow(number);
		return textObject;
	}

	RunningObject::Ref GetObj(WString text)
	{
		TextObject* textObject=new TextObject;
		textObject->text=text;
		return textObject;
	}

	RunningObject::Ref InvokeInternal(RunningObject::Ref self, Environment::Ref environment, const List<RunningObject::Ref>& arguments)
	{
		switch(type)
		{
		case Add:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) + GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Add函数的参数个数必须是2。");
				}
			}
		case Sub:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) - GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Sub函数的参数个数必须是2。");
				}
			}
		case Mul:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) * GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Mul函数的参数个数必须是2。");
				}
			}
		case Div:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) / GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Div函数的参数个数必须是2。");
				}
			}
		case Mod:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) % GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Mod函数的参数个数必须是2。");
				}
			}
		case LE:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) <= GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Le函数的参数个数必须是2。");
				}
			}
		case LT:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) < GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Lt函数的参数个数必须是2。");
				}
			}
		case GE:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) >= GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Ge函数的参数个数必须是2。");
				}
			}
		case GT:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) > GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Gt函数的参数个数必须是2。");
				}
			}
		case EQ:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) == GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Eq函数的参数个数必须是2。");
				}
			}
		case NE:
			{
				if(arguments.Count()==2)
				{
					return GetObj(GetInt(arguments.Get(0), environment) != GetInt(arguments.Get(1), environment));
				}
				else
				{
					throw Exception(L"Ne函数的参数个数必须是2。");
				}
			}
		case If:
			{
				if(arguments.Count()==3)
				{
					return Run(arguments.Get(GetInt(arguments.Get(0), environment)?1:2), environment);
				}
				else
				{
					throw Exception(L"If函数的参数个数必须是e。");
				}
			}
		case Loop:
			{
				if(arguments.Count()!=3)
				{
					throw Exception(L"Loop函数的参数个数必须是1。");
				}
				WString text;
				int count=GetInt(arguments.Get(0), environment);
				int start=GetInt(arguments.Get(1), environment);
				RunningObject::Ref function=Run(arguments.Get(2), environment);
				for(int i=0;i<count;i++)
				{
					List<RunningObject::Ref> arguments;
					arguments.Add(GetObj(i+start));
					text+=GetStr(Invoke(function, environment, arguments), environment);
				}
				return GetObj(text);
			}
		case LoopSep:
			{
				if(arguments.Count()!=4)
				{
					throw Exception(L"LoopSep函数的参数个数必须是1。");
				}
				WString text;
				int count=GetInt(arguments.Get(0), environment);
				int start=GetInt(arguments.Get(1), environment);
				RunningObject::Ref function=Run(arguments.Get(2), environment);
				WString sep=GetStr(arguments.Get(3), environment);
				for(int i=0;i<count;i++)
				{
					List<RunningObject::Ref> arguments;
					arguments.Add(GetObj(i+start));
					if(i)
					{
						text+=sep;
					}
					text+=GetStr(Invoke(function, environment, arguments), environment);
				}
				return GetObj(text);
			}
		case Head:
			{
				if(arguments.Count()!=1)
				{
					throw Exception(L"Head函数的参数个数必须是1。");
				}
				ArrayObject* arrayObject=dynamic_cast<ArrayObject*>(arguments.Get(0).Obj());
				if(!arrayObject)
				{
					throw Exception(L"Head函数的参数必须是数组。");
				}
				if(arrayObject->objects.Count()==0)
				{
					throw Exception(L"Head函数不能对空数组求值。");
				}
				return arrayObject->objects[0];
			}
		case Tail:
			{
				if(arguments.Count()!=1)
				{
					throw Exception(L"Tail函数的参数个数必须是1。");
				}
				ArrayObject* arrayObject=dynamic_cast<ArrayObject*>(arguments.Get(0).Obj());
				if(!arrayObject)
				{
					throw Exception(L"Tail函数的参数必须是数组。");
				}
				if(arrayObject->objects.Count()==0)
				{
					throw Exception(L"Tail函数不能对空数组求值。");
				}
				ArrayObject* newArray=new ArrayObject;
				for(int i=1;i<arrayObject->objects.Count();i++)
				{
					newArray->objects.Add(arrayObject->objects[i]);
				}
				return newArray;
			}
		case Length:
			{
				if(arguments.Count()!=1)
				{
					throw Exception(L"Length函数的参数个数必须是1。");
				}
				ArrayObject* arrayObject=dynamic_cast<ArrayObject*>(arguments.Get(0).Obj());
				if(!arrayObject)
				{
					throw Exception(L"Tail函数的参数必须是数组。");
				}
				return GetObj(arrayObject->objects.Count());
			}
		case Get:
			{
				if(arguments.Count()!=2)
				{
					throw Exception(L"Get函数的参数个数必须是2。");
				}
				ArrayObject* arrayObject=dynamic_cast<ArrayObject*>(arguments.Get(0).Obj());
				if(!arrayObject)
				{
					throw Exception(L"Get函数的第一个参数必须是数组。");
				}
				int index=GetInt(arguments.Get(1), environment);
				if(index>=0 && index<arrayObject->objects.Count())
				{
					return Run(arrayObject->objects[index], environment);
				}
				else
				{
					throw Exception(L"Get参数的第二个参数\""+itow(index)+L"\"越界。");
				}
			}
		default:
			throw Exception(L"未知错误。");
		}
	}
};

/***********************************************************************
Functional Macro Code Tree
***********************************************************************/

class Expression : public Object
{
public:
	typedef Ptr<Expression>			Ref;
	typedef List<Ref>				List;

	virtual void					ToString(const WString& prefix, TextWriter& writer)const=0;
	virtual RunningObject::Ref		CreateObject()=0;
};

class ConcatExpression : public Expression
{
public:
	Expression::List				expressions;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"CONCAT {");
		for(int i=0;i<expressions.Count();i++)
		{
			expressions.Get(i)->ToString(prefix+L"    ", writer);
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	RunningObject::Ref CreateObject()
	{
		ConcatObject* result=new ConcatObject;
		for(int i=0;i<expressions.Count();i++)
		{
			result->objects.Add(expressions[i]->CreateObject());
		}
		return result;
	}
};

class ArrayExpression : public Expression
{
public:
	Expression::List				elements;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"ARRAY {");
		for(int i=0;i<elements.Count();i++)
		{
			elements.Get(i)->ToString(prefix+L"    ", writer);
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	RunningObject::Ref CreateObject()
	{
		ArrayObject* result=new ArrayObject;
		for(int i=0;i<elements.Count();i++)
		{
			result->objects.Add(elements[i]->CreateObject());
		}
		return result;
	}
};

class InvokeExpression : public Expression
{
public:
	Expression::Ref					function;
	Expression::List				arguments;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"INVOKE {");
		function->ToString(prefix+L"    ", writer);

		writer.WriteString(prefix+L"    ");
		writer.WriteLine(L"ARGUMENTS {");
		for(int i=0;i<arguments.Count();i++)
		{
			arguments.Get(i)->ToString(prefix+L"        ", writer);
		}
		writer.WriteString(prefix+L"    ");
		writer.WriteLine(L"}");

		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	RunningObject::Ref CreateObject()
	{
		InvokeObject* result=new InvokeObject;
		result->target=function->CreateObject();
		for(int i=0;i<arguments.Count();i++)
		{
			result->arguments.Add(arguments[i]->CreateObject());
		}
		return result;
	}
};

class ReferenceExpression : public Expression
{
public:
	WString							name;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteString(L"NAME : ");
		writer.WriteLine(name);
	}

	RunningObject::Ref CreateObject()
	{
		PredefinedObject::Type type=PredefinedObject::Unknown;
		bool found=true;
		if(name==L"$add")type=PredefinedObject::Add;
		else if(name==L"$sub")type=PredefinedObject::Sub;
		else if(name==L"$mul")type=PredefinedObject::Mul;
		else if(name==L"$div")type=PredefinedObject::Div;
		else if(name==L"$mod")type=PredefinedObject::Mod;
		else if(name==L"$lt")type=PredefinedObject::LT;
		else if(name==L"$le")type=PredefinedObject::LE;
		else if(name==L"$gt")type=PredefinedObject::GT;
		else if(name==L"$ge")type=PredefinedObject::GE;
		else if(name==L"$eq")type=PredefinedObject::EQ;
		else if(name==L"$ne")type=PredefinedObject::NE;
		else if(name==L"$if")type=PredefinedObject::If;
		else if(name==L"$loop")type=PredefinedObject::Loop;
		else if(name==L"$loopsep")type=PredefinedObject::LoopSep;
		else if(name==L"$head")type=PredefinedObject::Head;
		else if(name==L"$tail")type=PredefinedObject::Tail;
		else if(name==L"$length")type=PredefinedObject::Length;
		else if(name==L"$get")type=PredefinedObject::Get;
		else found=false;
		if(found)
		{
			PredefinedObject* result=new PredefinedObject;
			result->type=type;
			return result;
		}
		else
		{
			ReferenceObject* result=new ReferenceObject;
			result->name=name;
			return result;
		}
	}
};

class TextExpression : public Expression
{
public:
	WString							text;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteString(L"TEXT : ");
		writer.WriteLine(text);
	}

	RunningObject::Ref CreateObject()
	{
		TextObject* result=new TextObject;
		result->text=text;
		return result;
	}
};

class Definition : public Object
{
public:
	typedef Ptr<Definition>			Ref;

	virtual void					ToString(const WString& prefix, TextWriter& writer)const=0;
	virtual RunningObject::Ref		CreateObject(RunningObject::Environment::Ref environment)=0;
};

class ExpressionDefinition : public Definition
{
public:
	Expression::Ref					expression;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		expression->ToString(prefix, writer);
	}

	RunningObject::Ref CreateObject(RunningObject::Environment::Ref environment)
	{
		return expression->CreateObject();
	}
};

class ReferenceDefinition : public Definition
{
public:
	WString							name;
	List<WString>					parameters;
	List<Definition::Ref>			definitions;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"REFERENCE {");
		writer.WriteString(prefix);
		writer.WriteString(L"    ");
		writer.WriteString(name);
		writer.WriteString(L" : ");
		for(int i=0;i<parameters.Count();i++)
		{
			if(i)writer.WriteString(L" , ");
			writer.WriteString(parameters.Get(i));
		}
		writer.WriteLine(L"");
		for(int i=0;i<definitions.Count();i++)
		{
			definitions.Get(i)->ToString(prefix+L"    ", writer);
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	RunningObject::Ref CreateObject(RunningObject::Environment::Ref environment)
	{
		ExecutableObject* result=new ExecutableObject;
		result->name=name;
		result->predefinedEnvironment=new RunningObject::Environment;
		for(int i=0;i<parameters.Count();i++)
		{
			result->parameters.Add(parameters[i]);
		}
		ConcatObject* concat=new ConcatObject;
		for(int i=0;i<definitions.Count();i++)
		{
			RunningObject::Ref item=definitions[i]->CreateObject(result->predefinedEnvironment);
			if(item)
			{
				if(concat->objects.Count())
				{
					TextObject* newLine=new TextObject;
					newLine->text=L"\r\n";
					concat->objects.Add(newLine);
				}
				concat->objects.Add(item);
			}
		}
		result->runningObject=concat;
		environment->objects.Add(name, result);
		return 0;
	}
};

class Macro : public Object
{
public:
	typedef Ptr<Macro>				Ref;

	List<Definition::Ref>			definitions;

	void ToString(const WString& prefix, TextWriter& writer)const
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"MACRO {");;
		for(int i=0;i<definitions.Count();i++)
		{
			definitions.Get(i)->ToString(prefix+L"    ", writer);
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}

	RunningObject::Ref CreateObject()
	{
		ExecutableObject* result=new ExecutableObject;
		result->predefinedEnvironment=new RunningObject::Environment;
		ConcatObject* concat=new ConcatObject;
		for(int i=0;i<definitions.Count();i++)
		{
			RunningObject::Ref item=definitions[i]->CreateObject(result->predefinedEnvironment);
			if(item)
			{
				if(concat->objects.Count())
				{
					TextObject* newLine=new TextObject;
					newLine->text=L"\r\n";
					concat->objects.Add(newLine);
				}
				concat->objects.Add(item);
			}
		}
		result->runningObject=concat;
		return result;
	}
};

/***********************************************************************
Syntax Analyzer
***********************************************************************/

class SyntaxAnalyzer : public Object
{
protected:
	Node<StringInput<wchar_t>, WString>						PLAIN_TEXT, BRACKET_OPEN, BRACKET_CLOSE, ARRAY_OPEN, ARRAY_CLOSE, NAME, COMMA, COMMENT, STRING, NEW_LINE, ESCAPE, SPACE, OTHERS;
	Node<StringInput<wchar_t>, Ptr<Expression::List>>		exp_list;
	Node<StringInput<wchar_t>, Ptr<List<WString>>>			name_list;
	Node<StringInput<wchar_t>, Ptr<List<Definition::Ref>>>	def_list;
	Node<StringInput<wchar_t>, Ptr<ReferenceDefinition>>	ref_head;
	Rule<StringInput<wchar_t>, Expression::Ref>				exp, unit_exp, concat_exp, array_exp, invoke_exp, reference_exp, text_exp;
	Rule<StringInput<wchar_t>, Expression::Ref>				exp_nc, unit_exp_nc, concat_exp_nc, text_exp_nc;
	Rule<StringInput<wchar_t>, Definition::Ref>				def, exp_def, ref_def;
	Rule<StringInput<wchar_t>, Macro::Ref>					macro_start;

	static Expression::Ref ToText(const WString& input)
	{
		TextExpression* expression=new TextExpression;
		if(input[0]==L'$')
		{
			expression->text=input[2];
		}
		else
		{
			expression->text=input;
		}
		return expression;
	}

	static Expression::Ref ToReference(const WString& input)
	{
		ReferenceExpression* expression=new ReferenceExpression;
		expression->name=input;
		return expression;
	}

	static Expression::Ref ToConcat(const Ptr<Expression::List>& input)
	{
		if(input->Count()==1)
		{
			return input->Get(0);
		}
		else
		{
			ConcatExpression* expression=new ConcatExpression;
			CopyFrom(expression->expressions.Wrap(), input->Wrap());
			return expression;
		}
	}

	static Expression::Ref ToArray(const Ptr<Expression::List>& input)
	{
		ArrayExpression* expression=new ArrayExpression;
		CopyFrom(expression->elements.Wrap(), input->Wrap());
		return expression;
	}

	static Expression::Ref ToInvoke(const ParsingPair<Expression::Ref, Ptr<Expression::List>>& input)
	{
		InvokeExpression* expression=new InvokeExpression;
		expression->function=input.First();
		CopyFrom(expression->arguments.Wrap(), input.Second()->Wrap());
		return expression;
	}

	static Expression::Ref ToBracket(const ParsingList<ParsingPair<Expression::Ref, ParsingList<WString>>>& input)
	{
		ConcatExpression* expression=new ConcatExpression;
		TextExpression* left=new TextExpression;
		left->text=L"(";
		expression->expressions.Add(left);
		if(input.Head())
		{
			expression->expressions.Add(input.Head()->Value().First());
			if(input.Head()->Value().Second().Head())
			{
				TextExpression* right=new TextExpression;
				right->text=L")";
				expression->expressions.Add(right);
			}
		}
		return expression;
	}

	static Definition::Ref ToExpDef(const Expression::Ref& input)
	{
		ExpressionDefinition* definition=new ExpressionDefinition;
		definition->expression=input;
		return definition;
	}

	static Ptr<ReferenceDefinition> ToRefDefHead(const ParsingPair<WString, Ptr<List<WString>>>& input)
	{
		ReferenceDefinition* definition=new ReferenceDefinition;
		definition->name=input.First();
		CopyFrom(definition->parameters.Wrap(), input.Second()->Wrap());
		return definition;
	}

	static Definition::Ref ToRefDef(const ParsingPair<Ptr<ReferenceDefinition>, Ptr<List<Definition::Ref>>>& input)
	{
		CopyFrom(input.First()->definitions.Wrap(), input.Second()->Wrap());
		return input.First();
	}

	static Macro::Ref ToMacro(const Ptr<List<Definition::Ref>>& input)
	{
		Macro* macro=new Macro;
		CopyFrom(macro->definitions.Wrap(), input->Wrap());
		return macro;
	}
public:
	typedef CombinatorException<StringInput<wchar_t>>		ExceptionType;

	SyntaxAnalyzer()
	{
		PLAIN_TEXT		= rgx(L"[^/$(), /t/r/n//\"]+");
		BRACKET_OPEN	= str(L"(");
		BRACKET_CLOSE	= str(L")");
		ARRAY_OPEN		= str(L"$[");
		ARRAY_CLOSE		= str(L"]");
		NAME			= rgx(L"/$[a-zA-Z_]/w*");
		COMMA			= str(L",");
		COMMENT			= rgx(L"(////[^/r/n]*|///*([^*]|/*+[^*//])*/*+//)");
		STRING			= rgx(L"\"([^\\\\\"]|\\\\\\.)*\"");
		NEW_LINE		= str(L"\r\n");
		ESCAPE			= rgx(L"/$/(/./)");
		SPACE			= rgx(L"[ /t]+");
		OTHERS			= rgx(L"[//\"]");

		exp_list		= list(opt(exp_nc + *(COMMA >> exp_nc)));
		name_list		= list(opt(NAME + *(*SPACE >> COMMA >> *SPACE >> NAME)));
		def_list		= list(+(*NEW_LINE >> def << *NEW_LINE));
		ref_head		= ((str(L"$$define")>>*SPACE>>NAME) + (*SPACE>>(BRACKET_OPEN >> name_list << BRACKET_CLOSE)))[ToRefDefHead]<<*SPACE;

		text_exp_nc		= (COMMENT | STRING | PLAIN_TEXT | ESCAPE | SPACE | OTHERS)[ToText];
		unit_exp_nc		= invoke_exp | array_exp | reference_exp | text_exp_nc | str(L"()")[ToText] | (BRACKET_OPEN >> opt(exp + opt(BRACKET_CLOSE)))[ToBracket];
		concat_exp_nc	= list(+unit_exp_nc)[ToConcat];
		exp_nc			= concat_exp_nc;

		array_exp		= (ARRAY_OPEN >> exp_list << ARRAY_CLOSE)[ToArray];
		reference_exp	= NAME[ToReference];
		text_exp		= (COMMA | COMMENT | STRING | PLAIN_TEXT | ESCAPE | SPACE | OTHERS)[ToText];
		invoke_exp		= (reference_exp + (BRACKET_OPEN >> exp_list << BRACKET_CLOSE))[ToInvoke];
		unit_exp		= invoke_exp | array_exp | reference_exp | text_exp | str(L"()")[ToText] | (BRACKET_OPEN >> opt(exp + opt(BRACKET_CLOSE)))[ToBracket];
		concat_exp		= list(+unit_exp)[ToConcat];
		exp				= concat_exp;

		exp_def			= exp[ToExpDef];
		ref_def			= (ref_head + (list(loop(exp_def, 1, 1)) | (str(L"$$begin") >> def_list << str(L"$$end"))))[ToRefDef];
		def				= exp_def | ref_def;

		macro_start		= def_list[ToMacro];
	}

	Macro::Ref Parse(const WString& code)
	{
		return macro_start.ParseFull(code, false);
	}
};

/***********************************************************************
Interpretor
***********************************************************************/

void RunMacro(const WString& input, TextWriter& output, TextWriter& log, SyntaxAnalyzer& analyzer)
{
	try
	{
		Macro::Ref macro=analyzer.Parse(input);
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(L"Syntax Tree");
		log.WriteLine(L"==============================================================================================");
		macro->ToString(L"", log);
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(L"Running Result");
		log.WriteLine(L"==============================================================================================");

		RunningObject::Ref object=macro->CreateObject();
		RunningObject::Environment::Ref environment=new RunningObject::Environment;
		List<RunningObject::Ref> arguments;
		try
		{
			object=RunningObject::Invoke(object, environment, arguments);
			TextObject* textObject=dynamic_cast<TextObject*>(object.Obj());
			if(textObject)
			{
				output.WriteString(textObject->text);
			}
			else
			{
				log.WriteLine(L"运行结果不是一个字符串。");
			}
		}
		catch(const Exception& e)
		{
			log.WriteLine(e.Message());
		}
	}
	catch(const CombinatorException<StringInput<wchar_t>>& e)
	{
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(L"Remain Text");
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(e.GetInput().CurrentString());
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(L"Errors");
		log.WriteLine(L"==============================================================================================");
		for(int i=0;i<e.GetGlobalInfo().errors.Count();i++)
		{
			log.WriteLine(e.GetGlobalInfo().errors.Get(i)->Message());
			log.WriteLine(e.GetGlobalInfo().errors.Get(i)->GetPosition().CurrentString());
			log.WriteLine(L"----------------------------------------------------------------------------------------------");
		}
		log.WriteLine(L"==============================================================================================");
		log.WriteLine(L"Candidate Errors");
		log.WriteLine(L"==============================================================================================");
		for(int i=0;i<e.GetGlobalInfo().candidateErrors.Count();i++)
		{
			log.WriteLine(e.GetGlobalInfo().candidateErrors.Get(i)->Message());
			log.WriteLine(e.GetGlobalInfo().candidateErrors.Get(i)->GetPosition().CurrentString());
			log.WriteLine(L"----------------------------------------------------------------------------------------------");
		}
	}
}

/***********************************************************************
Main function
***********************************************************************/

WString GetDirectory(const WString& fileName)
{
	int index=0;
	for(int i=0;i<fileName.Length();i++)
	{
		if(fileName[i]==L'\\')
		{
			index=i;
		}
	}
	return fileName.Left(index+1);
}

WString DereferenceMacroFile(const WString& inputPath)
{
	WString result;
	WString directory=GetDirectory(inputPath);

	FileStream inputFileStream(inputPath, FileStream::ReadOnly);
	BomDecoder inputDecoder;
	DecoderStream inputStream(inputFileStream, inputDecoder);
	StreamReader inputReader(inputStream);

	while(!inputReader.IsEnd())
	{
		WString line=inputReader.ReadLine();
		if(line.Length()>=10 && line.Left(10)==L"$$include:")
		{
			result+=DereferenceMacroFile(directory+line.Right(line.Length()-10));
		}
		else
		{
			result+=line+L"\r\n";
		}
	}
	return result;
}

int wmain(int argc, wchar_t* argv[])
{
	WString baseDirectory;
	{
		wchar_t currentDirectory[MAX_PATH]={0};
		GetCurrentDirectory(MAX_PATH, currentDirectory);
		baseDirectory=currentDirectory;
		if(baseDirectory[baseDirectory.Length()-1]!=L'\\')
		{
			baseDirectory+=L'\\';
		}
	}

	Console::SetTitle(L"Vczh Functional Macro for C++ Code Generation");
	Console::SetColor(false, true, false, true);
	Console::WriteLine(L"fpm>Files : "+itow(argc-1));
	SyntaxAnalyzer analyzer;
	for(int i=1;i<argc;i++)
	{
		WString inputPath=argv[i];
		if(inputPath.Length()<2 || inputPath[1]!=L':')
		{
			inputPath=baseDirectory+inputPath;
		}
		Console::WriteLine(L"pfm>Making : "+inputPath);
		if(inputPath.Length()<4 || inputPath.Right(4)!=L".fpm")
		{
			Console::SetColor(true, false, false, true);
			Console::WriteLine(L"error> The extenion name of the input file path must be \".fpm\".");
			Console::SetColor(false, true, false, true);
		}
		else
		{
			WString outputPath=inputPath.Left(inputPath.Length()-4);
			Console::WriteLine(L"fpm>Output path : "+outputPath);
			WString logPath=outputPath+L".log";
			Console::WriteLine(L"fpm>Log path : "+logPath);

			FileStream outputFileStream(outputPath, FileStream::WriteOnly);
			if(!outputFileStream.IsAvailable())
			{
				Console::SetColor(true, false, false, true);
				Console::WriteLine(L"error> \""+outputPath+L"\" can not be opened for writing.");
				Console::SetColor(false, true, false, true);
				continue;
			}
			BomEncoder outputEncoder(BomEncoder::Mbcs);
			EncoderStream outputStream(outputFileStream, outputEncoder);
			StreamWriter outputWriter(outputStream);

			FileStream logFileStream(logPath, FileStream::WriteOnly);
			BomEncoder logEncoder(BomEncoder::Mbcs);
			EncoderStream logStream(logFileStream, logEncoder);
			StreamWriter logWriter(logStream);

			RunMacro(DereferenceMacroFile(inputPath), outputWriter, logWriter, analyzer);
		}
	}
	Console::WriteLine(L"Finished!");
	Console::Read();
	return 0;
}