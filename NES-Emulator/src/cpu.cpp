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
    this->data_to_read = (0x00FF) & (this->nes.read(this->registers.r_PC + this->registers.r_iX));
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
    
    this->data_to_read = this->registers.r_PC + offset;
    if((this->registers.r_PC ^ this->data_to_read) & 0xFF00) //page crossed
        return true;
    else
        return false;
}




CPU::CPU(){
    //01 ORA
    (*this->instructions).at(0x01).function = &CPU::ORA;
    (*this->instructions).at(0x01).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x01).cycles = 6;
    //05 ORA
    (*this->instructions).at(0x05).function = &CPU::ORA;
    (*this->instructions).at(0x05).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x05).cycles = 3;
    //06 ASL
    (*this->instructions).at(0x06).function = &CPU::ASL;
    (*this->instructions).at(0x06).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x06).cycles = 5;
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
    //0D ORA
    (*this->instructions).at(0x0D).function = &CPU::ORA;
    (*this->instructions).at(0x0D).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x0D).cycles = 4;
    //0E ASL
    (*this->instructions).at(0x0E).function = &CPU::ASL;
    (*this->instructions).at(0x0E).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x0E).cycles = 6;
    //10 BPL
    (*this->instructions).at(0x10).function = &CPU::BPL;
    (*this->instructions).at(0x10).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x10).cycles = 2;
    //11 YZI
    (*this->instructions).at(0x11).function = &CPU::ORA;
    (*this->instructions).at(0x11).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x11).cycles = 5;
    //18 CLC
    (*this->instructions).at(0x18).function = &CPU::CLC;
    (*this->instructions).at(0x18).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x18).cycles = 2;
    //20 JSR
    (*this->instructions).at(0x20).function = &CPU::JSR;
    (*this->instructions).at(0x20).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x20).cycles = 6;
    //21 AND
    (*this->instructions).at(0x21).function = &CPU::AND;
    (*this->instructions).at(0x21).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x21).cycles = 6;
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
    //30 BMI
    (*this->instructions).at(0x30).function = &CPU::BMI;
    (*this->instructions).at(0x30).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x30).cycles = 2;
    //31 AND
    (*this->instructions).at(0x31).function = &CPU::AND;
    (*this->instructions).at(0x31).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x31).cycles = 5;
    //38 SEC
    (*this->instructions).at(0x38).function = &CPU::SEC;
    (*this->instructions).at(0x38).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x38).cycles = 2;
    //40 RTI
    (*this->instructions).at(0x40).function = &CPU::RTI;
    (*this->instructions).at(0x40).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x40).cycles = 6;
    //41 EOR
    (*this->instructions).at(0x41).function = &CPU::EOR;
    (*this->instructions).at(0x41).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x41).cycles = 6;
    //45 EOR
    (*this->instructions).at(0x45).function = &CPU::EOR;
    (*this->instructions).at(0x45).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x45).cycles = 3;
    //46 LSR
    (*this->instructions).at(0x46).function = &CPU::LSR;
    (*this->instructions).at(0x46).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x46).cycles = 5;
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
    //50 BVC
    (*this->instructions).at(0x50).function = &CPU::BVC;
    (*this->instructions).at(0x50).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x50).cycles = 2;
    //51 EOR
    (*this->instructions).at(0x51).function = &CPU::EOR;
    (*this->instructions).at(0x51).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x51).cycles = 5;
    //60 RTS
    (*this->instructions).at(0x60).function = &CPU::RTS;
    (*this->instructions).at(0x60).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x60).cycles = 6;
    //61 ADC
    (*this->instructions).at(0x61).function = &CPU::ADC;
    (*this->instructions).at(0x61).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x61).cycles = 6;
    //65 ADC
    (*this->instructions).at(0x65).function = &CPU::ADC;
    (*this->instructions).at(0x65).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x65).cycles = 3;
    //66 ROR
    (*this->instructions).at(0x66).function = &CPU::ROR;
    (*this->instructions).at(0x66).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x66).cycles = 5;
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
    //70 BVS
    (*this->instructions).at(0x70).function = &CPU::BVS;
    (*this->instructions).at(0x70).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x70).cycles = 2;
    //71 ADC
    (*this->instructions).at(0x71).function = &CPU::ADC;
    (*this->instructions).at(0x71).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x71).cycles = 5;
    //78 SEI
    (*this->instructions).at(0x78).function = &CPU::SEI;
    (*this->instructions).at(0x78).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x78).cycles = 2;
    //81 STA
    (*this->instructions).at(0x81).function = &CPU::STA;
    (*this->instructions).at(0x81).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x81).cycles = 6;
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
    //88 DEY
    (*this->instructions).at(0x88).function = &CPU::DEY;
    (*this->instructions).at(0x88).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x88).cycles = 2;
    //8A TXA
    (*this->instructions).at(0x8A).function = &CPU::TXA;
    (*this->instructions).at(0x8A).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x8A).cycles = 2;
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
    //90 BCC
    (*this->instructions).at(0x90).function = &CPU::BCC;
    (*this->instructions).at(0x90).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x90).cycles = 2;
    //91 STA
    (*this->instructions).at(0x91).function = &CPU::STA;
    (*this->instructions).at(0x91).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0x91).cycles = 6;
    //98 TYA
    (*this->instructions).at(0x98).function = &CPU::TYA;
    (*this->instructions).at(0x98).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x98).cycles = 2;
    //9A TXS
    (*this->instructions).at(0x9A).function = &CPU::TXS;
    (*this->instructions).at(0x9A).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x9A).cycles = 2;
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
    //B0 BCS
    (*this->instructions).at(0xB0).function = &CPU::BCS;
    (*this->instructions).at(0xB0).addressing_mode = &CPU::REL;
    (*this->instructions).at(0xB0).cycles = 2;
    //B1 LDA
    (*this->instructions).at(0xB1).function = &CPU::LDA;
    (*this->instructions).at(0xB1).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0xB1).cycles = 5;
    //BA TSX
    (*this->instructions).at(0xBA).function = &CPU::TSX;
    (*this->instructions).at(0xBA).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xBA).cycles = 2;
    //B8 CLV
    (*this->instructions).at(0xB8).function = &CPU::CLV;
    (*this->instructions).at(0xB8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xB8).cycles = 2;
    //C0 CPY
    (*this->instructions).at(0xC0).function = &CPU::CPY;
    (*this->instructions).at(0xC0).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xC0).cycles = 2;
    //C1 CMP
    (*this->instructions).at(0xC1).function = &CPU::CMP;
    (*this->instructions).at(0xC1).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0xC1).cycles = 6;
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
    //D0 BNE
    (*this->instructions).at(0xD0).function = &CPU::BNE;
    (*this->instructions).at(0xD0).addressing_mode = &CPU::REL;
    (*this->instructions).at(0xD0).cycles = 2;
    //D1 CMP
    (*this->instructions).at(0xD1).function = &CPU::CMP;
    (*this->instructions).at(0xD1).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0xD1).cycles = 5;
    //D8 CLD
    (*this->instructions).at(0xD8).function = &CPU::CLD;
    (*this->instructions).at(0xD8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xD8).cycles = 2;
    //E0 CPX
    (*this->instructions).at(0xE0).function = &CPU::CPX;
    (*this->instructions).at(0xE0).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xE0).cycles = 2;
    //E1 SBC
    (*this->instructions).at(0xE1).function = &CPU::SBC;
    (*this->instructions).at(0xE1).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0xE1).cycles = 6;
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
    //F0 BEQ
    (*this->instructions).at(0xF0).function = &CPU::BEQ;
    (*this->instructions).at(0xF0).addressing_mode = &CPU::REL;
    (*this->instructions).at(0xF0).cycles = 2;
    //F1 SBC
    (*this->instructions).at(0xF1).function = &CPU::SBC;
    (*this->instructions).at(0xF1).addressing_mode = &CPU::YZI;
    (*this->instructions).at(0xF1).cycles = 5;
    //F8 SED
    (*this->instructions).at(0xF8).function = &CPU::SED;
    (*this->instructions).at(0xF8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xF8).cycles = 2;
}

/* instructions */

//load
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
    return false;
}


