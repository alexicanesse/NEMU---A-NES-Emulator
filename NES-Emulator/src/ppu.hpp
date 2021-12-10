//
//  ppu.hpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 07/12/2021.
//

#ifndef ppu_hpp
#define ppu_hpp

#include <iostream>
#include <array>

#include "screen.hpp"


typedef uint8_t Byte;
typedef uint16_t Address;

class NES;

class PPU{
private:
    /*
     Graphics
    */
    GRAPHICS graphics = *(new GRAPHICS(256*2, 240*2));
    SDL_Event event;
    
    //https://wiki.nesdev.org/w/index.php?title=PPU_frame_timing
    bool odd_frame = true;
    
    /*
    positions
    */
    int scanline = 0;
    int row = 0;
    
    /*
     Registers
    */
#warning TODO init reg
    struct registers {
        Byte PPUCTRL = 0x00; // Address 0x2000       PPU control register
        Byte PPUMASK = 0x00; // Address 0x2001       PPU mask register
        Byte PPUSTATUS = 0x00; // Address 0x2002     PPU status register
        Byte OAMADDR = 0x00; // Address 0x2003       OAM address port
        Byte OAMDATA = 0x00; // Address 0x2004       OAM data port
        Byte PPUSCROLL = 0x00; // Address 0x2005     PPU scrolling position register
        Byte PPUADDR = 0x00; // Address 0x2006      PPU address register
        Byte PPUDATA = 0x00; // Address 0x2007       PPU data port
        Byte OAMDMA = 0x00; // Address 0x4014        OAM DMA register (high byte)
    } registers;
    
    struct Color{
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t o = 255; //opacity
    };
    
    //Memory
    //https://wiki.nesdev.org/w/index.php?title=PPU_memory_map
    std::array<std::array<Byte, 0x1000>, 2> *Pattern_table = new std::array<std::array<Byte, 0x1000>, 2>; //pattern table 0 and 1
    std::array<std::array<Byte, 0x0400>, 4> *Nametable = new std::array<std::array<Byte, 0x0400>, 4>;
    std::array<GRAPHICS::Color,64> *palette = new std::array<GRAPHICS::Color,64>; //all available colors
    /*
     Other
    */
    Byte read_buffer = 0x00;
public:
    /*
        Constructor
    */
    PPU(NES*);
    
    /*
     Registers
    */
#warning TODO properly
    Byte getPPUCTRL();    //get PPU control register
    Byte getPPUMASK();    //get PPU mask register
    Byte getPPUSTATUS();  //get PPU status register
    Byte getOAMADDR();    //OAM address port
    Byte getOAMDATA();    //get OAM data port
    Byte getPPUSCROLL();  //get scrolling position register
    Byte getPPUADDR();    //get address register
    Byte getPPUDATA();    //get PPU data port
    Byte getOAMDMA();     //get OAM DMA register (high byte)
    
    void setPPUCTRL(Byte);    //get PPU control register
    void setPPUMASK(Byte);    //get PPU mask register
    void setPPUSTATUS(Byte);  //get PPU status register
    void setOAMADDR(Byte);    //OAM address port
    void setOAMDATA(Byte);    //get OAM data port
    void setPPUSCROLL(Byte);  //get scrolling position register
    void setPPUADDR(Byte);    //get address register
    void setPPUDATA(Byte);    //get PPU data port
    void setOAMDMA(Byte);     //get OAM DMA register (high byte)
    
    /*
     chrROM
    */
    void write(Address, Byte);
    Byte read(Address);
    
    /*
     Other
    */
    NES *nes;
    void clock();
};






#endif /* ppu_hpp */
