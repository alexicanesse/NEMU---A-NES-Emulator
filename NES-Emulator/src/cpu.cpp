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
    if(value)
        this->registers.nv_bdizc |= flg;
    else
        this->registers.nv_bdizc &= ~flg;
}


//We change all byte to 0 except the one that interest us which is not modyfied. Then we know it's value.
bool CPU::getflag(Byte flg){
    return (this->registers.nv_bdizc & flg) != 0;

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
    
    bool addr = (this->*instr.addressing_mode)();
    bool func = (this->*instr.function)();
    //branch instructions handle cycles themseles
    if(addr & func) //add an additional cycle if both addr_mode and function requiere it. ie if opcode requiere it
        this->rem_cycles++;
    
    //branch instructions handle cycles themseles
    ;

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
    
    Byte high = this->nes.read(this->registers.r_PC); //read 8 high bits
    this->registers.r_PC++;
    
    this->data_to_read = (high << 8) | low; //concat them
    
    return false; //no additionnal cycle requiered
}

//X indexed absolute
bool CPU::XIA(){
    Byte low = this->nes.read(this->registers.r_PC); //read 8 low bits
    this->registers.r_PC++;
    
    Byte high = this->nes.read(this->registers.r_PC); //read 8 high bits
    this->registers.r_PC++;
    
    this->data_to_read = (high << 8) | low; //concat them
    
    this->data_to_read += this->registers.r_iX; //X-indexed
    
    //if a page is crossed, an additional cycle may be needed
    if((this->data_to_read & 0xFF00) == (high << 8)) //page not crossed
        return false;
    else
        return true;
}

//Y indexed absolute
bool CPU::YIA(){
    Byte low = this->nes.read(this->registers.r_PC); //read 8 low bits
    this->registers.r_PC++;
    
    Byte high = this->nes.read(this->registers.r_PC); //read 8 high bits
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
    Byte add = this->nes.read(this->registers.r_PC++);
    Byte low = this->nes.read((add + this->registers.r_iX) & 0x00FF); //discard carry
    
    Byte high = this->nes.read((add + 0x01 + this->registers.r_iX) & 0x00FF);

    this->data_to_read = (high << 8) | low; //concat them

    return false;
}

//Y-indexed zero page indirect
//In indirect indexed addressing, the second byte of the instruction points to a memory location in page zero.
//The contents of this memory location is added to the contents of the Y index register, the result being the
//low order eight bits of the effective address. The carry from this addition is added to the contents of the next
//page zero memory location, the result being the high order eight bits of the effective address.
bool CPU:: YZI(){
    Byte low = this->nes.read(this->nes.read(this->registers.r_PC));
    Byte high = this->nes.read((this->nes.read(this->registers.r_PC) + 1) & 0x00FF); //zeropage addressing
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
    
    //an “Offset" added to the contents of the lower eight bits of the program counter
    this->data_to_read = ((this->registers.r_PC + offset) & 0x01FF) | (this->registers.r_PC & 0xFF00);
    if((this->registers.r_PC ^ this->data_to_read) & 0xFF00) //page crossed
        return true;
    else
        return false;
}




CPU::CPU(){
    //00
    //01 ORA
    (*this->instructions).at(0x01).function = &CPU::ORA;
    (*this->instructions).at(0x01).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x01).cycles = 6;
    //02
    //03 SLO                                                   undocumented
    (*this->instructions).at(0x03).function = &CPU::SLO;
    (*this->instructions).at(0x03).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x03).cycles = 8;
    //04 NOP                                                   undocumented
    (*this->instructions).at(0x04).function = &CPU::NOP;
    (*this->instructions).at(0x04).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x04).cycles = 3;
    //05 ORA
    (*this->instructions).at(0x05).function = &CPU::ORA;
    (*this->instructions).at(0x05).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x05).cycles = 3;
    //06 ASL
    (*this->instructions).at(0x06).function = &CPU::ASL;
    (*this->instructions).at(0x06).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x06).cycles = 5;
    //07 SLO                                                   undocumented
    (*this->instructions).at(0x07).function = &CPU::SLO;
    (*this->instructions).at(0x07).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x07).cycles = 5;
    //08 PHP
    (*this->instructions).at(0x08).function = &CPU::PHP;
    (*this->instructions).at(0x08).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x08).cycles = 3;
    //09 ORA
    (*this->instructions).at(0x09).function = &CPU::ORA;
    (*this->instructions).at(0x09).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0x09).cycles = 2;
    //0A ASL
    (*this->instructions).at(0x0A).function = &CPU::ASL;
    (*this->instructions).at(0x0A).addressing_mode = &CPU::ACC;
    (*this->instructions).at(0x0A).cycles = 2;
    //0B
    //0C NOP                                                   undocumented
    (*this->instructions).at(0x0C).function = &CPU::NOP;
    (*this->instructions).at(0x0C).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x0C).cycles = 4;
    //0D ORA
    (*this->instructions).at(0x0D).function = &CPU::ORA;
    (*this->instructions).at(0x0D).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x0D).cycles = 4;
    //0E ASL
    (*this->instructions).at(0x0E).function = &CPU::ASL;
    (*this->instructions).at(0x0E).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x0E).cycles = 6;
    //0F SLO                                                   undocumented
    (*this->instructions).at(0x0F).function = &CPU::SLO;
    (*this->instructions).at(0x0F).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x0F).cycles = 6;
    //10 BPL
    (*this->instructions).at(0x10).function = &CPU::BPL;
    (*this->instructions).at(0x10).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x10).cycles = 2;
    //11 YZI
    (*this->instructions).at(0x11).function = &CPU::ORA;
    (*this->instructions).at(0x11).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x11).cycles = 5;
    //12
    //13 SLO                                                   undocumented
    (*this->instructions).at(0x13).function = &CPU::SLO;
    (*this->instructions).at(0x13).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x13).cycles = 8;
    //14 NOP                                                   undocumented
    (*this->instructions).at(0x14).function = &CPU::NOP;
    (*this->instructions).at(0x14).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x14).cycles = 4;
    //15 ORA
    (*this->instructions).at(0x15).function = &CPU::ORA;
    (*this->instructions).at(0x15).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x15).cycles = 4;
    //16 ASL
    (*this->instructions).at(0x16).function = &CPU::ASL;
    (*this->instructions).at(0x16).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x16).cycles = 6;
    //17 SLO                                                   undocumented
    (*this->instructions).at(0x17).function = &CPU::SLO;
    (*this->instructions).at(0x17).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x17).cycles = 6;
    //18 CLC
    (*this->instructions).at(0x18).function = &CPU::CLC;
    (*this->instructions).at(0x18).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x18).cycles = 2;
    //19 ORA
    (*this->instructions).at(0x19).function = &CPU::ORA;
    (*this->instructions).at(0x19).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0x19).cycles = 4;
    //1A NOP                                                   undocumented
    (*this->instructions).at(0x1A).function = &CPU::NOP;
    (*this->instructions).at(0x1A).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x1A).cycles = 2;
    //1B SLO                                                   undocumented
    (*this->instructions).at(0x1B).function = &CPU::SLO;
    (*this->instructions).at(0x1B).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0x1B).cycles = 7;
    //1C NOP                                                   undocumented
    (*this->instructions).at(0x1C).function = &CPU::NOP;
    (*this->instructions).at(0x1C).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x1C).cycles = 4;
    //1D ORA
    (*this->instructions).at(0x1D).function = &CPU::ORA;
    (*this->instructions).at(0x1D).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x1D).cycles = 4;
    //1E
    (*this->instructions).at(0x1E).function = &CPU::ASL;
    (*this->instructions).at(0x1E).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x1E).cycles = 7;
    //1F SLO                                                   undocumented
    (*this->instructions).at(0x1F).function = &CPU::SLO;
    (*this->instructions).at(0x1F).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x1F).cycles = 7;
    //20 JSR
    (*this->instructions).at(0x20).function = &CPU::JSR;
    (*this->instructions).at(0x20).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x20).cycles = 6;
    //21 AND
    (*this->instructions).at(0x21).function = &CPU::AND;
    (*this->instructions).at(0x21).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x21).cycles = 6;
    //22
    //23 RLA                                                   undocumented
    (*this->instructions).at(0x23).function = &CPU::RLA;
    (*this->instructions).at(0x23).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x23).cycles = 8;
    //24 BIT
    (*this->instructions).at(0x24).function = &CPU::BIT;
    (*this->instructions).at(0x24).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x24).cycles = 3;
    //25 AND
    (*this->instructions).at(0x25).function = &CPU::AND;
    (*this->instructions).at(0x25).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x25).cycles = 3;
    //26 ROL
    (*this->instructions).at(0x26).function = &CPU::ROL;
    (*this->instructions).at(0x26).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x26).cycles = 5;
    //27 RLA                                                   undocumented
    (*this->instructions).at(0x27).function = &CPU::RLA;
    (*this->instructions).at(0x27).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x27).cycles = 5;
    //28 PLP
    (*this->instructions).at(0x28).function = &CPU::PLP;
    (*this->instructions).at(0x28).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x28).cycles = 4;
    //29 AND
    (*this->instructions).at(0x29).function = &CPU::AND;
    (*this->instructions).at(0x29).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0x29).cycles = 2;
    //2A
    (*this->instructions).at(0x2A).function = &CPU::ROL;
    (*this->instructions).at(0x2A).addressing_mode = &CPU::ACC;
    (*this->instructions).at(0x2A).cycles = 2;
    //2B
    //2C BIT
    (*this->instructions).at(0x2C).function = &CPU::BIT;
    (*this->instructions).at(0x2C).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x2C).cycles = 4;
    //2D AND
    (*this->instructions).at(0x2D).function = &CPU::AND;
    (*this->instructions).at(0x2D).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x2D).cycles = 4;
    //2E ROL
    (*this->instructions).at(0x2E).function = &CPU::ROL;
    (*this->instructions).at(0x2E).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x2E).cycles = 6;
    //2F RLA                                                   undocumented
    (*this->instructions).at(0x2F).function = &CPU::RLA;
    (*this->instructions).at(0x2F).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x2F).cycles = 6;
    //30 BMI
    (*this->instructions).at(0x30).function = &CPU::BMI;
    (*this->instructions).at(0x30).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x30).cycles = 2;
    //31 AND
    (*this->instructions).at(0x31).function = &CPU::AND;
    (*this->instructions).at(0x31).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x31).cycles = 5;
    //32
    //33 RLA                                                   undocumented
    (*this->instructions).at(0x33).function = &CPU::RLA;
    (*this->instructions).at(0x33).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x33).cycles = 8;
    //34 NOP                                                   undocumented
    (*this->instructions).at(0x34).function = &CPU::NOP;
    (*this->instructions).at(0x34).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x34).cycles = 4;
    //35 AND
    (*this->instructions).at(0x35).function = &CPU::AND;
    (*this->instructions).at(0x35).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x35).cycles = 4;
    //36 ROL
    (*this->instructions).at(0x36).function = &CPU::ROL;
    (*this->instructions).at(0x36).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x36).cycles = 6;
    //37 RLA                                                   undocumented
    (*this->instructions).at(0x37).function = &CPU::RLA;
    (*this->instructions).at(0x37).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x37).cycles = 6;
    //38 SEC
    (*this->instructions).at(0x38).function = &CPU::SEC;
    (*this->instructions).at(0x38).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x38).cycles = 2;
    //39 AND
    (*this->instructions).at(0x39).function = &CPU::AND;
    (*this->instructions).at(0x39).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0x39).cycles = 4;
    //3A NOP                                                   undocumented
    (*this->instructions).at(0x3A).function = &CPU::NOP;
    (*this->instructions).at(0x3A).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x3A).cycles = 2;
    //3B RLA                                                   undocumented
    (*this->instructions).at(0x3B).function = &CPU::RLA;
    (*this->instructions).at(0x3B).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0x3B).cycles = 7;
    //3C NOP                                                   undocumented
    (*this->instructions).at(0x3C).function = &CPU::NOP;
    (*this->instructions).at(0x3C).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x3C).cycles = 4;
    //3D AND
    (*this->instructions).at(0x3D).function = &CPU::AND;
    (*this->instructions).at(0x3D).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x3D).cycles = 4;
    //3E ROL
    (*this->instructions).at(0x3E).function = &CPU::ROL;
    (*this->instructions).at(0x3E).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x3E).cycles = 7;
    //3F RLA                                                   undocumented
    (*this->instructions).at(0x3F).function = &CPU::RLA;
    (*this->instructions).at(0x3F).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x3F).cycles = 7;
    //40 RTI
    (*this->instructions).at(0x40).function = &CPU::RTI;
    (*this->instructions).at(0x40).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x40).cycles = 6;
    //41 EOR
    (*this->instructions).at(0x41).function = &CPU::EOR;
    (*this->instructions).at(0x41).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x41).cycles = 6;
    //42
    //43 SRE                                                   undocumented
    (*this->instructions).at(0x43).function = &CPU::SRE;
    (*this->instructions).at(0x43).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x43).cycles = 8;
    //44 NOP                                                   undocumented
    (*this->instructions).at(0x44).function = &CPU::NOP;
    (*this->instructions).at(0x44).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x44).cycles = 3;
    //45 EOR
    (*this->instructions).at(0x45).function = &CPU::EOR;
    (*this->instructions).at(0x45).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x45).cycles = 3;
    //46 LSR
    (*this->instructions).at(0x46).function = &CPU::LSR;
    (*this->instructions).at(0x46).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x46).cycles = 5;
    //47
    //48 PHA
    (*this->instructions).at(0x48).function = &CPU::PHA;
    (*this->instructions).at(0x48).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x48).cycles = 3;
    //49 EOR
    (*this->instructions).at(0x49).function = &CPU::EOR;
    (*this->instructions).at(0x49).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0x49).cycles = 2;
    //4A LSR
    (*this->instructions).at(0x4A).function = &CPU::LSR;
    (*this->instructions).at(0x4A).addressing_mode = &CPU::ACC;
    (*this->instructions).at(0x4A).cycles = 2;
    //4B
    //4C JMP
    (*this->instructions).at(0x4C).function = &CPU::JMP;
    (*this->instructions).at(0x4C).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x4C).cycles = 3;
    //4D EOR
    (*this->instructions).at(0x4D).function = &CPU::EOR;
    (*this->instructions).at(0x4D).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x4D).cycles = 4;
    //4E LSR
    (*this->instructions).at(0x4E).function = &CPU::LSR;
    (*this->instructions).at(0x4E).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x4E).cycles = 6;
    //4F
    //50 BVC
    (*this->instructions).at(0x50).function = &CPU::BVC;
    (*this->instructions).at(0x50).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x50).cycles = 2;
    //51 EOR
    (*this->instructions).at(0x51).function = &CPU::EOR;
    (*this->instructions).at(0x51).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x51).cycles = 5;
    //52
    //53
    //54 NOP                                                   undocumented
    (*this->instructions).at(0x54).function = &CPU::NOP;
    (*this->instructions).at(0x54).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x54).cycles = 4;
    //55 EOR
    (*this->instructions).at(0x55).function = &CPU::EOR;
    (*this->instructions).at(0x55).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x55).cycles = 4;
    //56 LSR
    (*this->instructions).at(0x56).function = &CPU::LSR;
    (*this->instructions).at(0x56).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x56).cycles = 6;
    //57
    //58
    //59 EOR
    (*this->instructions).at(0x59).function = &CPU::EOR;
    (*this->instructions).at(0x59).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0x59).cycles = 4;
    //5A NOP                                                   undocumented
    (*this->instructions).at(0x5A).function = &CPU::NOP;
    (*this->instructions).at(0x5A).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x5A).cycles = 2;
    //5B
    //5C NOP                                                   undocumented
    (*this->instructions).at(0x5C).function = &CPU::NOP;
    (*this->instructions).at(0x5C).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x5C).cycles = 4;
    //5D
    (*this->instructions).at(0x5D).function = &CPU::EOR;
    (*this->instructions).at(0x5D).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x5D).cycles = 4;
    //5E LSR
    (*this->instructions).at(0x5E).function = &CPU::LSR;
    (*this->instructions).at(0x5E).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x5E).cycles = 7;
    //5F
    //60 RTS
    (*this->instructions).at(0x60).function = &CPU::RTS;
    (*this->instructions).at(0x60).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x60).cycles = 6;
    //61 ADC
    (*this->instructions).at(0x61).function = &CPU::ADC;
    (*this->instructions).at(0x61).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x61).cycles = 6;
    //62
    //63
    //64 NOP                                                   undocumented
    (*this->instructions).at(0x64).function = &CPU::NOP;
    (*this->instructions).at(0x64).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x64).cycles = 3;
    //65 ADC
    (*this->instructions).at(0x65).function = &CPU::ADC;
    (*this->instructions).at(0x65).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x65).cycles = 3;
    //66 ROR
    (*this->instructions).at(0x66).function = &CPU::ROR;
    (*this->instructions).at(0x66).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x66).cycles = 5;
    //67
    //68 PLA
    (*this->instructions).at(0x68).function = &CPU::PLA;
    (*this->instructions).at(0x68).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x68).cycles = 4;
    //69 ADC
    (*this->instructions).at(0x69).function = &CPU::ADC;
    (*this->instructions).at(0x69).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0x69).cycles = 2;
    //6A ROR
    (*this->instructions).at(0x6A).function = &CPU::ROR;
    (*this->instructions).at(0x6A).addressing_mode = &CPU::ACC;
    (*this->instructions).at(0x6A).cycles = 2;
    //6B
    //6C JMP
    (*this->instructions).at(0x6C).function = &CPU::JMP;
    (*this->instructions).at(0x6C).addressing_mode = &CPU::IND;
    (*this->instructions).at(0x6C).cycles = 5;
    //6D ADC
    (*this->instructions).at(0x6D).function = &CPU::ADC;
    (*this->instructions).at(0x6D).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x6D).cycles = 4;
    //6E ROR
    (*this->instructions).at(0x6E).function = &CPU::ROR;
    (*this->instructions).at(0x6E).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x6E).cycles = 6;
    //6F
    //70 BVS
    (*this->instructions).at(0x70).function = &CPU::BVS;
    (*this->instructions).at(0x70).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x70).cycles = 2;
    //71 ADC
    (*this->instructions).at(0x71).function = &CPU::ADC;
    (*this->instructions).at(0x71).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x71).cycles = 5;
    //72
    //73
    //74 NOP                                                   undocumented
    (*this->instructions).at(0x74).function = &CPU::NOP;
    (*this->instructions).at(0x74).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x74).cycles = 4;
    //75 ADC
    (*this->instructions).at(0x75).function = &CPU::ADC;
    (*this->instructions).at(0x75).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x75).cycles = 4;
    //76 ROR
    (*this->instructions).at(0x76).function = &CPU::ROR;
    (*this->instructions).at(0x76).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x76).cycles = 6;
    //77
    //78 SEI
    (*this->instructions).at(0x78).function = &CPU::SEI;
    (*this->instructions).at(0x78).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x78).cycles = 2;
    //79 ADC
    (*this->instructions).at(0x79).function = &CPU::ADC;
    (*this->instructions).at(0x79).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0x79).cycles = 4;
    //7A NOP                                                   undocumented
    (*this->instructions).at(0x7A).function = &CPU::NOP;
    (*this->instructions).at(0x7A).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x7A).cycles = 2;
    //7B
    //7C NOP                                                   undocumented
    (*this->instructions).at(0x7C).function = &CPU::NOP;
    (*this->instructions).at(0x7C).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x7C).cycles = 4;
    //7D ADC
    (*this->instructions).at(0x7D).function = &CPU::ADC;
    (*this->instructions).at(0x7D).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x7D).cycles = 4;
    //7E ROR
    (*this->instructions).at(0x7E).function = &CPU::ROR;
    (*this->instructions).at(0x7E).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x7E).cycles = 7;
    //7F
    //80 NOP                                                   undocumented
    (*this->instructions).at(0x80).function = &CPU::NOP;
    (*this->instructions).at(0x80).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0x80).cycles = 2;
    //81 STA
    (*this->instructions).at(0x81).function = &CPU::STA;
    (*this->instructions).at(0x81).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x81).cycles = 6;
    //82
    //83 SAX                                                   undocumented
    (*this->instructions).at(0x83).function = &CPU::SAX;
    (*this->instructions).at(0x83).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x83).cycles = 6;
    //84 STY
    (*this->instructions).at(0x84).function = &CPU::STY;
    (*this->instructions).at(0x84).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x84).cycles = 3;
    //85 STA
    (*this->instructions).at(0x85).function = &CPU::STA;
    (*this->instructions).at(0x85).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x85).cycles = 3;
    //86 STX
    (*this->instructions).at(0x86).function = &CPU::STX;
    (*this->instructions).at(0x86).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x86).cycles = 3;
    //87 SAX                                                   undocumented
    (*this->instructions).at(0x87).function = &CPU::SAX;
    (*this->instructions).at(0x87).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x87).cycles = 3;
    //88 DEY
    (*this->instructions).at(0x88).function = &CPU::DEY;
    (*this->instructions).at(0x88).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x88).cycles = 2;
    //89
    //8A TXA
    (*this->instructions).at(0x8A).function = &CPU::TXA;
    (*this->instructions).at(0x8A).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x8A).cycles = 2;
    //8B
    //8C STY
    (*this->instructions).at(0x8C).function = &CPU::STY;
    (*this->instructions).at(0x8C).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x8C).cycles = 4;
    //8D STA
    (*this->instructions).at(0x8D).function = &CPU::STA;
    (*this->instructions).at(0x8D).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x8D).cycles = 4;
    //8E STX
    (*this->instructions).at(0x8E).function = &CPU::STX;
    (*this->instructions).at(0x8E).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x8E).cycles = 4;
    //8F SAX                                                   undocumented
    (*this->instructions).at(0x8F).function = &CPU::SAX;
    (*this->instructions).at(0x8F).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x8F).cycles = 4;
    //90 BCC
    (*this->instructions).at(0x90).function = &CPU::BCC;
    (*this->instructions).at(0x90).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x90).cycles = 2;
    //91 STA
    (*this->instructions).at(0x91).function = &CPU::STA;
    (*this->instructions).at(0x91).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x91).cycles = 6;
    //92
    //93
    //94 STY
    (*this->instructions).at(0x94).function = &CPU::STY;
    (*this->instructions).at(0x94).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x94).cycles = 4;
    //95 STA
    (*this->instructions).at(0x95).function = &CPU::STA;
    (*this->instructions).at(0x95).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0x95).cycles = 4;
    //96 STX
    (*this->instructions).at(0x96).function = &CPU::STX;
    (*this->instructions).at(0x96).addressing_mode = &CPU::YZP;
    (*this->instructions).at(0x96).cycles = 4;
    //97 SAX                                                   undocumented
    (*this->instructions).at(0x97).function = &CPU::SAX;
    (*this->instructions).at(0x97).addressing_mode = &CPU::YZP;
    (*this->instructions).at(0x97).cycles = 4;
    //98 TYA
    (*this->instructions).at(0x98).function = &CPU::TYA;
    (*this->instructions).at(0x98).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x98).cycles = 2;
    //99 STA
    (*this->instructions).at(0x99).function = &CPU::STA;
    (*this->instructions).at(0x99).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0x99).cycles = 5;
    //9A TXS
    (*this->instructions).at(0x9A).function = &CPU::TXS;
    (*this->instructions).at(0x9A).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x9A).cycles = 2;
    //9B
    //9C
    //9D STA
    (*this->instructions).at(0x9D).function = &CPU::STA;
    (*this->instructions).at(0x9D).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0x9D).cycles = 5;
    //9E
    //9F
    //A0 LDY
    (*this->instructions).at(0xA0).function = &CPU::LDY;
    (*this->instructions).at(0xA0).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xA0).cycles = 2;
    //A1 LDA
    (*this->instructions).at(0xA1).function = &CPU::LDA;
    (*this->instructions).at(0xA1).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0xA1).cycles = 6;
    //A2 LDX
    (*this->instructions).at(0xA2).function = &CPU::LDX;
    (*this->instructions).at(0xA2).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xA2).cycles = 2;
    //A3 LAX                                                   undocumented
    (*this->instructions).at(0xA3).function = &CPU::LAX;
    (*this->instructions).at(0xA3).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0xA3).cycles = 6;
    //A4 LDY
    (*this->instructions).at(0xA4).function = &CPU::LDY;
    (*this->instructions).at(0xA4).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xA4).cycles = 3;
    //A5 LDA
    (*this->instructions).at(0xA5).function = &CPU::LDA;
    (*this->instructions).at(0xA5).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xA5).cycles = 3;
    //A6 LDX
    (*this->instructions).at(0xA6).function = &CPU::LDX;
    (*this->instructions).at(0xA6).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xA6).cycles = 3;
    //A7 LAX                                                   undocumented
    (*this->instructions).at(0xA7).function = &CPU::LAX;
    (*this->instructions).at(0xA7).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xA7).cycles = 3;
    //A8 TAY
    (*this->instructions).at(0xA8).function = &CPU::TAY;
    (*this->instructions).at(0xA8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xA8).cycles = 2;
    //A9 LDA
    (*this->instructions).at(0xA9).function = &CPU::LDA;
    (*this->instructions).at(0xA9).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xA9).cycles = 2;
    //AA TAX
    (*this->instructions).at(0xAA).function = &CPU::TAX;
    (*this->instructions).at(0xAA).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xAA).cycles = 2;
    //AB
    //AC LDY
    (*this->instructions).at(0xAC).function = &CPU::LDY;
    (*this->instructions).at(0xAC).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xAC).cycles = 4;
    //AD LDA
    (*this->instructions).at(0xAD).function = &CPU::LDA;
    (*this->instructions).at(0xAD).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xAD).cycles = 4;
    //AE LDX
    (*this->instructions).at(0xAE).function = &CPU::LDX;
    (*this->instructions).at(0xAE).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xAE).cycles = 4;
    //AF LAX                                                   undocumented
    (*this->instructions).at(0xAF).function = &CPU::LAX;
    (*this->instructions).at(0xAF).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xAF).cycles = 4;
    //B0 BCS
    (*this->instructions).at(0xB0).function = &CPU::BCS;
    (*this->instructions).at(0xB0).addressing_mode = &CPU::REL;
    (*this->instructions).at(0xB0).cycles = 2;
    //B1 LDA
    (*this->instructions).at(0xB1).function = &CPU::LDA;
    (*this->instructions).at(0xB1).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0xB1).cycles = 5;
    //B2
    //B3 LAX                                                   undocumented
    (*this->instructions).at(0xB3).function = &CPU::LAX;
    (*this->instructions).at(0xB3).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0xB3).cycles = 5;
    //B4 LDY
    (*this->instructions).at(0xB4).function = &CPU::LDY;
    (*this->instructions).at(0xB4).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0xB4).cycles = 4;
    //B5 LDA
    (*this->instructions).at(0xB5).function = &CPU::LDA;
    (*this->instructions).at(0xB5).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0xB5).cycles = 4;
    //B6 LDX
    (*this->instructions).at(0xB6).function = &CPU::LDX;
    (*this->instructions).at(0xB6).addressing_mode = &CPU::YZP;
    (*this->instructions).at(0xB6).cycles = 4;
    //B7 LAX                                                   undocumented
    (*this->instructions).at(0xB7).function = &CPU::LAX;
    (*this->instructions).at(0xB7).addressing_mode = &CPU::YZP;
    (*this->instructions).at(0xB7).cycles = 4;
    //B8 CLV
    (*this->instructions).at(0xB8).function = &CPU::CLV;
    (*this->instructions).at(0xB8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xB8).cycles = 2;
    //B9 LDA
    (*this->instructions).at(0xB9).function = &CPU::LDA;
    (*this->instructions).at(0xB9).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0xB9).cycles = 4;
    //BA TSX
    (*this->instructions).at(0xBA).function = &CPU::TSX;
    (*this->instructions).at(0xBA).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xBA).cycles = 2;
    //BB
    //BC LDY
    (*this->instructions).at(0xBC).function = &CPU::LDY;
    (*this->instructions).at(0xBC).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0xBC).cycles = 4;
    //BD LDA
    (*this->instructions).at(0xBD).function = &CPU::LDA;
    (*this->instructions).at(0xBD).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0xBD).cycles = 4;
    //BE LDX
    (*this->instructions).at(0xBE).function = &CPU::LDX;
    (*this->instructions).at(0xBE).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0xBE).cycles = 4;
    //BF LAX                                                   undocumented
    (*this->instructions).at(0xBF).function = &CPU::LAX;
    (*this->instructions).at(0xBF).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0xBF).cycles = 4;
    //C0 CPY
    (*this->instructions).at(0xC0).function = &CPU::CPY;
    (*this->instructions).at(0xC0).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xC0).cycles = 2;
    //C1 CMP
    (*this->instructions).at(0xC1).function = &CPU::CMP;
    (*this->instructions).at(0xC1).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0xC1).cycles = 6;
    //C2
    //C3 DCP                                                   undocumented
    (*this->instructions).at(0xC3).function = &CPU::DCP;
    (*this->instructions).at(0xC3).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0xC3).cycles = 8;
    //C4 CPY
    (*this->instructions).at(0xC4).function = &CPU::CPY;
    (*this->instructions).at(0xC4).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xC4).cycles = 3;
    //C5 CMP
    (*this->instructions).at(0xC5).function = &CPU::CMP;
    (*this->instructions).at(0xC5).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xC5).cycles = 3;
    //C6 DEC
    (*this->instructions).at(0xC6).function = &CPU::DEC;
    (*this->instructions).at(0xC6).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xC6).cycles = 5;
    //C7 DCP                                                   undocumented
    (*this->instructions).at(0xC7).function = &CPU::DCP;
    (*this->instructions).at(0xC7).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xC7).cycles = 5;
    //C8 INY
    (*this->instructions).at(0xC8).function = &CPU::INY;
    (*this->instructions).at(0xC8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xC8).cycles = 2;
    //C9 CMP
    (*this->instructions).at(0xC9).function = &CPU::CMP;
    (*this->instructions).at(0xC9).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xC9).cycles = 2;
    //CA DEX
    (*this->instructions).at(0xCA).function = &CPU::DEX;
    (*this->instructions).at(0xCA).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xCA).cycles = 2;
    //CB
    //CC CPY
    (*this->instructions).at(0xCC).function = &CPU::CPY;
    (*this->instructions).at(0xCC).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xCC).cycles = 4;
    //CD CMP
    (*this->instructions).at(0xCD).function = &CPU::CMP;
    (*this->instructions).at(0xCD).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xCD).cycles = 4;
    //CE DEC
    (*this->instructions).at(0xCE).function = &CPU::DEC;
    (*this->instructions).at(0xCE).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xCE).cycles = 6;
    //CF DCP                                                   undocumented
    (*this->instructions).at(0xCF).function = &CPU::DCP;
    (*this->instructions).at(0xCF).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xCF).cycles = 6;
    //D0 BNE
    (*this->instructions).at(0xD0).function = &CPU::BNE;
    (*this->instructions).at(0xD0).addressing_mode = &CPU::REL;
    (*this->instructions).at(0xD0).cycles = 2;
    //D1 CMP
    (*this->instructions).at(0xD1).function = &CPU::CMP;
    (*this->instructions).at(0xD1).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0xD1).cycles = 5;
    //D2
    //D3 DCP                                                   undocumented
    (*this->instructions).at(0xD3).function = &CPU::DCP;
    (*this->instructions).at(0xD3).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0xD3).cycles = 8;
    //D4 NOP                                                   undocumented
    (*this->instructions).at(0xD4).function = &CPU::NOP;
    (*this->instructions).at(0xD4).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0xD4).cycles = 4;
    //D5 CMP
    (*this->instructions).at(0xD5).function = &CPU::CMP;
    (*this->instructions).at(0xD5).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0xD5).cycles = 4;
    //D6 DEC
    (*this->instructions).at(0xD6).function = &CPU::DEC;
    (*this->instructions).at(0xD6).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0xD6).cycles = 6;
    //D7 DCP                                                   undocumented
    (*this->instructions).at(0xD7).function = &CPU::DCP;
    (*this->instructions).at(0xD7).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0xD7).cycles = 6;
    //D8 CLD
    (*this->instructions).at(0xD8).function = &CPU::CLD;
    (*this->instructions).at(0xD8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xD8).cycles = 2;
    //D9 CMP
    (*this->instructions).at(0xD9).function = &CPU::CMP;
    (*this->instructions).at(0xD9).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0xD9).cycles = 4;
    //DA NOP                                                   undocumented
    (*this->instructions).at(0xDA).function = &CPU::NOP;
    (*this->instructions).at(0xDA).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xDA).cycles = 2;
    //DB DCP                                                   undocumented
    (*this->instructions).at(0xDB).function = &CPU::DCP;
    (*this->instructions).at(0xDB).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0xDB).cycles = 7;
    //DC NOP                                                   undocumented
    (*this->instructions).at(0xDC).function = &CPU::NOP;
    (*this->instructions).at(0xDC).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0xDC).cycles = 4;
    //DD CMP
    (*this->instructions).at(0xDD).function = &CPU::CMP;
    (*this->instructions).at(0xDD).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0xDD).cycles = 4;
    //DE DEC
    (*this->instructions).at(0xDE).function = &CPU::DEC;
    (*this->instructions).at(0xDE).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0xDE).cycles = 7;
    //DF DCP                                                   undocumented
    (*this->instructions).at(0xDF).function = &CPU::DCP;
    (*this->instructions).at(0xDF).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0xDF).cycles = 7;
    //E0 CPX
    (*this->instructions).at(0xE0).function = &CPU::CPX;
    (*this->instructions).at(0xE0).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xE0).cycles = 2;
    //E1 SBC
    (*this->instructions).at(0xE1).function = &CPU::SBC;
    (*this->instructions).at(0xE1).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0xE1).cycles = 6;
    //E2
    //E3 ISC                                                   undocumented
    (*this->instructions).at(0xE3).function = &CPU::ISC;
    (*this->instructions).at(0xE3).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0xE3).cycles = 8;
    //E4 CPX
    (*this->instructions).at(0xE4).function = &CPU::CPX;
    (*this->instructions).at(0xE4).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xE4).cycles = 3;
    //E5 SBC
    (*this->instructions).at(0xE5).function = &CPU::SBC;
    (*this->instructions).at(0xE5).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xE5).cycles = 3;
    //E6 INC
    (*this->instructions).at(0xE6).function = &CPU::INC;
    (*this->instructions).at(0xE6).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xE6).cycles = 5;
    //E7 ISC                                                   undocumented
    (*this->instructions).at(0xE7).function = &CPU::ISC;
    (*this->instructions).at(0xE7).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xE7).cycles = 5;
    //E8 INX
    (*this->instructions).at(0xE8).function = &CPU::INX;
    (*this->instructions).at(0xE8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xE8).cycles = 2;
    //E9 SBC
    (*this->instructions).at(0xE9).function = &CPU::SBC;
    (*this->instructions).at(0xE9).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xE9).cycles = 2;
    //EA NOP
    (*this->instructions).at(0xEA).function = &CPU::NOP;
    (*this->instructions).at(0xEA).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xEA).cycles = 2;
    //EB SBC                                                   undocumented
    (*this->instructions).at(0xEB).function = &CPU::SBC;
    (*this->instructions).at(0xEB).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xEB).cycles = 2;
    //EC CPX
    (*this->instructions).at(0xEC).function = &CPU::CPX;
    (*this->instructions).at(0xEC).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xEC).cycles = 4;
    //ED SBC
    (*this->instructions).at(0xED).function = &CPU::SBC;
    (*this->instructions).at(0xED).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xED).cycles = 4;
    //EE INC
    (*this->instructions).at(0xEE).function = &CPU::INC;
    (*this->instructions).at(0xEE).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xEE).cycles = 6;
    //EF ISC                                                   undocumented
    (*this->instructions).at(0xEF).function = &CPU::ISC;
    (*this->instructions).at(0xEF).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0xEF).cycles = 6;
    //F0 BEQ
    (*this->instructions).at(0xF0).function = &CPU::BEQ;
    (*this->instructions).at(0xF0).addressing_mode = &CPU::REL;
    (*this->instructions).at(0xF0).cycles = 2;
    //F1 SBC
    (*this->instructions).at(0xF1).function = &CPU::SBC;
    (*this->instructions).at(0xF1).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0xF1).cycles = 5;
    //F2
    //F3 ISC                                                   undocumented
    (*this->instructions).at(0xF3).function = &CPU::ISC;
    (*this->instructions).at(0xF3).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0xF3).cycles = 8;
    //F4 NOP                                                   undocumented
    (*this->instructions).at(0xF4).function = &CPU::NOP;
    (*this->instructions).at(0xF4).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0xF4).cycles = 4;
    //F5 SBC
    (*this->instructions).at(0xF5).function = &CPU::SBC;
    (*this->instructions).at(0xF5).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0xF5).cycles = 4;
    //F6 INC
    (*this->instructions).at(0xF6).function = &CPU::INC;
    (*this->instructions).at(0xF6).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0xF6).cycles = 6;
    //F7 ISC                                                   undocumented
    (*this->instructions).at(0xF7).function = &CPU::ISC;
    (*this->instructions).at(0xF7).addressing_mode = &CPU::XZP;
    (*this->instructions).at(0xF7).cycles = 6;
    //F8 SED
    (*this->instructions).at(0xF8).function = &CPU::SED;
    (*this->instructions).at(0xF8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xF8).cycles = 2;
    //F9 SBC
    (*this->instructions).at(0xF9).function = &CPU::SBC;
    (*this->instructions).at(0xF9).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0xF9).cycles = 4;
    //FA NOP                                                   undocumented
    (*this->instructions).at(0xFA).function = &CPU::NOP;
    (*this->instructions).at(0xFA).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xFA).cycles = 2;
    //FB ISC                                                   undocumented
    (*this->instructions).at(0xFB).function = &CPU::ISC;
    (*this->instructions).at(0xFB).addressing_mode = &CPU::YIA;
    (*this->instructions).at(0xFB).cycles = 7;
    //FC NOP                                                   undocumented
    (*this->instructions).at(0xFC).function = &CPU::NOP;
    (*this->instructions).at(0xFC).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0xFC).cycles = 4;
    //FD SBC
    (*this->instructions).at(0xFD).function = &CPU::SBC;
    (*this->instructions).at(0xFD).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0xFD).cycles = 4;
    //FE INC
    (*this->instructions).at(0xFE).function = &CPU::INC;
    (*this->instructions).at(0xFE).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0xFE).cycles = 7;
    //FF ISC                                                   undocumented
    (*this->instructions).at(0xFF).function = &CPU::ISC;
    (*this->instructions).at(0xFF).addressing_mode = &CPU::XIA;
    (*this->instructions).at(0xFF).cycles = 7;
}

/* instructions */

//load
//Load Accumulator and Index Register X From Memory            undocumented
bool CPU::LAX(){
    this->registers.r_A = this->nes.read(this->data_to_read);
    this->registers.r_iX = this->nes.read(this->data_to_read);
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    return true;
}
//Load Accumulator with Memory
bool CPU::LDA(){
    this->registers.r_A = this->nes.read(this->data_to_read);
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    return true;
}
//Load Index Register X From Memory
bool CPU::LDX(){
    this->registers.r_iX = this->nes.read(this->data_to_read);
    
    this->setflag(0x80, this->registers.r_iX & 0x80);
    this->setflag(0x02, this->registers.r_iX == 0);
    return true;
}
//Load Index Register Y From Memory
bool CPU::LDY(){
    this->registers.r_iY = this->nes.read(this->data_to_read);
    
    this->setflag(0x80, this->registers.r_iY & 0x80);
    this->setflag(0x02, this->registers.r_iY == 0);
    return true;
}
//Store Accumulator "AND" Index Register X in Memory           undocumented
bool CPU::SAX(){
    this->nes.write(this->data_to_read, (this->registers.r_iX & this->registers.r_A));
    return false;
}
//Store Accumulator in Memory
bool CPU::STA(){
    this->nes.write(this->data_to_read, this->registers.r_A);
    return false;
}
//Store Index Register X In Memory
bool CPU::STX(){
    this->nes.write(this->data_to_read, this->registers.r_iX);
    return false;
}
//Store Index Register Y In Memory
bool CPU::STY(){
    this->nes.write(this->data_to_read, this->registers.r_iY);
    return false;
}

//trans
//Transfer Accumulator To Index X
bool CPU::TAX(){
    this->registers.r_iX = this->registers.r_A;
    
    this->setflag(0x80, this->registers.r_iX & 0x80);
    this->setflag(0x02, this->registers.r_iX == 0);
    return false;
}
//Transfer Accumula Tor To Index Y
bool CPU::TAY(){
    this->registers.r_iY = this->registers.r_A;
    
    this->setflag(0x80, this->registers.r_iY & 0x80);
    this->setflag(0x02, this->registers.r_iY == 0);
    return false;
}
//Transfer Stack Pointer To Index X
bool CPU::TSX(){
    this->registers.r_iX = this->registers.r_SP;
    
    this->setflag(0x80, this->registers.r_iX & 0x80);
    this->setflag(0x02, this->registers.r_iX == 0);
    return false;
}
//Transfer Index X To Accumulator
bool CPU::TXA(){
    this->registers.r_A = this->registers.r_iX;
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    return false;
}
//Transfer Index X To Stack Pointer
bool CPU::TXS(){
    this->registers.r_SP = this->registers.r_iX;
    //does not affect any flag
    return false;
}
//Transfer Index Y To Accumulator
bool CPU::TYA(){
    this->registers.r_A = this->registers.r_iY;
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    return false;
}

//stack
//In the byte pushed, bit 5 is always set to 1, and bit 4 is 1 if from an instruction (PHP or BRK)
//Push Accumulator On Stack
bool CPU::PHA(){
    //0x0100 to offset
    this->nes.write(0x0100 + this->registers.r_SP--,this->registers.r_A);
    //sp-- because sp needs to point to the nest empty location on the stack
    return false;
}
//Push Processor Status On Stack
bool CPU::PHP(){
    //0x0100 to offset
    this->nes.write(0x0100 + this->registers.r_SP, this->registers.nv_bdizc | 0x14);
    this->registers.r_SP--; //always point to next address
    return false;
}
//Pull Accumulator From Stack
bool CPU::PLA(){
    //0x0100 to offset
    this->registers.r_A = this->nes.read(0x0100 + ++this->registers.r_SP);
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    return false;
}
//Pull Processor Status From Stack
bool CPU::PLP(){
    this->registers.nv_bdizc = this->nes.read(0x0100 + ++this->registers.r_SP);
    //stack pointer is incremanted before reading the value
    return false;
}

//shift
//Arithmetic Shift Left
bool CPU::ASL(){
    Byte data = 0x00;
    if(this->opcode == 0x0A){
        data = this->registers.r_A;
        this->registers.r_A = data << 1;
    }
    else{
        data = this->nes.read(this->data_to_read);
        this->nes.write(this->data_to_read, data << 1);
    }
    
    this->setflag(0x80, data & 0x40);
    this->setflag(0x02, (data & 0x7F) == 0);
    this->setflag(0x01, data & 0x80);
    return false;
}
//Logical Shift Right
bool CPU::LSR(){
    Byte data = 0x00;
    if(this->opcode == 0x4A){
        data = this->registers.r_A;
        this->registers.r_A = data >> 1;
    }
    else{
        data = this->nes.read(this->data_to_read);
        this->nes.write(this->data_to_read, data >> 1);
    }
    
    this->setflag(0x80, false);
    this->setflag(0x02, (data & 0xFE) == 0);
    this->setflag(0x01, data & 0x01);
    return false;
}
//Rotate Left
bool CPU::ROL(){
    Byte data = 0x00;
    if(this->opcode == 0x2A){
        data = this->registers.r_A;
        this->registers.r_A = data << 1 | (this->getflag(0x01) & 0x01);
    }
    else{
        data = this->nes.read(this->data_to_read);
        this->nes.write(this->data_to_read, data << 1 | (this->getflag(0x01) & 0x01));
    }
    
    this->setflag(0x80, data & 0x40);
    this->setflag(0x02, ((data & 0x7F) == 0) & (this->getflag(0x01) == 0));
    this->setflag(0x01, data & 0x80);
    return false;
}
//Rotate Right
bool CPU::ROR(){
    Byte data = 0x00;
    if(this->opcode == 0x6A){
        data = this->registers.r_A;
        this->registers.r_A = data >> 1 | this->getflag(0x01) << 7;
    }
    else{
        data = this->nes.read(this->data_to_read);
        this->nes.write(this->data_to_read, data >> 1 | this->getflag(0x01) << 7);
    }
    
    this->setflag(0x80, this->getflag(0x01));
    this->setflag(0x02, ((data & 0xFE) == 0) & (this->getflag(0x01) == 0));
    this->setflag(0x01, data & 0x01);
    return false;
}

//logic
//"AND" Memory with Accumulator
bool CPU::AND(){
    this->registers.r_A &= this->nes.read(this->data_to_read);
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    return true;
}
//Test Bits in Memory with Accumulator
bool CPU::BIT(){
    Byte memtested = this->nes.read(this->data_to_read);
    bool result = this->registers.r_A & memtested;
    
    this->setflag(0x80, memtested & 0x80);
    this->setflag(0x40, memtested & 0x40);
    this->setflag(0x02, result == 0);
    return false;
}
//"Exclusive OR" Memory with Accumulator
bool CPU::EOR(){
    this->registers.r_A ^= this->nes.read(this->data_to_read);
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    return true;
}
//"OR" Memory with Accumulator
bool CPU::ORA(){
    this->registers.r_A |= this->nes.read(this->data_to_read);
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    return true;
}

//arith
//Add Memory to Accumulator with Carry
bool CPU::ADC(){
    //allow us to look for carry
    int result = this->registers.r_A + this->nes.read(this->data_to_read) + (int) this->getflag(0x01);
    //bool -> int implicit conversion has every byte at one
    
    
    //Thanks internet! This flag was killing me
    this->setflag(0x40,(~(this->registers.r_A^this->nes.read(this->data_to_read)) & (result)) & 0x80);
        
    
    this->registers.r_A = result;

    this->setflag(0x80, (this->registers.r_A & 0x80) == 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    this->setflag(0x01, result > 255); //there is a carry if the result contains a 1 after the first byte.
    return true;
}
//Compare Memory and Accumulator
bool CPU::CMP(){
    Byte result = this->registers.r_A - this->nes.read(this->data_to_read);
    
    this->setflag(0x02, result == 0);
    this->setflag(0x80, (result & 0x80) == 0x80);
    this->setflag(0x01, this->nes.read(this->data_to_read) <= this->registers.r_A);
    return true;
}
//Compare Index Register X To Memory
bool CPU::CPX(){
    int result = this->registers.r_iX - this->nes.read(this->data_to_read);
    
    this->setflag(0x02, result == 0);
    this->setflag(0x80, (result & 0x80) == 0x80);
    this->setflag(0x01, this->nes.read(this->data_to_read) <= this->registers.r_iX);
    return false;
}
//Compare Index Register Y To Memory
bool CPU::CPY(){
    Byte result = this->registers.r_iY - this->nes.read(this->data_to_read);
    
    this->setflag(0x02, result == 0);
    this->setflag(0x80, (result & 0x80) == 0x80);
    this->setflag(0x01, this->nes.read(this->data_to_read) <= this->registers.r_iY);
    return false;
}
//Decrement Memory By One then Compare with Accumulator        undocumented
bool CPU::DCP(){
    DEC();
    CMP();
    return false;
}
//Increment Memory By One then SBC then Subtract Memory from Accumulator with Borrow       undocumented
bool CPU::ISC(){
    INC();
    SBC();
    
    return false;
}
//Rotate Left then "AND" with Accumulator                      undocumented
bool CPU::RLA(){
    ROL();
    AND();
    return false;
}
//Subtract Memory from Accumulator with Borrow
bool CPU::SBC(){
    //thanks internet, this function was killing me
    Byte value = this->nes.read(this->data_to_read) ^ 0xFF;
    
    int result = this->registers.r_A + value + (int) this->getflag(0x01);
    //bool -> int implicit conversion has every byte at one
    
    this->setflag(0x40,(this->registers.r_A^result) & (result^value) & 0x80);
        
    
    this->registers.r_A = result;

    this->setflag(0x80, (this->registers.r_A & 0x80) == 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    this->setflag(0x01, result & 0xFF00);
    
    return true;
}
//Arithmetic Shift Left then "OR" Memory with Accumulator      undocumented
bool CPU::SLO(){
    ASL();
    ORA();
    return false;
}
//Logical Shift Right then "Exclusive OR" Memory with Accumulator      undocumented
bool CPU::SRE(){
    LSR();
    EOR();
    return false;
}

//inc
//Decrement Memory By One
bool CPU::DEC(){
    Byte result = this->nes.read(this->data_to_read) - 1;
    this->nes.write(this->data_to_read, result);
    
    this->setflag(0x80, result & 0x80);
    this->setflag(0x02, result == 0);
    
    return false;
}
//Decrement Index Register X By One
bool CPU::DEX(){
    this->registers.r_iX--;
    
    this->setflag(0x80, this->registers.r_iX & 0x80);
    this->setflag(0x02, this->registers.r_iX == 0);
    
    return false;
}
//Decrement Index Register Y By One
bool CPU::DEY(){
    this->registers.r_iY--;
    
    this->setflag(0x80, this->registers.r_iY & 0x80);
    this->setflag(0x02, this->registers.r_iY == 0);
    
    return false;
}
//Increment Memory By One
bool CPU::INC(){
    Byte result = this->nes.read(this->data_to_read) + 1;
    this->nes.write(this->data_to_read, result);
    
    this->setflag(0x80, result & 0x80);
    this->setflag(0x02, result == 0);
    
    return false;
}
//Increment Index Register X By One
bool CPU::INX(){
    this->registers.r_iX++;
    
    this->setflag(0x80, this->registers.r_iX & 0x80);
    this->setflag(0x02, this->registers.r_iX == 0);
    
    return false;
}
//Increment Index Register Y By One
bool CPU::INY(){
    this->registers.r_iY++;
    
    this->setflag(0x80, this->registers.r_iY & 0x80);
    this->setflag(0x02, this->registers.r_iY == 0);
    
    return false;
}

//ctrl
//JMP Indirect
bool CPU::JMP(){
    this->registers.r_PC++;
    this->registers.r_PC = this->data_to_read;
    
    return false;
}
//Jump To Subroutine
bool CPU::JSR(){
    //0x0100 to offset
    this->nes.write(0x0100 + this->registers.r_SP, (this->registers.r_PC-1) >> 8); //high
    this->registers.r_SP--;
    
    this->nes.write(0x0100 + this->registers.r_SP, (this->registers.r_PC-1) & 0x00FF); //low
    this->registers.r_SP--;
    
    this->registers.r_PC = this->data_to_read;
    
    return false;
}
//Return From Interrupt
bool CPU::RTI(){
    //get processor statue
    this->registers.nv_bdizc = this->nes.read(0x0100 + ++this->registers.r_SP);
    
    //get program counter
    this->registers.r_PC = this->nes.read(0x0100 + ++this->registers.r_SP);//low
    this->registers.r_PC |= this->nes.read(0x0100 + ++this->registers.r_SP) << 8;//add high
    //SP is incremented twice to be set
    
    return false;
}
//Return From Subroutme
bool CPU::RTS(){
    this->registers.r_PC = this->nes.read(0x0100 + ++this->registers.r_SP);//low
    this->registers.r_PC |= this->nes.read(0x0100 + ++this->registers.r_SP) << 8;//add high
    //SP is incremented twice to be set
    this->registers.r_PC++; //point to next instruction
    
    return false;
}

//bra
//Branch on Carry Clear
bool CPU::BCC(){
    if(!this->getflag(0x01)){//take branch if carry flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
    
    return true;
}
//Branch on Carry Set
bool CPU::BCS(){
    if(this->getflag(0x01)){//take branch if carry flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
    
    return true;
}
//Branch on Result Zero
bool CPU::BEQ(){
    if(this->getflag(0x02)){//take branch if zero flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
    
    return true;
}
//Branch on Result Minus
bool CPU::BMI(){
    if(this->getflag(0x80)){//take branch if zero flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
    
    return true;
}
//Branch on Result Not Zero
bool CPU::BNE(){
    if(!this->getflag(0x02)){//take branch if zero flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
    
    return true;
}
//Branch on Result Plus
bool CPU::BPL(){
    if(!this->getflag(0x80)){//take branch if N flag is reset
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
    
    return true;
}
//Branch on Overflow Clear
bool CPU::BVC(){
    if(!this->getflag(0x40)){//take branch if overflow flag is reset
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
    
    return true;
}
//Branch on Overflow Set
bool CPU::BVS(){
    if(this->getflag(0x40)){//take branch if overflow flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
    
    return true;
}

//flags
//Clear Carry Flag
bool CPU::CLC(){
    this->setflag(0x01, false);
    return false;
}
//Clear Decimal Mode
bool CPU::CLD(){
    this->setflag(0x08, false);
    return false;
}
//Clear Overflow Flag
bool CPU::CLV(){
    this->setflag(0x40, false);
    return false;
}
//Set Carry Flag
bool CPU::SEC(){
    this->setflag(0x01, true);
    return false;
}
//Set Decimal Mode
bool CPU::SED(){
    this->registers.nv_bdizc |= 0x08;
    return false;
}
//Set Interrupt Disable
bool CPU::SEI(){
    this->registers.nv_bdizc |= 0x04;
    return false;
}

//nop
//No Operation
bool CPU::NOP(){
    return true; //only usefull for undocumented opcodes
}


