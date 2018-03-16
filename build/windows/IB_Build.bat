@echo off

setlocal enabledelayedexpansion

set BuildPath=%cd%
cd ..\..\
set ServerPath=%cd%
set CodePath=%cd%

mode con:cols=160 lines=80
rem 系统时间
set year=%date:~0,4%
set month=%date:~5,2%
set day=%date:~8,2%
set hour=%time:~0,2%
set minute=%time:~3,2%
set sesond=%time:~6,2%

echo =========================
echo update Server Code
echo =========================

(call svn update %CodePath% --username wexuv --password a111111** --non-interactive)
echo update Server Code successfull!

set Compliler="D:\Program Files (x86)\Xoreax\IncrediBuild\BuildConsole.exe"
%Compliler% %ServerPath%\MMDriver.sln /cfg="Debug|x64" /Prj="MMDriver" /ShowAgent /ShowTime

set Msg=""
if %errorlevel% equ 0 set Msg=compile successfull.
if %errorlevel% equ 1 set Msg=errors were encountered during the operation.
if %errorlevel% equ 2 set Msg=a fatal incredibuild error was encountered.
if %errorlevel% equ 3 set Msg=the operation was stopped before completing.

echo =========================
echo %Msg%
echo =========================
echo build finished.

echo =========================
echo copy MMDriver.exe
echo =========================
move %ServerPath%\x64\Debug\MMDriver.exe %ServerPath%\bin\
echo copy MMDriver.exe

echo =========================
echo svn commit
echo =========================
set cimsg=打版本,time-%year%-%month%-%day%-%hour%-%minute%-%second%
(call svn commit %BuildPath% -m %cimsg%)
echo svn commit successfull

@echo on
pause