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
    //7  bit  0
    //---- ----
    //VSO. ....
    //|||| ||||
    //|||+-++++- Least significant bits previously written into a PPU register
    //|||        (due to register not being updated for this address)
    return (this->registers.PPUSTATUS & 0xE0) | (this->read_buffer & 0x1F);
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
//get OAM DMA register (high byte)
Byte PPU::getOAMDMA(){
    return this->registers.OAMDMA;
}

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
    else if(addr <= 0x3EFF){ //nametables
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
            if(((addr & 0x0FFF) < 0x0400) | (((addr & 0x0FFF) >= 0x0800) & ((addr & 0x0FFF) < 0x0C00))){
                this->Nametable->at(0).at(addr & 0x03FF) = content; //&0x3FF because each nametable is 0x0400 wide
                this->Nametable->at(2).at(addr & 0x03FF) = content;
            }
            else{
                this->Nametable->at(1).at(addr & 0x03FF) = content;
                this->Nametable->at(3).at(addr & 0x03FF) = content;
            }
        }
        else{ //horizontal mirroring
            if(((addr & 0x0FFF) < 0x0800)){
                this->Nametable->at(0).at(addr & 0x03FF) = content;
                this->Nametable->at(1).at(addr & 0x03FF) = content;
            }
            else{
                this->Nametable->at(2).at(addr & 0x03FF) = content;
                this->Nametable->at(3).at(addr & 0x03FF) = content;
            }
        }
    }
    else if (addr <= 0x3FFF){ //palette
        //The palette for the background runs from VRAM $3F00 to $3F0F; the palette for the sprites runs from $3F10 to $3F1F. Each color takes up one byte.
        //Addresses $3F04/$3F08/$3F0C can contain unique data, though these values are not used by the PPU when normally rendering (since the pattern values that would otherwise select those cells select the backdrop color instead).
        //Addresses $3F10/$3F14/$3F18/$3F1C are mirrors of $3F00/$3F04/$3F08/$3F0C. Note that this goes for writing as well as reading.
        //$3F20-$3FFF    $00E0    Mirrors of $3F00-$3F1F
        this->Palette->at(addr & 0x001F) = content;
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
    else if(addr <= 0x3EFF){ //nametables
        if((addr & 0x0FFF) < 0x0400) return this->Nametable->at(0).at(addr & 0x03FF);
        if((addr & 0x0FFF) < 0x0800) return this->Nametable->at(1).at(addr & 0x03FF);
        if((addr & 0x0FFF) < 0x0C00) return this->Nametable->at(2).at(addr & 0x03FF);
        return this->Nametable->at(3).at(addr & 0x03FF);
    }
    else if (addr <= 0x3FFF) //palette
        //$3F20-$3FFF    $00E0    Mirrors of $3F00-$3F1F
        return this->Palette->at(addr & 0x001F);
    
    //won't happend
    return 0x00;
}

void PPU::reloadShifters(){//The shifters are reloaded during ticks 9, 17, 25, ..., 257.
    this->pattern_data_shift_register_1 = (this->pattern_data_shift_register_1 & 0xFF00) | this->pattern_data_shift_register_1_latch;
    this->pattern_data_shift_register_2 = (this->pattern_data_shift_register_1 & 0xFF00) | this->pattern_data_shift_register_2_latch;
    
    if(palette_attribute_shift_register_1_latch)
        this->palette_attribute_shift_register_1 = (this->palette_attribute_shift_register_1 & 0xFF00) | 0x00FF;
    else
        this->palette_attribute_shift_register_1 = (this->palette_attribute_shift_register_1 & 0xFF00) | 0x0000;
    
    if(palette_attribute_shift_register_2_latch)
        this->palette_attribute_shift_register_2 = (this->palette_attribute_shift_register_2 & 0xFF00) | 0x00FF;
    else
        this->palette_attribute_shift_register_2 = (this->palette_attribute_shift_register_2 & 0xFF00) | 0x0000;
}

void PPU::ntbyte(){
    //NT Byte
    this->next_pattern_data_shift_register_location = this->read(0x2000 | (this->vmem_addr & 0x0FFF));
}


void PPU::LowBGByteTile(){//Low BG Byte tile
    //Bit 4 of PPUCTRL: Background pattern table address (0: $0000; 1: $1000)
    //each tile row is 8bit wide and follow by a second one (msb). Therefor, we must multiply the tile location
    //fine y is used to choose the right row (0 ~ 7)
    this->pattern_data_shift_register_1_latch = this->read(((( (Address) this->registers.PPUCTRL) & 0x0010) << 8) + (((Address) this->next_pattern_data_shift_register_location) << 4) + ((this->vmem_addr & 0x7000) >> 12));
}


void PPU::HighBGByteTile(){
    //the high tile follows the low tile and is 8 Byte wide;
    this->pattern_data_shift_register_2_latch = this->read(((( (Address) this->registers.PPUCTRL) & 0x0010) << 8) + (((Address) this->next_pattern_data_shift_register_location) << 4) + ((this->vmem_addr & 0x7000) >> 12) + 8);
}

void PPU::ATByte(){
    Byte next_palette_attribute_shift_register_location = this->read(0x23C0 | (this->vmem_addr & 0x0C00) | ((this->vmem_addr >> 4) & 0x0038) | ((this->vmem_addr >> 2) & 0x0007));

    //https://wiki.nesdev.org/w/index.php?title=PPU_attribute_tables
    //7654 3210
    //|||| ||++- Color bits 3-2 for top left quadrant of this byte
    //|||| ++--- Color bits 3-2 for top right quadrant of this byte
    //||++------ Color bits 3-2 for bottom left quadrant of this byte
    //++-------- Color bits 3-2 for bottom right quadrant of this byte
    
    
#warning need to verify
//                if(this->vmem_addr & 0x0040){//bottom half
//                    if(this->vmem_addr & 0x0002){//right
//                        this->palette_attribute_shift_register_1_latch = (next_palette_attribute_shift_register_location & 0x40) == 0x40;
//                        this->palette_attribute_shift_register_2_latch = (next_palette_attribute_shift_register_location & 0x80) == 0x80;
//                    }
//                    else{//left
//                        this->palette_attribute_shift_register_1_latch = (next_palette_attribute_shift_register_location & 0x10) == 0x10;
//                        this->palette_attribute_shift_register_2_latch = (next_palette_attribute_shift_register_location & 0x20) == 0x20;
//                    }
//                }
//                else{//top half
//                    if(this->vmem_addr & 0x0002){//right
//                        this->palette_attribute_shift_register_1_latch = (next_palette_attribute_shift_register_location & 0x04) == 0x04;
//                        this->palette_attribute_shift_register_2_latch = (next_palette_attribute_shift_register_location & 0x08) == 0x08;
//                    }
//                    else{//left
//                        this->palette_attribute_shift_register_1_latch = (next_palette_attribute_shift_register_location & 0x01) == 0x01;
//                        this->palette_attribute_shift_register_2_latch = (next_palette_attribute_shift_register_location & 0x02) == 0x02;
//                    }
//                }
    if (((this->vmem_addr & 0x003E0) >> 5) & 0x02) next_palette_attribute_shift_register_location >>= 4;
    if ((this->vmem_addr & 0x001F) & 0x02) next_palette_attribute_shift_register_location >>= 2;
    next_palette_attribute_shift_register_location &= 0x03;

    if(next_palette_attribute_shift_register_location == 1)
        this->palette_attribute_shift_register_1_latch = true;
    else
        this->palette_attribute_shift_register_1_latch = false;

    if((next_palette_attribute_shift_register_location & 0x2) == 0x2)
        this->palette_attribute_shift_register_2_latch = true;
    else
        this->palette_attribute_shift_register_2_latch = false;
}

void PPU::incHori_v(){
    if((this->registers.PPUMASK & 0x08)){
        if((this->vmem_addr & 0x001F) == 0x1F){ //last tile on that line
            this->vmem_addr &= 0xFFE0;        //coarse X = 0
            this->vmem_addr ^= 0x0400;        //switch horizontal nametable
        }
        else
            this->vmem_addr++;                // increment coarse X
    }
}

void PPU::shift(){
    //shift shift registers so that the most significant bit is the data to fetch
    if((this->registers.PPUMASK & 0x08)){
        this->pattern_data_shift_register_1 = this->pattern_data_shift_register_1 << 1;
        this->pattern_data_shift_register_2 = this->pattern_data_shift_register_2 << 1;
        this->palette_attribute_shift_register_1 = this->palette_attribute_shift_register_1 << 1;
        this->palette_attribute_shift_register_2 = this->palette_attribute_shift_register_2 << 1;
    }
}

void PPU::incY(){
    if ((this->vmem_addr & 0x7000) != 0x7000)
        this->vmem_addr += 0x1000; //incr fine Y
    else{
        this->vmem_addr &= 0x0FFF; //fine Y = 0
        
        int y = (this->vmem_addr & 0x03E0) >> 5; //y = coarse Y

        if(y == 29){
            this->vmem_addr &= 0xFC1F; //coarse Y = 0
            this->vmem_addr &= 0xF7FF | (~(this->vmem_addr & 0x0800) & 0x0800); //switch vertical nametable
        }
        else if(y == 31)// coarse Y = 0, nametable not switched
            this->vmem_addr &= 0xFC1F; //coarse Y = 0
        else // increment coarse Y
            this->vmem_addr += 0x0020;
    }
}

//void PPU::clock(){
//    //Background evaluation
//    //Conceptually, the PPU does this 33 times for each scanline:
//    //Fetch a nametable entry from $2000-$2FBF.
//    //Fetch the corresponding attribute table entry from $23C0-$2FFF and increment the current VRAM address within the same row.
//    //Fetch the low-order byte of an 8x1 pixel sliver of pattern table from $0000-$0FF7 or $1000-$1FF7.
//    //Fetch the high-order byte of this sliver from an address 8 bytes higher.
//    //Turn the attribute data and the pattern table data into palette indices, and combine them with data from sprite data using priority.
//    //It also does a fetch of a 34th (nametable, attribute, pattern) tuple that is never used
//
//
//    // Pre-render scanline (-1 or 261)
//    // This is a dummy scanline, whose sole purpose is to fill the shift registers with the data for the first two tiles of the next scanline. Although no pixels are rendered for this scanline, the PPU still makes the same memory accesses it would for a regular scanline.
//    // This scanline varies in length, depending on whether an even or an odd frame is being rendered. For odd frames, the cycle at the end of the scanline is skipped
//    // During pixels 280 through 304 of this scanline, the vertical scroll bits are reloaded if rendering is enabled.
//    if(this->scanline == -1){
//        if(row == 0){
//            row++;
//            return;
//        }
//        //Vertical blank has started (0: not in vblank; 1: in vblank).
//        //Set at dot 1 of line 241 (the line *after* the post-render
//        //line); cleared after reading $2002 and at dot 1 of the
//        //pre-render line.
//        if(row == 1)
//            this->registers.PPUSTATUS &= 0x7F;
//
//
//        if((this->registers.PPUMASK & 0x40) & (this->row >= 280) & (this->row <= 304)){
//            //If rendering is enabled, at the end of vblank, shortly after the horizontal bits are copied from t to v at dot 257, the PPU will repeatedly copy the vertical bits from t to v from dots 280 to 304, completing the full initialization of v from t:
//            //v: GHI A.BC DEF. .... <- t: GHI A.BC DEF. ....
//            this->vmem_addr = (this->vmem_addr & 0x041F) | (this->addr_t & 0x7BE0);
//        }
//    }
//
//
//    //Vertical blank has started (0: not in vblank; 1: in vblank).
//    //Set at dot 1 of line 241 (the line *after* the post-render
//    //line); cleared after reading $2002 and at dot 1 of the
//    //pre-render line.
//    if((this->scanline == 241) && (this->row == 1)){
//        this->registers.PPUSTATUS |= 0x80;
//
//        if(this->registers.PPUCTRL & 0x80)
//            this->asknmi = true;
//    }
//
//    //https://wiki.nesdev.org/w/images/4/4f/Ppu.svg
//    if((this->scanline <= 239) & (this->row <= 336) & (((this->row <= 257) | (this->row >= 321)) & (this->row != 0))){
//        //The shifters are reloaded during ticks 9, 17, 25, ..., 257.
//        shift();
//
//        //https://wiki.nesdev.org/w/index.php?title=PPU_scrolling
//        //The high bits of v are used for fine Y during rendering, and addressing nametable data only requires 12 bits, with the high 2 CHR address lines fixed to the 0x2000 region. The address to be fetched during rendering can be deduced from v in the following way:
//        //tile address      = 0x2000 | (v & 0x0FFF)
//        //attribute address = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07)
//
//        //each opperation last for two cycles. Just like with the CPU, we're gonna do it on the first cycle and idle on the second one
//        switch (this->row % 8) {
//            case 0: //inc. hori(v)
//                incHori_v();
//                break;
//
//            case 1: //NT Byte
//                ntbyte();
//
//                break;
//
//            case 3:{ //AT Byte
//                ATByte();
//
//                break;
//            }
//
//            case 5: //Low BG Byte tile
//                LowBGByteTile();
//                break;
//
//            case 7: //High BG Byte tile
//                HighBGByteTile();
//#warning only 0
////                std::cout << std::hex << (int) this->pattern_data_shift_register_2_latch << "\n";
//                break;
//
//
//            //operation has already been executed
//            default:
//                break;
//        }
//
//
//        if((this->registers.PPUMASK & 0x08) & (this->row == 256)){//incr y
//            incY();
//        }
//
//
//        if((this->registers.PPUMASK & 0x08) & (this->row == 257) & (this->scanline <= 239)){ //hori(v) = hori(t)
//            this->vmem_addr = (this->vmem_addr & 0xFBE0) | (this->addr_t & 0x041F);
//        }
//
//
//    }
//
//
//    //we read the appropriate (defined by fine x) bit in the pattern shiffters
//    bool pixel_value_high = (this->pattern_data_shift_register_2 & (0x8000 >> (int) this->fine_x_scroll)) != 0;
//    bool pixel_value_low = (this->pattern_data_shift_register_1 & (0x8000 >> (int) this->fine_x_scroll)) != 0;
//
//    //we read the appropriate (defined by fine x) bit in the palette shiffters
//    bool palette_value_high = (palette_attribute_shift_register_2 & (0x8000 >> (int) this->fine_x_scroll)) != 0;
//    bool palette_value_low = (palette_attribute_shift_register_1 & (0x8000 >> (int) this->fine_x_scroll)) != 0;
//
////    if((((pixel_value_high << 1) | pixel_value_low | (palette_value_high << 3) | (palette_value_low << 2)) & 0x3F) != 0)
////       std::cout << std::hex << (int) (((pixel_value_high << 1) | pixel_value_low | (palette_value_high << 3) | (palette_value_low << 2)) & 0x3F);
////
//
//    //43210
//    //|||||
//    //|||++- Pixel value from tile data
//    //|++--- Palette number from attribute table or OAM
//    //+----- Background/Sprite select
////    std::cout << ((pixel_value_high << 1) | pixel_value_low | (palette_value_high << 3) | (palette_value_low << 2));
//    GRAPHICS::Color c = this->palette->at(this->read( 0x3F00 + ((pixel_value_high << 1) | pixel_value_low | (palette_value_high << 3) | (palette_value_low << 2)) ) & 0x3F);
//    graphics.DrawPixel(row - 1, scanline, c);
//
////    std::cout << std::hex << (int) this->read( 0x3F00 + (((pixel_value_high << 1) | pixel_value_low | (palette_value_high << 3) | (palette_value_low << 2))) & 0x3F) << "\n";
//    std::cout << std::hex << (int) (((pixel_value_high << 1) | pixel_value_low | (palette_value_high << 3) | (palette_value_low << 2)) ) << "\n";
////    if((this->row >= 0) && (this->row <= 255) && (this->scanline >= 0) && (this->scanline <= 239))
////        graphics.DrawPixel(row, scanline, this->palette->at(this->Palette->at(rand() % 0xf)));
//
//    row++;                                   //each cycle the ppu generate one pixel
//    if(this->row == 361){
//        this->row = 0;
//
//        this->scanline++;                    //switch to next line
//        if(this->scanline == 261){
//            this->scanline = -1;             //return to pre-render scanline
//
//            this->odd_frame = !this->odd_frame;
//            if(this->odd_frame)
//                this->row = 1;               //first cycle is skiped on odd frames
//
//
//
//            //DRAW
//            SDL_PollEvent(&event);           // Catching the poll event.
//            if(event.type == SDL_KEYDOWN) graphics.~GRAPHICS();
//            else graphics.update();
//        }
//
//
//
//    }
//}

void PPU::clock(){
    if(this->scanline <= 239){//it includes the pre-render line
        if((this->scanline == -1) & (this->row == 1))
            this->registers.PPUSTATUS &= 0x5F;

        if((this->row >= 1) && ((this->row <= 255) || ((this->row >= 321) && (this->row <= 338)))){
            shift();
            
            switch (this->row % 8) {
                case 0: //inc hori(v)
                    incHori_v();
                    break;

                case 1: //NT Byte
                    reloadShifters();
                    ntbyte();
                    break;

                case 3: // AT Byte
                    ATByte();
                    break;

                case 5: //low BG Tile
                    LowBGByteTile();
                    break;

                case 7://high BG Tile
                    HighBGByteTile();
                    break;

                default://second cycle of the opperation
                    break;
            }
        }

        if(this->row == 256)
            incY();

        if(this->row == 257){
            reloadShifters();
            this->vmem_addr = (this->vmem_addr & 0xFBE0) | (this->addr_t & 0x041F);
        }
        
#warning read NTByte at 337 and 339 ?

        if((this->scanline == -1) && (this->row >= 280) && (this->row <= 304)){
            this->vmem_addr = (this->vmem_addr & 0x041F) | (this->addr_t & 0x7BE0);
            
            if(this->registers.PPUCTRL & 0x80)
                this->asknmi = true;
        }
    }

    if((this->scanline == 241) && (this->row == 1))
        this->registers.PPUSTATUS |= 0x80;
    
    
    
    
    

    
    
    
    
        row++;                                   //each cycle the ppu generate one pixel
        if(this->row == 361){
            this->row = 0;
    
            this->scanline++;                    //switch to next line
            if(this->scanline == 261){
                this->scanline = -1;             //return to pre-render scanline
    
                this->odd_frame = !this->odd_frame;
                if(this->odd_frame)
                    this->row = 1;               //first cycle is skiped on odd frames
    
    
    
                //DRAW
                SDL_PollEvent(&event);           // Catching the poll event.
                if(event.type == SDL_KEYDOWN) graphics.~GRAPHICS();
                else graphics.update();
            }
        }
    
}
