@echo off


REM echo ----------------------------------------------------------------------
REM echo Game.SLN
REM echo ----------------------------------------------------------------------

set LOGFILE_NAME=CompileLog_Game_Fnl.log
if EXIST %LOGFILE_NAME% del %LOGFILE_NAME%

call Helper_Compile.bat game\game.sln "Game" "Final Win32" "Final|Win32" /REBUILD %LOGFILE_NAME%

if "%1" NEQ "NoPause" PAUSE
