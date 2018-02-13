echo on

echo C++��CSharp��Ϣ����������V1.0

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
rmdir /Q /S Message
mkdir Message
cd Message
mkdir c++
mkdir cs

cd "%protoDir%"
rmdir /Q /S Packet
mkdir Packet
cd Packet
mkdir c++
mkdir cs

cd "%protoDir%"
"%protoDir%"\protoc.exe -I=. --cpp_out=./Message/c++/ Message.proto
"%protoDir%"\protoc.exe -I=. --csharp_out=./Message/cs/ Message.proto

"%protoDir%"\protoc.exe -I=. --cpp_out=./Packet/c++/ Packet.proto
"%protoDir%"\protoc.exe -I=. --csharp_out=./Packet/cs/ Packet.proto

rem �Ȳ�copy�ˣ���ȷ�������·�����ļ���ʽ�����Զ�copy
rem copy /Y %SrcFolder%\*.* "%DstCppFolder%"\
rem copy /Y %SrcFolder%\cs\*.cs "%DstCSharpFolder%"\

cd "%protoDir%"
echo Complete!

pause


