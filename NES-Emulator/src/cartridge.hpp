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

class NES;

//Only mapper 0 will be implemented for now
class CARTRIDGE{
private:
    NES *nes;
    
public:
    //constructor
    CARTRIDGE(NES*);
    
    bool mirror_prgrom = false;
    bool mirror_chr_rom = false;
    bool mirroring_v = false;
    
    //addresses 0x0000 ~ 0x7FFF are useless
    std::array<Byte, 0xFFFF + 1> *prgROM = new std::array<Byte, 0xFFFF + 1>; //prg ROM + prg RAM
    
#warning TODO
    bool load(std::string);
    Byte readROM(Address);
};

#endif /* cartridge_hpp */
