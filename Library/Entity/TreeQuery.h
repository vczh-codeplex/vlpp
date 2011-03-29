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

		class ITreeQuerable : public Interface
		{
		public:
			virtual const WString&											GetName()const=0;
			virtual const WString&											GetValue()const=0;
			virtual const collections::IEnumerable<Ptr<ITreeQuerable>>&		QuerableAttributes()const=0;
			virtual const collections::IEnumerable<Ptr<ITreeQuerable>>&		QuerableElements()const=0;
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

		class TreeAttribute : public Object, private ITreeQuerable
		{
			friend class Ptr<ITreeQuerable>;
		public:
			WString												name;
			WString												value;

		private:
			collections::EmptyEnumerable<Ptr<ITreeQuerable>>	nothing;

			const WString&										GetName()const;
			const WString&										GetValue()const;
			const collections::IEnumerable<Ptr<ITreeQuerable>>&	QuerableAttributes()const;
			const collections::IEnumerable<Ptr<ITreeQuerable>>&	QuerableElements()const;
		};

		class TreeElement: public Object, private ITreeQuerable
		{
			friend class Ptr<ITreeQuerable>;
			typedef collections::SelectEnumerable<Ptr<TreeAttribute>, Ptr<ITreeQuerable>>	QuerableAttributeEnumerable;
			typedef collections::SelectEnumerable<Ptr<TreeElement>, Ptr<ITreeQuerable>>		QuerableElementEnumerable;
		private:
			struct Querables
			{
				QuerableAttributeEnumerable			querableAttributes;
				QuerableElementEnumerable			querableElements;

				Querables(
					const collections::IEnumerable<Ptr<TreeAttribute>>& attributes,
					const collections::IEnumerable<Ptr<TreeElement>>& elements
					);
			};

			Ptr<Querables>										querables;
		public:
			WString												name;
			collections::List<Ptr<TreeAttribute>>				attributes;
			collections::List<Ptr<TreeElement>>					elements;

			TreeElement();
		private:

			static Ptr<ITreeQuerable>							ConvertAttribute(Ptr<TreeAttribute> value);
			static Ptr<ITreeQuerable>							ConvertElement(Ptr<TreeElement> value);
			const WString&										GetName()const;
			const WString&										GetValue()const;
			const collections::IEnumerable<Ptr<ITreeQuerable>>&	QuerableAttributes()const;
			const collections::IEnumerable<Ptr<ITreeQuerable>>&	QuerableElements()const;
		};
	}
}