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

//#include "nes.hpp"

namespace nes {

typedef uint8_t Byte;
typedef uint16_t Address;

class CPU{

    

    
private:
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
