@echo off

call Compile_Final.bat NoPause
call Compile_Debug.bat NoPause
call Compile_Release.bat NoPause

if "%1" NEQ "NoPause" PAUSE