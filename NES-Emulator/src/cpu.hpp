//
//  cpu.hpp
//  Proj1
//
//  Created by Alexi Canesse on 17/11/2021.
//

#ifndef cpu_hpp
#define cpu_hpp

//#include <stdio.h>
#include <cstdint>
#include <map>

#include "nes.hpp"


typedef uint8_t Byte;
typedef uint16_t Address;

class NES;


class CPU{

    

    
private:
    /*
     Registers
    */
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
        Byte r_SP = 0;   //stack pointer
        Address r_PC = 0; //program counter (PC)
        Byte nv_bdizc = 0b11111111; //Processor status register
                                    //_ = expansion | No CPU effect
    } registers;
  
    int cycles = 0; //Total number of cycles
    int additionnal_cycles = 0; //additionnal cycles for the current instruction
    
    
    /*
     instructions
    */
    struct instruction { //instructions type
        bool *function(void) = NULL; //function doing the instructions's job
        bool *addressing_mode(void) = NULL; // addressing mode function
        int cycles = 0; //number of necessary cycles
    };
#warning todo size
    map<instruction, 256> *instructions = new map<instruction, 256>; //we use an arry even thogh many cases are not used
    
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
#warning TODO
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
    bool CLI(); //Clear Interrupt Disable
    bool CLV(); //Clear Overflow Flag
    bool SEC(); //Set Carry Flag
    bool SED(); //Set Decimal Mode
    bool SEI(); //Set Interrupt Disable
    
    //nop
    bool nop(); //No Operation
    
    
    
    /*
    Other
    */
    Byte opcode = 0x00;
public:
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
    Address data_to_read = 0x0000;
    void clock();
    
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
