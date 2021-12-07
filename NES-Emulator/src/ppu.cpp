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
