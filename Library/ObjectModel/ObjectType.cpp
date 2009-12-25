#include "ObjectModelIncludes.h"
#include "..\GlobalStorage.h"
#include "..\Collections\Dictionary.h"
#include "..\Collections\List.h"

namespace vl
{
	namespace objectmodel
	{
		using namespace collections;

BEGIN_GLOBAL_STORAGE_CLASS(ObjectTypeManager)
	Ptr<Dictionary<WString, Ptr<ObjectType>>>		types;

	INITIALIZE_GLOBAL_STORAGE_CLASS
		types=new Dictionary<WString, Ptr<ObjectType>>;

	FINALIZE_GLOBAL_STORAGE_CLASS
		types=0;
END_GLOBAL_STORAGE_CLASS(ObjectTypeManager)

		void RegisterObjectType(const WString& name, ObjectType* type)
		{
			GetObjectTypeManager().types->Add(name, type);
		}

		ObjectType* GetObjectType(const WString& name)
		{
			return GetObjectTypeManager().types->Get(name).Obj();
		}

		bool IsObjectTypeRegistered(const WString& name)
		{
			return GetObjectTypeManager().types->Keys().Contains(name);
		}

/***********************************************************************
¸¨Öúº¯Êý
***********************************************************************/

		bool FindBaseClass(const ObjectType* sub, const ObjectType* base, List<int>& baseToSubPositions, List<const ObjectType*>& baseToSubTypes)
		{
			for(int i=0;i<sub->BaseClasses().Count();i++)
			{
				ObjectType* currentBase=sub->BaseClasses()[i];
				if(currentBase==base || FindBaseClass(currentBase, base, baseToSubPositions, baseToSubTypes))
				{
					baseToSubPositions.Add(i);
					baseToSubTypes.Add(sub);
					return true;
				}
			}
			return false;
		}

		ObjectMember* FindClassField(const ObjectType* type, const WString& fieldName)
		{
			int index=type->Fields().Keys().IndexOf(fieldName);
			if(index==-1)
			{
				ObjectMember* member=0;
				for(int i=0;!member && i<type->BaseClasses().Count();i++)
				{
					member=FindClassField(type->BaseClasses()[i], fieldName);
				}
				return member;
			}
			else
			{
				return type->Fields().Values()[index];
			}
		}

		ObjectType* ObjectTypeRemoveCVR(const ObjectType* type)
		{
			while(type->Category()==ObjectType::Decorator || type->Category()==ObjectType::Reference)
			{
				type=type->ElementType();
			}
			return const_cast<ObjectType*>(type);
		}

		bool InvokeMethod(const collections::IReadonlyList<ObjectMember*>& methods, void* object, const collections::IArray<ObjectValue>& arguments, ObjectValue& result, ObjectType* resultType)
		{
			for(int i=0;i<methods.Count();i++)
			{
				ObjectMember* member=methods[i];
				const ObjectType* type=member->Type();
				if(type->Category()==ObjectType::Function && type->ParameterTypes().Count()==arguments.Count())
				{
					Array<ObjectValue> convertedArguments;
					convertedArguments.Resize(arguments.Count());
					for(int j=0;j<arguments.Count();j++)
					{
						const ObjectType* dstType=type->ParameterTypes()[j];
						if(dstType->Category()==ObjectType::Reference)
						{
							dstType=dstType->ElementType();
						}
						ObjectValue argument=arguments.Get(j).ImplicitlyConvertTo(dstType);
						if(argument)
						{
							convertedArguments[j]=argument;
						}
						else
						{
							goto FINISH_CURRENT_METHOD;
						}
					}

					void* invokingResult=0;
					if(arguments.Count())
					{
						Array<void*> pointers;
						pointers.Resize(arguments.Count());
						for(int j=0;j<arguments.Count();j++)
						{
							pointers[j]=convertedArguments[j].GetValue();
						}
						invokingResult=member->Invoke(object, &pointers[0]);
					}
					else
					{
						invokingResult=member->Invoke(object, 0);
					}

					if(type->ReturnType()==TypeOf<void>())
					{
						result=ObjectValue();
					}
					else
					{
						result=ObjectValue(resultType?resultType:type->ReturnType(), invokingResult, true);
					}
					return true;
				}
FINISH_CURRENT_METHOD:;
			}
			return false;
		}

/***********************************************************************
ObjectValue
***********************************************************************/

		bool ObjectValue::InvokeMethod(const ObjectType* type, void* object, const WString& name, const collections::IArray<ObjectValue>& arguments, ObjectValue& result)const
		{
			int index=type->Methods().Keys().IndexOf(name);
			if(index==-1)return false;
			const IReadonlyList<ObjectMember*>& methods=type->Methods().GetByIndex(index);
			if(vl::objectmodel::InvokeMethod(methods, object, arguments, result))
			{
				return true;
			}
			for(int i=0;i<type->BaseClasses().Count();i++)
			{
				ObjectValue base=CastToBaseClass(type->BaseClasses()[i]);
				if(base.InvokeMethod(base.GetType(), base.GetValue(), name, arguments, result))
				{
					return true;
				}
			}
			return false;
		}

		ObjectValue::InternalValue::InternalValue(const ObjectType* _objectType)
			:objectType(_objectType)
			,objectValue(_objectType->CreateInstance())
			,needToDestroy(true)
		{
		}

		ObjectValue::InternalValue::InternalValue(const ObjectType* _objectType, void* _objectValue, bool _needToDestroy)
			:objectType(_objectType)
			,objectValue(_objectValue)
			,needToDestroy(_needToDestroy)
		{
		}

		ObjectValue::InternalValue::~InternalValue()
		{
			if(needToDestroy && objectValue!=0)
			{
				objectType->DestroyInstance(objectValue);
			}
		}

		ObjectValue::ObjectValue()
			:objectValue(0)
			,objectType(0)
		{
		}

		ObjectValue::ObjectValue(const ObjectValue& value)
			:internalValue(value.internalValue)
			,objectValue(value.objectValue)
			,objectType(value.objectType)
		{
		}

		ObjectValue::ObjectValue(const ObjectValue& value, const ObjectType* _objectType, void* _objectValue)
			:internalValue(value.internalValue)
			,objectValue(_objectValue)
			,objectType(_objectType)
		{
		}

		ObjectValue::ObjectValue(const ObjectType* _objectType)
		{
			if(_objectType->CanCreateInstance())
			{
				internalValue=new InternalValue(_objectType);
				objectValue=internalValue->objectValue;
				objectType=internalValue->objectType;
			}
			else
			{
				objectValue=0;
				objectType=0;
			}
		}

		ObjectValue::ObjectValue(const ObjectType* _objectType, void* _objectValue, bool needToDestroy)
		{
			internalValue=new InternalValue(_objectType, _objectValue, needToDestroy);
			objectValue=internalValue->objectValue;
			objectType=internalValue->objectType;
		}

		ObjectValue::~ObjectValue()
		{
		}

		ObjectValue::operator bool()const
		{
			return internalValue;
		}

		bool ObjectValue::operator!()const
		{
			return !internalValue;
		}

		bool ObjectValue::operator==(const ObjectValue& objectValue)const
		{
			return internalValue==objectValue.internalValue;
		}

		bool ObjectValue::operator!=(const ObjectValue& objectValue)const
		{
			return internalValue!=objectValue.internalValue;
		}
		
		bool ObjectValue::operator==(bool value)const
		{
			return internalValue==value;
		}

		bool ObjectValue::operator!=(bool value)const
		{
			return internalValue!=value;
		}

		ObjectType* ObjectValue::GetType()const
		{
			return const_cast<ObjectType*>(objectType);
		}

		void* ObjectValue::GetValue()const
		{
			return objectValue;
		}

		ObjectValue ObjectValue::CastToBaseClass(const ObjectType* type)const
		{
			if(!objectValue)return ObjectValue();
			if(objectType==type)return *this;
			if(objectType->Category()!=ObjectType::Class)return ObjectValue();
			if(type->Category()!=ObjectType::Class)return ObjectValue();

			List<int> baseToSubPositions;
			List<const ObjectType*> baseToSubTypes;
			if(FindBaseClass(objectType, type, baseToSubPositions, baseToSubTypes))
			{
				const ObjectType* currentType=objectType;
				void* instance=objectValue;
				for(int i=baseToSubPositions.Count()-1;i>=0;i--)
				{
					instance=currentType->CastToBaseClass(baseToSubPositions[i], instance);
					currentType=currentType->BaseClasses()[i];
				}
				return ObjectValue(*this, type, instance);
			}
			else
			{
				return ObjectValue();
			}
		}

		ObjectValue ObjectValue::CastToSubClass(const ObjectType* type)const
		{
			if(!objectValue)return ObjectValue();
			if(objectType==type)return *this;
			if(objectType->Category()!=ObjectType::Class)return ObjectValue();
			if(type->Category()!=ObjectType::Class)return ObjectValue();

			List<int> baseToSubPositions;
			List<const ObjectType*> baseToSubTypes;
			if(FindBaseClass(type, objectType, baseToSubPositions, baseToSubTypes))
			{
				void* instance=objectValue;
				for(int i=0;i<baseToSubPositions.Count();i++)
				{
					instance=baseToSubTypes[i]->CastToSubClass(baseToSubPositions[i], instance);
				}
				return ObjectValue(*this, type, instance);
			}
			else
			{
				return ObjectValue();
			}
		}

		ObjectValue ObjectValue::CastToClass(const ObjectType* type)const
		{
			if(!objectValue)return ObjectValue();
			if(objectType==type)return *this;
			if(objectType->Category()!=ObjectType::Class)return ObjectValue();
			if(type->Category()!=ObjectType::Class)return ObjectValue();

			if(ObjectValue result=CastToBaseClass(type))
			{
				return result;
			}
			else
			{
				return CastToSubClass(type);
			}
		}

		ObjectValue ObjectValue::ImplicitlyConvertTo(const ObjectType* type)const
		{
			if(!objectValue)return ObjectValue();
			if(!type)return ObjectValue();
			if(objectType==type)return *this;

			ObjectType* srcType=ObjectTypeRemoveCVR(objectType);
			ObjectType* dstType=ObjectTypeRemoveCVR(type);
			if(srcType->Category()==dstType->Category())
			{
				if(srcType->Category()==ObjectType::Class)
				{
					return ObjectValue(*this, srcType, objectValue).CastToBaseClass(dstType);
				}
				else if(srcType->Category()==ObjectType::Pointer)
				{
					ObjectType* srcClass=ObjectTypeRemoveCVR(srcType->ElementType());
					ObjectType* dstClass=ObjectTypeRemoveCVR(dstType->ElementType());
					void* value=*(void**)objectValue;
					if(srcClass->Category()==dstClass->Category() && srcClass->Category()==ObjectType::Class)
					{
						value=ObjectValue(srcClass, value, false).CastToBaseClass(dstClass).GetValue();
						if(value)
						{
							ObjectMember* constructor=dstType->Constructors()[1];
							void* argument=&value;
							value=constructor->Invoke(0, &argument);
							return ObjectValue(type, value, true);
						}
					}
				}
			}
			return ObjectValue();
		}

		ObjectValue ObjectValue::GetField(const WString& name)const
		{
			if(!objectValue)return ObjectValue();
			if(objectType->Category()!=ObjectType::Class)return ObjectValue();
			ObjectMember* field=FindClassField(objectType, name);
			if(field)
			{
				ObjectValue value=CastToBaseClass(field->OwnerType());
				return ObjectValue(field->Type(), field->GetValue(value.GetValue()), false);
			}
			else
			{
				return ObjectValue();
			}
		}

		bool ObjectValue::SetField(const WString& name, const ObjectValue& value)const
		{
			if(!objectValue)return false;
			if(objectType->Category()!=ObjectType::Class)return false;
			ObjectMember* field=FindClassField(objectType, name);
			if(!field)return false;

			ObjectValue destValue=value.ImplicitlyConvertTo(field->Type());
			if(!destValue)return false;
			ObjectValue ownerValue=CastToBaseClass(field->OwnerType());
			field->SetValue(ownerValue.GetValue(), destValue.GetValue());
			return true;
		}

		bool ObjectValue::InvokeMethod(const WString& name, const collections::IArray<ObjectValue>& arguments, ObjectValue& result)const
		{
			if(!objectValue)return false;
			if(objectType->Category()!=ObjectType::Class)false;
			return InvokeMethod(objectType, objectValue, name, arguments, result);
		}

/***********************************************************************
Ô¤¶¨Òå
***********************************************************************/

		class ObjectTypeManagerInitializer
		{
		public:
			ObjectTypeManagerInitializer()
			{
				RegisterObjectType<void>();
				RegisterObjectType<signed __int8>();
				RegisterObjectType<unsigned __int8>();
				RegisterObjectType<signed __int16>();
				RegisterObjectType<unsigned __int16>();
				RegisterObjectType<signed __int32>();
				RegisterObjectType<unsigned __int32>();
				RegisterObjectType<signed __int64>();
				RegisterObjectType<unsigned __int64>();
				RegisterObjectType<char>();
				RegisterObjectType<wchar_t>();
				RegisterObjectType<bool>();
				RegisterObjectType<float>();
				RegisterObjectType<double>();
			}
		};

		void EnsureObjectTypeInitialized()
		{
			static ObjectTypeManagerInitializer initializer;
		}

		class ObjectTypeManagerInitializerInvoker
		{
		public:
			ObjectTypeManagerInitializerInvoker()
			{
				EnsureObjectTypeInitialized();
			}
		} objectTypeManagerInitializerInvoker;
	}
}