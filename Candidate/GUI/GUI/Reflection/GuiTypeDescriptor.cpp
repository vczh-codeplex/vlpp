#include "GuiTypeDescriptor.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
DescriptableObject
***********************************************************************/

		DescriptableObject::DescriptableObject()
			:objectSize(0)
			,typeDescriptor(0)
		{
		}

		DescriptableObject::~DescriptableObject()
		{
		}

/***********************************************************************
DescriptableValue
***********************************************************************/

		void DescriptableValue::Initialize()
		{
			type=0;
			dataSInt=0;
			dataUInt=0;
			dataFloat=0;
			dataBool=false;
			dataChar=0;
			dataDescriptableObjectPointer=0;
			dataObjectPointer=0;
			dataInterfacePointer=0;
			dataRawPointer=0;
		}

		DescriptableValue::DescriptableValue()
		{
			Initialize();
		}

		DescriptableValue::DescriptableValue(IType* type, signed __int64 value)
		{
			Initialize();
			type=type;
			dataSInt=value;
		}

		DescriptableValue::DescriptableValue(signed __int64 value)
		{
			Initialize();
			type=GetTypeProvider()->SInt64();
			dataSInt=value;
		}

		DescriptableValue::DescriptableValue(unsigned __int64 value)
		{
			Initialize();
			type=GetTypeProvider()->UInt64();
			dataUInt=value;
		}

		DescriptableValue::DescriptableValue(double value)
		{
			Initialize();
			type=GetTypeProvider()->Float();
			dataFloat=value;
		}

		DescriptableValue::DescriptableValue(bool value)
		{
			Initialize();
			type=GetTypeProvider()->Bool();
			dataBool=value;
		}

		DescriptableValue::DescriptableValue(wchar_t value)
		{
			Initialize();
			type=GetTypeProvider()->Char();
			dataChar=value;
		}

		DescriptableValue::DescriptableValue(const WString& value)
		{
			Initialize();
			type=GetTypeProvider()->String();
			dataString=value;
		}

		DescriptableValue::DescriptableValue(Ptr<DescriptableObject> value)
		{
			Initialize();
			if(value)
			{
				type=(*value->typeDescriptor)->GetType();
				dataDescriptableObject=value;
				dataDescriptableObjectPointer=value.Obj();
			}
		}

		DescriptableValue::DescriptableValue(DescriptableObject* value)
		{
			Initialize();
			if(value)
			{
				type=(*value->typeDescriptor)->GetType();
				dataDescriptableObjectPointer=value;
			}
		}

		DescriptableValue::DescriptableValue(IType* _type, Ptr<Object> value)
		{
			Initialize();
			if(value)
			{
				type=_type;
				dataObject=value;
				dataObjectPointer=value.Obj();
			}
		}

		DescriptableValue::DescriptableValue(IType* _type, Object* value)
		{
			Initialize();
			if(value)
			{
				type=_type;
				dataObjectPointer=value;
			}
		}

		DescriptableValue::DescriptableValue(IType* _type, Ptr<Interface> value)
		{
			Initialize();
			if(value)
			{
				type=_type;
				dataInterface=value;
				dataInterfacePointer=value.Obj();
			}
		}

		DescriptableValue::DescriptableValue(IType* _type, Interface* value)
		{
			Initialize();
			if(value)
			{
				type=_type;
				dataInterfacePointer=value;
			}
		}

		DescriptableValue::DescriptableValue(IType* _type, void* value)
		{
			Initialize();
			if(value)
			{
				type=_type;
				dataRawPointer=value;
			}
		}

		DescriptableValue::DescriptableValue(const DescriptableValue& value)
			:type(value.type)
			,dataSInt(value.dataSInt)
			,dataUInt(value.dataUInt)
			,dataFloat(value.dataFloat)
			,dataBool(value.dataBool)
			,dataChar(value.dataChar)
			,dataString(value.dataString)
			,dataDescriptableObject(value.dataDescriptableObject)
			,dataDescriptableObjectPointer(value.dataDescriptableObjectPointer)
			,dataObject(value.dataObject)
			,dataObjectPointer(value.dataObjectPointer)
			,dataInterface(value.dataInterface)
			,dataInterfacePointer(value.dataInterfacePointer)
			,dataRawPointer(value.dataRawPointer)
		{
		}

		IType* DescriptableValue::GetType()const
		{
			return type;
		}

		signed __int64 DescriptableValue::GetSInt()const
		{
			return dataSInt;
		}

		unsigned __int64 DescriptableValue::GetUInt()const
		{
			return dataUInt;
		}

		double DescriptableValue::GetFloat()const
		{
			return dataFloat;
		}

		bool DescriptableValue::GetBool()const
		{
			return dataBool;
		}

		wchar_t DescriptableValue::GetChar()const
		{
			return dataChar;
		}

		WString DescriptableValue::GetString()const
		{
			return dataString;
		}
			
		Ptr<DescriptableObject> DescriptableValue::GetDescriptableObject()const
		{
			return dataDescriptableObject;
		}

		DescriptableObject* DescriptableValue::GetDescriptableObjectPoitner()const
		{
			return dataDescriptableObjectPointer;
		}

		Ptr<Object> DescriptableValue::GetRawObject()const
		{
			return dataObject;
		}

		Object* DescriptableValue::GetRawObjectPointer()const
		{
			return dataObjectPointer;
		}

		Ptr<Interface> DescriptableValue::GetInterface()const
		{
			return dataInterface;
		}

		Interface* DescriptableValue::GetInterfacePointer()const
		{
			return dataInterfacePointer;
		}

		void* DescriptableValue::GetRawPointer()const
		{
			return dataRawPointer;
		}

		bool DescriptableValue::CanDelete()const
		{
			return !(dataDescriptableObject || dataObject || dataInterface) && (dataDescriptableObjectPointer || dataObjectPointer || dataInterfacePointer);
		}

		void DescriptableValue::Delete()
		{
			if(dataDescriptableObjectPointer) delete dataDescriptableObjectPointer;
			if(dataObjectPointer) delete dataObjectPointer;
			if(dataInterfacePointer) delete dataInterfacePointer;
			Initialize();
		}

/***********************************************************************
Helper Functions
***********************************************************************/

		ITypeProvider* reflectionTypeProvider=0;

		ITypeProvider* GetTypeProvider()
		{
			return reflectionTypeProvider;
		}

		void SetTypeProvider(ITypeProvider* value)
		{
			if(reflectionTypeProvider!=value)
			{
				if(reflectionTypeProvider)
				{
					delete reflectionTypeProvider;
					reflectionTypeProvider=0;
				}
			}
			reflectionTypeProvider=value;
		}
	}
}