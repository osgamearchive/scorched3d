
del t:\scorchedsrc.zip
cd i:\scorched\
del /q debug\*.*
del /q debugfst\*.*
del /q debugfast\*.*
l:\compress\PKZIP250\PKZIP.EXE -rp -x@i:\scorched\scripts\exclude.txt t:\scorchedsrc.zip *.* 
