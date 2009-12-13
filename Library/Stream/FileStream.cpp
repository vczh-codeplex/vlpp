#include "FileStream.h"

namespace vl
{
	namespace stream
	{
/***********************************************************************
FileStream
***********************************************************************/

		FileStream::FileStream(const WString& fileName, AccessRight _accessRight)
			:accessRight(_accessRight)
		{
			const wchar_t* mode=L"rb";
			switch(accessRight)
			{
			case ReadOnly:
				mode=L"rb";
				break;
			case WriteOnly:
				mode=L"wb";
				break;
			case ReadWrite:
				mode=L"w+b";
				break;
			}

			if(_wfopen_s(&file, fileName.Buffer(), mode)!=0)
			{
				file=0;
			}
		}

		FileStream::~FileStream()
		{
			Close();
		}

		bool FileStream::CanRead()const
		{
			return file!=0 && (accessRight==ReadOnly || accessRight==ReadWrite);
		}

		bool FileStream::CanWrite()const
		{
			return file!=0 && (accessRight==WriteOnly || accessRight==ReadWrite);
		}

		bool FileStream::CanSeek()const
		{
			return file!=0;
		}

		bool FileStream::CanPeek()const
		{
			return file!=0 && (accessRight==ReadOnly || accessRight==ReadWrite);
		}

		bool FileStream::IsLimited()const
		{
			return file!=0 && accessRight==ReadOnly;
		}

		bool FileStream::IsAvailable()const
		{
			return file!=0;
		}

		void FileStream::Close()
		{
			if(file!=0)
			{
				fclose(file);
				file=0;
			}
		}

		pos_t FileStream::Position()const
		{
			if(file!=0)
			{
				fpos_t position=0;
				if(fgetpos(file, &position)==0)
				{
					return position;
				}
			}
			return -1;
		}

		pos_t FileStream::Size()const
		{
			if(file!=0)
			{
				fpos_t position=0;
				if(fgetpos(file, &position)==0)
				{
					if(fseek(file, 0, SEEK_END)==0)
					{
						pos_t size=Position();
						if(fsetpos(file, &position)==0)
						{
							return size;
						}
					}
				}
			}
			return -1;
		}

		void FileStream::Seek(pos_t _size)
		{
			if(Position()+_size>Size())
			{
				_fseeki64(file, 0, SEEK_END);
			}
			else if(Position()+_size<0)
			{
				_fseeki64(file, 0, SEEK_SET);
			}
			else
			{
				_fseeki64(file, _size, SEEK_CUR);
			}
		}

		void FileStream::SeekFromBegin(pos_t _size)
		{
			if(_size>Size())
			{
				_fseeki64(file, 0, SEEK_END);
			}
			else if(_size<0)
			{
				_fseeki64(file, 0, SEEK_SET);
			}
			else
			{
				_fseeki64(file, _size, SEEK_SET);
			}
		}

		void FileStream::SeekFromEnd(pos_t _size)
		{
			if(_size<0)
			{
				_fseeki64(file, 0, SEEK_END);
			}
			else if(_size>Size())
			{
				_fseeki64(file, 0, SEEK_SET);
			}
			else
			{
				_fseeki64(file, -_size, SEEK_END);
			}
		}

		int FileStream::Read(void* _buffer, int _size)
		{
			CHECK_ERROR(file!=0, L"FileStream::Read(pos_t)#流处于关闭状态，不可执行此操作。");
			CHECK_ERROR(_size>=0, L"FileStream::Read(void*, int)#参数size不可为负。");
			return fread(_buffer, 1, _size, file);
		}

		int FileStream::Write(void* _buffer, int _size)
		{
			CHECK_ERROR(file!=0, L"FileStream::Write(pos_t)#流处于关闭状态，不可执行此操作。");
			CHECK_ERROR(_size>=0, L"FileStream::Write(void*, int)#参数size不可为负。");
			return fwrite(_buffer, 1, _size, file);
		}

		int FileStream::Peek(void* _buffer, int _size)
		{
			CHECK_ERROR(file!=0, L"FileStream::Peek(pos_t)#流处于关闭状态，不可执行此操作。");
			CHECK_ERROR(_size>=0, L"FileStream::Peek(void*, int)#参数size不可为负。");
			fpos_t position=0;
			if(fgetpos(file, &position)==0)
			{
				size_t count=fread(_buffer, 1, _size, file);
				if(fsetpos(file, &position)==0)
				{
					return count;
				}
			}
			return -1;
		}
	}
}