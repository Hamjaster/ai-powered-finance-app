@echo off
echo Building AI Expense Manager...
g++ -o main.exe src/main.cpp -std=c++17
if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Run with: main.exe
) else (
    echo Build failed!
)
