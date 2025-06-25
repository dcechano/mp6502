#!/bin/bash

clang-format -i ./*.h ./*.hpp ./*.c ./*.cpp > /dev/null 2>&1

SOURCE_FILES=(
    "src/dev/apu.cpp"
    "src/dev/cpu.cpp"
    "src/dev/bus.cpp"
    "src/op/operation.cpp"
    "src/op/addr_mode.cpp"
    "src/io/rom.cpp"
)

if [ ! -d bin ]; then
    mkdir bin
elif [ -f bin/main ]; then
    rm bin/main
fi

g++ src/main.cpp ${SOURCE_FILES[@]} -o bin/main
./bin/main
