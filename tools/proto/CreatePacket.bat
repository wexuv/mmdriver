echo on

echo C++和CSharp消息包创建工具V1.0

set protoDir=%cd%
echo %protoDir%
cd..
cd..
set homeDir=%cd%

rem 设置Cpp目标文件夹
set DstCppFolder=%homeDir%\test\

rem 设置Csharp目标文件夹
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

rem 先不copy了，等确定了相关路径和文件格式，再自动copy
rem copy /Y %SrcFolder%\*.* "%DstCppFolder%"\
rem copy /Y %SrcFolder%\cs\*.cs "%DstCSharpFolder%"\

cd "%protoDir%"
echo Complete!

pause


