//
//  cpu.cpp
//  Proj1
//
//  Created by Alexi Canesse on 17/11/2021.
//

#include <stdio.h>
#include <array>

#include "cpu.hpp"

namespace nes{

typedef uint8_t Byte;
typedef uint16_t Address;

//Renvoie un boul ayany la valeur du drapeau donné en argument.
void CPU::setflag(Byte flg, bool value){
    this->registers.nv_bdizc &= ~flg | (value & flg)
}


//We change all byte to 0 except the one that interest us which is not modyfied. Then we know it's value.
bool CPU::getflag(Byte flg){
    return (this->registers.nv_bdizc & flg) != 0;
}

}


int main(){
    #warning TODO handle args
    return 0;
}
