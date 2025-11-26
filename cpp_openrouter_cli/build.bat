@echo off
if not exist build mkdir build
g++ -std=c++17 -I./include main.cpp -o build/chat.exe
if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)
echo Build successful! Run build\chat.exe
