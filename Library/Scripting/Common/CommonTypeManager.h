/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::Common

Classes:
	CommonTypeRecord<I>							：类型对象
	CommonFlagTypeRecord<I, T>					：标号类型对象
	CommonDecoratorTypeRecord<I, vint>			：修饰类型对象
	CommonParameterizedTypeRecord<I, T>			：带参数修饰类型对象
	CommonTypeManager<I>						：类型管理器
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

		template<typename I>
		class CommonTypeManager;

		template<typename I>
		class CommonTypeRecord : public I
		{
			friend class CommonTypeManager<I>;
		private:
			collections::Dictionary<vint, CommonTypeRecord*>	decoratorTypes;

		protected:
		};

/***********************************************************************
具体类型对象
***********************************************************************/

		template<typename I, typename T>
		class CommonFlagTypeRecord : public CommonTypeRecord<I>
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

		template<typename I, vint flag>
		class CommonDecoratorTypeRecord : public CommonTypeRecord<I>
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

		template<typename I, typename P>
		class CommonParameterizedTypeRecord : public CommonTypeRecord<I>
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

		template<typename I>
		class CommonTypeManager : public Object
		{
		private:
			collections::List<Ptr<CommonTypeRecord<I>>>		allocatedTypeRecords;
		protected:
			void RegisterTypeRecord(CommonTypeRecord<I>* typeRecord)
			{
				allocatedTypeRecords.Add(typeRecord);
			}

			template<typename F>
			CommonTypeRecord<I>* GetFlagTypeRecord(
				F flag,
				collections::IDictionary<F, CommonTypeRecord<I>*>& typeMap,
				CommonFlagTypeRecord<I, F>*(*allocator)(F)
				)
			{
				vint index=typeMap.Keys().IndexOf(flag);
				if(index==-1)
				{
					CommonFlagTypeRecord<I, F>* type=allocator(flag);
					RegisterTypeRecord(type);
					typeMap.Add(flag, type);
					return type;
				}
				else
				{
					return typeMap.Values()[index];
				}
			}

			template<vint flag>
			CommonTypeRecord<I>* GetDecoratorTypeRecord(
				CommonTypeRecord<I>* elementType,
				CommonDecoratorTypeRecord<I, flag>*(*allocator)(CommonTypeRecord<I>*)
				)
			{
				vint index=elementType->decoratorTypes.Keys().IndexOf(flag);
				if(index==-1)
				{
					CommonDecoratorTypeRecord<I, flag>* type=allocator(elementType);
					RegisterTypeRecord(type);
					elementType->decoratorTypes.Add(flag, type);
					return type;
				}
				else
				{
					return elementType->decoratorTypes.Values()[index];
				}
			}

			template<typename P>
			CommonTypeRecord<I>* GetCommonParameterizedTypeRecord(
				CommonTypeRecord<I>* elementType,
				P parameter,
				collections::IDictionary<P, CommonTypeRecord<I>*>& typeMap,
				CommonParameterizedTypeRecord<I, P>*(*allocator)(CommonTypeRecord<I>*, P)
				)
			{
				vint index=typeMap.Keys().IndexOf(parameter);
				if(index==-1)
				{
					CommonParameterizedTypeRecord<I, P>* type=allocator(elementType, parameter);
					RegisterTypeRecord(type);
					typeMap.Add(parameter, type);
					return type;
				}
				else
				{
					return typeMap.Values()[index];
				}
			}

			template<typename P>
			CommonTypeRecord<I>* GetComplexTypeRecord(
				P& parameter,
				collections::IList<CommonTypeRecord<I>*>& types,
				CommonTypeRecord<I>*(*allocator)(P&),
				bool(*comparer)(P&, CommonTypeRecord<I>*))
			{
				for(vint i=0;i<types.Count();i++)
				{
					if(comparer(parameter, types.Get(i)))
					{
						return types.Get(i);
					}
				}
				CommonTypeRecord<I>* type=allocator(parameter);
				types.Add(type);
				RegisterTypeRecord(type);
				return type;
			}
		public:
		};
	}
}

#endif