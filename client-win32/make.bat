@echo off

echo start to compile all examples
echo.

echo making client.cpp ...
cl /wd 4530 /wd 4996 /nologo client.cpp
echo.

echo making client1.cpp ...
cl /wd 4530 /wd 4996 /nologo client1.cpp
echo.

del *.obj

echo done. bye.
pause
