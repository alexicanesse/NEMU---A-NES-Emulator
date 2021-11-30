//
//  nes.cpp
//  Proj1
//
//  Created by Alexi Canesse on 17/11/2021.
//
//#include <cstdio>
#include <array>

#include "nes.hpp"


 


/*
    Read/Write Memory
*/
#warning bool useless
bool NES::write(Address adr, Byte content){
    if(adr <= 0x1FFF){
        //Si on essaye d'écrire sur les miroirs, on ecrit simplement sur la ram.
        this->ram->at(adr & 0x07FF) = content;
        return true;
    }
    else if(adr <= 0x3FFF){
    #warning TODO write PPU registers
        return true;
    }
    else if (adr <= 4017){
    #warning TODO write APU/IO
        return true;
    }
    //adr >= 0x800
    this->rom->at(adr) = content;
    return true;
}


Byte NES::read(Address adr){
    if(adr <= 0x1FFF){
        //Si on essaye de lire sur les miroirs, on lit simplement sur la ram.
        return this->ram->at(adr & 0x07FF);
    }
    else if(adr <= 0x3FFF){
    #warning TODO read PPU registers
        return 0x00;
    }
    else if (adr <= 4017){
    #warning TODO read APU/IO
        return 0x00;
        
    }
    //adr >= 0x800
    return this->rom->at(adr);
}


