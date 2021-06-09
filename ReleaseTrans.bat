@echo off
set PATH=C:\Qt\Qt5.12.11\5.12.11\msvc2015_64\bin;%PATH%
cd ./

set w32debug=%cd%\bin\win32\debug\translations\
set w32release=%cd%\bin\win32\release\translations\
set x64debug=%cd%\bin\x64\debug\translations\
set x64release=%cd%\bin\x64\release\translations\

lrelease ./LVGLBuilder.pro
copy /y en.qm %w32debug%
copy /y en.qm %w32release%
copy /y en.qm %x64debug%
copy /y en.qm %x64release%

copy /y sc_zh.qm %w32debug%
copy /y sc_zh.qm %w32release%
copy /y sc_zh.qm %x64debug%
copy /y sc_zh.qm %x64release%
pause