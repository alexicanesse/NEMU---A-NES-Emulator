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
class NES;
class PPU;

#warning JUST TEMP

class Debugger{
#warning TODO befriend everyone
public:
    Debugger(NES *nes, CPU *cpu, PPU *ppu); //constructor
    
    void show_registers();
    void show_stack();
    void show_ppu_register();
    void show_interrupts();
    void show_position();
    void show_state();
    void logging();
    
    NES *nes;
    CPU *cpu;
    PPU *ppu;
    
    Address old_pc = 0x0000; //store the last opcode read as a buffer
    std::ofstream log; //used for logging
};


#endif /* debug */
