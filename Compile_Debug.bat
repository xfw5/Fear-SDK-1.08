@echo off

REM echo ----------------------------------------------------------------------
REM echo Game.SLN
REM echo ----------------------------------------------------------------------

set LOGFILE_NAME=CompileLog_Game_Dbg.log
if EXIST %LOGFILE_NAME% del %LOGFILE_NAME%

call Helper_Compile.bat game\game.sln "Game" "Debug Win32" "Debug|Win32" /Rebuild %LOGFILE_NAME%

if "%1" NEQ "NoPause" PAUSE
