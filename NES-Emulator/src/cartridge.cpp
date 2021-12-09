//
//  cartridge.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 08/12/2021.
//

#include "cartridge.hpp"

bool CARTRIDGE::load(std::string link){
    std::ifstream ROMfile;
    ROMfile.open(link, std::ios::binary);
    if(ROMfile.fail()) return false;
    
    char buffer = 0x00;
    for(int i = 0; i < 4; i++) ROMfile.read(&buffer,1); //skip ines file header
    
    //number of 16kB rom banks (= 1 or 2)
    bool mirror_prgrom = false;
    ROMfile.read(&buffer,1);
    if(buffer == 0x01) mirror_prgrom = true;
    
#warning TODO handle vrom
    //number of 8kB vrom banks
    ROMfile.read(&buffer,1);
    
    //skip rest of header
    for(int i = 0; i < 10; i++) ROMfile.read(&buffer,1);
    
    if(prgrom){
        for(Address pos = 0; ((int) pos) < 0x4000; pos++){ //load prgrom
            ROMfile.read(&buffer,1);
            this->prgrom->at(pos + 0x8000) = (Byte) buffer;
            this->prgrom->at(pos + 0xC000) = (Byte) buffer;
        }
    }
    else{
        for(Address pos = 0x8000; ((int) pos) < 0x10000; pos++){ //load prgrom
            ROMfile.read(&buffer,1);
            this->prgrom->at(pos) = (Byte) buffer;
        }
    }
    
#warning TODO load chrom
    
    
    ROMfile.close();
    return true;
}


Byte CARTRIDGE::readROM(Address addr){
    return this->prgrom->at(addr);
}
