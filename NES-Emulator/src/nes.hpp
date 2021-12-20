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

typedef uint8_t Byte;
typedef uint16_t Address;


class NES{
private:
    int cycle; //it will be used to make the cpu run at a third of ppu speed 
public:
    CPU *cpu;
    PPU *ppu;
    CARTRIDGE *cartridge;
    
    /*
        Constructor
    */
    NES();
    
    
    //controller (only one because it's annoying to map tow controllers to one keyboard)
    Byte controler_state = 0x00;
    Byte controler_shifter = 0x00;
    
#warning TODO init matrix at init
    std::array<Byte, 2048> *ram = new std::array<Byte, 2048>;
    
    void write(Address adr, Byte content);
    Byte read(Address adr);
    
    void clock();
};


#endif /* nes_hpp */
