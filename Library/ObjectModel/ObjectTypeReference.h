/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
ObjectModel::ObjectType for reference

Classes:
***********************************************************************/

#ifndef VCZH_OBJECTMODEL_OBJECTTYPEREFERENCE
#define VCZH_OBJECTMODEL_OBJECTTYPEREFERENCE

#include "ObjectType.h"

namespace vl
{
	namespace objectmodel
	{
		template<typename T>
		class ObjectTypeReference : public SpecifiedObjectType<T&>
		{
		protected:
			ObjectType*							elementType;
		public:
			ObjectTypeReference()
			{
				elementType=TypeOf<T>();
			}

			ObjectCategory Category()const
			{
				return ObjectType::Reference;
			}

			const ObjectType* ElementType()const
			{
				return elementType;
			}
		};

		template<typename T>
		class ObjectTypeInformation<T&>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<T>::GetName()+L"&";
			}

			typedef ObjectTypeReference<T> ObjectTypeImplementation;
		};
	}
}

#endif