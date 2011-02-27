Steps to generate the release content

1. Open "Tools\Release\VleSource\VleSource.sln"
2. Compile using "Release" configuration
3. Run "Tools\Release\VleSource\Release\Deploy.bat"

File Description:
  Assembly.syscrnat.assembly        Vle Assembly for "Core Native" library
  Assembly.syscsnat.assembly        Vle Assembly for "Console Native" library
  Assembly.sysutnat.assembly        Vle Assembly for "Unit Test Native" library
  Header.syscrnat.NativeX.txt       Auto generated NativeX header file for "Core Native" library
  Header.syscsnat.NativeX.txt       Auto generated NativeX header file for "Console Native" library
  Header.sysutnat.NativeX.txt       Auto generated NativeX header file for "Unit Test Native" library
  Vle.exe                           Compiler with a VM for console and unit test program
  VlScript.dll                      Vle script engine runtime
  VlScript.h                        Header file for VlScript.dll
  VlScript.lib                      Symbol file for VlScript.dll
  VlScriptDotNet.dll                .Net wrapper for VlScript.dll