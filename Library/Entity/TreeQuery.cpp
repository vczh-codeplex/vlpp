#include "TreeQuery.h"
#include "TreeXml.h"
#include "TreeJson.h"
#include "..\Collections\Operation.h"
#include <stdlib.h>

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

		collections::Enumerable<WString> TreeNode::GetAttributeNames()const
		{
			return Enumerable<WString>();
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

		collections::Enumerable<WString> TreeElement::GetAttributeNames()const
		{
			return attributes.Keys();
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
ÎÄµµ¶ÁÐ´
***********************************************************************/

		//---------------------------------------------------------------------------

		Ptr<TreeNode> LoadXmlRawDocument(XmlReader& xml, bool readElementHeadOpening)
		{
			if(!readElementHeadOpening || (xml.Next() && xml.CurrentComponentType()==XmlReader::ElementHeadOpening))
			{
				Ptr<TreeElement> element=new TreeElement;
				element->name=xml.CurrentName();

				while(true)
				{
					if(!xml.Next()) return 0;
					if(xml.CurrentComponentType()==XmlReader::ElementHeadClosing)
					{
						break;
					}
					else if(xml.CurrentComponentType()==XmlReader::Attribute)
					{
						element->attributes.Add(xml.CurrentName(), new TreeText(xml.CurrentValue()));
					}
					else
					{
						return 0;
					}
				}

				while(true)
				{
					if(!xml.Next()) return 0;
					switch(xml.CurrentComponentType())
					{
					case XmlReader::ElementHeadOpening:
						{
							Ptr<TreeNode> child=LoadXmlRawDocument(xml, false);
							if(!child) return 0;
							element->children.Add(child);
						}
						break;
					case XmlReader::Text:
						element->children.Add(new TreeText(xml.CurrentValue()));
						break;
					case XmlReader::CData:
						element->children.Add(new TreeText(xml.CurrentValue()));
						break;
					case XmlReader::Comment:
						element->children.Add(new TreeComment(xml.CurrentValue()));
						break;
					case XmlReader::ElementClosing:
						goto OUT_OF_ELEMENT_CHILDREN_READING;
					default:
						return 0;
					}
				}
				OUT_OF_ELEMENT_CHILDREN_READING:

				return element;
			}
			return 0;
		}

		Ptr<TreeNode> LoadXmlRawDocument(stream::TextReader& reader)
		{
			XmlReader xml(reader);
			return LoadXmlRawDocument(xml, true);
		}

		//---------------------------------------------------------------------------

		void SaveXmlRawDocument(XmlWriter& xml, Ptr<ITreeQuerable> node)
		{
			xml.OpenElement(node->GetName());

			FOREACH(WString, attribute, node->GetAttributeNames())
			{
				xml.WriteAttribute(attribute, node->GetAttribute(attribute)->GetText());
			}

			FOREACH(Ptr<ITreeQuerable>, element, node->GetChildren())
			{
				if(element->IsTextNode())
				{
					WString value=element->GetText();
					const wchar_t* reading=value.Buffer();

					vint textNodeLength=0;
					vint cdataState=0;
					bool invalidCData=false;
					bool spacePrefix=true;
					bool spacePostfix=true;

					while(wchar_t c=*reading++)
					{
						switch(c)
						{
						case ' ':case '\t':case '\r':case '\n':
							spacePrefix=spacePrefix&true;
							spacePostfix=true;
							if(spacePrefix)
							{
								goto FINISH_VALUE_EXAMING;
							}
							break;
						case L']':
							spacePrefix=false;
							spacePostfix=false;
							textNodeLength++;
							if(cdataState<2) cdataState++;
							break;
						case L'\'':
							spacePrefix=false;
							spacePostfix=false;
							textNodeLength+=6;
							cdataState=0;
							break;
						case L'\"':
							spacePrefix=false;
							spacePostfix=false;
							textNodeLength+=6;
							cdataState=0;
							break;
						case L'&':
							spacePrefix=false;
							spacePostfix=false;
							textNodeLength+=5;
							cdataState=0;
							break;
						case L'<':
							spacePrefix=false;
							spacePostfix=false;
							textNodeLength+=4;
							cdataState=0;
							break;
						case L'>':
							spacePrefix=false;
							spacePostfix=false;
							textNodeLength+=4;
							if(cdataState==2)
							{
								invalidCData=true;
								goto FINISH_VALUE_EXAMING;
							}
							else
							{
								cdataState=0;
							}
							break;
						default:
							spacePrefix=false;
							spacePostfix=false;
							textNodeLength++;
							cdataState=0;
						}
					}
					FINISH_VALUE_EXAMING:
					if(textNodeLength<value.Length()+12 || invalidCData || spacePrefix || spacePostfix)
					{
						xml.WriteText(value);
					}
					else
					{
						xml.WriteCData(value);
					}
				}
				else
				{
					SaveXmlRawDocument(xml, element);
				}
			}

			xml.CloseElement();
		}

		bool SaveXmlRawDocument(stream::TextWriter& writer, Ptr<ITreeQuerable> node, bool validate)
		{
			if(validate && !IsValidXmlRawDocument(node))
			{
				return false;
			}
			else
			{
				XmlWriter xml(writer);
				SaveXmlRawDocument(xml, node);
				return true;
			}
		}

		//---------------------------------------------------------------------------

		bool IsValidXmlName(const WString& name)
		{
			const wchar_t* reading=name.Buffer();
			while(wchar_t c=*reading++)
			{
				if(!(L'0'<=c && c<='9' || L'A'<=c && c<='Z' || L'a'<=c && c<='z' || c==L'.' || c==L'-' || c==L':' || c==L'_'))
				{
					return false;
				}
			}
			return name.Length()>0;
		}

		bool IsValidXmlRawDocument(Ptr<ITreeQuerable> node)
		{
			if(!node->IsTextNode())
			{
				if(!IsValidXmlName(node->GetName()))
				{
					return false;
				}

				FOREACH(WString, attribute, node->GetAttributeNames())
				{
					if(!IsValidXmlName(attribute))
					{
						return false;
					}
					Ptr<ITreeQuerable> value=node->GetAttribute(attribute);
					if(!value || !value->IsTextNode())
					{
						return false;
					}
				}

				FOREACH(Ptr<ITreeQuerable>, element, node->GetChildren())
				{
					if(!element || (!element->IsTextNode() && !IsValidXmlRawDocument(element)))
					{
						return false;
					}
				}

				return true;
			}
			else
			{
				return false;
			}
		}

		//***************************************************************************

		Ptr<TreeNode> LoadXmlObjectDocument(XmlReader& xml)
		{
			return 0;
		}

		Ptr<TreeNode> LoadXmlObjectDocument(stream::TextReader& reader)
		{
			XmlReader xml(reader);
			return LoadXmlObjectDocument(xml);
		}

		//---------------------------------------------------------------------------

		void SaveXmlObjectDocument(XmlWriter& xml, Ptr<ITreeQuerable> node)
		{
			if(node->GetName().Length()>6 && node->GetName().Sub(0, 6)==L"array:")
			{
				xml.OpenElement(node->GetName());
				FOREACH(Ptr<ITreeQuerable>, element, node->GetChildren())
				{
					SaveXmlObjectDocument(xml, element);
				}
				xml.CloseElement();
			}
			else if(node->GetName()==L"primitive:null")
			{
				xml.OpenElement(node->GetName());
				xml.CloseElement();
			}
			else if(node->GetName().Length()>10 && node->GetName().Sub(0, 10)==L"primitive:")
			{
				xml.OpenElement(node->GetName());
				xml.WriteAttribute(L"value", (node->GetChildren()>>First())->GetText());
				xml.CloseElement();
			}
			else
			{
				xml.OpenElement(node->GetName());
				FOREACH(WString, attribute, node->GetAttributeNames())
				{
					xml.OpenElement(attribute);
					SaveXmlObjectDocument(xml, node->GetAttribute(attribute));
					xml.CloseElement();
				}
				xml.CloseElement();
			}
		}

		bool SaveXmlObjectDocument(stream::TextWriter& writer, Ptr<ITreeQuerable> node, bool validate)
		{
			if(validate && !IsValidObjectDocument(node))
			{
				return false;
			}
			else
			{
				XmlWriter xml(writer);
				SaveXmlObjectDocument(xml, node);
				return true;
			}
		}

		//***************************************************************************

		Ptr<TreeNode> LoadJsonRawDocument(JsonReader& json, bool readElementHeadOpening)
		{
			if(readElementHeadOpening && !json.Next()) return 0;
			switch(json.CurrentComponentType())
			{
			case JsonReader::Null:
				{
					Ptr<TreeElement> element=new TreeElement;
					element->name=L"json:null";
					return element;
				}
			case JsonReader::Int:
				{
					Ptr<TreeElement> element=new TreeElement;
					element->name=L"json:integer";
					element->attributes.Add(L"json:value", new TreeText(json.CurrentValue()));
					return element;
				}
			case JsonReader::Double:
				{
					Ptr<TreeElement> element=new TreeElement;
					element->name=L"json:double";
					element->attributes.Add(L"json:value", new TreeText(json.CurrentValue()));
					return element;
				}
			case JsonReader::String:
				{
					Ptr<TreeElement> element=new TreeElement;
					element->name=L"json:string";
					element->children.Add(new TreeText(json.CurrentValue()));
					return element;
				}
			case JsonReader::Bool:
				{
					Ptr<TreeElement> element=new TreeElement;
					element->name=L"json:boolean";
					element->attributes.Add(L"json:value", new TreeText(json.CurrentValue()));
					return element;
				}
			case JsonReader::ArrayOpening:
				{
					Ptr<TreeElement> element=new TreeElement;
					element->name=L"json:array";
					while(true)
					{
						if(!json.Next()) return 0;
						if(json.CurrentComponentType()==JsonReader::ArrayClosing)
						{
							break;
						}
						else
						{
							Ptr<TreeNode> value=LoadJsonRawDocument(json, false);
							if(!value) return 0;
							element->children.Add(value);
						}
					}
					return element;
				}
			case JsonReader::ObjectOpening:
				{
					Ptr<TreeElement> element=new TreeElement;
					element->name=L"json:object";
					while(true)
					{
						if(!json.Next()) return 0;
						if(json.CurrentComponentType()==JsonReader::ObjectClosing)
						{
							break;
						}
						else if(json.CurrentComponentType()==JsonReader::Field)
						{
							WString field=json.CurrentValue();
							Ptr<TreeNode> value=LoadJsonRawDocument(json, true);
							if(!value) return 0;
							element->attributes.Add(field, value);
						}
						else
						{
							return 0;
						}
					}
					return element;
				}
			default:
				return 0;
			}
		}

		Ptr<TreeNode> LoadJsonRawDocument(stream::TextReader& reader)
		{
			JsonReader json(reader);
			return LoadJsonRawDocument(json, true);
		}

		//---------------------------------------------------------------------------

		void SaveJsonRawDocument(JsonWriter& json, Ptr<ITreeQuerable> node)
		{
			if(node->IsTextNode())
			{
				json.WriteString(node->GetText());
			}
			else if(node->GetName()==L"json:null")
			{
				json.WriteNull();
			}
			else if(node->GetName()==L"json:integer")
			{
				json.WriteInt(wtoi(node->GetAttribute(L"json:value")->GetText()));
			}
			else if(node->GetName()==L"json:double")
			{
				json.WriteDouble(wtof(node->GetAttribute(L"json:value")->GetText()));
			}
			else if(node->GetName()==L"json:string")
			{
				json.WriteString((node->GetChildren()>>First())->GetText());
			}
			else if(node->GetName()==L"json:boolean")
			{
				json.WriteBool(node->GetAttribute(L"json:value")->GetText()==L"true");
			}
			else if(node->GetName()==L"json:array")
			{
				json.OpenArray();
				FOREACH(Ptr<ITreeQuerable>, element, node->GetChildren())
				{
					SaveJsonRawDocument(json, element);
				}
				json.CloseArray();
			}
			else if(node->GetName()==L"json:object")
			{
				json.OpenObject();
				FOREACH(WString, attribute, node->GetAttributeNames())
				{
					json.AddField(attribute);
					SaveJsonRawDocument(json, node->GetAttribute(attribute));
				}
				json.CloseObject();
			}
		}

		bool SaveJsonRawDocument(stream::TextWriter& writer, Ptr<ITreeQuerable> node, bool validate)
		{
			if(validate && !IsValidJsonRawDocument(node))
			{
				return false;
			}
			else
			{
				JsonWriter json(writer);
				SaveJsonRawDocument(json, node);
				return true;
			}
		}

		//---------------------------------------------------------------------------

		bool IsValidJsonRawDocument(Ptr<ITreeQuerable> node)
		{
			if(!node->IsTextNode())
			{
				if(node->GetName()==L"json:array")
				{
					if(!(node->GetAttributeNames()>>IsEmpty() && node->GetChildren()>>All(IsValidJsonRawDocument)))
						return false;
				}
				else if(node->GetName()==L"json:object")
				{
					if(!(node->GetChildren()>>IsEmpty())) return false;
					FOREACH(WString, attribute, node->GetAttributeNames())
					{
						if(!IsValidJsonRawDocument(node->GetAttribute(attribute)))
							return false;
					}
				}
				else if(node->GetName()==L"json:integer")
				{
					if(node->GetAttributeNames()>>Count()==1 && node->IsAttributeExists(L"json:value") && node->GetChildren()>>IsEmpty())
					{
						Ptr<ITreeQuerable> valueNode=node->GetAttribute(L"json:value");
						if(!valueNode->IsTextNode()) return false;
						WString value=valueNode->GetText();
						wchar_t* endptr=0;
						wcstol(value.Buffer(), &endptr, 10);
						if(*endptr) return false;
					}
					else
					{
						return false;
					}
				}
				else if(node->GetName()==L"json:double")
				{
					if(node->GetAttributeNames()>>Count()==1 && node->IsAttributeExists(L"json:value") && node->GetChildren()>>IsEmpty())
					{
						Ptr<ITreeQuerable> valueNode=node->GetAttribute(L"json:value");
						if(!valueNode->IsTextNode()) return false;
						WString value=valueNode->GetText();
						wchar_t* endptr=0;
						wcstod(value.Buffer(), &endptr);
						if(*endptr) return false;
					}
					else
					{
						return false;
					}
				}
				else if(node->GetName()==L"json:string")
				{
					if(!(node->GetAttributeNames()>>IsEmpty()
						&& node->GetChildren()>>Count()==1
						&& (node->GetChildren()>>First())->IsTextNode()))
						return false;
				}
				else if(node->GetName()==L"json:boolean")
				{
					if(node->GetAttributeNames()>>Count()==1 && node->IsAttributeExists(L"json:value") && node->GetChildren()>>IsEmpty())
					{
						Ptr<ITreeQuerable> valueNode=node->GetAttribute(L"json:value");
						if(!valueNode->IsTextNode()) return false;
						WString value=valueNode->GetText();
						if(value!=L"true" && value!=L"false")
							return false;
					}
					else
					{
						return false;
					}
				}
				else if(node->GetName()==L"json:null")
				{
					if(!(node->GetAttributeNames()>>IsEmpty() && node->GetChildren()>>IsEmpty()))
						return false;
				}
				else
				{
					return false;
				}
			}
			return true;
		}

		//***************************************************************************

		Ptr<TreeNode> LoadJsonObjectDocument(JsonReader& json)
		{
			return 0;
		}

		Ptr<TreeNode> LoadJsonObjectDocument(stream::TextReader& reader)
		{
			JsonReader json(reader);
			return LoadJsonObjectDocument(json);
		}

		//---------------------------------------------------------------------------

		void SaveJsonObjectDocument(JsonWriter& json, Ptr<ITreeQuerable> node)
		{
			if(node->GetName().Length()>6 && node->GetName().Sub(0, 6)==L"array:")
			{
				WString typeName=node->GetName().Sub(6, node->GetName().Length()-6);
				json.OpenObject();
				json.AddField(L"$array");
				json.WriteString(typeName);
				json.AddField(L"value");
				json.OpenArray();
				FOREACH(Ptr<ITreeQuerable>, element, node->GetChildren())
				{
					SaveJsonObjectDocument(json, element);
				}
				json.CloseArray();
				json.CloseObject();
			}
			else if(node->GetName()==L"primitive:null")
			{
				json.WriteNull();
			}
			else if(node->GetName().Length()>10 && node->GetName().Sub(0, 10)==L"primitive:")
			{
				WString typeName=node->GetName().Sub(10, node->GetName().Length()-10);
				json.OpenObject();
				json.AddField(L"$primitive");
				json.WriteString(typeName);
				json.AddField(L"value");
				json.WriteString((node->GetChildren()>>First())->GetText());
				json.CloseObject();
			}
			else
			{
				WString typeName=node->GetName();
				json.OpenObject();
				json.AddField(L"$object");
				json.WriteString(typeName);
				FOREACH(WString, attribute, node->GetAttributeNames())
				{
					json.AddField(attribute);
					SaveJsonObjectDocument(json, node->GetAttribute(attribute));
				}
				json.CloseObject();
			}
		}

		bool SaveJsonObjectDocument(stream::TextWriter& writer, Ptr<ITreeQuerable> node, bool validate)
		{
			if(validate && !IsValidObjectDocument(node))
			{
				return false;
			}
			else
			{
				JsonWriter json(writer);
				SaveJsonObjectDocument(json, node);
				return true;
			}
		}

		//***************************************************************************

		bool IsValidObjectName(const WString& name, bool singleName)
		{
			const wchar_t* reading=name.Buffer();
			while(wchar_t c=*reading++)
			{
				if(!(L'0'<=c && c<='9' || L'A'<=c && c<='Z' || L'a'<=c && c<='z' || (!singleName && c==L'.') || c==L'_'))
				{
					return false;
				}
			}
			return name.Length()>0;
		}

		bool IsValidObjectDocument(Ptr<ITreeQuerable> node)
		{
			if(node->IsTextNode())
				return false;
			if(node->GetName().Length()>=6 && node->GetName().Sub(0, 6)==L"array:")
			{
				if(!IsValidObjectName(node->GetName().Sub(6, node->GetName().Length()-6), false))
					return false;
				if(!(node->GetAttributeNames()>>IsEmpty() && node->GetChildren()>>All(IsValidObjectDocument)))
					return false;
			}
			else if(node->GetName()==L"primitive:null")
			{
				if(!(node->GetAttributeNames()>>IsEmpty() && node->GetChildren()>>IsEmpty()))
					return false;
			}
			else if(node->GetName().Length()>=10 && node->GetName().Sub(0, 10)==L"primitive:")
			{
				if(!(node->GetAttributeNames()>>IsEmpty()))
					return false;
				if(!IsValidObjectName(node->GetName().Sub(10, node->GetName().Length()-10), false))
					return false;
				if(!(node->GetChildren()>>Count()==1 && (node->GetChildren()>>First())->IsTextNode()))
					return false;
			}
			else
			{
				if(!(node->GetChildren()>>IsEmpty()))
					return false;
				if(!IsValidObjectName(node->GetName(), false))
					return false;
				FOREACH(WString, attribute, node->GetAttributeNames())
				{
					if(!IsValidObjectName(attribute, true))
						return false;
					if(!IsValidObjectDocument(node->GetAttribute(attribute)))
						return false;
				}
			}
			return true;
		}
	}
}