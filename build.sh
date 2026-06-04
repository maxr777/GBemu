#!/bin/bash
set -e

RAYLIB=external/raylib
RAYLIB_LIB=$RAYLIB/libraylib.a
RAYLIB_OBJECTS=(
    "$RAYLIB/rcore.o"
    "$RAYLIB/rshapes.o"
    "$RAYLIB/rtextures.o"
    "$RAYLIB/rtext.o"
    "$RAYLIB/rmodels.o"
    "$RAYLIB/utils.o"
    "$RAYLIB/rglfw.o"
)

if [ ! -f "$RAYLIB_LIB" ]; then
    gcc -c "$RAYLIB/rcore.c" -o "$RAYLIB/rcore.o" -O2 \
        -I"$RAYLIB" \
        -I"$RAYLIB/external/glfw/include" \
        -DPLATFORM_DESKTOP \
        -DGRAPHICS_API_OPENGL_33 \
        -D_GLFW_X11 \
        -std=c11

    gcc -c "$RAYLIB/rshapes.c" -o "$RAYLIB/rshapes.o" -O2 -I"$RAYLIB" -std=c11
    gcc -c "$RAYLIB/rtextures.c" -o "$RAYLIB/rtextures.o" -O2 -I"$RAYLIB" -std=c11
    gcc -c "$RAYLIB/rtext.c" -o "$RAYLIB/rtext.o" -O2 -I"$RAYLIB" -std=c11
    gcc -c "$RAYLIB/rmodels.c" -o "$RAYLIB/rmodels.o" -O2 -I"$RAYLIB" -std=c11
    gcc -c "$RAYLIB/utils.c" -o "$RAYLIB/utils.o" -O2 -I"$RAYLIB" -std=c11
    gcc -c "$RAYLIB/rglfw.c" -o "$RAYLIB/rglfw.o" -O2 \
        -I"$RAYLIB" \
        -I"$RAYLIB/external/glfw/include" \
        -D_GLFW_X11 \
        -std=c11

    ar rcs "$RAYLIB_LIB" "${RAYLIB_OBJECTS[@]}"
fi

gcc -o GBemu desktop_gbemu.c -s -O2 \
    -I"$RAYLIB" \
    "$RAYLIB_LIB" \
    -lGL -lm -lpthread -ldl -lrt -lX11 \
    -std=c11 -Wall

gcc -o tests/test_runner tests/test_runner.c -std=c11 -Wall
