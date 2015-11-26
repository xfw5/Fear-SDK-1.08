
@echo off
setlocal

rem set INCREDIBUILD=1
set REBUILD=/REBUILD
set REBUILDTAG=Re
if "%5" EQU "/Build" set REBUILD=
if "%5" EQU "/Build" set REBUILDTAG=


REM ****************************************************


if "%INCREDIBUILD%" EQU "1" goto INCREDIBUILD

echo - %REBUILDTAG%Building %3 %2
"c:\program files\microsoft visual studio .NET 2003\common7\ide\devenv" %1 %5 %3 >> %6
if errorlevel 1 goto Errors

goto END

:INCREDIBUILD

echo - %REBUILDTAG%IncrediBuilding %3 %2
c:\progra~1\xoreax\incred~1\buildconsole %1 %REBUILD% /PRJ="*" /CFG=%3 /LOG=%6.temp /SILENT /ALL
if errorlevel 1 goto Errors

if EXIST %6  copy /y/a %6 + %6.temp   %6.new >> nul:
if NOT EXIST %6 copy /y/a %6.temp   %6.new >> nul:
move /y %6.new %6
del %6.temp
Goto END

REM ****************************************************


:Errors
@echo Found Errors in Helper_Compile.bat %1 %2 %3 %4 %5 %6
Pause

:END

endlocal
