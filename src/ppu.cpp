//
//  ppu.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 07/12/2021.
//
#include <thread>
#include <iostream>
#include <array>


#include "screen.hpp"
#include "ppu.hpp"
#include "nes.hpp"


/*
    Constructor
*/
PPU::PPU(NES *nes, float screen_coef){
    this->nes = nes;
    this->graphics = new GRAPHICS(screen_coef);
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
/* Get PPU control register */
Byte PPU::getPPUCTRL(){
    return this->registers.PPUCTRL;
}

/* Get PPU mask register */
Byte PPU::getPPUMASK(){
    return this->registers.PPUMASK;
}

/* Get PPU status register */
Byte PPU::getPPUSTATUS(){
    //7  bit  0
    //---- ----
    //VSO. ....
    //|||| ||||
    //|||+-++++- Least significant bits previously written into a PPU register
    //|||        (due to register not being updated for this address)
    return (this->registers.PPUSTATUS & 0xE0) | (this->read_buffer & 0x1F);
}

/* OAM address port */
Byte PPU::getOAMADDR(){
    return this->registers.OAMADDR;
}

/* Get OAM data port */
Byte PPU::getOAMDATA(){
    return ((uint8_t *) this->OAM)[this->registers.OAMADDR]; /* We convert it to a pointer in order to read the appropriate byte without having to deal with arithmetic as the array is 64*4 */
}

/* Get scrolling position register */
Byte PPU::getPPUSCROLL(){
    return this->registers.PPUSCROLL;
}

/* Get address register */
Byte PPU::getPPUADDR(){
    return this->registers.PPUADDR;
}

/* Get OAM DMA register (high byte) */
Byte PPU::getOAMDMA(){
    return this->registers.OAMDMA;
}

/* Set PPU control register */
void PPU::setPPUCTRL(Byte data){
    this->registers.PPUCTRL = data;
}

/* Set PPU mask register */
void PPU::setPPUMASK(Byte data){
    this->registers.PPUMASK = data;
}

/* Set PPU status register */
void PPU::setPPUSTATUS(Byte data){
    this->registers.PPUSTATUS = data;
}

/* OAM address port */
void PPU::setOAMADDR(Byte data){
    this->registers.OAMADDR = data;
}

/* Set OAM data port */
void PPU::setOAMDATA(Byte data){
    ((uint8_t *)this->OAM)[this->registers.OAMADDR++] = data; /* oamaddr is incremented after the write */
    /* We convert it to a pointer in order to write the appropriate byte location */
}

/* Set scrolling position register */
void PPU::setPPUSCROLL(Byte data){
    this->registers.PPUSCROLL = data;
}

/* Set address register */
void PPU::setPPUADDR(Byte data){
    this->registers.PPUADDR = data;
}

/* Set OAM DMA register (high byte) */
void PPU::setOAMDMA(Byte data){
    this->registers.OAMDMA = data;
}

void PPU::setOAM_with_addr(Byte content, Address addr){
    ((uint8_t *)this->OAM)[addr] = content;
    /* We convert it to a pointer in order to write the appropriate byte location */
}


void PPU::write(Address addr, Byte content){
    //The pattern table is divided into two 256-tile sections: $0000-$0FFF, nicknamed "left", and $1000-$1FFF, nicknamed "right". The nicknames come from how emulators with a debugger display the pattern table. Traditionally, they are displayed as two side-by-side 128x128 pixel sections, each representing 16x16 tiles from the pattern table, with $0000-$0FFF on the left and $1000-$1FFF on the right.
    if (addr <= 0x1FFF) /* Tablepattern */
        switch (addr & 0x1000) {
            case 0:
                (*this->Pattern_table)[0][addr] = content;
                break;
                
            case 0x1000:
                (*this->Pattern_table)[1][addr & 0xFFF] = content;
                break;
        }
    else if(addr <= 0x3EFF) { /* Nametables */
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
        if (this->nes->cartridge->mirroring_v) { /* Vertical mirroring */
            if (((addr & 0x0FFF) < 0x0400) || (((addr & 0x0FFF) >= 0x0800) && ((addr & 0x0FFF) < 0x0C00))) {
                (*this->Nametable)[0][addr & 0x03FF] = content; /* &0x3FF because each nametable is 0x0400 wide */
                (*this->Nametable)[2][addr & 0x03FF] = content;
            } else {
                (*this->Nametable)[1][addr & 0x03FF] = content;
                (*this->Nametable)[3][addr & 0x03FF] = content;
            }
        }
        else{ /* Horizontal mirroring */
            if (((addr & 0x0FFF) < 0x0800)) {
                (*this->Nametable)[0][addr & 0x03FF] = content;
                (*this->Nametable)[1][addr & 0x03FF] = content;
            } else {
                (*this->Nametable)[2][addr & 0x03FF] = content;
                (*this->Nametable)[3][addr & 0x03FF] = content;
            }
        }
    }
    else if (addr <= 0x3FFF){ /* Palette */
        //The palette for the background runs from VRAM $3F00 to $3F0F; the palette for the sprites runs from $3F10 to $3F1F. Each color takes up one byte.
        //Addresses $3F04/$3F08/$3F0C can contain unique data, though these values are not used by the PPU when normally rendering (since the pattern values that would otherwise select those cells select the backdrop color instead).
        //Addresses $3F10/$3F14/$3F18/$3F1C are mirrors of $3F00/$3F04/$3F08/$3F0C. Note that this goes for writing as well as reading.
        //$3F20-$3FFF    $00E0    Mirrors of $3F00-$3F1F
        (*this->Palette)[addr & 0x001F] = content;
        switch (addr & 0x000F) {
            case 0x00:
                (*this->Palette)[0x00] = content;
                (*this->Palette)[0x10] = content;
                break;
                
            case 0x04:
                (*this->Palette)[0x04] = content;
                (*this->Palette)[0x14] = content;
                break;
                
            case 0x08:
                (*this->Palette)[0x08] = content;
                (*this->Palette)[0x18] = content;
                break;
                
            case 0x0C:
                (*this->Palette)[0x08] = content;
                (*this->Palette)[0x1C] = content;
                break;
                
            default:
                break;
        }
    }
}

Byte PPU::read(Address addr) {
    /* The pattern table is divided into two 256-tile sections:
     * $0000-$0FFF, nicknamed "left", and $1000-$1FFF, nicknamed "right".
     * The nicknames come from how emulators with a debugger display the pattern table.
     * Traditionally, they are displayed as two side-by-side 128x128 pixel sections,
     * each representing 16x16 tiles from the pattern table, with $0000-$0FFF on the left
     * and $1000-$1FFF on the right.
     */
    if(addr <= 0x1FFF) { /* Tablepattern */
        switch (addr & 0x1000) {
            case 0:
                return this->Pattern_table->at(0).at(addr);
                break;

            case 0x1000:
                return this->Pattern_table->at(1).at(addr & 0xFFF);
                break;

            default: /* Won't happend */
                return 0x00;
                break;
        }
    } else if (addr <= 0x3EFF) { /* Nametables */
        if ((addr & 0x0FFF) < 0x0400) return (*this->Nametable)[0][addr & 0x03FF];
        if ((addr & 0x0FFF) < 0x0800) return (*this->Nametable)[1][addr & 0x03FF];
        if ((addr & 0x0FFF) < 0x0C00) return (*this->Nametable)[2][addr & 0x03FF];
        return (*this->Nametable)[3][addr & 0x03FF];
    } else if (addr <= 0x3FFF) { /* Palette */
        /* $3F20-$3FFF    $00E0    Mirrors of $3F00-$3F1F */
        return (*this->Palette)[addr & 0x001F];
    }
    
    /* Won't happend */
    return 0x00;
}

void PPU::reloadShifters(){ /* The shifters are reloaded during ticks 9, 17, 25, ..., 257. */
    this->pattern_data_shift_register_1 = (this->pattern_data_shift_register_1 & 0xFF00) | this->pattern_data_shift_register_1_latch;
    this->pattern_data_shift_register_2 = (this->pattern_data_shift_register_2 & 0xFF00) | this->pattern_data_shift_register_2_latch;
    
    if(palette_attribute_shift_register_1_latch)
        this->palette_attribute_shift_register_1 |= 0x00FF;
    else
        this->palette_attribute_shift_register_1 &= 0xFF00;
    
    if(palette_attribute_shift_register_2_latch)
        this->palette_attribute_shift_register_2 |= 0x00FF;
    else
        this->palette_attribute_shift_register_2 &= 0xFF00;
}

void PPU::ntbyte(){
    /* NT Byte */
    this->next_pattern_data_shift_register_location = this->read(0x2000 | (this->vmem_addr & 0x0FFF));
}


void PPU::LowBGByteTile(){ /*Low BG Byte tile */
    /*
     * Bit 4 of PPUCTRL: Background pattern table address (0: $0000; 1: $1000)
     * each tile row is 8bit wide and follow by a second one (msb). Therefor, we must multiply the tile location
     * fine y is used to choose the right row (0 ~ 7)
     */
    this->pattern_data_shift_register_1_latch = this->read(((( (Address) this->registers.PPUCTRL) & 0x0010) << 8) + (((Address) this->next_pattern_data_shift_register_location) << 4) + ((this->vmem_addr & 0x7000) >> 12));
}


void PPU::HighBGByteTile(){
    /* The high tile follows the low tile and is 8 Bytes wide; */
    this->pattern_data_shift_register_2_latch = this->read((((( (Address) this->registers.PPUCTRL) & 0x0010) << 8) + (((Address) this->next_pattern_data_shift_register_location) << 4) + ((this->vmem_addr & 0x7000) >> 12)) + 8);
}

void PPU::ATByte(){
    Byte next_palette_attribute_shift_register_location = this->read(0x23C0 | (this->vmem_addr & 0x0C00) | ((this->vmem_addr >> 4) & 0x0038) | ((this->vmem_addr >> 2) & 0x0007));
    //https://wiki.nesdev.org/w/index.php?title=PPU_attribute_tables
    //7654 3210
    //|||| ||++- Color bits 3-2 for top left quadrant of this byte
    //|||| ++--- Color bits 3-2 for top right quadrant of this byte
    //||++------ Color bits 3-2 for bottom left quadrant of this byte
    //++-------- Color bits 3-2 for bottom right quadrant of this byte
    if(this->vmem_addr & 0x0040){ /* Bottom half */
        if(this->vmem_addr & 0x0002){ /* Right */
            this->palette_attribute_shift_register_1_latch = (next_palette_attribute_shift_register_location & 0x40) == 0x40;
            this->palette_attribute_shift_register_2_latch = (next_palette_attribute_shift_register_location & 0x80) == 0x80;
        } else{ /* Left */
            this->palette_attribute_shift_register_1_latch = (next_palette_attribute_shift_register_location & 0x10) == 0x10;
            this->palette_attribute_shift_register_2_latch = (next_palette_attribute_shift_register_location & 0x20) == 0x20;
        }
    } else{ /* Top half */
        if(this->vmem_addr & 0x0002){ /* Right */
            this->palette_attribute_shift_register_1_latch = (next_palette_attribute_shift_register_location & 0x04) == 0x04;
            this->palette_attribute_shift_register_2_latch = (next_palette_attribute_shift_register_location & 0x08) == 0x08;
        } else{ /* Left */
            this->palette_attribute_shift_register_1_latch = (next_palette_attribute_shift_register_location & 0x01) == 0x01;
            this->palette_attribute_shift_register_2_latch = (next_palette_attribute_shift_register_location & 0x02) == 0x02;
        }
    }
}

void PPU::incHori_v(){
    if((this->vmem_addr & 0x001F) == 0x1F){ /* Last tile on that line */
        this->vmem_addr &= 0xFFE0;          /* coarse X = 0 */
        this->vmem_addr ^= 0x0400;          /* Switch horizontal nametable */
    } else {
        ++this->vmem_addr;                  /* Increment coarse X */
    }
}

void PPU::shift(){
    /* Shift shift registers so that the most significant bit is the data to fetch */
    if((this->registers.PPUMASK & 0x08)){/* Background rendering is enabled */
        this->pattern_data_shift_register_1      <<= 1;
        this->pattern_data_shift_register_2      <<= 1;
        this->palette_attribute_shift_register_1 <<= 1;
        this->palette_attribute_shift_register_2 <<= 1;
    }
    
    if((this->registers.PPUMASK & 0x08) && (this->cycle >= 1) && (this->cycle <= 257)){ /* Foreground rendering is enabled */
        for(int i = 0; i < 8; i++){
            if(i < this->number_of_sprites){               /* For each sprite we found */
                if((*this->sprite_counters)[i] != 0) {     /* We have not reached the sprite yet */
                    --(*this->sprite_counters)[i];
                } else{                                    /* We need to shift the pattern registers */
                    if((*this->sprite_latches)[i] & 0x40){ /* We shift in the other direction because the sprite is flipped horizontaly */
                        (*this->sprite_shift_registers)[i][0] >>= 1;
                        (*this->sprite_shift_registers)[i][1] >>= 1;
                    } else{
                        (*this->sprite_shift_registers)[i][0] <<= 1;
                        (*this->sprite_shift_registers)[i][1] <<= 1;
                    }
                }
            }
        }
    }
}

void PPU::incY(){
    if ((this->vmem_addr & 0x7000) != 0x7000)
        this->vmem_addr += 0x1000; /* incr fine Y */
    else{
        this->vmem_addr &= 0x0FFF; /* fine Y = 0 */
        
        int y = (this->vmem_addr & 0x03E0) >> 5; /* y = coarse Y */

        if(y == 29){
            this->vmem_addr &= 0xFC1F; /* Coarse Y = 0 */
            this->vmem_addr &= 0xF7FF | ((~(this->vmem_addr & 0x0800)) & 0x0800); /* Switch vertical nametable */
        } else if(y == 31) { /* Coarse Y = 0, nametable not switched */
            this->vmem_addr &= 0xFC1F; /* Coarse Y = 0 */
        } else { /* Increment coarse Y */
            this->vmem_addr += 0x0020;
        }
    }
}



void PPU::clock(){
    /* https://wiki.nesdev.org/w/index.php?title=File:Ntsc_timing.png */
    if((this->registers.PPUMASK & 0x18) && (this->scanline <= 239)){ /* It includes the pre-render line */
        if((this->scanline == -1) && (this->cycle == 1)){
            this->registers.PPUSTATUS &= 0x1F; /* Clear vblank, sprite overflow and sprite 0 hit */
        }

        if((this->cycle >= 1) && ((this->cycle <= 256) || ((this->cycle >= 321) && (this->cycle <= 337)))){
            shift();
            
            switch (this->cycle % 8) {
                case 0: /* inc hori(v) */
                    incHori_v();
                    break;

                case 1: /* NT Byte */
                    reloadShifters();
                    ntbyte();
                    break;

                case 3: /* AT Byte */
                    ATByte();
                    break;

                case 5: /* Low BG Tile */
                    LowBGByteTile();
                    break;

                case 7: /* High BG Tile */
                    HighBGByteTile();
                    break;

                default: /* Second cycle of the opperation */
                    break;
            }
        }

        if(this->cycle == 256)
            incY();

        else if(this->cycle == 257){
            reloadShifters();
            this->vmem_addr = (this->vmem_addr & 0xFBE0) | (this->addr_t & 0x041F);
        }

        else if((this->scanline == -1) && (this->cycle >= 280) && (this->cycle <= 304)){
            this->vmem_addr = (this->vmem_addr & 0x041F) | (this->addr_t & 0x7BE0);
        }
    }

    
    
    if((this->scanline == 241) && (this->cycle == 1)){
        this->registers.PPUSTATUS |= 0x80;
        if(this->registers.PPUCTRL & 0x80){
            if(this->registers.PPUMASK & 0x1E) this->asknmi = true;
        }
    }
    


    
    /*
     Sprits
     */
    //Source: https://wiki.nesdev.org/w/index.php?title=PPU_sprite_evaluation
    //During all visible scanlines, the PPU scans through OAM to determine which sprites to render on the next scanline. Sprites found to be within range are copied into the secondary OAM, which is then used to initialize eight internal sprite output units.
    //OAM[n][m] below refers to the byte at offset 4*n + m within OAM, i.e. OAM byte m (0-3) of sprite n (0-63).
    //During each pixel clock (341 total per scanline), the PPU accesses OAM in the following pattern:
    
    
    if((this->registers.PPUMASK & 0x18) && this->scanline <= 239){ /* This includes the pre-render line */
        /* Cycles 1-64: Secondary OAM (32-byte buffer for current sprites on scanline) is initialized to $FF
         * Attempting to read $2004 will return $FF.
         * Internally, the clear operation is implemented by reading from the OAM and writing into the secondary OAM as usual,
         * only a signal is active that makes the read always return $FF.
         */
        
        /* It is not cycle accurate but who cares ? (I do everything during the first cycle and idle during the others */
        if((this->cycle == 1) && (this->scanline >= 0)){ /* (does not append during the pre render line) */
            this->last_available_slot = 0;               /* Secondary OAM is empty */
            is_sprite_0_there         = false;           /* We have not found sprite zero yet */
            for(int i = 0; i < 8; i++){
                (*this->Sec_OAM)[i][0] = 0xFF;
                (*this->Sec_OAM)[i][1] = 0xFF;
                (*this->Sec_OAM)[i][2] = 0xFF;
                (*this->Sec_OAM)[i][3] = 0xFF;
            }
        }

//        if(this->cycle >= 1 && this->cycle <= 64){
            //already done at cycle == 1
//        }

        /* Cycles 65-256: Sprite evaluation */
        else if((this->cycle >= 65) && (this->cycle <= 256) && (this->scanline >= 0)){ /* Does not append during the pre render line */
            /* Sprite evaluation occurs if either the sprite layer or background layer is enabled via $2001. Unless both layers are disabled, it merely hides sprite rendering. */
            /* Sprite evaluation */
            if(this->cycle == 65){ /* It ain't accurate as I'm not using OAMADDR but it only matter when rendering is enable at the middle of the screen because oamaddr is reset at the beggining of rendering */
                this->n                   = 0;
                this->sprite_cycle        = 0;
                this->last_available_slot = 0;
                sprite_search_is_done     = false;
            }
            

            if(!sprite_search_is_done && this->last_available_slot < 8){ /* We have not reached the 8 sprites limit */
                switch (this->sprite_cycle) {
                    case 0:{
                        this->sprite_data_read = (*this->OAM)[n][0];
                        this->sprite_cycle = 1;
                        break;
                    }
                      
                    case 1:{
                        (*this->Sec_OAM)[this->last_available_slot][0] = this->sprite_data_read;

                        /* Is this sprite in range ? */
                        int sprite_height = 0;
                        if(this->registers.PPUCTRL & 0x20) /* Sprite size (0: 8x8 pixels; 1: 8x16 pixels) */
                            sprite_height = 16;
                        else
                            sprite_height = 8;

                        if(((int) (*this->Sec_OAM)[last_available_slot][0] <= this->scanline)
                                && (((int) (*this->Sec_OAM)[last_available_slot][0] + sprite_height) > this->scanline)) /* In range */
                            this->sprite_cycle = 2;
                        else{
                            this->sprite_cycle = 0;
                            n++;        /* Get ready to read the next sprite */
                            if(n == 64) /* n overflow */
                                sprite_search_is_done = true;
                        }
                        break;
                    }
                        
                    case 2:{
                        if(n == 0)
                            this->is_sprite_0_there = true; /* We will render sprite zero during this scanline */
                        this->sprite_data_read = (*this->OAM)[n][1];
                        this->sprite_cycle = 3;
                        break;
                    }
                        
                    case 3:{
                        (*this->Sec_OAM)[this->last_available_slot][1] = this->sprite_data_read;
                        this->sprite_cycle = 4;
                        break;
                    }
                        
                    case 4:{
                        this->sprite_data_read = (*this->OAM)[n][2];
                        this->sprite_cycle = 5;
                        break;
                    }
                        
                    case 5:{
                        (*this->Sec_OAM)[this->last_available_slot][2] = this->sprite_data_read;
                        this->sprite_cycle = 6;
                        break;
                    }
                     
                    case 6:{
                        this->sprite_data_read = (*this->OAM)[n][3];
                        this->sprite_cycle = 7;
                        break;
                    }

                    case 7:{
                        (*this->Sec_OAM)[this->last_available_slot][3] = this->sprite_data_read;
                        this->last_available_slot++; /* We took a slot in secondary oam */
                        this->n++;                   /* Get ready to read the next sprite */
                        if(n == 64)                  /* n overflow */
                            sprite_search_is_done = true;

                        this->sprite_cycle = 0; //this sprite is processed!
                        break;
                    }
                }
                /*
                 * We have fetched 8 sprites
                 * we should look for an other sprite in order to set the sprite overflow flag if it needs to be set
                 * though, this is bugged as hell
                 * During sprite evaluation, if eight in-range sprites have been found so far, the sprite evaluation logic continues to scan the primary OAM looking for one more in-range sprite to determine whether to set the sprite overflow flag. The first such check correctly checks the y coordinate of the next OAM entry, but after that the logic breaks and starts scanning OAM "diagonally", evaluating the tile number/attributes/X-coordinates of subsequent OAM entries as Y-coordinates (due to incorrectly incrementing m when moving to the next sprite). This results in inconsistent sprite overflow behavior showing both false positives and false negatives.
                 * therefor only tricky to emulate games uses this
                 * I'm not taking care of this for now
                 * proof that it doesn't matter that much:
                 * "The sprite overflow flag is rarely used, mainly due to bugs when exactly 8 sprites are present on a scanline. No games rely on the buggy behavior."
                 */
            }
        }

        /* Hblank begins after dot 256, and ends at dot 320 when the first tile of the next line is fetched. */

        /* Cycles 257-320: Sprite fetches (8 sprites total, 8 cycles per sprite) */
        else if((this->registers.PPUMASK & 0x08) && (this->cycle >= 257) && (this->cycle <= 320)){ /* Does append during the pre render line */
            /* OAMADDR is set to 0 during each of ticks 257-320 (the sprite tile loading interval) of the pre-render and visible scanlines. */
            this->registers.OAMADDR = 0x00;
            
            /*
             * I could be cycle accurate
             * but the cpu cannot mess with secondary oam
             * so only the PPU has access to it
             * and therefor I can do all of it during a single cycle
             * it is easier to implement and runs faster
             */
            if(this->cycle == 257){
                this->number_of_sprites = this->last_available_slot;
                
                for(int i = 0; i < this->last_available_slot; i++){
                    /* Generate pattern addr from which we'll fetch the sprite pattern data */
                    Address pattern_table_addr = 0x0000;
                    //Byte 1: For 8x8 sprites, this is the tile number of this sprite within the pattern table selected in bit 3 of PPUCTRL ($2000).
                    //        For 8x16 sprites, the PPU ignores the pattern table selection and selects a pattern table from bit 0 of this number.
                    //76543210
                    //||||||||
                    //|||||||+- Bank ($0000 or $1000) of tiles
                    //+++++++-- Tile number of top of sprite (0 to 254; bottom half gets the next tile)
                    if(this->registers.PPUCTRL & 0x20){ /* 8x16 sprites */
                        pattern_table_addr = ((*this->Sec_OAM)[i][1] & 0x01) << 12; /* see Byte 1 explanations */
                        
                        /*
                         * Each tile is 8*8 wide
                         * and separated in a low bit tile and a high bit tile
                         * therefor each tile is 16Bytes wide
                         */
                        pattern_table_addr |= ((*this->Sec_OAM)[i][1] & 0xFE) << 4; /* Offset to the right tile */
                        
                        /* Is the tile fliped vertically ? */
                        if((*this->Sec_OAM)[i][2] & 0x80){ /* If the sprite is flipped vertically */
                            /* Are we reading the top or the bottom tile ? */
                            if((this->scanline - (*this->Sec_OAM)[i][0]) <= 7) /* Top half */
                                pattern_table_addr += 0x0010;
                        }
                        else{
                            /* Are we reading the top or the bottom tile ? */
                            if((this->scanline - (*this->Sec_OAM)[i][0]) <= 7) /* Bottom half */
                                pattern_table_addr += 0x0010;
                        }
                    }
                    else{ /* 8x8 sprites */
                        pattern_table_addr = (this->registers.PPUCTRL & 0x08) << 9; /* Select the pattern table */
                        /*
                         * Each tile is 8*8 wide
                         * and separated in a low bit tile and a high bit tile
                         * therefor each tile is 16Bytes wide
                         */
                        pattern_table_addr |= ((*this->Sec_OAM)[i][1] << 4);
                    }
                    
                    /* Select the row */
                    if((*this->Sec_OAM)[i][2] & 0x80){/* If the sprite is flipped vertically */
                        pattern_table_addr |= (7 - (this->scanline - (*this->Sec_OAM)[i][0])) & 0x07; /* 7- result is done to flip horizontally: we read from the bottom row to the top one */
                        /* See the other case for detail on the rest */
                    }
                    else{/* If the sprite is not flipped vertically */
                        pattern_table_addr |= (this->scanline - (*this->Sec_OAM)[i][0]) & 0x07; /* We offset to select the right row */
                        /* This offset is ANDed whith 0x07 because for the case of 8x16 sprites, the tile id is used to handle row offset that are greater than 8 */
                    }
                    
                    
                    /* We now have the address were to get the pattern data from! */
                    (*this->sprite_shift_registers)[i][0] = this->read(pattern_table_addr);     /* Low bit row */
                    (*this->sprite_shift_registers)[i][1] = this->read(pattern_table_addr + 8); /* High bit row */
                    
                    /* We still need to horizontally flip the pattern data if it needs to. */
                    /* It will be achieved at rendering */

                    (*this->sprite_latches)[i] = (*this->Sec_OAM)[i][2];
                    //76543210
                    //||||||||
                    //||||||++- Palette (4 to 7) of sprite
                    //|||+++--- Unimplemented
                    //||+------ Priority (0: in front of background; 1: behind background)
                    //|+------- Flip sprite horizontally
                    //+-------- Flip sprite vertically
                    (*this->sprite_counters)[i] = (*this->Sec_OAM)[i][3];
                }
            }
        }

        /* Cycles 321-340+0: Background render pipeline initialization */
        /* Read the first byte in secondary OAM (while the PPU fetches the first two background tiles for the next scanline) */
//        else if((this->cycle >= 320) && (this->cycle <= 340)){
//            //useless
//        }
    }
    
    
    /*
     * Actual rendering
     */
    if(this->cycle <= 257 && this->scanline <= 239){ /* When a pixel can be drown and durring the pre render line because it costs more to check each time if we're not in the prerender line */
        /* Foreground */
        Byte fg_pixel   = 0x00;
        Byte fg_palette = 0x00;
        bool priority         = false;
        is_sprite_0_rendering = false;                            /* We assume we're not rendering sprite 0 */
        if(this->registers.PPUMASK & 0x10){                       /* If sprite rendering in enabled */
            for(int i = 0; i < this->number_of_sprites; i++){     /* If this sprite has been fetched */
                    if((*this->sprite_counters)[i] == 0){      /* If it's time to render the sprite */
                        if((*this->sprite_latches)[i] & 0x40) /* Horizontal flip */
                            fg_pixel = (((*this->sprite_shift_registers)[i][1] & 0x01) << 1) | (((*this->sprite_shift_registers)[i][0] & 0x01));
                        else /* No horizontal flip */
                            fg_pixel = (((*this->sprite_shift_registers)[i][1] & 0x80) >> 6) | (((*this->sprite_shift_registers)[i][0] & 0x80) >> 7);
                        
                        fg_palette = ((*this->sprite_latches)[i] & 0x03) | 0x04; /* |0x04 added to offset in the sprite palette */
                        priority   = ((*this->sprite_latches)[i] & 0x20) == 0;   /* 0 = in front of background */
                        
                        if(fg_pixel != 0x00){                                 /* if the pixel is not transparent */
                            if(this->is_sprite_0_there && i == 0)             /* Currently rendering sprite 0! */
                                is_sprite_0_rendering = true;
                            break;                                            /* Sprites are looked at from the highest priority to the lowest */
                        }
                }
            }
        }
        
        /* Background */
        /* We read the appropriate (defined by fine x) bit in the pattern shiffters */
        bool pixel_value_high = (this->pattern_data_shift_register_2 & (0x8000 >> (int) this->fine_x_scroll)) != 0;
        bool pixel_value_low  = (this->pattern_data_shift_register_1 & (0x8000 >> (int) this->fine_x_scroll)) != 0;

        /* We read the appropriate (defined by fine x) bit in the palette shiffters */
        bool palette_value_high = (palette_attribute_shift_register_2 & (0x8000 >> (int) this->fine_x_scroll)) != 0;
        bool palette_value_low  = (palette_attribute_shift_register_1 & (0x8000 >> (int) this->fine_x_scroll)) != 0;


        Byte bg_pixel = (pixel_value_high << 1) | pixel_value_low;
        Byte bg_palette = (palette_value_high << 1) | palette_value_low;
        
        Byte final_pixel = 0x00;
        Byte final_palette = 0x00;
        
        
        /* How sprite priority works: https://wiki.nesdev.org/w/index.php?title=PPU_sprite_priority */
        if(fg_pixel != 0){                             /* Foreground pixel is not transparent */
            if((bg_pixel == 0) || priority){           /* Background is transparent so draw the fg_pixel or sprite has priority */
                final_pixel = fg_pixel;
                final_palette = fg_palette;
                if(is_sprite_0_rendering)              /* We just drown sprite 0 */
                    this->registers.PPUSTATUS |= 0x40; /* Set sprite 0 hit flag */
            } else {
                final_pixel = bg_pixel;
                final_palette = bg_palette;
            }
        } else{/*
              * Foreground pixel is transparent
              * if bg_pixel if transparent, bg color will be drown
              * else bg_pixel will be drown
              * in any case,
              */
            final_pixel = bg_pixel;
            final_palette = bg_palette;
        }
        
        //43210
        //|||||
        //|||++- Pixel value from tile data
        //|++--- Palette number from attribute table or OAM
        //+----- Background/Sprite select
        GRAPHICS::Color c = (*this->palette)[this->read( 0x3F00 + ( final_pixel | (final_palette << 2) )) & 0x3F];
        graphics->DrawPixel(cycle, scanline, c);
    }

    cycle++;                                  /* Each cycle the ppu generate one pixel */
    if(this->cycle == 361){
        this->cycle = 0;

        this->scanline++;                     /* Switch to next line */
        if(this->scanline == 261){            /* We rendered the last scanline */
            this->scanline = -1;              /* Return to pre-render scanline */

            this->odd_frame = !this->odd_frame;
            if(this->odd_frame)
                this->cycle = 1;               /* First cycle is skiped on odd frames */
            
            
            
            /* The following code is meant to write the current frame per seconds as the title */
            frames_last_seconde++;
            int currentTime = SDL_GetTicks();
            if (currentTime > last_time + 1000) {
                std::string re = "FPS: " + std::to_string(frames_last_seconde);
                graphics->ChangeTitle(re.c_str());
                last_time = currentTime;
                frames_last_seconde = 0;
            }

            /* Update the screen with the new frame */
            graphics->update();
        }
    }
}
    

