Vle.exe make "..\..\..\..\UnitTest\Binary\ScriptCoreLibrary\System.CoreNative.Make.txt"
Vle.exe make "..\..\..\..\UnitTest\Binary\ScriptCoreLibrary\System.UnitTestNative.Make.txt"
Vle.exe make "..\..\..\..\UnitTest\Binary\ScriptCoreLibrary\System.ConsoleNative.Make.txt"
copy Vle.exe "..\..\vlpp\Vle.exe"
copy VlScript.dll "..\..\vlpp\VlScript.dll"
copy VlScript.lib "..\..\vlpp\VlScript.lib"
copy VlScript.lib "..\..\vlpp\VlScript.lib"
copy "..\VlScript\VlScript.h" "..\..\vlpp\VlScript.h"
copy Developer.LanguageProvider.dll "..\..\vlpp\Developer.LanguageProvider.dll"
copy Developer.LanguageServices.NativeX.dll "..\..\vlpp\Developer.LanguageServices.NativeX.dll"
copy Developer.WinFormControls.dll "..\..\vlpp\Developer.WinFormControls.dll"
copy Developer.RibbonFramework.dll "..\..\vlpp\Developer.RibbonFramework.dll"
copy VlTurtle.exe "..\..\vlpp\VlTurtle.exe"
copy "..\..\..\..\UnitTest\Binary\ScriptCoreLibrary\Binary\Assembly.syscrnat.assembly" "..\..\vlpp\Assembly.syscrnat.assembly"
copy "..\..\..\..\UnitTest\Binary\ScriptCoreLibrary\Binary\Assembly.sysutnat.assembly" "..\..\vlpp\Assembly.sysutnat.assembly"
copy "..\..\..\..\UnitTest\Binary\ScriptCoreLibrary\Binary\Assembly.syscsnat.assembly" "..\..\vlpp\Assembly.syscsnat.assembly"
Vle.exe header NativeX "..\..\vlpp\Assembly.syscrnat.assembly" "..\..\vlpp\Header.syscrnat.NativeX.txt"
Vle.exe header NativeX "..\..\vlpp\Assembly.sysutnat.assembly" "..\..\vlpp\Header.sysutnat.NativeX.txt"
Vle.exe header NativeX "..\..\vlpp\Assembly.syscsnat.assembly" "..\..\vlpp\Header.syscsnat.NativeX.txt"

Vle.exe header NativeX "..\..\vlpp\Assembly.syscrnat.assembly" "Header.syscrnat.NativeX.txt"