/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Framework::Entity

Classes:
	ITreeQuerable								���ɲ�ѯ��
	TreeQueryParser								����ѯ����
	TreeQuery									����ѯ����

	TreeNode									��ȱʡʵ�֣����ڵ�
	TreeElement									��ȱʡʵ�֣�Ԫ�ؽڵ�
	TreeComment									��ȱʡʵ�֣�ע�ͽڵ�
	TreeText									��ȱʡʵ�֣��ַ����ڵ�
***********************************************************************/

#ifndef VCZH_ENTITY_TREEQUERY
#define VCZH_ENTITY_TREEQUERY

#include "..\String.h"
#include "..\Pointer.h"
#include "..\Collections\Dictionary.h"
#include "..\Collections\OperationEnumerable.h"
#include "..\Stream\Accessor.h"

namespace vl
{
	namespace entities
	{
		namespace queryexpression
		{
		}

/***********************************************************************
�ӿ�
***********************************************************************/

		class ITreeQuerable : public Interface
		{
		public:
			virtual bool											IsTextNode()const=0;
			virtual const WString&									GetText()const=0;
			virtual const WString&									GetName()const=0;
			virtual bool											IsAttributeExists(const WString& name)const=0;
			virtual collections::Enumerable<WString>				GetAttributeNames()const=0;
			virtual Ptr<ITreeQuerable>								GetAttribute(const WString& name)const=0;
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
ȱʡʵ��
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
			collections::Enumerable<WString>						GetAttributeNames()const;
			Ptr<ITreeQuerable>										GetAttribute(const WString& name)const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetElement(const WString& name)const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetElements()const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetChildren()const;
		};

		class TreeElement : public TreeNode
		{
		public:
			WString													name;
			collections::List<Ptr<TreeNode>>						children;
			collections::Dictionary<WString, Ptr<TreeNode>>			attributes;

			bool													IsContent()const;
		protected:
			
			const WString&											GetName()const;
			bool													IsAttributeExists(const WString& name)const;
			collections::Enumerable<WString>						GetAttributeNames()const;
			Ptr<ITreeQuerable>										GetAttribute(const WString& name)const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetElement(const WString& name)const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetElements()const;
			collections::Enumerable<Ptr<ITreeQuerable>>				GetChildren()const;
		};

		class TreeComment : public TreeNode
		{
		public:
			WString													value;

			TreeComment();
			TreeComment(const WString& _value);

			bool													IsContent()const;
		protected:
		};

		class TreeText : public TreeNode
		{
		public:
			WString													value;

			TreeText();
			TreeText(const WString& _value);

			bool													IsContent()const;
		protected:

			bool													IsTextNode()const;
			const WString&											GetText()const;
		};

/***********************************************************************
�ĵ���д
***********************************************************************/

		extern Ptr<TreeNode>					LoadXmlRawDocument(stream::TextReader& reader);
		extern bool								SaveXmlRawDocument(stream::TextWriter& writer, Ptr<ITreeQuerable> node, bool validate);
		extern bool								IsValidXmlRawDocument(Ptr<ITreeQuerable> node);

		extern Ptr<TreeNode>					LoadXmlObjectDocument(stream::TextReader& reader);
		extern bool								SaveXmlObjectDocument(stream::TextWriter& writer, Ptr<ITreeQuerable> node, bool validate);

		extern Ptr<TreeNode>					LoadJsonRawDocument(stream::TextReader& reader);
		extern bool								SaveJsonRawDocument(stream::TextWriter& writer, Ptr<ITreeQuerable> node, bool validate);
		extern bool								IsValidJsonRawDocument(Ptr<ITreeQuerable> node);

		extern Ptr<TreeNode>					LoadJsonObjectDocument(stream::TextReader& reader);
		extern bool								SaveJsonObjectDocument(stream::TextWriter& writer, Ptr<ITreeQuerable> node, bool validate);

		extern bool								IsValidObjectDocument(Ptr<ITreeQuerable> node);
	}
}

#endif