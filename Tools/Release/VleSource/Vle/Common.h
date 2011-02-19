#ifndef VLESOURCE_COMMON
#define VLESOURCE_COMMON

#include "..\..\..\..\Library\Basic.h"
#include "..\..\..\..\Library\Console.h"
#include "..\..\..\..\Library\Collections\List.h"
#include "..\..\..\..\Library\Stream\FileStream.h"
#include "..\..\..\..\Library\Stream\CharFormat.h"
#include "..\..\..\..\Library\Scripting\BasicIL\BasicILException.h"
#include "..\..\..\..\Library\Scripting\Languages\BasicFunctionExecutor.h"
#include "..\..\..\..\Library\Scripting\Utility\ScriptingUtilityMake.h"

using namespace vl;
using namespace vl::console;
using namespace vl::collections;
using namespace vl::stream;
using namespace vl::scripting;
using namespace vl::scripting::basicil;
using namespace vl::scripting::utility;

extern wchar_t* ProcVersion();
extern void InitHost(LanguageHost& host);
extern Ptr<LanguageState> LoadAssembly(LanguageHost& host, const List<WString>& fileNames, List<Ptr<LanguageAssembly>>& assemblies);
extern void ReadAssemblyListFile(const WString& fileName, List<WString>& fileNames);

#endif