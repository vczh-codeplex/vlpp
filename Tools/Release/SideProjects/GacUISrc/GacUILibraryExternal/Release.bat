cd ..\Debug\
mkdir DocRefFolder
cd DocRefFolder
del *.* /Q
cd ..
Pdb2Xml.exe GacUISrc.pdb GacUISrcPdb.xml
Xml2Doc.exe GacUISrcPdb.xml GacUISrc.xml GacUISrcDoc.xml
DocRef.exe GacUISrcDoc.xml DocRefFolder
DocIndex.exe DocRefFolder ..\GacUILibraryExternal\DocFolder
cd ..\GacUILibraryExternal