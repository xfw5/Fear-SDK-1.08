@echo off

REM echo ----------------------------------------------------------------------
REM echo Game.SLN
REM echo ----------------------------------------------------------------------

set LOGFILE_NAME=CompileLog_Game_Rel.log
if EXIST %LOGFILE_NAME% del %LOGFILE_NAME%

call Helper_Compile.bat game\game.sln "Game" "Release Win32" "Release|Win32" /REBUILD %LOGFILE_NAME%

if "%1" NEQ "NoPause" PAUSE
