#include "..\String.h"
#include "..\Pointer.h"
#include "..\Collections\Dictionary.h"
#include "..\Collections\Operation.h"

namespace vl
{
	namespace entities
	{
		namespace queryexpression
		{
		}

/***********************************************************************
接口
***********************************************************************/

		class ITreeQuerableAttribute : public Interface
		{
		public:
			virtual const WString&			GetName()const=0;
			virtual const WString&			GetValue()const=0;
		};

		class ITreeQuerableElement : public Interface
		{
		public:
			virtual const collections::IEnumerable<Ptr<ITreeQuerableAttribute>>&	QuerableAttributes()const=0;
			virtual const collections::IEnumerable<Ptr<ITreeQuerableElement>>&		QuerableElements()const=0;
		};

		class TreeQueryParser : public Object
		{
		};

		class TreeQuery : public Object
		{
		};

/***********************************************************************
缺省实现
***********************************************************************/

		class TreeAttribute : public Object, protected ITreeQuerableAttribute
		{
			friend class Ptr<ITreeQuerableAttribute>;
		public:
			WString				name;
			WString				value;

		protected:

			const WString& GetName()const
			{
				return name;
			}

			const WString& GetValue()const
			{
				return value;
			}
		};

		class TreeElement: public Object, protected ITreeQuerableElement
		{
			friend class Ptr<ITreeQuerableElement>;
			typedef collections::SelectEnumerable<Ptr<TreeAttribute>, Ptr<ITreeQuerableAttribute>>	QuerableAttributeEnumerable;
			typedef collections::SelectEnumerable<Ptr<TreeElement>, Ptr<ITreeQuerableElement>>		QuerableElementEnumerable;
		private:
			struct Querables
			{
				QuerableAttributeEnumerable			querableAttributes;
				QuerableElementEnumerable			querableElements;

				Querables(
					const collections::IEnumerable<Ptr<TreeAttribute>>& attributes,
					const collections::IEnumerable<Ptr<TreeElement>>& elements
					)
					:querableAttributes(attributes, ConvertAttribute)
					,querableElements(elements, ConvertElement)
				{
				}
			};

			Ptr<Querables>							querables;
		public:
			collections::List<Ptr<TreeAttribute>>	attributes;
			collections::List<Ptr<TreeElement>>		elements;

			TreeElement()
			{
				querables=new Querables(attributes.Wrap(), elements.Wrap());
			}
		protected:

			static Ptr<ITreeQuerableAttribute> ConvertAttribute(Ptr<TreeAttribute> value)
			{
				return value;
			}

			static Ptr<ITreeQuerableElement> ConvertElement(Ptr<TreeElement> value)
			{
				return value;
			}
			
			const collections::IEnumerable<Ptr<ITreeQuerableAttribute>>& QuerableAttributes()const
			{
				return querables->querableAttributes;
			}

			const collections::IEnumerable<Ptr<ITreeQuerableElement>>&	 QuerableElements()const
			{
				return querables->querableElements;
			}
		};
	}
}