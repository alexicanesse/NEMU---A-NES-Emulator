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
    

    
    if((this->*instr.addressing_mode)())
        this->rem_cycles++;
    
    //branch instructions handle cycles themseles
    (this->*instr.function)();

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
    if((this->registers.r_PC ^ this->data_to_read) & 0xFF00) //page crossed
        return true;
    else
        return false;
}




CPU::CPU(){
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
    //10 BPL
    (*this->instructions).at(0x10).function = &CPU::BPL;
    (*this->instructions).at(0x10).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x10).cycles = 2;
    //18 CLC
    (*this->instructions).at(0x18).function = &CPU::CLC;
    (*this->instructions).at(0x18).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x18).cycles = 2;
    //20 JSR
    (*this->instructions).at(0x20).function = &CPU::JSR;
    (*this->instructions).at(0x20).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x20).cycles = 6;
    //2A
    (*this->instructions).at(0x2A).function = &CPU::ROL;
    (*this->instructions).at(0x2A).addressing_mode = &CPU::ACC;
    (*this->instructions).at(0x2A).cycles = 2;
    //24 BIT
    (*this->instructions).at(0x24).function = &CPU::BIT;
    (*this->instructions).at(0x24).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x24).cycles = 3;
    //28 PLP
    (*this->instructions).at(0x28).function = &CPU::PLP;
    (*this->instructions).at(0x28).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x28).cycles = 4;
    //29 AND
    (*this->instructions).at(0x29).function = &CPU::AND;
    (*this->instructions).at(0x29).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0x29).cycles = 2;
    //30 BMI
    (*this->instructions).at(0x30).function = &CPU::BMI;
    (*this->instructions).at(0x30).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x30).cycles = 2;
    //38 SEC
    (*this->instructions).at(0x38).function = &CPU::SEC;
    (*this->instructions).at(0x38).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x38).cycles = 2;
    //40 RTI
    (*this->instructions).at(0x40).function = &CPU::RTI;
    (*this->instructions).at(0x40).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x40).cycles = 6;
    //4A LSR
    (*this->instructions).at(0x4A).function = &CPU::LSR;
    (*this->instructions).at(0x4A).addressing_mode = &CPU::ACC;
    (*this->instructions).at(0x4A).cycles = 2;
    //48 PHA
    (*this->instructions).at(0x48).function = &CPU::PHA;
    (*this->instructions).at(0x48).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x48).cycles = 3;
    //49 EOR
    (*this->instructions).at(0x49).function = &CPU::EOR;
    (*this->instructions).at(0x49).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0x49).cycles = 2;
    //4C JMP
    (*this->instructions).at(0x4c).function = &CPU::JMP;
    (*this->instructions).at(0x4c).addressing_mode = &CPU::ABS;
    (*this->instructions).at(0x4c).cycles = 3;
    //50 BVC
    (*this->instructions).at(0x50).function = &CPU::BVC;
    (*this->instructions).at(0x50).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x50).cycles = 2;
    //60 RTS
    (*this->instructions).at(0x60).function = &CPU::RTS;
    (*this->instructions).at(0x60).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x60).cycles = 6;
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
    //70 BVS
    (*this->instructions).at(0x70).function = &CPU::BVS;
    (*this->instructions).at(0x70).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x70).cycles = 2;
    //78 SEI
    (*this->instructions).at(0x78).function = &CPU::SEI;
    (*this->instructions).at(0x78).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x78).cycles = 2;
    //81 STA
    (*this->instructions).at(0x81).function = &CPU::STA;
    (*this->instructions).at(0x81).addressing_mode = &CPU::XZI;
    (*this->instructions).at(0x81).cycles = 6;
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
    //A5 LDA
    (*this->instructions).at(0xA5).function = &CPU::LDA;
    (*this->instructions).at(0xA5).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0xA5).cycles = 3;
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
    //D0 BNE
    (*this->instructions).at(0xD0).function = &CPU::BNE;
    (*this->instructions).at(0xD0).addressing_mode = &CPU::REL;
    (*this->instructions).at(0xD0).cycles = 2;
    //D8 CLD
    (*this->instructions).at(0xD8).function = &CPU::CLD;
    (*this->instructions).at(0xD8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xD8).cycles = 2;
    //E0 CPX
    (*this->instructions).at(0xE0).function = &CPU::CPX;
    (*this->instructions).at(0xE0).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xE0).cycles = 2;
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
    //F0 BEQ
    (*this->instructions).at(0xF0).function = &CPU::BEQ;
    (*this->instructions).at(0xF0).addressing_mode = &CPU::REL;
    (*this->instructions).at(0xF0).cycles = 2;
    //F8 SED
    (*this->instructions).at(0xF8).function = &CPU::SED;
    (*this->instructions).at(0xF8).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0xF8).cycles = 2;
}

/* instructions */

//load
//Load Accumulator with Memory
void CPU::LDA(){
    this->registers.r_A = this->nes.read(this->data_to_read);
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
}
//Load Index Register X From Memory
void CPU::LDX(){
    this->registers.r_iX = this->nes.read(this->data_to_read);
    
    this->setflag(0x80, this->registers.r_iX & 0x80);
    this->setflag(0x02, this->registers.r_iX == 0);
}
//Load Index Register Y From Memory
void CPU::LDY(){
    this->registers.r_iY = this->nes.read(this->data_to_read);
    
    this->setflag(0x80, this->registers.r_iY & 0x80);
    this->setflag(0x02, this->registers.r_iY == 0);
}
//Store Accumulator in Memory
void CPU::STA(){
    this->nes.write(this->data_to_read, this->registers.r_A);
}
//Store Index Register X In Memory
void CPU::STX(){
    this->nes.write(this->data_to_read, this->registers.r_iX);
}

//trans
//Transfer Accumulator To Index X
void CPU::TAX(){
    this->registers.r_iX = this->registers.r_A;
    
    this->setflag(0x80, this->registers.r_iX & 0x80);
    this->setflag(0x02, this->registers.r_iX == 0);
}
//Transfer Accumula Tor To Index Y
void CPU::TAY(){
    this->registers.r_iY = this->registers.r_A;
    
    this->setflag(0x80, this->registers.r_iY & 0x80);
    this->setflag(0x02, this->registers.r_iY == 0);
}
//Transfer Stack Pointer To Index X
void CPU::TSX(){
    this->registers.r_iX = this->registers.r_SP;
    
    this->setflag(0x80, this->registers.r_iX & 0x80);
    this->setflag(0x02, this->registers.r_iX == 0);
}
//Transfer Index X To Accumulator
void CPU::TXA(){
    this->registers.r_A = this->registers.r_iX;
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
}
//Transfer Index X To Stack Pointer
void CPU::TXS(){
    this->registers.r_SP = this->registers.r_iX;
    //does not affect any flag
}
//Transfer Index Y To Accumulator
void CPU::TYA(){
    this->registers.r_A = this->registers.r_iY;
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
}

//stack
//In the byte pushed, bit 5 is always set to 1, and bit 4 is 1 if from an instruction (PHP or BRK)
//Push Accumulator On Stack
void CPU::PHA(){
    //0x0100 to offset
    this->nes.write(0x0100 + this->registers.r_SP--,this->registers.r_A);
    //sp-- because sp needs to point to the nest empty location on the stack
}
//Push Processor Status On Stack
void CPU::PHP(){
    //0x0100 to offset
    this->nes.write(0x0100 + this->registers.r_SP, this->registers.nv_bdizc | 0x14);
    this->registers.r_SP--; //always point to next address
}
//Pull Accumulator From Stack
void CPU::PLA(){
    //0x0100 to offset
    this->registers.r_A = this->nes.read(0x0100 + ++this->registers.r_SP);
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
}
//Pull Processor Status From Stack
void CPU::PLP(){
    this->registers.nv_bdizc = this->nes.read(0x0100 + ++this->registers.r_SP);
    //stack pointer is incremanted before reading the value
}

//shift
//Arithmetic Shift Left
void CPU::ASL(){
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
}
//Logical Shift Right
void CPU::LSR(){
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
}
//Rotate Left
void CPU::ROL(){
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
}
//Rotate Right
void CPU::ROR(){
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
}

//logic
//"AND" Memory with Accumulator
void CPU::AND(){
    this->registers.r_A &= this->nes.read(this->data_to_read);
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
}
//Test Bits in Memory with Accumulator
void CPU::BIT(){
    Byte memtested = this->nes.read(this->data_to_read);
    bool result = this->registers.r_A & memtested;
    
    this->setflag(0x80, memtested & 0x80);
    this->setflag(0x40, memtested & 0x40);
    this->setflag(0x02, result == 0);
}
//"Exclusive OR" Memory with Accumulator
void CPU::EOR(){
    this->registers.r_A ^= this->nes.read(this->data_to_read);
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
}
//"OR" Memory with Accumulator
void CPU::ORA(){
    this->registers.r_A |= this->nes.read(this->data_to_read);
    
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
}

//arith
//Add Memory to Accumulator with Carry
void CPU::ADC(){
    //allow us to look for carry
    int result = this->registers.r_A + this->nes.read(this->data_to_read) + (int) this->getflag(0x01);
    //bool -> int implicit conversion has every byte at one
    
    
    //Thanks internet! This flag was killing me
    this->setflag(0x40,(~(this->registers.r_A^this->nes.read(this->data_to_read)) & (result)) & 0x80);
        
    
    this->registers.r_A = result;

    this->setflag(0x80, (this->registers.r_A & 0x80) == 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    this->setflag(0x01, result > 255); //there is a carry if the result contains a 1 after the first byte.
}
//Compare Memory and Accumulator
void CPU::CMP(){
    Byte result = this->registers.r_A - this->nes.read(this->data_to_read);
    
    this->setflag(0x02, result == 0);
    this->setflag(0x80, (result & 0x80) == 0x80);
    this->setflag(0x01, this->nes.read(this->data_to_read) <= this->registers.r_A);
}
//Compare Index Register X To Memory
void CPU::CPX(){
    int result = this->registers.r_iX - this->nes.read(this->data_to_read);
    
    this->setflag(0x02, result == 0);
    this->setflag(0x80, (result & 0x80) == 0x80);
    this->setflag(0x01, this->nes.read(this->data_to_read) <= this->registers.r_iX);
}
//Compare Index Register Y To Memory
void CPU::CPY(){
    Byte result = this->registers.r_iY - this->nes.read(this->data_to_read);
    
    this->setflag(0x02, result == 0);
    this->setflag(0x80, (result & 0x80) == 0x80);
    this->setflag(0x01, this->nes.read(this->data_to_read) <= this->registers.r_iY);
}
//Subtract Memory from Accumulator with Borrow
void CPU::SBC(){
    //thanks internet, this function was killing me
    Byte value = this->nes.read(this->data_to_read) ^ 0xFF;
    
    int result = this->registers.r_A + value + (int) this->getflag(0x01);
    //bool -> int implicit conversion has every byte at one
    
    this->setflag(0x40,(this->registers.r_A^result) & (result^value) & 0x80);
        
    
    this->registers.r_A = result;

    this->setflag(0x80, (this->registers.r_A & 0x80) == 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    this->setflag(0x01, result & 0xFF00);
}

//inc
//Decrement Index Register X By One
void CPU::DEX(){
    this->registers.r_iX--;
    
    this->setflag(0x80, this->registers.r_iX & 0x80);
    this->setflag(0x02, this->registers.r_iX == 0);
}
//Decrement Index Register Y By One
void CPU::DEY(){
    this->registers.r_iY--;
    
    this->setflag(0x80, this->registers.r_iY & 0x80);
    this->setflag(0x02, this->registers.r_iY == 0);
}
//Increment Index Register X By One
void CPU::INX(){
    this->registers.r_iX++;
    
    this->setflag(0x80, this->registers.r_iX & 0x80);
    this->setflag(0x02, this->registers.r_iX == 0);
}
//Increment Index Register Y By One
void CPU::INY(){
    this->registers.r_iY++;
    
    this->setflag(0x80, this->registers.r_iY & 0x80);
    this->setflag(0x02, this->registers.r_iY == 0);
}

//ctrl
//JMP Indirect
void CPU::JMP(){
    this->registers.r_PC++;
    this->registers.r_PC = this->data_to_read;
}
//Jump To Subroutine
void CPU::JSR(){
    //0x0100 to offset
    this->nes.write(0x0100 + this->registers.r_SP, (this->registers.r_PC-1) >> 8); //high
    this->registers.r_SP--;
    
    this->nes.write(0x0100 + this->registers.r_SP, (this->registers.r_PC-1) & 0x00FF); //low
    this->registers.r_SP--;
    
    this->registers.r_PC = this->data_to_read;
}
//Return From Interrupt
void CPU::RTI(){
    //get processor statue
    this->registers.nv_bdizc = this->nes.read(0x0100 + ++this->registers.r_SP);
    
    //get program counter
    this->registers.r_PC = this->nes.read(0x0100 + ++this->registers.r_SP);//low
    this->registers.r_PC |= this->nes.read(0x0100 + ++this->registers.r_SP) << 8;//add high
    //SP is incremented twice to be set
}
//Return From Subroutme
void CPU::RTS(){
    this->registers.r_PC = this->nes.read(0x0100 + ++this->registers.r_SP);//low
    this->registers.r_PC |= this->nes.read(0x0100 + ++this->registers.r_SP) << 8;//add high
    //SP is incremented twice to be set
    this->registers.r_PC++; //point to next instruction
}

//bra
//Branch on Carry Clear
void CPU::BCC(){
    if(!this->getflag(0x01)){//take branch if carry flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
}
//Branch on Carry Set
void CPU::BCS(){
    if(this->getflag(0x01)){//take branch if carry flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
}
//Branch on Result Zero
void CPU::BEQ(){
    if(this->getflag(0x02)){//take branch if zero flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
}
//Branch on Result Minus
void CPU::BMI(){
    if(this->getflag(0x80)){//take branch if zero flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
}
//Branch on Result Not Zero
void CPU::BNE(){
    if(!this->getflag(0x02)){//take branch if zero flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
}
//Branch on Result Plus
void CPU::BPL(){
    if(!this->getflag(0x80)){//take branch if N flag is reset
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
}
//Branch on Overflow Clear
void CPU::BVC(){
    if(!this->getflag(0x40)){//take branch if overflow flag is reset
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
}
//Branch on Overflow Set
void CPU::BVS(){
    if(this->getflag(0x40)){//take branch if overflow flag is set
        this->registers.r_PC = this->data_to_read;
        this->rem_cycles ++;
    }
    //if page was crossed but the branch is not taken, no additionnal cycle is requiered
    else if((this->registers.r_PC & 0xFF00) != (this->data_to_read & 0xFF00))
        this->rem_cycles--;
}

//flags
//Clear Carry Flag
void CPU::CLC(){
    this->setflag(0x01, false);
}
//Clear Decimal Mode
void CPU::CLD(){
    this->setflag(0x08, false);
}
//Clear Overflow Flag
void CPU::CLV(){
    this->setflag(0x40, false);
}
//Set Carry Flag
void CPU::SEC(){
    this->setflag(0x01, true);
}
//Set Decimal Mode
void CPU::SED(){
    this->registers.nv_bdizc |= 0x08;
}
//Set Interrupt Disable
void CPU::SEI(){
    this->registers.nv_bdizc |= 0x04;
}

//nop
//No Operation
void CPU::NOP(){}


