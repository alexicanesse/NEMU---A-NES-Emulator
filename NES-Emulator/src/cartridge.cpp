//
//  cartridge.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 08/12/2021.
//

#include "cartridge.hpp"

#include <iostream>

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

    //skip rest of header
    for(int i = 0; i < 10; i++) ROMfile.read(&buffer,1);
    
#warning for debug
//    for(int i = 0; i < 10; i++){ ROMfile.read(&buffer,1); std::cout<<std::hex<<(int) buffer<<"\n";}
    
    
    if(mirror_prgrom){
        for(int pos = 0x8000; pos <= 0xBFFF; pos++){ //load prgrom
            ROMfile.read(&buffer,1);
            this->prgROM->at(pos) = (Byte) buffer;
            this->prgROM->at(pos + 0xC000 - 0x8000) = (Byte) buffer;
            std::cout << std::hex << (int) pos + 0xC000 - 0x8000 << "  " << (int) this->prgROM->at(pos) << (int) this->prgROM->at(pos + 0xC000 - 0x8000) << "\n";
        }
    }
    else{
        for(Address pos = 0x8000; ((int) pos) < 0x10000; pos++){ //load prgrom
            ROMfile.read(&buffer,1);
            this->prgROM->at(pos) = (Byte) buffer;
        }
    }
    
    if(mirror_chr_rom){
        
    }
    else{
        
    }
    
    
    ROMfile.close();
    return true;
}


Byte CARTRIDGE::readROM(Address addr){
    return this->prgROM->at(addr);
}
