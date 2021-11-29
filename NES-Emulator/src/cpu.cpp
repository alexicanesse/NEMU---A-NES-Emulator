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
    this->cycles++; //increase total number of cycles
    if(this->rem_cycles != 0){
        this->rem_cycles--;
        return;
    }
    
    //fetch opcode
    this->opcode = this->nes.read(this->registers.r_PC);
    //the pc register is incremented to be prepared for the next read.
    this->registers.r_PC++;
    
    instruction instr = (*this->instructions)[opcode]; //get the instruction
    
    this->rem_cycles = instr.cycles - 1; //-1 because this cycle is the first cycle
    
    //run addr_mode and add an additionnal cycle if requiered
    bool add_cycle1 = instr.addressing_mode(); //(page)
    
    //run instruction and add an additionnal cycle if requiered
    bool add_cycle2 = instr.function(); //branch cycle is directly added by functions 
    
    
    if(add_cycle1 & add_cycle2)
        this->rem_cycles++;
    
# warning TODO Handle number of cycles + use addrmode + opperate
}




/*
 
    Addressing modes
*/
//implied
bool CPU::IMP(){
    return false; //no additionnal cycle requiered
}

//accumulator
bool CPU::ACC(){
    return false; //no additionnal cycle requiered
}

//immediate
bool CPU::IMM(){
    //program counter is increamented to be prepared
    this->data_to_read = this->registers.r_PC++;
    return false; //no additionnal cycle requiered
}

//absolute
bool CPU::ABS(){
    Byte low = this->nes.read(this->registers.r_PC); //read 8 low bits
    this->registers.r_PC++;
    
    Byte high = this->nes.read(this->registers.r_PC++); //read 8 high bits
    this->registers.r_PC++;
    
    this->data_to_read = (high << 8) | low; //concat them
    
    return false; //no additionnal cycle requiered
}

//X indexed absolute
bool CPU::XIA(){
    Byte low = this->nes.read(this->registers.r_PC); //read 8 low bits
    this->registers.r_PC++;
    
    Byte high = this->nes.read(this->registers.r_PC++); //read 8 high bits
    this->registers.r_PC++;
    
    this->data_to_read = (high << 8) | low; //concat them
    
    this->data_to_read += this->registers.r_iX; //X-indexed
    
    //if a page is crossed, an additional cycle may be needed
    if((this->data_to_read & 0xFF00) == (high << 8))
        return false;
    else
        return true;
}

//Y indexed absolute
bool CPU::YIA(){
    Byte low = this->nes.read(this->registers.r_PC); //read 8 low bits
    this->registers.r_PC++;
    
    Byte high = this->nes.read(this->registers.r_PC++); //read 8 high bits
    this->registers.r_PC++;
    
    this->data_to_read = (high << 8) | low; //concat them
    
    this->data_to_read += this->registers.r_iY; //Y-indexed
    
    //if a page is crossed, an additional cycle may be needed
    if((this->data_to_read & 0xFF00) == (high << 8))
        return false;
    else
        return true;
}

//absolute indirect
bool CPU::IND(){
    Byte low = this->nes.read(this->registers.r_PC); //read 8 low bits
    this->registers.r_PC++;
    
    Byte high = this->nes.read(this->registers.r_PC); //read 8 high bits
    this->registers.r_PC++;
    
    Address temp = (high << 8) | low; //concat them
    
    //There is a bug on the chip
    //The indirect jump instruction does not increment the page address when
    //the indirect pointer crosses a page boundary.
    //JMP ($xxFF) will fetch the address from $xxFF and $xx00.
    if(low != 0x00FF)
        this->data_to_read = this->nes.read(temp) | (this->nes.read(temp + 1) << 8);
    else
        this->data_to_read = this->nes.read(temp) | (this->nes.read(temp & 0xFF00) << 8);
        

    return false;
}

//zero page
bool CPU::ZPA(){
    this->data_to_read = (0x00FF) & this->nes.read(this->registers.r_PC);
    this->registers.r_PC++;
    
    return false;
}

//X-indexed zero page
bool CPU::XZP(){
    //like ZPA but we must add an offset
    this->data_to_read = (0x00FF) & (this->nes.read(this->registers.r_PC) + this->registers.r_iX);
    this->registers.r_PC++;
    
    return false;
}

//Y-indexed zero page
bool CPU::YZP(){
    //like ZPA but we must add an offset
    this->data_to_read = (0x00FF) & (this->nes.read(this->registers.r_PC) + this->registers.r_iY);
    this->registers.r_PC++;
    
    return false;
}

//X-indexed zero page indirect
bool CPU::XZI(){
    Byte low = this->nes.read((this->registers.r_PC + this->registers.r_iX) & 0x00FF); //discard carry
    this->registers.r_PC++;
    
    Byte high = this->nes.read((this->registers.r_PC + this->registers.r_iX) & 0x00FF);
    this->registers.r_PC++;

    this->data_to_read = (high << 8) | low; //concat them

    return false;
}

//Y-indexed zero page indirect
//In indirect indexed addressing, the second byte of the instruction points to a memory location in page zero.
//The contents of this memory location is added to the contents of the Y index register, the result being the
//low order eight bits of the effective address. The carry from this addition is added to the contents of the next
//page zero memory location, the result being the high order eight bits of the effective address.
bool CPU:: YZI(){
    Byte low = this->nes.read(this->registers.r_PC) & 0x00FF;
    this->registers.r_PC++;
    
    Byte high = this->nes.read(this->registers.r_PC) & 0x00FF;
    this->registers.r_PC++;
    
    Address without_offset = (high << 8) | low; //concat them;

    this->data_to_read = without_offset + this->registers.r_iY;
    
    if((this->data_to_read ^ without_offset) >> 8) //check if page crossed
        return true;
    else
        return false;
}

//relative
//instructions will handle the t additionnal cycle
bool CPU::REL(){
    Byte offset = this->nes.read(this->registers.r_PC);
    this->registers.r_PC++;
    
    this->data_to_read = this->registers.r_PC + offset;
    if((this->registers.r_PC - this->data_to_read) & 0xFF00) //no page crossed
        return false;
    else
        return true;
#warning t branch cycle
}


