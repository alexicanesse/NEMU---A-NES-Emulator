//
//  debug.hpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 19/11/2021.
//

#ifndef debug_hpp
#define debug_hpp


#include <iostream>
#include <array>
#include <string>
#include <sstream>

#include <ncurses.h>



typedef uint8_t Byte;
typedef uint16_t Address;

class CPU;

#warning TODO hpp file
void show_ram(std::array<Byte, 2048> ram);
void show_registers(CPU cpu);
void show_state(CPU cpu, std::array<Byte, 2048> ram);



#endif /* debug */
