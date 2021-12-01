//
//  cpu.hpp
//  Proj1
//
//  Created by Alexi Canesse on 17/11/2021.
//

#ifndef cpu_hpp
#define cpu_hpp

#include <iostream>
#include <cstdint>
#include <map>

#include "nes.hpp"


typedef uint8_t Byte;
typedef uint16_t Address;

class NES;


class CPU{

#warning TODO use singleton
    

    
private:
    /*
     Registers
    */
    
#warning useless
    struct flgs {
        Byte N = 0x80; //negative result | After most instructions that have a value result, this flag will contain bit 7 of that result.
        Byte V = 0x40; //overflow
        Byte B = 0x10; //break command | No CPU effect
        Byte D = 0x08; //decimal mode | On the NES, this flag has no effect
        Byte I = 0x04; //interrupt disable | When set, all interrupts except the NMI are inhibited.
        Byte Z = 0x02; //zero result | After most instructions that have a value result, if that value is zero, this flag will be set.
        Byte C = 0x01; //carry
    };
    
    
    #warning TODO adr_pc
    struct registers {
        Byte r_A = 0;    //accumulator register (A)
        Byte r_iX = 0;   //index register X
        Byte r_iY = 0;   //index register Y
        //Memory space [0x0100 - 0x01FF] is used for stack. The stack pointer holds the address of the top of that space
        //It grows from top to bottom
        Address r_SP = 0x0FF;   //stack pointer
        Address r_PC = 0xC000; //program counter (PC)
        //While there are only six flags in the processor status register within the CPU, when transferred to the stack, there are two additional bits. These do not represent a register that can hold a value but can be used to distinguish how the flags were pushed.
        Byte nv_bdizc = 0b00100000; //Processor status register
                                    //_ = expansion | No CPU effect
    } registers;
  

    int additionnal_cycles = 0; //additionnal cycles for the current instruction
    
    
    /*
     Addressing modes
     https://www.pagetable.com/c64ref/6502/?tab=3
     */
    bool IMP(); //implied
    bool ACC(); //accumulator
    bool IMM(); //immediate
    bool ABS(); //absolute
    bool XIA(); //X indexed absolute
    bool YIA(); //Y indexed absolute
    bool IND(); //absolute indirect
    bool ZPA(); //zero page
    bool XZP(); //X-indexed zero page
    bool YZP(); //Y-indexed zero page
    bool XZI(); //X-indexed zero page indirect
    bool YZI(); //Y-indexed zero page indirect
    bool REL(); //relative


    
    /*
     OPCODES
     https://www.pagetable.com/c64ref/6502/?tab=2
    */
    //load
    void LDA(); //Load Accumulator with Memory
    void LDX(); //Load Index Register X From Memory
#warning TODO
    void LDY(); //Load Index Register Y From Memory
    void STA(); //Store Accumulator in Memory
    void STX(); //Store Index Register X In Memory
#warning TODO
    void STY(); //Store Index Register Y In Memory
    
    //trans
#warning TODO
    void TAX(); //Transfer Accumulator To Index X
#warning TODO
    void TAY(); //Transfer Accumula Tor To Index Y
#warning TODO
    void TSX(); //Transfer Stack Pointer To Index X
#warning TODO
    void TXA(); //Transfer Index X To Accumulator
#warning TODO
    void TXS(); //Transfer Index X To Stack Pointer
#warning TODO
    void TYA(); //Transfer Index Y To Accumulator
    
    //stack
    void PHA(); //Push Accumulator On Stack
    void PHP(); //Push Processor Status On Stack
    void PLA(); //Pull Accumulator From Stack
    void PLP(); //Pull Processor Status From Stack
    
    //shift
#warning TODO
    void ASL(); //Arithmetic Shift Left
#warning TODO
    void LSR(); //Logical Shift Right
#warning TODO
    void ROL(); //Rotate Left
#warning TODO
    void ROR(); //Rotate Right
    
    //logic
    void AND(); //"AND" Memory with Accumulator
    void BIT(); //Test Bits in Memory with Accumulator
    void EOR(); //"Exclusive OR" Memory with Accumulator
    void ORA(); //"OR" Memory with Accumulator
    
    //arith
    void ADC(); //Add Memory to Accumulator with Carr
    void CMP(); //Compare Memory and Accumulator
#warning TODO
    void CPX(); //Compare Index Register X To Memory
#warning TODO
    void CPY(); //Compare Index Register Y To Memory
#warning TODO
    void SBC(); //Subtract Memory from Accumulator with Borrow
    
    //inc
#warning TODO
    void DEC(); //Decrement Memory By One
#warning TODO
    void DEX(); //Decrement Index Register X By One
#warning TODO
    void DEY(); //Decrement Index Register Y By One
#warning TODO
    void INC(); //Increment Memory By One
#warning TODO
    void INX(); //Increment Index Register X By One
#warning TODO
    void INY(); //Increment Index Register Y By One
    
    //ctrl
#warning TODO
    void BRK(); //Break Command
    void JMP(); //JMP Indirect
    void JSR(); //Jump To Subroutine
#warning TODO
    void RTI(); //Return From Interrupt
    void RTS(); //Return From Subroutme
    
    //bra
    void BCC(); //Branch on Carry Clear
    void BCS(); //Branch on Carry Set
    void BEQ(); //Branch on Result Zero
    void BMI(); //Branch on Result Minus
    void BNE(); //Branch on Result Not Zero
    void BPL(); //Branch on Result Plus
    void BVC(); //Branch on Overflow Clear
    void BVS(); //Branch on Overflow Set
    
    //flags
    void CLC(); //Clear Carry Flag
    void CLD(); //Clear Decimal Mode
#warning TODO
    void CLI(); //Clear Interrupt Disable
    void CLV(); //Clear Overflow Flag
    void SEC(); //Set Carry Flag
    void SED(); //Set Decimal Mode
    void SEI(); //Set Interrupt Disable
    
    //nop
    void NOP(); //No Operation
    
    
    /*
     instructions
    */
    struct instruction { //instructions type
        void (CPU::*function)(void) = NULL; //function doing the instructions's job
        bool (CPU::*addressing_mode)() = NULL; // addressing mode function
        int cycles = 0; //number of necessary cycles
    };
    std::array<instruction, 256> *instructions = new std::array<instruction, 256>; //we use an arry even though many cases are not used

    
    /*
    Other
    */
    uint64_t opcode = 0x00;
    Address data_to_read = 0x0000;
public:
    CPU(); //constructor 
    
    /*
     Registers
    */
    flgs flags;
    
    void setflag(Byte, bool);
    bool getflag(Byte flg);
    Byte get_register_A();
    Byte get_register_X();
    Byte get_register_Y();
    Byte get_register_SP();
    Address get_register_PC();
    
    #warning TODO
    bool reset();
    
    /*
     Other
    */
    NES nes;
    void clock();
    int rem_cycles = 0;
    int cycles = 7; //Total number of cycles start at 7 because of init
};



#endif /* cpu_hpp */

/* Memory map*/
//--------------------- $10000   --------------------- $10000
//
//                                PRG-ROM Upper Bank
//
//                               --------------------- $C000
//
//Cartridge Space                 PRG-ROM Lower Bank
//
//                               --------------------- $8000
//                                SRAM
//                               --------------------- $6000
//                                Expansion ROM
//--------------------- $4020    --------------------- $4020
//                                APU & I/O Registers
//                               --------------------- $4000
//
// I/O Registers                  Mirrors $2000-$2007
//
//                               --------------------- $2008
//                                PPU Registers
//--------------------- $2000    --------------------- $2000
//
//                                Mirrors $0000-$07FF
//
//                               --------------------- $0800
// RAM                            RAM
//                               --------------------- $0200
//                                Stack
//                               --------------------- $0100
//                                Zero Page
//--------------------- $0000    --------------------- $0000
