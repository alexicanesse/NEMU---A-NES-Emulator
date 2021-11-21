//
//  nes.hpp
//  Proj1
//
//  Created by Alexi Canesse on 17/11/2021.
//

#ifndef nes_hpp
#define nes_hpp

#include <stdio.h>
#include <array>
#include <errno.h>

#include "cpu.hpp"

typedef uint8_t Byte;
typedef uint16_t Address;

namespace nes {

class NES{
public:
#warning TODO init matrix at init
    std::array<Byte, 2048> *ram = new std::array<Byte, 2048>; //le reste et un miroire de cette tranche mémoire.
    
#warning TODO WRITE/READ
    bool write(Address adr, Byte content);
    Byte read(Address adr); //errno = 22 en cas d'erreur.
};

}

#endif /* nes_hpp */
