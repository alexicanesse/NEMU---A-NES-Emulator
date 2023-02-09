//
//  debug.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 19/11/2021.
//
#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <iomanip>
#include <ncurses.h>

#include "cpu.hpp"
#include "nes.hpp"
#include "debug.hpp"
#include "ppu.hpp"
#include "cartridge.hpp"
#include "screen.hpp"



//Show stack content
void Debugger::show_stack(){
    std::stringstream buffer;
    buffer << "\n\nStack:\n";
    for(int i = 0x100; i <= 0x1FF; i++){
        buffer << "0x" << std::hex << std::setw(2) << std::left << (int) this->nes->ram->at(i) << " ";
        if(i%8 == 7)
            buffer << "\n";
    }
    addstr(buffer.str().c_str());
}

//Show the register flags statue
void Debugger::show_registers(){
    std::stringstream buffer;
    buffer << "Registers:\n";
    buffer << "N = " << cpu->getflag(cpu->flags.N)
           << "\nV = " << cpu->getflag(cpu->flags.V)
           << "\nB = " << cpu->getflag(cpu->flags.B)
           << "\nD = " << cpu->getflag(cpu->flags.D)
           << "\nI = " << cpu->getflag(cpu->flags.I)
           << "\nZ = " << cpu->getflag(cpu->flags.Z)
           << "\nC = " << cpu->getflag(cpu->flags.C)
           << "\nA : 0x" << std::hex << (int) cpu->registers.r_A
           << "\nX : 0x" << std::hex << (int) cpu->registers.r_iX
           << "\nY : 0x" << std::hex << (int) cpu->registers.r_iY
           << "\nStack pointer : 0x" << std::hex << (int) cpu->registers.r_SP
           << "\nProgram counter : 0x" << std::hex << (int) cpu->registers.r_PC
           << "\nCycles : " << std::dec << cpu->cycles
           << "\nOPcode : " << std::hex << (int) cpu->opcode;
    addstr(buffer.str().c_str());
}

//Show the ppu register statue
void Debugger::show_ppu_register(){
    move(0,30);
    std::stringstream buffer;
    buffer << "PPU Registers:"; addstr(buffer.str().c_str()); move(1,30); buffer.str("");
    buffer << std::hex << "PPUCTRL:   0x" << (int) ppu->getPPUCTRL();   addstr(buffer.str().c_str()); move(2,30); buffer.str("");
    buffer << std::hex << "PPUMASK:   0x" << (int) ppu->getPPUMASK();   addstr(buffer.str().c_str()); move(3,30); buffer.str("");
    buffer << std::hex << "PPUSTATUS: 0x" << (int) ppu->getPPUSTATUS(); addstr(buffer.str().c_str()); move(4,30); buffer.str("");
    buffer << std::hex << "OAMADDR:   0x" << (int) ppu->getOAMADDR();   addstr(buffer.str().c_str()); move(5,30); buffer.str("");
    buffer << std::hex << "OAMDATA:   0x" << (int) ppu->getOAMDATA();   addstr(buffer.str().c_str()); move(6,30); buffer.str("");
    buffer << std::hex << "PPUSCROLL: 0x" << (int) ppu->getPPUSCROLL(); addstr(buffer.str().c_str()); move(7,30); buffer.str("");
    buffer << std::hex << "PPUADDR:   0x" << (int) ppu->getPPUADDR();   addstr(buffer.str().c_str()); move(8,30); buffer.str("");
    buffer << std::hex << "OAMDMA:    0x" << (int) ppu->getOAMDMA();    addstr(buffer.str().c_str());
}

//Show interrupts locations
void Debugger::show_interrupts(){
    move(0,50);
    std::stringstream buffer;
    buffer << "Interupts :"; addstr(buffer.str().c_str()); move(1,50); buffer.str("");
    buffer << std::hex << "Reset:   0x" << (int) (nes->read(0xFFFD) << 8 | nes->read(0xFFFC)); addstr(buffer.str().c_str()); move(2,50); buffer.str("");
    buffer << std::hex << "BRK:   0x"   << (int) (nes->read(0xFFFF) << 8 | nes->read(0xFFFE)); addstr(buffer.str().c_str()); move(3,50); buffer.str("");
    buffer << std::hex << "IRQ/NMI: 0x" << (int) (nes->read(0xFFFB) << 8 | nes->read(0xFFFA)); addstr(buffer.str().c_str());
}

void Debugger::show_position(){
    move(6,50);
    std::stringstream buffer;
    buffer << "PPU position:"; addstr(buffer.str().c_str()); move(7,50); buffer.str("");
    buffer << std::dec << "Scanline: " << ppu->get_scanline(); addstr(buffer.str().c_str()); move(8,50); buffer.str("");
    buffer << std::dec << "Cycle: "    << ppu->get_cycle();     addstr(buffer.str().c_str()); move(9,50); buffer.str("");
}

void Debugger::show_state(){
    move(0,0);
    show_registers();
    show_stack();
    show_ppu_register();
    show_interrupts();
    show_position();
    refresh();
}

void Debugger::logging(){
    std::ofstream log;
    log.open("./log.log", std::ostream::app);
    
    log << "\n" << std::hex << std::setw(4) << std::left << (int) this->cpu->get_register_PC()
        << "  CYC:" << std::dec << cpu->cycles << std::hex
        << "  A:" << std::setw(2) << std::left << (int) cpu->registers.r_A
        << "  X:" << std::setw(2) << std::left << (int) cpu->registers.r_iX
        << "  Y:" << std::setw(2) << std::left << (int) cpu->registers.r_iY
        <<  "  Stack:" << std::hex << (int) cpu->registers.r_SP
        << "  opcode: 0x" << std::setw(2) << std::left << (int) cpu->opcode
        << "  Addr:" << std::setw(4) << std::left << (int) cpu->data_to_read
        << "  Data: 0x" << std::setw(2) << std::left << (int) nes->read(cpu->data_to_read)
        << "  Flags (nv_bdizc):" << cpu->getflag(0x80)
                                 << cpu->getflag(0x40)
                                 << "_"
                                 << cpu->getflag(0x10)
                                 << cpu->getflag(0x08)
                                 << cpu->getflag(0x04)
                                 << cpu->getflag(0x02)
                                 << cpu->getflag(0x01)
        << "  Scanline:" << std::dec << std::setw(3) << std::left << ppu->get_scanline()
        << "  Row:" << std::setw(3) << std::left << ppu->get_cycle()
        << "  vram_addr:0x" <<std::hex  << std::setw(4) << std::left << (int) ppu->vmem_addr
        << "  t_addr:0x" << std::setw(4) << std::left << ppu->addr_t
        << "  ctrl:0x" << std::setw(2) << std::left << (int) ppu->getPPUCTRL()
        << "  mask:0x" << std::setw(2) << std::left << (int) ppu->getPPUMASK()
        << "  status:0x" << std::setw(2) << std::left << (int) ppu->r().PPUSTATUS
        << "  oamddr:0x" << std::setw(2) << std::left << (int) ppu->getOAMADDR()
        << "  oamdata:0x" << std::setw(2) << std::left << (int) ppu->getOAMDATA()
        << "  scroll:0x" << std::setw(2) << std::left << (int) ppu->getPPUSCROLL()
        << "  oamdma:0x" << std::setw(2) << std::left << (int) ppu->getOAMDMA();
    log.close();
}


Debugger::Debugger(NES *nes, CPU *cpu, PPU *ppu){
    this->nes = nes;
    this->cpu = cpu;
    this->ppu = ppu;
    
    initscr();
    
    log.open("./log.log", std::ifstream::trunc);
    log.close();
    logging();
}
