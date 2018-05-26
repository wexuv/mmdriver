echo on

echo C++数据库存储结构创建工具V1.0

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
rmdir /Q /S DBStruct
mkdir DBStruct
cd DBStruct
mkdir c++

cd "%protoDir%"
"%protoDir%"\protoc.exe -I=. --cpp_out=./DBStruct/c++/ DBStruct.proto

rem 先不copy了，等确定了相关路径和文件格式，再自动copy
rem copy /Y %SrcFolder%\*.* "%DstCppFolder%"\
rem copy /Y %SrcFolder%\cs\*.cs "%DstCSharpFolder%"\

cd "%protoDir%"
echo Complete!

pause


