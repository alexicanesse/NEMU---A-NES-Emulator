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
    
    struct registers {
        Byte r_A = 0;    //accumulator register (A)
        Byte r_iX = 0;   //index register X
        Byte r_iY = 0;   //index register Y
        //Memory space [0x0100 - 0x01FF] is used for stack. The stack pointer holds the address of the top of that space
        //It grows from top to bottom
        Byte r_SP = 0xFD;   //stack pointer
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
    //Returns true iff is may requiere an additional cycle
    
    //load
    bool LDA(); //Load Accumulator with Memory
    bool LDX(); //Load Index Register X From Memory
    bool LDY(); //Load Index Register Y From Memory
    bool STA(); //Store Accumulator in Memory
    bool STX(); //Store Index Register X In Memory
    bool STY(); //Store Index Register Y In Memory
    
    //trans
    bool TAX(); //Transfer Accumulator To Index X
    bool TAY(); //Transfer Accumula Tor To Index Y
    bool TSX(); //Transfer Stack Pointer To Index X
    bool TXA(); //Transfer Index X To Accumulator
    bool TXS(); //Transfer Index X To Stack Pointer
    bool TYA(); //Transfer Index Y To Accumulator
    
    //stack
    bool PHA(); //Push Accumulator On Stack
    bool PHP(); //Push Processor Status On Stack
    bool PLA(); //Pull Accumulator From Stack
    bool PLP(); //Pull Processor Status From Stack
    
    //shift
    bool ASL(); //Arithmetic Shift Left
    bool LSR(); //Logical Shift Right
    bool ROL(); //Rotate Left
    bool ROR(); //Rotate Right
    
    //logic
    bool AND(); //"AND" Memory with Accumulator
    bool BIT(); //Test Bits in Memory with Accumulator
    bool EOR(); //"Exclusive OR" Memory with Accumulator
    bool ORA(); //"OR" Memory with Accumulator
    
    //arith
    bool ADC(); //Add Memory to Accumulator with Carr
    bool CMP(); //Compare Memory and Accumulator
    bool CPX(); //Compare Index Register X To Memory
    bool CPY(); //Compare Index Register Y To Memory
    bool SBC(); //Subtract Memory from Accumulator with Borrow
    
    //inc
    bool DEC(); //Decrement Memory By One
    bool DEX(); //Decrement Index Register X By One
    bool DEY(); //Decrement Index Register Y By One
    bool INC(); //Increment Memory By One
    bool INX(); //Increment Index Register X By One
    bool INY(); //Increment Index Register Y By One
    
    //ctrl
#warning TODO
    bool BRK(); //Break Command
    bool JMP(); //JMP Indirect
    bool JSR(); //Jump To Subroutine
    bool RTI(); //Return From Interrupt
    bool RTS(); //Return From Subroutme
    
    //bra
    bool BCC(); //Branch on Carry Clear
    bool BCS(); //Branch on Carry Set
    bool BEQ(); //Branch on Result Zero
    bool BMI(); //Branch on Result Minus
    bool BNE(); //Branch on Result Not Zero
    bool BPL(); //Branch on Result Plus
    bool BVC(); //Branch on Overflow Clear
    bool BVS(); //Branch on Overflow Set
    
    //flags
    bool CLC(); //Clear Carry Flag
    bool CLD(); //Clear Decimal Mode
#warning TODO
    bool CLI(); //Clear Interrupt Disable
    bool CLV(); //Clear Overflow Flag
    bool SEC(); //Set Carry Flag
    bool SED(); //Set Decimal Mode
    bool SEI(); //Set Interrupt Disable
    
    //nop
    bool NOP(); //No Operation
    
    
    /*
     instructions
    */
    struct instruction { //instructions type
        bool (CPU::*function)(void) = NULL; //function doing the instructions's job
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
