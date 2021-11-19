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

void CPU::setflag(flag flg, bool value){
    this->registers.nv_bdizc = (this->registers.nv_bdizc || flg);
}

bool CPU::getflag(flag flg){
    return (this->registers.nv_bdizc & flg);
}



}








int main(){
    #warning TODO handle args
    return 0;
}
