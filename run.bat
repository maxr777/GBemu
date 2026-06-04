@echo off
setlocal

call build.bat
if errorlevel 1 exit /b %errorlevel%
cls
tests\test_runner.exe
if errorlevel 1 exit /b %errorlevel%
GBemu.exe
