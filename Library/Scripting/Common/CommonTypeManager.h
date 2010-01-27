/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::Common

Classes:
	CommonTypeRecord							：类型对象
	CommonFlagTypeRecord<T>						：标号类型对象
	CommonDecoratorTypeRecord<int>				：修饰类型对象
	CommonParameterizedTypeRecord<T>			：带参数修饰类型对象
	CommonTypeManager							：类型管理器
***********************************************************************/

#ifndef VCZH_SCRIPTING_COMMON_COMMONTYPEMANAGER
#define VCZH_SCRIPTING_COMMON_COMMONTYPEMANAGER

#include "..\..\Collections\List.h"
#include "..\..\Collections\Dictionary.h"
#include "..\..\Pointer.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
类型对象
***********************************************************************/

		class CommonTypeRecord : public Object, private NotCopyable
		{
			friend class CommonTypeManager;
		private:
			collections::Dictionary<int, CommonTypeRecord*>	decoratorTypes;
		};

/***********************************************************************
具体类型对象
***********************************************************************/

		template<typename T>
		class CommonFlagTypeRecord : public CommonTypeRecord
		{
		private:
			T												flag;

		protected:
			CommonFlagTypeRecord(T _flag)
				:flag(_flag)
			{
			}

			T InternalGetFlag()
			{
				return flag;
			}
		};

		template<int flag>
		class CommonDecoratorTypeRecord : public CommonTypeRecord
		{
		private:
			CommonTypeRecord*								elementType;

		protected:
			CommonDecoratorTypeRecord(CommonTypeRecord* _elementType)
				:elementType(_elementType)
			{
			}

			CommonTypeRecord* InternalGetElementType()
			{
				return elementType;
			}
		};

		template<typename P>
		class CommonParameterizedTypeRecord : public CommonTypeRecord
		{
		private:
			CommonTypeRecord*								elementType;
			P												parameter;

		protected:
			CommonParameterizedTypeRecord(CommonTypeRecord* _elementType, P _parameter)
				:elementType(_elementType)
				,parameter(_parameter)
			{
			}

			CommonTypeRecord* InternalGetElementType()
			{
				return elementType;
			}

			P InternalGetParameter()
			{
				return parameter;
			}
		};

/***********************************************************************
类型管理器
***********************************************************************/

		class CommonTypeManager : public Object
		{
		private:
			collections::List<Ptr<CommonTypeRecord>>		allocatedTypeRecords;
		protected:
			void											RegisterTypeRecord(CommonTypeRecord* typeRecord);

			template<typename T, typename F>
			CommonTypeRecord* GetFlagTypeRecord(F flag, collections::IDictionary<F, CommonTypeRecord>& typeMap)
			{
				int index=typeMap.Keys().IndexOf(flag);
				if(index==-1)
				{
					CommonFlagTypeRecord<F>* type=new T(flag);
					RegisterTypeRecord(type);
					typeMap.Add(flag, type);
					return type;
				}
				else
				{
					return typeMap.Values()[index];
				}
			}

			template<typename T, int flag>
			CommonTypeRecord* GetDecoratorTypeRecord(CommonTypeRecord* elementType)
			{
				int index=elementType->decoratorTypes.Keys().IndexOf(flag);
				if(index==-1)
				{
					CommonDecoratorTypeRecord<flag>* type=new T(flag);
					RegisterTypeRecord(type);
					elementType->decoratorTypes.Add(flag, type);
				}
				else
				{
					return elementType->declratorTypes.Values()[index];
				}
			}

			template<typename T, typename P>
			CommonTypeRecord* GetCommonParameterizedTypeRecord(CommonTypeRecord* elementType, P parameter, collections::IDictionary<P, CommonTypeRecord>& typeMap)
			{
				int index=typeMap.Keys().IndexOf(parameter);
				if(index==-1)
				{
					CommonParameterizedTypeRecord<P>* type=new T(elementType, parameter);
					RegisterTypeRecord(type);
					typeMap.Add(parameter, type);
					return type;
				}
				else
				{
					return typeMap.Values()[index];
				}
			}

			template<typename T, typename P>
			CommonTypeRecord* GetComplexTypeRecord(P& parameter, collections::IList<CommonTypeRecord*>& types, CommonTypeRecord*(*allocator)(P&), bool(*comparer)(P&, collections::IList<CommonTypeRecord*>&))
			{
				for(int i=0;i<types.Count();i++)
				{
					if(comparer(parameter, types))
					{
						return types[i];
					}
				}
				CommonTypeRecord* type=allocator(parameter);
				RegisterTypeRecord(type);
				return type;
			}
		public:
		};
	}
}

#endif