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
    bool odd_frame = false;
    
    /*
    positions
    */
    int scanline = -1;
    int row = 0;

    
    /*
     Registers
    */
    //power-up state
    //https://wiki.nesdev.org/w/index.php?title=PPU_power_up_state
    struct registers {
        Byte PPUCTRL = 0x00; // Address 0x2000       PPU control register
        Byte PPUMASK = 0x00; // Address 0x2001       PPU mask register
        Byte PPUSTATUS = 0x00; // Address 0x2002     PPU status register
        Byte OAMADDR = 0x00; // Address 0x2003       OAM address port
        Byte OAMDATA = 0x00; // Address 0x2004       OAM data port
        Byte PPUSCROLL = 0x00; // Address 0x2005     PPU scrolling position register
        Byte PPUADDR = 0x00; // Address 0x2006      PPU address register
//      // Address 0x2007       PPU data port: vmem_addr(declared in public)
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
    std::array<Byte, 0x0020> *Palette = new std::array<Byte, 0x0020>;
    std::array<GRAPHICS::Color,64> *palette = new std::array<GRAPHICS::Color,64>; //all available colors
    
    
    //background rendering
    //2 16-bit shift registers - These contain the pattern table data for two tiles. Every 8 cycles, the data for the next tile is loaded into the upper 8 bits of this shift register. Meanwhile, the pixel to render is fetched from one of the lower 8 bits.
    Address pattern_data_shift_register_1 = 0x0000;
    Address pattern_data_shift_register_2 = 0x0000;
    //2 8-bit shift registers - These contain the palette attributes for the lower 8 pixels of the 16-bit shift register. These registers are fed by a latch which contains the palette attribute for the next tile. Every 8 cycles, the latch is loaded with the palette attribute for the next tile.
    Address palette_attribute_shift_register_1 = 0x0000;
    Address palette_attribute_shift_register_2 = 0x0000;
    
    //associated latches
    Byte pattern_data_shift_register_1_latch = 0x00;
    Byte pattern_data_shift_register_2_latch = 0x00;
    Byte next_pattern_data_shift_register_location = 0x00; //buffer to store the location of the next tile untile we fetch it 
    
    bool palette_attribute_shift_register_1_latch = false;
    bool palette_attribute_shift_register_2_latch = false;
    
    
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
    Byte getOAMDMA();     //get OAM DMA register (high byte)
    
    void setPPUCTRL(Byte);    //get PPU control register
    void setPPUMASK(Byte);    //get PPU mask register
    void setPPUSTATUS(Byte);  //get PPU status register
    void setOAMADDR(Byte);    //OAM address port
    void setOAMDATA(Byte);    //get OAM data port
    void setPPUSCROLL(Byte);  //get scrolling position register
    void setPPUADDR(Byte);    //get address register
    void setOAMDMA(Byte);     //get OAM DMA register (high byte)
    
    
    //rendering functions
    void reloadShifters();
    void ntbyte();
    void LowBGByteTile();
    void HighBGByteTile();
    void ATByte();
    void incHori_v();
    void shift();
    void incY();
    
    
    
    //The 15 bit registers t and v are composed this way during rendering:
        //yyy NN YYYYY XXXXX
        //||| || ||||| +++++-- coarse X scroll
        //||| || +++++-------- coarse Y scroll
        //||| ++-------------- nametable select
        //+++----------------- fine Y scroll
    //Current VRAM address (15 bits)
    Address vmem_addr = 0x0000;
    //Temporary VRAM address (15 bits); can also be thought of as the address of the top left onscreen tile.
    Address addr_t = 0x00;
    //Fine X scroll (3 bits)
    Byte fine_x_scroll = 0x00;
    //First or second write toggle (1 bit)
    bool write_toggle = false;
    
    bool asknmi = false;
    
    Byte read_buffer = 0x00;
    
    /*
     chrROM
    */
    void write(Address, Byte);
    Byte read(Address);
    
    /*
     Other
    */
//    Byte address_latch = 0x00;
    NES *nes;
    void clock();
    
    //debug
    int get_scanline(){ return scanline; }
    int get_row(){ return row; }
    struct registers r() {return registers; }
    int getrow(){ return row;}
    int getscanline(){ return scanline;}
    GRAPHICS getgraphics() { return graphics; }
    
#warning debug
    int frame = 0;
};






#endif /* ppu_hpp */
