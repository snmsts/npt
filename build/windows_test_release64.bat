@echo off
cd /d %~dp0
cd ..

if not exist test (
  echo directory error
  goto error
)

.\build\x64\Release\npt --degrade
if errorlevel 1 (
  echo test error
  goto error
)

pause
exit /b 0
:error
pause
exit /b 1
