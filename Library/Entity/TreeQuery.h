#include "..\String.h"
#include "..\Pointer.h"
#include "..\Collections\Dictionary.h"
#include "..\Collections\OperationEnumerable.h"

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
			virtual bool											IsTextNode()const=0;
			virtual const WString&									GetText()const=0;
			virtual const WString&									GetName()const=0;
			virtual bool											IsAttributeExists(const WString& name)const=0;
			virtual const WString&									GetAttribute(const WString& name)const=0;
			virtual collections::Enumerable<Ptr<ITreeQuerable>>		GetElement(const WString& name)const=0;
			virtual collections::Enumerable<Ptr<ITreeQuerable>>		GetElements()const=0;
			virtual collections::Enumerable<Ptr<ITreeQuerable>>		GetChildren()const=0;
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

		class TreeNode : public Object, protected ITreeQuerable
		{
			friend class Ptr<ITreeQuerable>;
		public:

			virtual bool											IsContent()const=0;
		protected:

			bool													IsTextNode()const;
			const WString&											GetText()const;
			const WString&											GetName()const;
			bool													IsAttributeExists(const WString& name)const;
			const WString&											GetAttribute(const WString& name)const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetElement(const WString& name)const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetElements()const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetChildren()const;
		};

		class TreeElement : public TreeNode
		{
		public:
			WString										name;
			collections::List<Ptr<TreeNode>>			children;
			collections::Dictionary<WString, WString>	attributes;

			bool													IsContent()const;
		protected:
			
			const WString&											GetName()const;
			bool													IsAttributeExists(const WString& name)const;
			const WString&											GetAttribute(const WString& name)const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetElement(const WString& name)const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetElements()const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetChildren()const;
		};

		class TreeComment : public TreeNode
		{
		public:
			WString										value;

			TreeComment();
			TreeComment(const WString& _value);

			bool													IsContent()const;
		protected:
		};

		class TreeText : public TreeNode
		{
		public:
			WString										value;

			TreeText();
			TreeText(const WString& _value);

			bool													IsContent()const;
		protected:

			bool													IsTextNode()const;
			const WString&											GetText()const;
		};
	}
}