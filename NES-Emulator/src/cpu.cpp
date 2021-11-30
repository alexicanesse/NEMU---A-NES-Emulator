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
    
    //run addr_mode and add an additionnal cycle if requiered
    if((this->*instr.addressing_mode)()) //(page)
        this->rem_cycles++;
    
    //run instruction and add an additionnal cycle if requiered
    if((this->*instr.function)()) //branch cycle is directly added by functions
        this->rem_cycles++;
    
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
    if((this->registers.r_PC ^ this->data_to_read) & 0xFF00) //page crossed
        return true;
    else
        return false;
#warning t branch cycle
}




CPU::CPU(){
    //08 PHP
    (*this->instructions).at(0x08).function = &CPU::PHP;
    (*this->instructions).at(0x08).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x08).cycles = 3;
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
    //24 BIT
    (*this->instructions).at(0x24).function = &CPU::BIT;
    (*this->instructions).at(0x24).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x24).cycles = 3;
    //38 SEC
    (*this->instructions).at(0x38).function = &CPU::SEC;
    (*this->instructions).at(0x38).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x38).cycles = 2;
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
    //70 BVS
    (*this->instructions).at(0x78).function = &CPU::SEI;
    (*this->instructions).at(0x78).addressing_mode = &CPU::IMP;
    (*this->instructions).at(0x78).cycles = 2;
    //78 SEI
    (*this->instructions).at(0x70).function = &CPU::BVS;
    (*this->instructions).at(0x70).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x70).cycles = 2;
    //85 STA
    (*this->instructions).at(0x85).function = &CPU::STA;
    (*this->instructions).at(0x85).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x85).cycles = 3;
    //86 STX
    (*this->instructions).at(0x86).function = &CPU::STX;
    (*this->instructions).at(0x86).addressing_mode = &CPU::ZPA;
    (*this->instructions).at(0x86).cycles = 3;
    //90 BCC
    (*this->instructions).at(0x90).function = &CPU::BCC;
    (*this->instructions).at(0x90).addressing_mode = &CPU::REL;
    (*this->instructions).at(0x90).cycles = 2;
    //A2 LDX
    (*this->instructions).at(0xA2).function = &CPU::LDX;
    (*this->instructions).at(0xA2).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xA2).cycles = 2;
    //A9 LDA
    (*this->instructions).at(0xA9).function = &CPU::LDA;
    (*this->instructions).at(0xA9).addressing_mode = &CPU::IMM;
    (*this->instructions).at(0xA9).cycles = 2;
    //B0 BCS
    (*this->instructions).at(0xB0).function = &CPU::BCS;
    (*this->instructions).at(0xB0).addressing_mode = &CPU::REL;
    (*this->instructions).at(0xB0).cycles = 2;
    //D0 BNE
    (*this->instructions).at(0xD0).function = &CPU::BNE;
    (*this->instructions).at(0xD0).addressing_mode = &CPU::REL;
    (*this->instructions).at(0xD0).cycles = 2;
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

//(*this->instructions).at(0x4c).function = JMP();
//*instructions[0x4c].addressing_mode = ABS();
//*instructions[0x4c].cycles = 3;

//load
//Load Accumulator with Memory
bool CPU::LDA(){
    this->registers.r_A = this->nes.read(this->data_to_read);
    this->setflag(0x80, this->registers.r_A & 0x80);
    this->setflag(0x02, this->registers.r_A == 0);
    return false;
}
//Load Index Register X From Memory
bool CPU::LDX(){
    this->registers.r_iX = this->nes.read(this->data_to_read);
    
    if(this->registers.r_iX & 0x80)//handle N flag
        this->setflag(0x80, 1);
    else
        this->setflag(0x80, 0);
    
    if(this->registers.r_iX == 0)
        this->setflag(0x02, 1);
    else
        this->setflag(0x02, 0);
    
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
    return 0;
}

//stack
//In the byte pushed, bit 5 is always set to 1, and bit 4 is 1 if from an instruction (PHP or BRK)
//Push Processor Status On Stack
bool CPU::PHP(){
    this->nes.write(this->registers.r_SP, this->registers.nv_bdizc | 0x14);
    this->registers.r_SP--; //always point to next address
    return false;
}
//Pull Accumulator From Stack
bool CPU::PLA(){
    this->registers.r_A = this->nes.read(++this->registers.r_SP);
    return false;
}

//logic
//Test Bits in Memory with Accumulator
bool CPU::BIT(){
    Byte memtested = this->nes.read(this->data_to_read);
    bool result = this->registers.r_A & memtested;
    
    this->setflag(0x80, memtested & 0x80);
    this->setflag(0x40, memtested & 0x40);
    this->setflag(0x02, result == 0);
    return false;
}

//ctrl
//JMP Indirect
bool CPU::JMP(){
    this->registers.r_PC = this->data_to_read;
    return false;
}
//Jump To Subroutine
bool CPU::JSR(){
    this->nes.write(this->registers.r_SP, (this->registers.r_PC - 1) >> 8); //high
    this->registers.r_SP--;
    
    this->nes.write(this->registers.r_SP, (this->registers.r_PC - 1) & 0x00FF); //low
    this->registers.r_SP--;
    
    this->registers.r_PC = this->data_to_read;
    
    return false;
}
//Return From Subroutme
bool CPU::RTS(){
    this->registers.r_PC = this->nes.read(++this->registers.r_SP);//low
    this->registers.r_PC |= this->nes.read(++this->registers.r_SP) << 8;//add high
    //SP is incremented twice to be set
    
    //The stack pointer is adjusted by incrementing it twice.
    this->registers.r_SP += 2;
    
    return false;
}

//bra
//Branch on Carry Clear
bool CPU::BCC(){
    if(!this->getflag(0x01)){//take branch if carry flag is set
        this->registers.r_PC = this->data_to_read;
        return true;
    }
    return false;
}
//Branch on Carry Set
bool CPU::BCS(){
    if(this->getflag(0x01)){//take branch if carry flag is set
        this->registers.r_PC = this->data_to_read;
        return true;
    }
    return false;
}
//Branch on Result Zero
bool CPU::BEQ(){
    if(this->getflag(0x02)){//take branch if zero flag is set
        this->registers.r_PC = this->data_to_read;
        return true;
    }
    return false;
}
//Branch on Result Not Zero
bool CPU::BNE(){
    if(!this->getflag(0x02)){//take branch if zero flag is set
        this->registers.r_PC = this->data_to_read;
        return true;
    }
    return false;
}
//Branch on Result Plus
bool CPU::BPL(){
    if(!this->getflag(0x80)){//take branch if zero flag is set
        this->registers.r_PC = this->data_to_read;
        return true;
    }
    return false;
}
//Branch on Overflow Clear
bool CPU::BVC(){
    if(!this->getflag(0x40)){//take branch if overflow flag is set
        this->registers.r_PC = this->data_to_read;
        return true;
    }
    return false;
}
//Branch on Overflow Set
bool CPU::BVS(){
    if(this->getflag(0x40)){//take branch if overflow flag is set
        this->registers.r_PC = this->data_to_read;
        return true;
    }
    return false;
}

//flags
//Clear Carry Flag
bool CPU::CLC(){
    this->setflag(0x01, false);
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


