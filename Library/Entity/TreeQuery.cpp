#include "TreeQuery.h"

namespace vl
{
	namespace entities
	{
		using namespace collections;

		namespace queryexpression
		{
		}

/***********************************************************************
TreeAttribute
***********************************************************************/

		const WString& TreeAttribute::GetName()const
		{
			return name;
		}

		const WString& TreeAttribute::GetValue()const
		{
			return value;
		}
			
		const IEnumerable<Ptr<ITreeQuerable>>& TreeAttribute::QuerableAttributes()const
		{
			return nothing;
		}

		const IEnumerable<Ptr<ITreeQuerable>>& TreeAttribute::QuerableElements()const
		{
			return nothing;
		}

/***********************************************************************
TreeElement
***********************************************************************/

		TreeElement::Querables::Querables(
			const collections::IEnumerable<Ptr<TreeAttribute>>& attributes,
			const collections::IEnumerable<Ptr<TreeElement>>& elements
			)
			:querableAttributes(attributes, ConvertAttribute)
			,querableElements(elements, ConvertElement)
		{
		}

		TreeElement::TreeElement()
		{
			querables=new Querables(attributes.Wrap(), elements.Wrap());
		}

		Ptr<ITreeQuerable> TreeElement::ConvertAttribute(Ptr<TreeAttribute> value)
		{
			return value;
		}

		Ptr<ITreeQuerable> TreeElement::ConvertElement(Ptr<TreeElement> value)
		{
			return value;
		}

		const WString& TreeElement::GetName()const
		{
			return name;
		}

		const WString& TreeElement::GetValue()const
		{
			return WString::Empty;
		}
			
		const IEnumerable<Ptr<ITreeQuerable>>& TreeElement::QuerableAttributes()const
		{
			return querables->querableAttributes;
		}

		const IEnumerable<Ptr<ITreeQuerable>>& TreeElement::QuerableElements()const
		{
			return querables->querableElements;
		}
	}
}