@echo off

setlocal enabledelayedexpansion

echo copy Script start
echo =========================

xcopy ..\..\Script .\Script /s /e

echo =========================
echo copy Script finished

@echo on
pause