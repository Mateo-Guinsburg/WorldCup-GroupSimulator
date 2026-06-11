@echo off
g++ main.cpp team.cpp match.cpp group.cpp tournament.cpp -o worldcup -lws2_32 -std=c++17 -D_WIN32_WINNT=0x0A00 -static
echo Compilation ended.
pause