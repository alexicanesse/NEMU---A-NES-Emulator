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
#include "cartridge.hpp"




/*
    Constructor
*/
NES::NES(){
    this->cpu = new CPU(this);
    this->ppu = new PPU(this);
    this->cartridge = new CARTRIDGE(this);
}

/*
    Read/Write Memory
*/
void NES::write(Address adr, Byte content){
    if(adr <= 0x1FFF){
        //if we try to write the mirros, we juste write there
        this->ram->at(adr & 0x07FF) = content;
    }
    else if(adr <= 0x3FFF){
        //The PPU exposes eight memory-mapped registers to the CPU. These nominally sit at $2000 through $2007 in the CPU's address space, but because they're incompletely decoded, they're mirrored in every 8 bytes from $2008 through $3FFF, so a write to $3456 is the same as a write to $2006.
        switch (adr % 8) {
            case 0: //ppuctrl
                this->ppu->setPPUCTRL(content);
                //https://wiki.nesdev.org/w/index.php?title=PPU_scrolling
                //t: ... GH.. .... .... <- d: ......GH
                this->ppu->addr_t = (this->ppu->addr_t & 0xF3FF) | ((content & 0x03) << 10);
                break;
                
            case 1: //ppumask
                this->ppu->setPPUMASK(content);
                break;
  
            case 3:
                ppu->setOAMADDR(content);
                break;

            case 4:
                ppu->setOAMDATA(content);
                break;
                
            case 5:{
                //https://wiki.nesdev.org/w/index.php?title=PPU_scrolling
                //t: ....... ...ABCDE <- d: ABCDE...
                //x:              FGH <- d: .....FGH
                //w:                  <- 1
                if(!this->ppu->write_toggle){//first write
                    this->ppu->addr_t = (this->ppu->addr_t & 0xFFE0) | (content >> 3);
                    this->ppu->fine_x_scroll = content & 0x07;
                    this->ppu->write_toggle = true;
                }
                //t: FGH ..AB CDE. .... <- d: ABCDEFGH
                //w:                  <- 0
                else{//second write
//                    this->ppu->addr_t = ((((content & 0x07) << 12) | (this->ppu->addr_t & 0x0FFF)) & 0xFC1F) | ((content & 0xF8) << 5);
                    this->ppu->addr_t &= 0x0C1F;
                    this->ppu->addr_t |= ((content >> 3) << 5);
                    this->ppu->addr_t |= ((content & 0x7) << 12);
                    this->ppu->write_toggle = false;
                }
                break;
            }

            case 6:{
                //https://wiki.nesdev.org/w/index.php?title=PPU_scrolling
                //t: .CD EFGH .... .... <- d: ..CD EFGH
                //       <unused>     <- d: AB......
                //t: Z.. .... .... .... <- 0 (bit Z is cleared)
                //w:                  <- 1
                if(!this->ppu->write_toggle){//first write
                    this->ppu->addr_t &= 0x00FF;
                    this->ppu->addr_t |= (( (Address) (content & 0x3F) << 8));
//                    this->ppu->addr_t = (this->ppu->addr_t & 0x00FF) | ((content & 0x3F) << 8);
                    this->ppu->write_toggle = true;
                }
                //t: ....... ABCDEFGH <- d: ABCDEFGH
                //v: <...all bits...> <- t: <...all bits...>
                //w:                  <- 0
                else{//second write
                    //Valid addresses are $0000-$3FFF; higher addresses will be mirrored down.
                    this->ppu->addr_t = (this->ppu->addr_t & 0xFF00) | content;
                    this->ppu->vmem_addr = this->ppu->addr_t;
                    this->ppu->write_toggle = false;
                }
                break;
            }
                
            case 7:{ //you can read or write data from VRAM through this port
                this->ppu->write(this->ppu->vmem_addr, content);

                //VRAM read/write data register. After access, the video memory address will increment by an amount determined by bit 2 of $2000.
                //(0: add 1, going across; 1: add 32, going down)
                if(this->ppu->getPPUCTRL() & 0x04)
                    this->ppu->vmem_addr += 32;
                else
                    this->ppu->vmem_addr += 1;
                break;
            }
                
            //some registers are read-only
            default:
                break;
        }
    }
    else if(adr == 0x4014){//initiate a DMA transfer
        this->ppu->setOAMDMA(content);
        this->transfert_dma = true;
    }
    else if(adr == 0x4016){
        //0 - A
        //1 - B
        //2 - Select
        //3 - Start
        //4 - Up
        //5 - Down
        //6 - Left
        //7 - Right
        this->controler_shifter = 0x00;
        SDL_Event event;
        SDL_PollEvent(&event);
        const Byte *keys = SDL_GetKeyboardState(NULL); //keyboard is handled as qwerty
        if(keys[SDL_SCANCODE_W]) // z
            this->controler_shifter |= 0x08;
        if(keys[SDL_SCANCODE_A]) // q
            this->controler_shifter |= 0x02;
        if(keys[SDL_SCANCODE_S])
            this->controler_shifter |= 0x04;
        if(keys[SDL_SCANCODE_D])
            this->controler_shifter |= 0x01;
        if(keys[SDL_SCANCODE_G])
            this->controler_shifter |= 0x20;
        if(keys[SDL_SCANCODE_H])
            this->controler_shifter |= 0x10;
        if(keys[SDL_SCANCODE_K])
            this->controler_shifter |= 0x80;
        if(keys[SDL_SCANCODE_L])
            this->controler_shifter |= 0x40;
    }
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
                this->ppu->write_toggle = false;  //Reading the status register will clear the address latch used by PPUSCROLL and PPUADDR.
                return buffer;
                break;
            }

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
    else if (addr <= 0x401F){
        switch (addr) {
            case 0x4014:
                return this->ppu->getOAMDMA();
                break;
            case 0x4016:{
                bool value = ((this->controler_shifter & 0x80) != 0);
                this->controler_shifter <<= 1;
                return value;
                break;
            }
                
            default:
                return 0x00;
                break;
        }
    }
    else if(addr >= 0x8000) return this->cartridge->readROM(addr);
    
    
    //should not happend
    return 0x00;
}


void NES::clock(){
#warning TODO Handle speed
    this->ppu->clock();
    if(this->cycle % 3 == 0){ //this cycle also concerns the cpu (which runs 3 times slower than the ppu
        if(this->transfert_dma){
            if(this->dma_idle_cycle_done){
                //read is done on even cycles and write on odd cycles
                //I'll do both during odd cycles. It won't matter as CPU is disabled
                if(this->cycle & 0x01){
                    this->ppu->setOAM_with_addr( this->read((this->ppu->getOAMDMA() << 8) + this->dma_offset), this->dma_offset);
//                    this->ppu->setOAM_with_addr( this->dma_offset, this->dma_offset);

                
                    if(this->dma_offset == 0xFF){//dma transfert is done
                        this->transfert_dma = false;
                        this->dma_idle_cycle_done = false;
                        this->dma_offset = 0x00;
                    }
                    else
                        this->dma_offset++;
                }
            }
            else if(this->cycle & 0x01)//we start the dma transfert on an even cycle and at least one idle cycle happen
                dma_idle_cycle_done = true;
        }

        else if(this->ppu->asknmi){
            this->cpu->NMI();
            this->ppu->asknmi = false;
        }
        else
            this->cpu->clock();
    }
    

    
    cycle++;
}

