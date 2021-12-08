//
//  cartridge.hpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 08/12/2021.
//

#ifndef cartridge_hpp
#define cartridge_hpp

#include <array>
#include <string>
#include <fstream>

typedef uint8_t Byte;
typedef uint16_t Address;

class CARTRIDGE{
private:
    
public:
    //addresses 0x0000 ~ 0x7FFF are useless
    std::array<Byte, 0xFFFF + 1> *prgrom = new std::array<Byte, 0xFFFF + 1>; //ROM
    
#warning TODO chrom
    
#warning TODO
    bool load(std::string);
    Byte readROM(Address);
};

#endif /* cartridge_hpp */
