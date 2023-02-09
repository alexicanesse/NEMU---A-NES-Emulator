#I choose version 3.22.1 minimum because I'm working with it and I'm too lazy to test older versions
cmake_minimum_required(VERSION 3.22.1)

#c++11 is requiered because initializing non static members wasn't legal before c++11
set (CMAKE_CXX_STANDARD 11)

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
set(CMAKE_CXX_FLAGS "-lncurses -Ofast -pipe")

find_package(SDL2 REQUIRED)
find_package(Boost 1.45.0 COMPONENTS program_options)
include_directories(NES_Emulator ${SDL2_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(NES_Emulator ${SRCS} ${HEADERS})
target_link_libraries(NES_Emulator ${SDL2_LIBRARIES} ${Boost_LIBRARIES})


