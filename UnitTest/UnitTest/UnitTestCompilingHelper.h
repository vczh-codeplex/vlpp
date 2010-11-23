#include "..\..\Library\Function.h"
#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Stream\FileStream.h"
#include "..\..\Library\Stream\CharFormat.h"

#include "..\..\Library\Scripting\BasicIL\BasicILInterpretor.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageTypeManager.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageWriter.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageAnalyzer.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageCodeGeneration.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageCommentProvider.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::scripting;
using namespace vl::scripting::basiclanguage;
using namespace vl::scripting::basicil;
using namespace vl::stream;

extern WString GetPath();
extern void SetConfiguration(BasicAlgorithmConfiguration& config);
extern Ptr<BasicIL> Compile(Ptr<BasicProgram> program, const WString& name, const WString& fileName);
extern void LogInterpretor(BasicILInterpretor& interpretor, const WString& fileName);

extern void PrintNativeXProgram(Ptr<BasicProgram> program, TextWriter& writer);
extern void ConvertToNativeXProgram(Ptr<BasicProgram>& program);

typedef void (*InterpretorInitializer)(BasicILInterpretor& interpretor);

template<typename T>
void RunBasicProgramInternal(Ptr<BasicProgram> program, T result, const WString& name, InterpretorInitializer initializer)
{
	Ptr<BasicIL> il=Compile(program, name, name);
	BasicILInterpretor interpretor(65536);
	if(initializer)
	{
		initializer(interpretor);
	}
	vint key=interpretor.LoadIL(il.Obj());

	BasicILStack stack(&interpretor);
	stack.Reset(0, key, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==65536);
	vint ins=il->labels[0].instructionIndex;
	stack.Reset(ins, key, sizeof(T));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==65536-sizeof(T));
	TEST_ASSERT(stack.GetEnv()->Pop<T>()==result);

	if(name!=L"")
	{
		LogInterpretor(interpretor, name+L"[Interpretor]");
	}
}

template<typename T>
void RunBasicProgram(Ptr<BasicProgram> program, T result, const WString& name, InterpretorInitializer initializer=0)
{
	RunBasicProgramInternal(program, result, L"", initializer);
	ConvertToNativeXProgram(program);
	RunBasicProgramInternal(program, result, name, initializer);
}