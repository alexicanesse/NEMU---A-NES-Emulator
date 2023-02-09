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
    NES *nes; //only the ppu is necessary. Though, I prefer to go through the nes to access the ppu just because it makes more sence hardwarewise.
    
public:
    //constructor
    CARTRIDGE(NES*);
    
    bool mirror_prgrom = false;
    bool mirroring_v = true;
    
    //addresses 0x0000 ~ 0x7FFF are useless but it makes it easier to address
    std::array<Byte, 0xFFFF + 1> *prgROM = new std::array<Byte, 0xFFFF + 1>; //prg ROM + prg RAM
    
    void load(std::string);
    Byte readROM(Address);
};

#endif /* cartridge_hpp */
