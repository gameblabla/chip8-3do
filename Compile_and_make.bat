PATH=C:\3DODev\bin;%PATH% 
PATH=C:\ARM211A\bin;%PATH% 
PATH=C:\ARM211A;%PATH% 
del swing.iso
make -f Makefile.3DO clean
make -f Makefile.3DO
pause
start /d "OperaFS" OperaFS.exe
