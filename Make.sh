#!/bin/bash

TOOL_DIR="/mnt/c/Users/diego/Documents/Vircon32/Vircon32.DevTools-23.3.7-Windows/Vircon32/DevTools"


# define an abort function to call on error
abort_build()
{
    echo
    echo BUILD FAILED
    exit 1
}

# create obj and bin folders if non exiting, since
# the development tools will not create them themselves
mkdir -p obj
mkdir -p bin

echo
echo Compile the C code
echo --------------------------
$TOOL_DIR/compile.exe MemoryGame.c -o obj/MemoryGame.asm || abort_build

echo
echo Assemble the ASM code
echo --------------------------
$TOOL_DIR/assemble.exe obj/MemoryGame.asm -o obj/MemoryGame.vbin || abort_build

echo
echo Convert the PNG textures
echo --------------------------
$TOOL_DIR/png2vircon.exe MainTexture.png -o obj/MainTexture.vtex || abort_build

echo
echo Convert the WAV sounds
echo --------------------------
$TOOL_DIR/wav2vircon.exe CardFlip.wav -o obj/CardFlip.vsnd     || abort_build
$TOOL_DIR/wav2vircon.exe SoundWin.wav -o obj/SoundWin.vsnd     || abort_build
$TOOL_DIR/wav2vircon.exe Correct.wav -o obj/Correct.vsnd     || abort_build
$TOOL_DIR/wav2vircon.exe Fail.wav -o obj/Fail.vsnd     || abort_build

echo
echo Pack the ROM
echo --------------------------
$TOOL_DIR/packrom.exe MemoryGame.xml -o bin/MemoryGame.v32 || abort_build


echo
echo BUILD SUCCESSFUL
