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

#warning JUST TEMP

class Debugger{
#warning TODO befriend everyone
public:
    void show_ram(std::array<Byte, 2048> ram);
    void show_registers(CPU cpu);
    void show_state(CPU cpu, std::array<Byte, 2048> ram);
    void show_stack(std::array<Byte, 2048> ram);
    void show_ppu_register(PPU ppu);
    void show_interrupts(NES nes);
    void show_position(PPU ppu);
    void show_state(CPU cpu, std::array<Byte, 2048> ram, PPU ppu, NES nes);
    void logging(CPU cpu, Address pc, PPU ppu);
    
    NES nes;
    CPU *cpu = nes.cpu;
    PPU *ppu = nes.ppu;
    CARTRIDGE *cartridge = nes.cartridge;
};


#endif /* debug */
