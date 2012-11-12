@echo off

echo start to compile all examples
echo.

echo making server99.cpp...
cl /wd 4530 /wd 4996 /nologo server99.cpp
echo.

del *.obj

echo done. bye.
pause
