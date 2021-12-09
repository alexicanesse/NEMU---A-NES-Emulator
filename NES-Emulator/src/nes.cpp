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
        //if we try to write the mirros we juste write there
        switch (adr % 8) {
            case 0:
                this->ppu->setPPUCTRL(content);
                break;
                
            case 1:
                this->ppu->setPPUMASK(content);
                break;
                
            case 2:
                this->ppu->setPPUSTATUS(content);
                break;
                
            case 3:
                this->ppu->setOAMADDR(content);
                break;
               
            case 4:
                this->ppu->setOAMDATA(content);
                break;
                
            case 5:
                this->ppu->setPPUSCROLL(content);
                break;
                
            case 6:
                this->ppu->setPPUADDR(content);
                break;
                
            case 7:
                this->ppu->setPPUDATA(content);
                break;
                
            //just in case something goes terribly wrong
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
#warning TODO
        //if we try to read the mirros, we just read there
        switch (addr % 8) {
            case 2:
                return this->ppu->getPPUSTATUS();
                break;
                
            case 3:
                return this->ppu->getOAMADDR();
                break;
               
            case 4:
                return this->ppu->getOAMDATA();
                break;
                
            case 7:
                return this->ppu->getPPUDATA();
                break;
                
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


