using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FvCalculation.PrimitiveExpressions;

namespace FvCalculation
{
    public abstract class Expression
    {
        public const double ZeroNumber = 0.000001;

        public static Expression Parse(string s)
        {
            return ExpressionParser.Parse(s);
        }

        public abstract double Execute(Dictionary<string, double> variables);
        public abstract Expression Apply(Dictionary<string, double> variables);
        public abstract Expression Different(string variable);
        public abstract bool ContainsVariable(string variable);
        public abstract Expression SimplifyInternal();
        public abstract string ToCode();

        public Expression Simplify()
        {
            try
            {
                return new NumberExpression
                {
                    Number = Execute(new Dictionary<string, double>()),
                };
            }
            catch (KeyNotFoundException)
            {
                Expression s = SimplifyInternal();
                try
                {
                    return new NumberExpression
                    {
                        Number = s.Execute(new Dictionary<string, double>()),
                    };
                }
                catch (KeyNotFoundException)
                {
                    return s;
                }
            }
        }

        public override string ToString()
        {
            return ToCode();
        }
    }

    public class FunctionNameAttribute : Attribute
    {
        public string Name { get; set; }

        public FunctionNameAttribute(string name)
        {
            this.Name = name;
        }
    };

    public abstract class FunctionExpression : Expression
    {
        private static Dictionary<string, Type> functionExpressionTypes = null;

        private string name = null;

        public Expression Op { get; set; }

        public string Name
        {
            get
            {
                if (this.name == null)
                {
                    this.name = ((FunctionNameAttribute)this.GetType().GetCustomAttributes(typeof(FunctionNameAttribute), false).First()).Name;
                }
                return this.name;
            }
        }

        public override Expression Apply(Dictionary<string, double> variables)
        {
            FunctionExpression f = (FunctionExpression)this.GetType().GetConstructor(new Type[] { }).Invoke(new object[] { });
            f.Op = this.Op.Apply(variables);
            return f;
        }

        public override Expression SimplifyInternal()
        {
            FunctionExpression f = (FunctionExpression)this.GetType().GetConstructor(new Type[] { }).Invoke(new object[] { });
            f.Op = this.Op.Simplify();
            return f;
        }

        public override string ToCode()
        {
            return this.Name + "(" + this.Op.ToCode() + ")";
        }

        public static FunctionExpression FromName(string name)
        {
            if (functionExpressionTypes == null)
            {
                functionExpressionTypes = new Dictionary<string, Type>();
                foreach (var type in typeof(FunctionExpression).Assembly.GetTypes())
                {
                    if (!type.IsAbstract && typeof(FunctionExpression).IsAssignableFrom(type))
                    {
                        FunctionNameAttribute att = (FunctionNameAttribute)type.GetCustomAttributes(typeof(FunctionNameAttribute), false).First();
                        functionExpressionTypes.Add(att.Name, type);
                    }
                }
            }
            Type funcType = null;
            if (functionExpressionTypes.TryGetValue(name, out funcType))
            {
                return (FunctionExpression)funcType.GetConstructor(new Type[] { }).Invoke(new object[] { });
            }
            else
            {
                return null;
            }
        }
    }
}
