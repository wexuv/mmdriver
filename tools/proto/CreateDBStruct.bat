echo on

echo C++���ݿ�洢�ṹ��������V1.0

set protoDir=%cd%
echo %protoDir%
cd..
cd..
set homeDir=%cd%

rem ����CppĿ���ļ���
set DstCppFolder=%homeDir%\test\

rem ����CsharpĿ���ļ���
set DstCSharpFolder=%homeDir%\null

cd "%protoDir%"
rmdir /Q /S DBStruct
mkdir DBStruct
cd DBStruct
mkdir c++

cd "%protoDir%"
"%protoDir%"\protoc.exe -I=. --cpp_out=./DBStruct/c++/ DBStruct.proto

rem �Ȳ�copy�ˣ���ȷ�������·�����ļ���ʽ�����Զ�copy
rem copy /Y %SrcFolder%\*.* "%DstCppFolder%"\
rem copy /Y %SrcFolder%\cs\*.cs "%DstCSharpFolder%"\

cd "%protoDir%"
echo Complete!

pause


