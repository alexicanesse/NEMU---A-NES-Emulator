cmake_minimum_required (VERSION 3.20)

#c++11 is requiered because initializing non static members wasn't legal before c++11
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Ofast")


project(NES_Emulator)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()


###############################################################################
## file globbing ##############################################################
###############################################################################


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

###############################################################################
## target definitions #########################################################
###############################################################################

add_executable(NES_Emulator ${SRCS} ${HEADERS})

###############################################################################
## dependencies ###############################################################
###############################################################################

#remove -pipe if your system does not have much memory
#set(CMAKE_CXX_FLAGS "-lncurses -Ofast -pipe")

find_package(SDL2 REQUIRED)
find_package(Boost 1.45.0 COMPONENTS program_options)
find_package(Curses)


include_directories(NES_Emulator ${SDL2_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(NES_Emulator ${SDL2_LIBRARIES} ${Boost_LIBRARIES} ${CURSES_LIBRARY})


