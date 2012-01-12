/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Reflection

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUITYPEDESCRIPTOR
#define VCZH_PRESENTATION_REFLECTION_GUITYPEDESCRIPTOR

#include "..\NativeWindow\GuiTypes.h"

namespace vl
{
	namespace presentation
	{
		class ITypeDescriptor;

		class DescriptableObject
		{
			template<typename T>
			friend class Description;
		protected:
			size_t							objectSize;
			ITypeDescriptor**				typeDescriptor;
		public:
			DescriptableObject();
			virtual ~DescriptableObject();
		};

		class IDescriptable : public virtual Interface, public virtual DescriptableObject
		{
		public:
			~IDescriptable(){}
		};
		
		template<typename T>
		class Description : public virtual DescriptableObject
		{
			template<typename T>
			friend class DescriptionAssigner;
		protected:
			static ITypeDescriptor*			associatedTypeDescriptor;
		public:
			Description()
			{
				if(objectSize<sizeof(T))
				{
					objectSize=sizeof(T);
					typeDescriptor=&associatedTypeDescriptor;
				}
			}
		};

		template<typename T>
		ITypeDescriptor* Description<T>::associatedTypeDescriptor=0;
	}
}

#endif