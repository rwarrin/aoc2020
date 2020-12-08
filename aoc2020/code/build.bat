@echo off

SET CompilerFlags=/nologo /Z7 /Od /Oi /fp:fast /FC /MTd -WX -W4 -D_CRT_SECURE_NO_WARNINGS -wd4189 -wd4100 -wd4505 -wd4456 -wd4611
SET LinkerFlags=/incremental:no /opt:ref

IF NOT EXIST build mkdir build

pushd build

cl.exe %CompilerFlags% /Feapp.exe ..\%1 /link %LinkerFlags%

popd
