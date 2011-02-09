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
				o->ref=0;
				o->repeat=repeat;
				o->mark=false;
				usedSize+=size;

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
			return o&&o->meta?GetObjectAddress(o):0;
		}

		bool GcSingleThread::DecreaseHandlePin(GcHandle* handle)
		{
			ObjectHead* o=GetObjectHead(handle);
			return o&&o->meta;
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

		bool GcSingleThread::ReadHandle(GcHandle* handle, vint offset, vint length, char* buffer)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta)
			{
				char* source=GetObjectAddress(o);
				vint size=GetObjectSize(o);
				if(offset>=0 && length>=0 && offset+length<=size)
				{
					memcpy(buffer, source+offset, length);
					return true;
				}
			}
			return false;
		}

		bool GcSingleThread::WriteHandle(GcHandle* handle, vint offset, vint length, char* buffer)
		{
			ObjectHead* o=GetObjectHead(handle);
			if(o&&o->meta)
			{
				char* source=GetObjectAddress(o);
				vint size=GetObjectSize(o);
				if(offset>=0 && length>=0 && offset+length<=size)
				{
					memcpy(source+offset, buffer, length);
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
				if(o)roots.Add(o);
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
				if(o->ref)roots.Add(o);
				o=o->next;
			}

			while(roots.Count())
			{
				ObjectHead* current=roots[roots.Count()-1];
				roots.RemoveAt(roots.Count()-1);
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
					usedSize-=GetObjectSize(o);
					pool.Free((char*)n);
				}
				o=n;
			}

			return true;
		}
	}
}