#include "GcSingleThreadEntity.h"
#include "..\Collections\List.h"
#include <memory.h>

namespace vl
{
	namespace entities
	{
		using namespace collections;

/***********************************************************************
GcSingleThread
***********************************************************************/

		GcSingleThread::GcSingleThread(Callback _callback, void* _userData, vint poolUnitSize, vint poolUnitCount)
			:callback(_callback)
			,userData(_userData)
			,pool(poolUnitSize, poolUnitCount)
			,maxSize(poolUnitSize*poolUnitCount)
			,usedSize(0)
			,firstObject(0)
			,lastObject(0)
		{
		}

		GcSingleThread::~GcSingleThread()
		{
		}

		GcHandle* GcSingleThread::CreateHandle(GcMeta* meta, vint repeat)
		{
			if(repeat<0)repeat=0;
			vint size=sizeof(ObjectHead)+meta->mainSegment.size+repeat*meta->repeatSegment.size;
			if(usedSize+size>maxSize)
			{
				Collect();
			}
			ObjectHead* o=(ObjectHead*)pool.Alloc(size);
			if(o)
			{
				o->meta=meta;
				o->prev=0;
				o->next=0;
				o->ref=1;
				o->pin=0;
				o->repeat=(__int32)repeat;
				o->mark=false;
				usedSize+=size;
				memset(GetObjectAddress(o), 0, GetObjectSize(o));

				if(firstObject&&lastObject)
				{
					lastObject->next=o;
					o->prev=lastObject;
					lastObject=o;
				}
				else
				{
					firstObject=o;
					lastObject=o;
				}
			}
			return (GcHandle*)o;
		}

		bool GcSingleThread::IsValidHandle(GcHandle* handle)
		{
			return pool.IsValid((char*)handle);
		}

		GcMeta* GcSingleThread::GetHandleMeta(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return o?o->meta:0;
		}

		bool GcSingleThread::IncreaseHandleRef(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->ref!=MaxRef)
			{
				o->ref++;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool GcSingleThread::DecreaseHandleRef(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->ref)
			{
				o->ref--;
				return true;
			}
			else
			{
				return false;
			}
		}

		char* GcSingleThread::IncreaseHandlePin(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta&&o->pin!=MaxPin)
			{
				o->pin++;
				return GetObjectAddress(o);
			}
			else
			{
				return 0;
			}
		}

		bool GcSingleThread::DecreaseHandlePin(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta&&o->pin)
			{
				o->pin--;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool GcSingleThread::DisposeHandle(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta)
			{
				o->meta=0;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool GcSingleThread::IsHandleDisposed(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return !(&o->meta);
		}

		vint GcSingleThread::GetHandleSize(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return o&&o->meta?GetObjectSize(o):-1;
		}

		vint GcSingleThread::GetHandleRepeat(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return o&&o->meta?o->repeat:-1;
		}

		GcHandle** GcSingleThread::GetHandleAddress(GcHandle* handle, vint repeat, vint index)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta)
			{
				GcMetaSegment* segment=0;
				vint segmentOffset=0;
				if(repeat<0)
				{
					segment=&o->meta->mainSegment;
				}
				else if(repeat<o->repeat)
				{
					segment=&o->meta->repeatSegment;
					segmentOffset=o->meta->mainSegment.size+repeat*o->meta->repeatSegment.size;
				}
				if(segment&&0<=index&&index<segment->subHandleCount)
				{
					vint offset=segmentOffset+segment->subHandles[index];
					if(0<=offset&&(vint)(offset+sizeof(GcHandle*))<=GetObjectSize(o))
					{
						return (GcHandle**)(GetObjectAddress(o)+offset);
					}
				}
			}
			return 0;
		}

		bool GcSingleThread::ReadHandle(GcHandle* handle, vint repeat, vint index, GcHandle** value, bool increaseRef)
		{
			GcHandle** address=GetHandleAddress(handle, repeat, index);
			if(address)
			{
				*value=*address;
				if(increaseRef&&*value&&!IncreaseHandleRef(*value))return false;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool GcSingleThread::WriteHandle(GcHandle* handle, vint repeat, vint index, GcHandle* value, bool decreaseRef)
		{
			GcHandle** address=GetHandleAddress(handle, repeat, index);
			if(address)
			{
				*address=value;
				if(decreaseRef&&value&&!DecreaseHandleRef(value))return false;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool GcSingleThread::Read(GcHandle* handle, vint offset, vint length, char* buffer)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta)
			{
				vint size=GetObjectSize(o);
				if(offset>=0 && length>=0 && offset+length<=size)
				{
					char* source=GetObjectAddress(o);
					memmove(buffer, source+offset, length);
					return true;
				}
			}
			return false;
		}

		bool GcSingleThread::Write(GcHandle* handle, vint offset, vint length, char* buffer)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta)
			{
				vint size=GetObjectSize(o);
				if(offset>=0 && length>=0 && offset+length<=size)
				{
					char* source=GetObjectAddress(o);
					memmove(source+offset, buffer, length);
					return true;
				}
			}
			return false;
		}

		bool GcSingleThread::Copy(GcHandle* hDst, vint oDst, GcHandle* hSrc, vint oSrc, vint length)
		{
			ObjectHead* w=GetObjectHead(hDst);
			ObjectHead* r=GetObjectHead(hSrc);
			if(w&&w->meta&&r&&r->meta)
			{
				vint sw=GetObjectSize(w);
				vint sr=GetObjectSize(r);
				if(length>=0 && oDst>=0 && oDst+length<=sw && oSrc>=0 && oSrc+length<=sr)
				{
					char* aw=GetObjectAddress(w);
					char* ar=GetObjectAddress(r);
					memmove(aw+oDst, ar+oSrc, length);
					return true;
				}
			}
			return false;
		}

		void GcSingleThread::MarkSegment(GcMetaSegment* segment, char* address, collections::List<ObjectHead*>& roots)
		{
			for(vint i=0;i<segment->subHandleCount;i++)
			{
				GcHandle* h=*(GcHandle**)(address+segment->subHandles[i]);
				ObjectHead* o=GetObjectHead(h);
				if(o && !o->mark)roots.Add(o);
			}
		}

		bool GcSingleThread::Collect()
		{
			ObjectHead* o=firstObject;
			while(o)
			{
				o->mark=false;
				o=o->next;
			}

			List<ObjectHead*> roots;
			o=firstObject;
			while(o)
			{
				if(o->ref||o->pin)roots.Add(o);
				o=o->next;
			}

			while(roots.Count())
			{
				ObjectHead* current=roots[roots.Count()-1];
				roots.RemoveAt(roots.Count()-1);
				if(!current->mark)
				{
					current->mark=true;
					if(current->meta)
					{
						char* address=GetObjectAddress(current);
						MarkSegment(&current->meta->mainSegment, address, roots);

						address+=current->meta->mainSegment.size;
						for(vint i=0;i<current->repeat;i++)
						{
							MarkSegment(&current->meta->repeatSegment, address, roots);
							address+=current->meta->repeatSegment.size;
						}
					}
				}
			}
			
			o=firstObject;
			while(o)
			{
				ObjectHead* n=o->next;
				if(!o->mark)
				{
					if(o->prev)o->prev->next=o->next;
					if(o->next)o->next->prev=o->prev;
					if(!o->prev)firstObject=o->next;
					if(!o->next)lastObject=o->prev;
					callback(this, (GcHandle*)o, userData);
					usedSize-=GetObjectSize(o);
					pool.Free((char*)o);
				}
				o=n;
			}

			return true;
		}
	}
}
