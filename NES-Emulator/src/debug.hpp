//
//  debug.hpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 19/11/2021.
//

#ifndef debug_hpp
#define debug_hpp

#include <chrono>
#include <thread>


#include <iostream>
#include <array>
#include <ncurses.h>
#include <string>
#include <sstream>




typedef uint8_t Byte;
typedef uint16_t Address;

class CPU;

#warning TODO hpp file
void show_ram(std::array<Byte, 2048> ram);
void show_registers(CPU cpu);
void show_state(CPU cpu, std::array<Byte, 2048> ram);



#endif /* debug */
