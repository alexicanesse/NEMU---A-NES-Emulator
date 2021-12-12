//
//  nes.cpp
//  Proj1
//
//  Created by Alexi Canesse on 17/11/2021.
//
//#include <cstdio>
#include <array>

#include "nes.hpp"
#include "ppu.hpp"



/*
    Constructor
*/
NES::NES(){
    this->cpu = new CPU(this);
    this->ppu = new PPU(this);
    this->cartridge = new CARTRIDGE;
}

/*
    Read/Write Memory
*/
#warning bool useless
void NES::write(Address adr, Byte content){
    if(adr <= 0x1FFF){
        //if we try to write the mirros, we juste write there
        this->ram->at(adr & 0x07FF) = content;
    }
    else if(adr <= 0x3FFF){
#warning TODO
        //The PPU exposes eight memory-mapped registers to the CPU. These nominally sit at $2000 through $2007 in the CPU's address space, but because they're incompletely decoded, they're mirrored in every 8 bytes from $2008 through $3FFF, so a write to $3456 is the same as a write to $2006.
        switch (adr % 8) {
            case 0: //ppuctrl
                this->ppu->setPPUCTRL(content);
                break;
                
            case 1: //ppumask
                this->ppu->setPPUMASK(content);
                break;
  
#warning TODO
            case 3:
                break;
#warning TODO
            case 4:
                break;
#warning TODO
            case 5:{
                break;
            }
#warning TODO
            case 6:{
                break;
            }
                
            case 7:{
                this->ppu->write(this->ppu->vmem_addr, content);
                
                //VRAM read/write data register. After access, the video memory address will increment by an amount determined by bit 2 of $2000.
                //(0: add 1, going across; 1: add 32, going down)
                if(this->ppu->getPPUCTRL() & 0x04)
                    this->ppu->vmem_addr += 32;
                else
                    this->ppu->vmem_addr += 1;
                break;
            }
                
            //some registers are write-only
            default:
                break;
        }
    }
    else if (adr <= 0x4020){
    #warning TODO write APU/IO
    }
#warning DO NOT WRITE ROM
    
}


Byte NES::read(Address addr){
    if(addr <= 0x1FFF){
        //if we try to read the mirrors, we just read there
        return this->ram->at(addr & 0x07FF);
    }
    else if(addr <= 0x3FFF){
        //The PPU exposes eight memory-mapped registers to the CPU. These nominally sit at $2000 through $2007 in the CPU's address space, but because they're incompletely decoded, they're mirrored in every 8 bytes from $2008 through $3FFF, so a write to $3456 is the same as a write to $2006.
        switch (addr % 8) {
            case 2:{
                Byte buffer = this->ppu->getPPUSTATUS(); //reading the register affect it's value
                this->ppu->setPPUSTATUS(buffer & 0x7F); //Reading the status register will clear bit 7
                this->ppu->address_latch = 0x00;  //Reading the status register will clear the address latch used by PPUSCROLL and PPUADDR.
                return buffer;
                break;
            }
#warning TODO
            case 3:{
                return this->ppu->getOAMADDR();
                break;
            }
#warning TODO
            case 4:{
                return this->ppu->getOAMDATA();
                break;
            }
                
            case 7:{
                //When reading while the VRAM address is in the range 0-$3EFF (i.e., before the palettes), the read will return the contents of an internal read buffer. This internal buffer is updated only when reading PPUDATA, and so is preserved across frames. After the CPU reads and gets the contents of the internal buffer, the PPU will immediately update the internal buffer with the byte at the current VRAM address. Thus, after setting the VRAM address, one should first read this register to prime the pipeline and discard the result.
                //Reading palette data from $3F00-$3FFF works differently. The palette data is placed immediately on the data bus, and hence no priming read is required. Reading the palettes still updates the internal buffer though, but the data placed in it is the mirrored nametable data that would appear "underneath" the palette.
                Byte buffer = this->ppu->read_buffer;
                this->ppu->read_buffer = this->ppu->read(this->ppu->vmem_addr);
                
                Byte data_to_return = 0x00;
                
                if(this->ppu->vmem_addr <= 0x3EFF)
                    data_to_return = buffer;
                else
                    data_to_return = this->ppu->read_buffer;
                
                //VRAM read/write data register. After access, the video memory address will increment by an amount determined by bit 2 of $2000.
                //(0: add 1, going across; 1: add 32, going down)
                if(this->ppu->getPPUCTRL() & 0x04)
                    this->ppu->vmem_addr += 32;
                else
                    this->ppu->vmem_addr += 1;
                
                return data_to_return;
                break;
            }
                
            //not all registers are readable
            default:
                return 0x00;
                break;
        }
    }
    else if (addr <= 4020){
    #warning TODO read APU/IO
        switch (addr) {
            case 0x4014:
                return this->ppu->getOAMDMA();
                break;
                
            default:
                return 0x00;
                break;
        }
    }
#warning TODO fill blank there
    else if(addr >= 0x8000) return this->cartridge->readROM(addr);
    
    
    //should not happend
    return 0x00;
}


