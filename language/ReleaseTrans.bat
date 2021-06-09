@echo off
set PATH=C:\Qt\Qt5.12.11\5.12.11\msvc2015_64\bin;%PATH%
cd ../

set w32debug=%cd%\bin\win32\debug\translations\
set w32release=%cd%\bin\win32\release\translations\
set x64debug=%cd%\bin\x64\debug\translations\
set x64release=%cd%\bin\x64\release\translations\

cd ./language

lrelease en.ts -qm en.qm
lrelease jp.ts -qm jp.qm
lrelease sc_zh.ts -qm sc_zh.qm
lrelease tc_zh.ts -qm tc_zh.qm

copy /y *.qm %w32debug%
copy /y *.qm %w32release%
copy /y *.qm %x64debug%
copy /y *.qm %x64release%

pause