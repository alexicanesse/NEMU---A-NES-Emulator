//
//  nes.hpp
//  Proj1
//
//  Created by Alexi Canesse on 17/11/2021.
//

#ifndef nes_hpp
#define nes_hpp


#include <array>

#include "cpu.hpp"
#include "ppu.hpp"
#include "cartridge.hpp"
#include "debug.hpp"

typedef uint8_t Byte;
typedef uint16_t Address;


class NES{
private:
    int cycle; //it will be used to make the cpu run at a third of ppu speed
    
    Debugger *debug;
    
    bool transfert_dma = false;
    Byte dma_offset = 0x00;
    bool dma_idle_cycle_done = false;
public:
    CPU *cpu;
    PPU *ppu;
    CARTRIDGE *cartridge;
    
    /*
        Constructor
    */
    NES();
    
    
    //controller (only one because it's annoying to map two controllers to one keyboard)
    Byte controler_shifter = 0x00;
    
    std::array<Byte, 2048> *ram = new std::array<Byte, 2048>; //the power-up state doesn't matter so the array doesn't have to be initialized
    
    void write(Address adr, Byte content);
    Byte read(Address adr);
    
    void clock();
    
    void debug_loop(bool log);
    void usual_loop();
};


#endif /* nes_hpp */
