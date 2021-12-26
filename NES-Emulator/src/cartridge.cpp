//
//  cartridge.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 08/12/2021.
//

#include <iostream>
#include <array>
#include <string>
#include <fstream>

#include "cartridge.hpp"
#include "nes.hpp"



CARTRIDGE::CARTRIDGE(NES *nes){
    this->nes = nes;
}

//see https://wiki.nesdev.org/w/index.php?title=INES for details about the header
//see https://wiki.nesdev.org/w/index.php?title=NROM for details about the mapper I implemented
void CARTRIDGE::load(std::string link){
    std::ifstream ROMfile;
    ROMfile.open(link, std::ios::binary);
    if(ROMfile.fail()){
        std::cout << "Cannot open rom file";
        std::exit(0);
    }
    
    char buffer = 0x00;
    for(int i = 0; i < 4; i++) ROMfile.read(&buffer,1); //skip ines file header
    
    //number of 16kB rom banks (= 1 or 2)
    ROMfile.read(&buffer,1);
    if(buffer == 0x01) mirror_prgrom = true;

    //number of 2kB vrom banks
    ROMfile.read(&buffer,1); //skip nbr_chrom_banks because mapper 0 only have one 8kiB chrom bank
    
    ROMfile.read(&buffer,1); //flag 6
    this->mirroring_v = (buffer & 0x01) == 0x01;
    
    if(buffer & 0x02){
        std::cout << "Cartridge contains battery-backed PRG RAM which has not been implemented";
        std::exit(0);
    }
    
    bool has_trainer = false;
    if(buffer & 0x04)
        has_trainer = true;
    
    if(buffer & 0x08){
        std::cout << "Four-screen vram has not been implemented";
        std::exit(0);
    }
    
    if(buffer & 0xF0){
        std::cout << "The rom uses a mapper different from mapper 0. It has not been implemented (yet ?)";
        std::exit(0);
    }
    
    ROMfile.read(&buffer,1); //flag 7
    if(buffer & 0xF0){
        std::cout << "The rom uses a mapper different from mapper 0. It has not been implemented (yet ?)";
        std::exit(0);
    }
    
    //skip rest of header
    for(int i = 0; i < 8; i++)
        ROMfile.read(&buffer,1);
    
    if(has_trainer){//skip it
        for(int i = 0; i < 512; i++)
            ROMfile.read(&buffer,1);
    }
    
    if(mirror_prgrom){
        for(int pos = 0x8000; pos <= 0xBFFF; pos++){ //load prgrom
            ROMfile.read(&buffer,1);
            this->prgROM->at(pos) = (Byte) buffer;
            this->prgROM->at(pos + 0xC000 - 0x8000) = (Byte) buffer;
        }
    }
    else{
        for(Address pos = 0x8000; ((int) pos) < 0x10000; pos++){ //load prgrom
            ROMfile.read(&buffer,1);
            this->prgROM->at(pos) = (Byte) buffer;
        }
    }
    
    
    for(int pos = 0x0000; pos <= 0x1FFF; pos++){ //load chrom
        ROMfile.read(&buffer,1);
        this->nes->ppu->write(pos, (Byte) buffer);
    }
    ROMfile.close();
}


Byte CARTRIDGE::readROM(Address addr){
    return this->prgROM->at(addr);
}
