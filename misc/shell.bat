@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

subst w: C:\Projects\aoc2020

pushd w:
start C:\Development\Vim\vim82\gvim.exe

cls
echo Shell Started
