@echo off & color 0A

:: protoc������  protoc.exe ��gen.bat ��ͬһĿ¼�� 
set "PROTOC_EXE=protoc.exe"
:: .proto�ļ��������ݾ����ļ��޸�
set "PROTOC_FILE_NAME=protocol.proto"

set "PROTOC_PATH=%cd%"
set "CPP_OUT_PATH=%cd%"

::����.h��.cc
::"%PROTOC_PATH%\%PROTOC_EXE%" --proto_path="%PROTOC_PATH%" --cpp_out="%CPP_OUT_PATH%" "%PROTOC_PATH%\%PROTOC_FILE_NAME%"

cd %~dp0
for /f "delims=\" %%a in ('dir /b /a-d /o-d "*.proto"') do (
"%PROTOC_EXE%" --proto_path="%PROTOC_PATH%" --cpp_out="%CPP_OUT_PATH%" "%PROTOC_PATH%\%%a"
)

::�������ϲ�Ŀ¼
xcopy .\*.h ..\ /y
xcopy .\*.cc ..\ /y

del .\*.h /f/s/q/a 
del .\*.cc /f/s/q/a 

pause