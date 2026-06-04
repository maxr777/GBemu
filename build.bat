@echo off
setlocal

set "RAYLIB=external\raylib"
set "RAYLIB_LIB=%RAYLIB%\libraylib_win.a"
set "RAYLIB_OBJECTS=%RAYLIB%\rcore_win.obj %RAYLIB%\rshapes_win.obj %RAYLIB%\rtextures_win.obj %RAYLIB%\rtext_win.obj %RAYLIB%\rmodels_win.obj %RAYLIB%\utils_win.obj %RAYLIB%\rglfw_win.obj"

if not exist "%RAYLIB_LIB%" (
    gcc -c "%RAYLIB%\rcore.c" -o "%RAYLIB%\rcore_win.obj" -O2 ^
        -I"%RAYLIB%" ^
        -I"%RAYLIB%\external\glfw\include" ^
        -DPLATFORM_DESKTOP ^
        -DGRAPHICS_API_OPENGL_33 ^
        -std=c11 -Wall
    if errorlevel 1 exit /b %errorlevel%

    gcc -c "%RAYLIB%\rshapes.c" -o "%RAYLIB%\rshapes_win.obj" -O2 -I"%RAYLIB%" -std=c11 -Wall
    if errorlevel 1 exit /b %errorlevel%

    gcc -c "%RAYLIB%\rtextures.c" -o "%RAYLIB%\rtextures_win.obj" -O2 -I"%RAYLIB%" -std=c11 -Wall
    if errorlevel 1 exit /b %errorlevel%

    gcc -c "%RAYLIB%\rtext.c" -o "%RAYLIB%\rtext_win.obj" -O2 -I"%RAYLIB%" -std=c11 -Wall
    if errorlevel 1 exit /b %errorlevel%

    gcc -c "%RAYLIB%\rmodels.c" -o "%RAYLIB%\rmodels_win.obj" -O2 -I"%RAYLIB%" -std=c11 -Wall
    if errorlevel 1 exit /b %errorlevel%

    gcc -c "%RAYLIB%\utils.c" -o "%RAYLIB%\utils_win.obj" -O2 -I"%RAYLIB%" -std=c11 -Wall
    if errorlevel 1 exit /b %errorlevel%

    gcc -c "%RAYLIB%\rglfw.c" -o "%RAYLIB%\rglfw_win.obj" -O2 ^
        -I"%RAYLIB%" ^
        -I"%RAYLIB%\external\glfw\include" ^
        -std=c11 -Wall
    if errorlevel 1 exit /b %errorlevel%

    ar rcs "%RAYLIB_LIB%" %RAYLIB_OBJECTS%
    if errorlevel 1 exit /b %errorlevel%
)

gcc -o GBemu.exe desktop_gbemu.c -s -O2 ^
    -I"%RAYLIB%" ^
    "%RAYLIB_LIB%" ^
    -lopengl32 -lgdi32 -lwinmm ^
    -std=c11 -Wall
if errorlevel 1 exit /b %errorlevel%

if not exist "tests\ROMs\cartridge_header_test.gb" (
    where bash >nul 2>nul
    if errorlevel 1 (
        echo tests\ROMs\cartridge_header_test.gb is missing and bash was not found to run tests\ROMs\compile_roms.sh
        exit /b 1
    )

    pushd tests\ROMs
    bash compile_roms.sh
    if errorlevel 1 (
        popd
        exit /b %errorlevel%
    )
    popd
)

gcc -o tests\test_runner.exe tests\test_runner.c -std=c11 -Wall
if errorlevel 1 exit /b %errorlevel%
