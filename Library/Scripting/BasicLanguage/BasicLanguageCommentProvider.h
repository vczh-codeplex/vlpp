/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECOMMENTPROVIDER
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECOMMENTPROVIDER

#include "BasicLanguageException.h"
#include "..\BasicIL\BasicILDefinition.h"
#include "..\..\Stream\MemoryStream.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicLanguageCommentProvider : public basicil::BasicIL::ICommentProvider
			{
				typedef collections::List<Ptr<collections::List<WString>>> CodeFileList;
			protected:
				CodeFileList				codes;
				stream::MemoryStream*		memoryStream;
				stream::StreamWriter*		streamWriter;
				vint						lastLine;
				vint						lastCode;

			public:
				BasicLanguageCommentProvider();
				~BasicLanguageCommentProvider();

				stream::TextWriter*			OpenWriter();
				void						CloseWriter();
				bool						IsOpening();

				void						StartProvideComment();
				void						AppendComment(stream::TextWriter& writer, void* userData);
			};
		}
	}
}

#endif