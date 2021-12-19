//
//  cartridge.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 08/12/2021.
//

#include "cartridge.hpp"
#include "nes.hpp"

#include <iostream>

CARTRIDGE::CARTRIDGE(NES *nes){
    this->nes = nes;
}

bool CARTRIDGE::load(std::string link){
    std::ifstream ROMfile;
    ROMfile.open(link, std::ios::binary);
    if(ROMfile.fail()) return false;
    
    char buffer = 0x00;
    for(int i = 0; i < 4; i++) ROMfile.read(&buffer,1); //skip ines file header

    //number of 16kB rom banks (= 1 or 2)
    ROMfile.read(&buffer,1);
    if(buffer == 0x01) mirror_prgrom = true;

    //number of 2kB vrom banks
    ROMfile.read(&buffer,1);
    if(buffer == 0x01) mirror_chr_rom = true;

    //flag6
    ROMfile.read(&buffer,1);
    if(buffer & 0x01) this->mirroring_v = true;
    
    //skip rest of header
    for(int i = 0; i < 9; i++) ROMfile.read(&buffer,1);
    
    
    if(mirror_prgrom){
        for(int pos = 0x8000; pos <= 0xBFFF; pos++){ //load prgrom
            ROMfile.read(&buffer,1);
            this->prgROM->at(pos) = (Byte) buffer;
            this->prgROM->at(pos + 0xC000 - 0x8000) = (Byte) buffer;
//            std::cout << std::hex << (int) pos + 0xC000 - 0x8000 << "  " << (int) this->prgROM->at(pos) << (int) this->prgROM->at(pos + 0xC000 - 0x8000) << "\n";
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
    return true;
}


Byte CARTRIDGE::readROM(Address addr){
    return this->prgROM->at(addr);
}
