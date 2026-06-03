@echo off
setlocal

cls
call build.bat
if errorlevel 1 exit /b %errorlevel%
cls
GBemu.exe
