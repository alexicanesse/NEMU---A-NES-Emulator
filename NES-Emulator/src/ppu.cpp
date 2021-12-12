//
//  ppu.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 07/12/2021.
//

#include "ppu.hpp"
#include "nes.hpp"


/*
    Constructor
*/
PPU::PPU(NES *nes){
    this->nes = nes;

    
    /*
    define the palette
    https://wiki.nesdev.org/w/index.php/PPU_palettes#Palettes
    */
    this->palette->at(0x00).r = 84;
    this->palette->at(0x00).g = 84;
    this->palette->at(0x00).b = 84;
    
    this->palette->at(0x10).r = 152;
    this->palette->at(0x10).g = 150;
    this->palette->at(0x10).b = 152;
    
    this->palette->at(0x20).r = 236;
    this->palette->at(0x20).g = 238;
    this->palette->at(0x20).b = 236;
    
    this->palette->at(0x30).r = 236;
    this->palette->at(0x30).g = 238;
    this->palette->at(0x30).b = 236;
    
    this->palette->at(0x11).r = 0;
    this->palette->at(0x11).g = 30;
    this->palette->at(0x11).b = 116;
    
    this->palette->at(0x11).r = 8;
    this->palette->at(0x11).g = 76;
    this->palette->at(0x11).b = 196;
    
    this->palette->at(0x21).r = 76;
    this->palette->at(0x21).g = 154;
    this->palette->at(0x21).b = 236;
    
    this->palette->at(0x31).r = 168;
    this->palette->at(0x31).g = 204;
    this->palette->at(0x31).b = 236;
    
    this->palette->at(0x02).r = 8;
    this->palette->at(0x02).g = 16;
    this->palette->at(0x02).b = 144;
    
    this->palette->at(0x12).r = 48;
    this->palette->at(0x12).g = 50;
    this->palette->at(0x12).b = 236;
    
    this->palette->at(0x22).r = 120;
    this->palette->at(0x22).g = 124;
    this->palette->at(0x22).b = 236;
    
    this->palette->at(0x32).r = 188;
    this->palette->at(0x32).g = 188;
    this->palette->at(0x32).b = 236;
    
    this->palette->at(0x03).r = 48;
    this->palette->at(0x03).g = 0;
    this->palette->at(0x03).b = 136;
    
    this->palette->at(0x13).r = 92;
    this->palette->at(0x13).g = 30;
    this->palette->at(0x13).b = 228;
    
    this->palette->at(0x23).r = 176;
    this->palette->at(0x23).g = 98;
    this->palette->at(0x23).b = 236;
    
    this->palette->at(0x33).r = 212;
    this->palette->at(0x33).g = 178;
    this->palette->at(0x33).b = 236;
    
    this->palette->at(0x04).r = 68;
    this->palette->at(0x04).g = 0;
    this->palette->at(0x04).b = 100;
    
    this->palette->at(0x14).r = 136;
    this->palette->at(0x14).g = 20;
    this->palette->at(0x14).b = 176;

    this->palette->at(0x24).r = 228;
    this->palette->at(0x24).g = 84;
    this->palette->at(0x24).b = 236;
    
    this->palette->at(0x34).r = 236;
    this->palette->at(0x34).g = 174;
    this->palette->at(0x34).b = 236;
    
    this->palette->at(0x05).r = 92;
    this->palette->at(0x05).g = 0;
    this->palette->at(0x05).b = 48;
    
    this->palette->at(0x15).r = 160;
    this->palette->at(0x15).g = 20;
    this->palette->at(0x15).b = 100;

    this->palette->at(0x25).r = 236;
    this->palette->at(0x25).g = 88;
    this->palette->at(0x25).b = 180;
    
    this->palette->at(0x35).r = 236;
    this->palette->at(0x35).g = 174;
    this->palette->at(0x35).b = 212;
    
    this->palette->at(0x06).r = 84;
    this->palette->at(0x06).g = 4;
    this->palette->at(0x06).b = 0;
    
    this->palette->at(0x16).r = 152;
    this->palette->at(0x16).g = 34;
    this->palette->at(0x16).b = 32;

    this->palette->at(0x26).r = 236;
    this->palette->at(0x26).g = 106;
    this->palette->at(0x26).b = 100;
    
    this->palette->at(0x36).r = 236;
    this->palette->at(0x36).g = 180;
    this->palette->at(0x36).b = 176;
    
    this->palette->at(0x07).r = 60;
    this->palette->at(0x07).g = 24;
    this->palette->at(0x07).b = 0;
    
    this->palette->at(0x17).r = 120;
    this->palette->at(0x17).g = 60;
    this->palette->at(0x17).b = 0;

    this->palette->at(0x27).r = 212;
    this->palette->at(0x27).g = 136;
    this->palette->at(0x27).b = 32;
    
    this->palette->at(0x37).r = 228;
    this->palette->at(0x37).g = 196;
    this->palette->at(0x37).b = 144;
    
    this->palette->at(0x08).r = 32;
    this->palette->at(0x08).g = 42;
    this->palette->at(0x08).b = 0;
    
    this->palette->at(0x18).r = 84;
    this->palette->at(0x18).g = 90;
    this->palette->at(0x18).b = 0;
    
    this->palette->at(0x28).r = 160;
    this->palette->at(0x28).g = 170;
    this->palette->at(0x28).b = 0;
    
    this->palette->at(0x38).r = 204;
    this->palette->at(0x38).g = 210;
    this->palette->at(0x38).b = 120;
    
    this->palette->at(0x09).r = 8;
    this->palette->at(0x09).g = 58;
    this->palette->at(0x09).b = 0;
    
    this->palette->at(0x19).r = 40;
    this->palette->at(0x19).g = 114;
    this->palette->at(0x19).b = 0;
    
    this->palette->at(0x29).r = 116;
    this->palette->at(0x29).g = 196;
    this->palette->at(0x29).b = 0;
    
    this->palette->at(0x39).r = 180;
    this->palette->at(0x39).g = 222;
    this->palette->at(0x39).b = 120;
    
    this->palette->at(0x0A).r = 0;
    this->palette->at(0x0A).g = 64;
    this->palette->at(0x0A).b = 0;
    
    this->palette->at(0x1A).r = 8;
    this->palette->at(0x1A).g = 124;
    this->palette->at(0x1A).b = 0;
    
    this->palette->at(0x2A).r = 76;
    this->palette->at(0x2A).g = 208;
    this->palette->at(0x2A).b = 32;
    
    this->palette->at(0x3A).r = 168;
    this->palette->at(0x3A).g = 226;
    this->palette->at(0x3A).b = 144;
    
    this->palette->at(0x0B).r = 0;
    this->palette->at(0x0B).g = 60;
    this->palette->at(0x0B).b = 0;
    
    this->palette->at(0x1B).r = 0;
    this->palette->at(0x1B).g = 118;
    this->palette->at(0x1B).b = 40;
    
    this->palette->at(0x2B).r = 56;
    this->palette->at(0x2B).g = 204;
    this->palette->at(0x2B).b = 108;
    
    this->palette->at(0x3B).r = 152;
    this->palette->at(0x3B).g = 226;
    this->palette->at(0x3B).b = 180;
    
    this->palette->at(0x0C).r = 0;
    this->palette->at(0x0C).g = 50;
    this->palette->at(0x0C).b = 60;
    
    this->palette->at(0x1C).r = 0;
    this->palette->at(0x1C).g = 102;
    this->palette->at(0x1C).b = 120;
    
    this->palette->at(0x2C).r = 56;
    this->palette->at(0x2C).g = 180;
    this->palette->at(0x2C).b = 204;
    
    this->palette->at(0x3C).r = 160;
    this->palette->at(0x3C).g = 214;
    this->palette->at(0x3C).b = 228;
    
    this->palette->at(0x0D).r = 0;
    this->palette->at(0x0D).g = 0;
    this->palette->at(0x0D).b = 0;
    
    this->palette->at(0x1D).r = 0;
    this->palette->at(0x1D).g = 0;
    this->palette->at(0x1D).b = 0;
    
    this->palette->at(0x2D).r = 60;
    this->palette->at(0x2D).g = 60;
    this->palette->at(0x2D).b = 60;
    
    this->palette->at(0x3D).r = 160;
    this->palette->at(0x3D).g = 162;
    this->palette->at(0x3D).b = 160;
    
    this->palette->at(0x0E).r = 0;
    this->palette->at(0x0E).g = 0;
    this->palette->at(0x0E).b = 0;
    
    this->palette->at(0x1E).r = 0;
    this->palette->at(0x1E).g = 0;
    this->palette->at(0x1E).b = 0;
    
    this->palette->at(0x2E).r = 0;
    this->palette->at(0x2E).g = 0;
    this->palette->at(0x2E).b = 0;
    
    this->palette->at(0x3E).r = 0;
    this->palette->at(0x3E).g = 0;
    this->palette->at(0x3E).b = 0;
    
    this->palette->at(0x0F).r = 0;
    this->palette->at(0x0F).g = 0;
    this->palette->at(0x0F).b = 0;
    
    this->palette->at(0x1F).r = 0;
    this->palette->at(0x1F).g = 0;
    this->palette->at(0x1F).b = 0;
    
    this->palette->at(0x2F).r = 0;
    this->palette->at(0x2F).g = 0;
    this->palette->at(0x2F).b = 0;
    
    this->palette->at(0x3F).r = 0;
    this->palette->at(0x3F).g = 0;
    this->palette->at(0x3F).b = 0;
}


/*
 Registers
*/
//get PPU control register
Byte PPU::getPPUCTRL(){
    return this->registers.PPUCTRL;
}
//get PPU mask register
Byte PPU::getPPUMASK(){
    return this->registers.PPUMASK;
}
//get PPU status register
Byte PPU::getPPUSTATUS(){
    return this->registers.PPUSTATUS;
}
//OAM address port
Byte PPU::getOAMADDR(){
    return this->registers.OAMADDR;
}
//get OAM data port
Byte PPU::getOAMDATA(){
    return this->registers.OAMDATA;
}
//get scrolling position register
Byte PPU::getPPUSCROLL(){
    return this->registers.PPUSCROLL;
}
//get address register
Byte PPU::getPPUADDR(){
    return this->registers.PPUADDR;
}
//get PPU data port
Byte PPU::getPPUDATA(){
    return this->registers.PPUDATA;
}
//get OAM DMA register (high byte)
Byte PPU::getOAMDMA(){
    return this->registers.OAMDMA;
}

#warning i need to check this later 
//set PPU control register
void PPU::setPPUCTRL(Byte data){
    this->registers.PPUCTRL = data;
}
//set PPU mask register
void PPU::setPPUMASK(Byte data){
    this->registers.PPUMASK = data;
}
//set PPU status register
void PPU::setPPUSTATUS(Byte data){
    this->registers.PPUSTATUS = data;
}
//OAM address port
void PPU::setOAMADDR(Byte data){
    this->registers.OAMADDR = data;
}
//set OAM data port
void PPU::setOAMDATA(Byte data){
    this->registers.OAMDATA = data;
}
//set scrolling position register
void PPU::setPPUSCROLL(Byte data){
    this->registers.PPUSCROLL = data;
}
//set address register
void PPU::setPPUADDR(Byte data){
    this->registers.PPUADDR = data;
}
//set PPU data port
void PPU::setPPUDATA(Byte data){
    this->registers.PPUDATA = data;
}
//set OAM DMA register (high byte)
void PPU::setOAMDMA(Byte data){
    this->registers.OAMDMA = data;
}


void PPU::write(Address addr, Byte content){
    //The pattern table is divided into two 256-tile sections: $0000-$0FFF, nicknamed "left", and $1000-$1FFF, nicknamed "right". The nicknames come from how emulators with a debugger display the pattern table. Traditionally, they are displayed as two side-by-side 128x128 pixel sections, each representing 16x16 tiles from the pattern table, with $0000-$0FFF on the left and $1000-$1FFF on the right.
    if(addr <= 0x1FFF) //tablepattern
        switch (addr & 0x1000) {
            case 0:
                this->Pattern_table->at(0).at(addr) = content;
                break;
                
            case 0x1000:
                this->Pattern_table->at(1).at(addr & 0xFFF) = content;
                break;
        }
    else if(addr <= 0X3EFF){ //nametables
    //    (0,0)     (256,0)     (511,0)
    //       +-----------+-----------+
    //       |           |           |
    //       |           |           |
    //       |   $2000   |   $2400   |
    //       |           |           |
    //       |           |           |
    //(0,240)+-----------+-----------+(511,240)
    //       |           |           |
    //       |           |           |
    //       |   $2800   |   $2C00   |
    //       |           |           |
    //       |           |           |
    //       +-----------+-----------+
    //     (0,479)   (256,479)   (511,479)
    //    The NES has four logical nametables, arranged in a 2x2 pattern. Each occupies a 1 KiB chunk of PPU address space, starting at $2000 at the top left, $2400 at the top right, $2800 at the bottom left, and $2C00 at the bottom right.
    //    But the NES system board itself has only 2 KiB of VRAM (called CIRAM, stored in a separate SRAM chip), enough for two physical nametables; hardware on the cartridge controls address bit 10 of CIRAM to map one nametable on top of another.
    //    Vertical mirroring: $2000 equals $2800 and $2400 equals $2C00 (e.g. Super Mario Bros.)
    //    Horizontal mirroring: $2000 equals $2400 and $2800 equals $2C00 (e.g. Kid Icarus)
        if(this->nes->cartridge->mirroring_v){ //vertical mirroring
            if((addr & 0x07FF) <= 0x400){
                this->Nametable->at(0).at(addr & 0x0FFF) = content;
                this->Nametable->at(2).at(addr & 0x0FFF) = content;
            }
            else{
                this->Nametable->at(1).at(addr & 0x0FFF) = content;
                this->Nametable->at(3).at(addr & 0x0FFF) = content;
            }
        }
        else{ //horizontal mirroring
            if((addr & 0x07FF) <= 0x400){
                this->Nametable->at(0).at(addr & 0x07FF) = content;
                this->Nametable->at(1).at(addr & 0x07FF) = content;
            }
            else{
                this->Nametable->at(2).at(addr & 0x07FF) = content;
                this->Nametable->at(3).at(addr & 0x07FF) = content;
            }
        }
    }
    else if (addr <= 0x3F1F){ //palette
        //The palette for the background runs from VRAM $3F00 to $3F0F; the palette for the sprites runs from $3F10 to $3F1F. Each color takes up one byte.
        //Addresses $3F04/$3F08/$3F0C can contain unique data, though these values are not used by the PPU when normally rendering (since the pattern values that would otherwise select those cells select the backdrop color instead).
        //Addresses $3F10/$3F14/$3F18/$3F1C are mirrors of $3F00/$3F04/$3F08/$3F0C. Note that this goes for writing as well as reading.
        this->Palette->at(addr & 0x00FF) = content;
        switch (addr & 0x000F) {
            case 0x00:
                this->Palette->at(0x00) = content;
                this->Palette->at(0x10) = content;
                break;
                
            case 0x04:
                this->Palette->at(0x04) = content;
                this->Palette->at(0x14) = content;
                break;
                
            case 0x08:
                this->Palette->at(0x08) = content;
                this->Palette->at(0x18) = content;
                break;
                
            case 0x0C:
                this->Palette->at(0x08) = content;
                this->Palette->at(0x1C) = content;
                break;
                
            default:
                break;
        }
    }
}

Byte PPU::read(Address addr){
    //The pattern table is divided into two 256-tile sections: $0000-$0FFF, nicknamed "left", and $1000-$1FFF, nicknamed "right". The nicknames come from how emulators with a debugger display the pattern table. Traditionally, they are displayed as two side-by-side 128x128 pixel sections, each representing 16x16 tiles from the pattern table, with $0000-$0FFF on the left and $1000-$1FFF on the right.
    if(addr <= 0x1FFF) //tablepattern
        switch (addr & 0x1000) {
            case 0:
                return this->Pattern_table->at(0).at(addr);
                break;
                
            case 0x1000:
                return this->Pattern_table->at(1).at(addr & 0xFFF);
                break;
                
            default: //won't happend
                return 0x00;
                break;
        }
    else if(addr <= 0X3EFF){ //nametables
        if(addr < 0x2400) return this->Nametable->at(0).at(addr & 0x03FF);
        if(addr < 0x2800) return this->Nametable->at(1).at(addr & 0x03FF);
        if(addr < 0xC200) return this->Nametable->at(3).at(addr & 0x03FF);
        return this->Nametable->at(4).at(addr & 0x03FF);
    }
    else if (addr <= 0x3F1F) //palette
        return this->Palette->at(addr & 0x00FF);
}



void PPU::clock(){
#warning todo handle oddframes
    graphics.DrawPixel(row, scanline, palette->at(rand() % 64));

    row++; //each cycle the ppu generate one pixel
    
    

    
    if(row == 361){
        row = 0;
        scanline++; //switch to next line
        
        if(scanline == 261){
            scanline = -1; //return to beggining
            SDL_PollEvent(&event);  // Catching the poll event.
            if(event.type == SDL_KEYDOWN) graphics.~GRAPHICS();
            else graphics.update();
        }
    }
}
