//
//  cpu.cpp
//  Proj1
//
//  Created by Alexi Canesse on 17/11/2021.
//

//#include <stdio.h>
#include <array>

#include "cpu.hpp"



void CPU::setflag(Byte flg, bool value){
    this->registers.nv_bdizc &= ~flg | (value & flg);
}


//We change all byte to 0 except the one that interest us which is not modyfied. Then we know it's value.
bool CPU::getflag(Byte flg){
//    return (this->registers.nv_bdizc & flg) != 0;
    return true;
}

Byte CPU::get_register_A(){
    return this->registers.r_A;
}

Byte CPU::get_register_X(){
    return this->registers.r_iX;
}

Byte CPU::get_register_Y(){
    return this->registers.r_iY;
}

Byte CPU::get_register_SP(){
    return this->registers.r_SP;
}

Address CPU::get_register_PC(){
    return this->registers.r_PC;
}

#warning todo
//Emulate one cycle
void CPU::clock(){
    //fetch opcode
    this->opcode = this->nes.read(this->registers.r_PC);
    
    
    
}






//int main(){
//    #warning TODO handle args
//    return 0;
//}
