@echo off
setlocal

cls
call build.bat
if errorlevel 1 exit /b %errorlevel%
tests\test_runner.exe
if errorlevel 1 exit /b %errorlevel%
GBemu.exe
