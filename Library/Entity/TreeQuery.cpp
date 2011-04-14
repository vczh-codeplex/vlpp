#include "TreeQuery.h"
#include "TreeXml.h"
#include "TreeJson.h"
#include "..\Collections\Operation.h"

namespace vl
{
	namespace entities
	{
		using namespace collections;

		namespace queryexpression
		{
		}

/***********************************************************************
TreeNode
***********************************************************************/

		bool TreeNode::IsTextNode()const
		{
			return false;
		}

		const WString& TreeNode::GetText()const
		{
			return WString::Empty;
		}

		const WString& TreeNode::GetName()const
		{
			return WString::Empty;
		}

		bool TreeNode::IsAttributeExists(const WString& name)const
		{
			return false;
		}

		Ptr<ITreeQuerable> TreeNode::GetAttribute(const WString& name)const
		{
			return 0;
		}

		Enumerable<Ptr<ITreeQuerable>> TreeNode::GetElement(const WString& name)const
		{
			return Enumerable<Ptr<ITreeQuerable>>();
		}

		Enumerable<Ptr<ITreeQuerable>> TreeNode::GetElements()const
		{
			return Enumerable<Ptr<ITreeQuerable>>();
		}

		Enumerable<Ptr<ITreeQuerable>> TreeNode::GetChildren()const
		{
			return Enumerable<Ptr<ITreeQuerable>>();
		}

/***********************************************************************
TreeElement
***********************************************************************/

		namespace TreeElementHelper
		{
			Ptr<ITreeQuerable> ConvertNode(Ptr<TreeNode> node)
			{
				return node;
			}

			bool IsContentNode(Ptr<TreeNode> node)
			{
				return node->IsContent();
			}

			bool IsElementNode(Ptr<TreeNode> node)
			{
				return node.Cast<TreeElement>();
			}

			bool IsElementWithName(const WString& name, Ptr<TreeNode> node)
			{
				Ptr<TreeElement> element=node.Cast<TreeElement>();
				return element && element->name==name;
			}
		};
		using namespace TreeElementHelper;
		
		bool TreeElement::IsContent()const
		{
			return true;
		}

		const WString& TreeElement::GetName()const
		{
			return name;
		}

		bool TreeElement::IsAttributeExists(const WString& name)const
		{
			return attributes.Keys().Contains(name);
		}

		Ptr<ITreeQuerable> TreeElement::GetAttribute(const WString& name)const
		{
			vint index=attributes.Keys().IndexOf(name);
			return index==-1?0:attributes.Values()[index];
		}

		Enumerable<Ptr<ITreeQuerable>> TreeElement::GetElement(const WString& name)const
		{
			return children.Wrap()>>Where(Curry(IsElementWithName)(name))>>Select(ConvertNode);
		}

		Enumerable<Ptr<ITreeQuerable>> TreeElement::GetElements()const
		{
			return children.Wrap()>>Where(IsElementNode)>>Select(ConvertNode);
		}

		Enumerable<Ptr<ITreeQuerable>> TreeElement::GetChildren()const
		{
			return children.Wrap()>>Where(IsContentNode)>>Select(ConvertNode);
		}

/***********************************************************************
TreeComment
***********************************************************************/

		TreeComment::TreeComment()
		{
		}

		TreeComment::TreeComment(const WString& _value)
			:value(_value)
		{
		}
		
		bool TreeComment::IsContent()const
		{
			return false;
		}

/***********************************************************************
TreeText
***********************************************************************/

		TreeText::TreeText()
		{
		}

		TreeText::TreeText(const WString& _value)
			:value(_value)
		{
		}
		
		bool TreeText::IsContent()const
		{
			return true;
		}

		bool TreeText::IsTextNode()const
		{
			return true;
		}

		const WString& TreeText::GetText()const
		{
			return value;
		}

/***********************************************************************
�ĵ���д
***********************************************************************/

		Ptr<TreeNode> LoadXmlRawDocument(stream::TextReader& reader)
		{
			return 0;
		}

		Ptr<TreeNode> LoadXmlObjectDocument(stream::TextReader& reader)
		{
			return 0;
		}

		bool SaveXmlRawDocument(stream::TextWriter& writer, Ptr<TreeNode> node)
		{
			return false;
		}

		bool SaveXmlObjectDocument(stream::TextWriter& writer, Ptr<TreeNode> node)
		{
			return false;
		}

		bool IsValidXmlRawDocument(Ptr<TreeNode> node)
		{
			return false;
		}

		Ptr<TreeNode> LoadJsonRawDocument(stream::TextReader& reader)
		{
			return 0;
		}

		Ptr<TreeNode> LoadJsonObjectDocument(stream::TextReader& reader)
		{
			return 0;
		}

		bool SaveJsonRawDocument(stream::TextWriter& writer, Ptr<TreeNode> node)
		{
			return false;
		}

		bool SaveJsonObjectDocument(stream::TextWriter& writer, Ptr<TreeNode> node)
		{
			return false;
		}

		bool IsValidJsonRawDocument(Ptr<TreeNode> node)
		{
			return false;
		}

		bool IsValidObjectDocument(Ptr<TreeNode> node)
		{
			return false;
		}
	}
}