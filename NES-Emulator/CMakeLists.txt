#I choose version 3.22.1 minimum because I'm working with it and I'm too lazy to text older versions
cmake_minimum_required(VERSION 3.22.1)

project(NES_Emulator)

#List of files to compile
set(SRCS
    ./src/debug.cpp
    ./src/ppu.cpp
    ./src/cartridge.cpp
    ./src/cpu.cpp
    ./src/nes.cpp
    ./src/screen.cpp
    )
    
set(HEADERS
    ./src/debug.hpp
    ./src/ppu.hpp
    ./src/cartridge.hpp
    ./src/cpu.hpp
    ./src/nes.hpp
    ./src/screen.hpp
    )

#remove -pipe if your system does not have much memory
set(CMAKE_CXX_FLAGS "-lncurses -O2 -pipe")

find_package(SDL2 REQUIRED)
include_directories(NES_Emulator ${SDL2_INCLUDE_DIRS})

add_executable(NES_Emulator ${SRCS} ${HEADERS})
target_link_libraries(NES_Emulator ${SDL2_LIBRARIES})


