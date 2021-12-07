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
#warning TODO need proper rom
    //adr >= 0x4020
    this->rom->at(adr) = content;
}


Byte NES::read(Address adr){
    if(adr <= 0x1FFF){
        //if we try to read the mirrors, we just read there
        return this->ram->at(adr & 0x07FF);
    }
    else if(adr <= 0x3FFF){
        //if we try to read the mirros, we just read there
        switch (adr % 8) {
            case 0:
                return this->ppu->getPPUCTRL();
                break;
                
            case 1:
                return this->ppu->getPPUMASK();
                break;
                
            case 2:
                return this->ppu->getPPUSTATUS();
                break;
                
            case 3:
                return this->ppu->getOAMADDR();
                break;
               
            case 4:
                return this->ppu->getOAMDATA();
                break;
                
            case 5:
                return this->ppu->getPPUSCROLL();
                break;
                
            case 6:
                return this->ppu->getPPUADDR();
                break;
                
            case 7:
                return this->ppu->getPPUDATA();
                break;
                
            //just in case something goes terribly wrong
            default:
                return 0x00;
                break;
        }
    }
    else if (adr <= 4020){
    #warning TODO read APU/IO
        switch (adr) {
            case 0x4014:
                return this->ppu->getOAMDMA();
                break;
                
            default:
                return 0x00;
                break;
        }
    }
#warning TODO need proper rom
    //adr >= 0x4020
    return this->rom->at(adr);
}


