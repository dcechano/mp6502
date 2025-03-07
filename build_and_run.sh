#!/bin/bash


SOURCE_FILES=(
    "apu.cpp"
    "cpu.cpp"
    "bus.cpp"
)

clang-format -i ./*.h ./*.hpp ./*.c ./*.cpp > /dev/null 2>&1
if [ -f main ]; then
    rm main
fi

echo "Compiling..."

g++ main.cpp ${SOURCE_FILES[@]} -o main

echo ""
echo "Compilation complete with exit code $?"

./main
