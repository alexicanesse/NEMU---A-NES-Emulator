//
//  ppu.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 07/12/2021.
//

#include "ppu.hpp"


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
