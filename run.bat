@echo off
setlocal

set "RUN_CPU=0"

:parse_args
if "%~1"=="" goto args_done
if /I "%~1"=="--cpu" (
    set "RUN_CPU=1"
    shift
    goto parse_args
)
echo Unknown argument: %~1 1>&2
echo Usage: %~nx0 [--cpu] 1>&2
exit /b 2

:args_done
cls
call build.bat
if errorlevel 1 exit /b %errorlevel%

if "%RUN_CPU%"=="1" (
    tests\cpu_test_runner.exe
    if errorlevel 1 exit /b 1
)

tests\test_runner.exe
if errorlevel 1 exit /b %errorlevel%
GBemu.exe
