//
//  cpu.hpp
//  Proj1
//
//  Created by Alexi Canesse on 17/11/2021.
//

#ifndef cpu_hpp
#define cpu_hpp

#include <stdio.h>
#include <stdint.h>

#include "nes.hpp"

namespace nes {

typedef uint8_t Byte;
typedef uint16_t Address;

class CPU{
public:
    /*
     Registers
    */
    enum flags {
        N = 0b10000000, //negative result | After most instructions that have a value result, this flag will contain bit 7 of that result.
        V = 0b01000000, //overflow
        B = 0b00010000, //break command | No CPU effect
        D = 0b00001000, //decimal mode | On the NES, this flag has no effect
        I = 0b00000100, //interrupt disable | When set, all interrupts except the NMI are inhibited.
        Z = 0b00000010, //zero result | After most instructions that have a value result, if that value is zero, this flag will be set.
        C = 0b00000001, //carry
    } flags;
    typedef enum flags flag;
    
    void setflag(flag, bool);
    void getflag(flag);
    
    #warning TODO
    bool reset();
    

    
private:
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
        
};

}

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
