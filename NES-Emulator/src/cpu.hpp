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
#include <array>


typedef uint8_t Byte;
typedef uint16_t Address;

class NES;


class CPU{
//private:

    
public:
    CPU(NES *nes); //constructor
    
    /*
     Registers
    */
    struct flgs { //used to reference flags without having to remember their order
        Byte N = 0x80; //negative result | After most instructions that have a value result, this flag will contain bit 7 of that result.
        Byte V = 0x40; //overflow
        Byte B = 0x10; //break command | No CPU effect
        Byte D = 0x08; //decimal mode | On the NES, this flag has no effect
        Byte I = 0x04; //interrupt disable | When set, all interrupts except the NMI are inhibited.
        Byte Z = 0x02; //zero result | After most instructions that have a value result, if that value is zero, this flag will be set.
        Byte C = 0x01; //carry
    } flags;
    
    void setflag(Byte, bool);
    bool getflag(Byte flg);
    Byte get_register_A();
    Byte get_register_X();
    Byte get_register_Y();
    Byte get_register_SP();
    Address get_register_PC();
    
    
    /*
    Interruptions
    */
    //interruptions are all public for consistency because some requiere to be callable from outside
    void IRQ();
    void NMI();
    bool BRK();
    void reset();
    
    /*
     Other
    */
    NES *nes;
    Byte opcode = 0x00; //the opcode is stored for debugging purposses
    int rem_cycles = 0; //remaining cycle until we fetch the next instruction
    int cycles = 1; //initialized at 1 because clock() is called for the first time when the cpu has already finished it's reset
    Address data_to_read = 0x0000; //used to store the data fetched until its use
    
    
    void clock(); //main function of the cpu
private:
    /*
     Registers
    */
    struct registers {
        Byte r_A = 0;    //accumulator register (A)
        Byte r_iX = 0;   //index register X
        Byte r_iY = 0;   //index register Y
        //Memory space [0x0100 - 0x01FF] is used for stack. The stack pointer holds the address of the top of that space
        //It grows from top to bottom
        Byte r_SP = 0xFF;   //stack pointer
        Address r_PC = 0x0000; //program counter (PC)
        //While there are only six flags in the processor status register within the CPU, when transferred to the stack, there are two additional bits. These do not represent a register that can hold a value but can be used to distinguish how the flags were pushed.
        Byte nv_bdizc = 0b00100000; //Processor status register
                                    //_ = expansion | No CPU effect
    } registers;

    
    
    
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
     //commented declarations have not been implemented because they're unused
    */
    //Returns true iff is may requiere an additional cycle
    
    //load
//    bool LAS(); //"AND" Memory with Stack Pointer                              undocumented
    bool LAX(); //Load Accumulator and Index Register X From Memory            undocumented
    bool LDA(); //Load Accumulator with Memory
    bool LDX(); //Load Index Register X From Memory
    bool LDY(); //Load Index Register Y From Memory
    bool SAX(); //Store Accumulator "AND" Index Register X in Memory           undocumented
//    bool SHA(); //Store Accumulator "AND" Index Register X "AND" Value         undocumented
//    bool SHX(); //Store Index Register X "AND" Value                           undocumented
//    bool SHY(); //Store Index Register Y "AND" Value                           undocumented
    bool STA(); //Store Accumulator in Memory
    bool STX(); //Store Index Register X In Memory
    bool STY(); //Store Index Register Y In Memory
    
    //trans
//    bool SHS(); //Transfer Accumulator "AND" Index Register X to Stack Pointer then Store Stack Pointer "AND" Hi-Byte In Memory            undocumented
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
//    bool ANC(); //"AND" Memory with Accumulator then Move Negative Flag to Carry Flag       undocumented
//    bool ARR(); //"AND" Accumulator then Rotate Right                          undocumented
//    bool ASR(); //"AND" then Logical Shift Right                               undocumented
    bool CMP(); //Compare Memory and Accumulator
    bool CPX(); //Compare Index Register X To Memory
    bool CPY(); //Compare Index Register Y To Memory
    bool DCP(); //Decrement Memory By One then Compare with Accumulator        undocumented
    bool ISC(); //Increment Memory By One then SBC then Subtract Memory from Accumulator with Borrow       undocumented
    bool RLA(); //Rotate Left then "AND" with Accumulator                      undocumented
    bool RRA(); //Rotate Right and Add Memory to Accumulator                   undocumented
    bool SBC(); //Subtract Memory from Accumulator with Borrow
//    bool SBX(); //Subtract Memory from Accumulator "AND" Index Register X      undocumented
    bool SLO(); //Arithmetic Shift Left then "OR" Memory with Accumulator      undocumented
    bool SRE(); //Logical Shift Right then "Exclusive OR" Memory with Accumulator      undocumented
//    bool XAA(); //Non-deterministic Operation of Accumulator, Index Register X, Memory and Bus Contents      undocumented
    
    //inc
    bool DEC(); //Decrement Memory By One
    bool DEX(); //Decrement Index Register X By One
    bool DEY(); //Decrement Index Register Y By One
    bool INC(); //Increment Memory By One
    bool INX(); //Increment Index Register X By One
    bool INY(); //Increment Index Register Y By One
    
    //ctrl
    //Break Command (in interuptions)
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
    
    //kill
//    bool JAM(); //Halt the CPU                                                 undocumented
    
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
    //this array is used to map all opcodes to their respective combinaison of addressing mode and function
    std::array<instruction, 256> *instructions = new std::array<instruction, 256>;
    //Some instructions may requiere an additional cycle in some cases
    int additionnal_cycles = 0;
};



#endif /* cpu_hpp */


