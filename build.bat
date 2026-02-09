@echo off
setlocal

cmake -B build
if %errorlevel% neq 0 exit /b %errorlevel%

echo Building Debug...
cmake --build build --config Debug
if %errorlevel% neq 0 exit /b %errorlevel%

echo Building Release...
cmake --build build --config Release
if %errorlevel% neq 0 exit /b %errorlevel%

echo Done!