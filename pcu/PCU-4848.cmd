@echo off
setlocal
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0PCU-4848.ps1" %*
set "RC=%ERRORLEVEL%"
echo.
if not "%RC%"=="0" echo PCU termino con codigo %RC%.
pause
exit /b %RC%
