//
//  debug.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 19/11/2021.
//
#include <fstream>

#include "cpu.hpp"
#include "nes.hpp"
#include "debug.hpp"
#include "ppu.hpp"
#include "cartridge.hpp"
#include "screen.hpp"


//chrom
//void show_chrom(PPU ppu){
//    move(0,0);
//    std::stringstream buffer;
//    buffer << "\n\nchROM::\n";
//    for(int i = 0x100; i <= 0x1FFF; i++){
//        buffer << "0x" << std::hex << std::setw(2) << std::left << (int) ppu.read(i) << " ";
//        if(i%8 == 7)
//            buffer << "\n";
//    }
//    addstr(buffer.str().c_str());
//
//}


//Show ram value
void show_stack(std::array<Byte, 2048> ram){
    std::stringstream buffer;
    buffer << "\n\nStack:\n";
    for(int i = 0x100; i <= 0x1FF; i++){
        buffer << "0x" << std::hex << std::setw(2) << std::left << (int) ram[i] << " ";
        if(i%8 == 7)
            buffer << "\n";
    }
    addstr(buffer.str().c_str());
}

//Show the register flags statue
void show_registers(CPU cpu){
    std::stringstream buffer;
    buffer << "Registers:\n";
    buffer << "N = " << cpu.getflag(cpu.flags.N)
           << "\nV = " << cpu.getflag(cpu.flags.V)
           << "\nB = " << cpu.getflag(cpu.flags.B)
           << "\nD = " << cpu.getflag(cpu.flags.D)
           << "\nI = " << cpu.getflag(cpu.flags.I)
           << "\nZ = " << cpu.getflag(cpu.flags.Z)
           << "\nC = " << cpu.getflag(cpu.flags.C)
           << "\nA : 0x" << std::hex << (int) cpu.get_register_A()
           << "\nX : 0x" << std::hex << (int) cpu.get_register_X()
           << "\nY : 0x" << std::hex << (int) cpu.get_register_Y()
           << "\nStack pointer : 0x" << std::hex << (int) cpu.get_register_SP()
           << "\nProgram counter : 0x" << std::hex << (int) cpu.get_register_PC()
           << "\nCycles : " << std::dec << cpu.cycles;
    addstr(buffer.str().c_str());
}

//Show the ppu register statue
void show_ppu_register(PPU ppu){
    move(0,30);
    std::stringstream buffer;
    buffer << "PPU Registers:"; addstr(buffer.str().c_str()); move(1,30); buffer.str("");
    buffer << std::hex << "PPUCTRL:   0x" << (int) ppu.getPPUCTRL(); addstr(buffer.str().c_str()); move(2,30); buffer.str("");
    buffer << std::hex << "PPUMASK:   0x" << (int) ppu.getPPUMASK(); addstr(buffer.str().c_str()); move(3,30); buffer.str("");
    buffer << std::hex << "PPUSTATUS: 0x" << (int) ppu.getPPUSTATUS(); addstr(buffer.str().c_str()); move(4,30); buffer.str("");
    buffer << std::hex << "OAMADDR:   0x" << (int) ppu.getOAMADDR(); addstr(buffer.str().c_str()); move(5,30); buffer.str("");
    buffer << std::hex << "OAMDATA:   0x" << (int) ppu.getOAMDATA(); addstr(buffer.str().c_str()); move(6,30); buffer.str("");
    buffer << std::hex << "PPUSCROLL: 0x" << (int) ppu.getPPUSCROLL(); addstr(buffer.str().c_str()); move(7,30); buffer.str("");
    buffer << std::hex << "PPUADDR:   0x" << (int) ppu.getPPUADDR(); addstr(buffer.str().c_str()); move(8,30); buffer.str("");
    buffer << std::hex << "OAMDMA:    0x" << (int) ppu.getOAMDMA(); addstr(buffer.str().c_str());
}

//Show interrupts locations
void show_interrupts(NES nes){
    move(0,50);
    std::stringstream buffer;
    buffer << "Interupts :"; addstr(buffer.str().c_str()); move(1,50); buffer.str("");
    buffer << std::hex << "Reset:   0x" << (int) nes.read(0xFFFD) << (int) nes.read(0xFFFC); addstr(buffer.str().c_str()); move(2,50); buffer.str("");
    buffer << std::hex << "BRK:   0x" << (int) nes.read(0xFFFF) << (int) nes.read(0xFFFE); addstr(buffer.str().c_str()); move(3,50); buffer.str("");
    buffer << std::hex << "IRQ/NMI: 0x" << (int) nes.read(0xFFFB) << (int) nes.read(0xFFFA); addstr(buffer.str().c_str());
}

void show_position(PPU ppu){
    move(6,50);
    std::stringstream buffer;
    buffer << "PPU position:"; addstr(buffer.str().c_str()); move(7,50); buffer.str("");
    buffer << std::dec << "Scanline: " << ppu.getscanline();; addstr(buffer.str().c_str()); move(8,50); buffer.str("");
    buffer << std::dec << "Row: " << ppu.getrow(); addstr(buffer.str().c_str()); move(9,50); buffer.str("");
}

void show_state(CPU cpu, std::array<Byte, 2048> ram, PPU ppu){
    move(0,0);
    show_registers(cpu);
    show_stack(ram);
    show_ppu_register(ppu);
    show_interrupts(*cpu.nes);
    show_position(ppu);
//    show_chrom(ppu);
    refresh();
}

void logging(CPU cpu, Address pc, PPU ppu){
    std::ofstream log;
    log.open("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/nestest/lognesttest.log", std::ostream::app);
    log << "\n" << std::hex << std::setw(4) << std::left << (int) pc << "  CYC:" << std::dec << cpu.cycles << std::hex <<"  A:" << std::setw(2) << std::left << (int) cpu.get_register_A() << "  X:" << std::setw(2) << std::left << (int) cpu.get_register_X() << "  Y:" << std::setw(2) << std::left << (int) cpu.get_register_Y() <<  "  Stack:" << std::hex << (int) cpu.get_register_SP() << "  opcode: 0x" << std::setw(2) << std::left << (int) cpu.opcode << "  Addr:" << std::setw(4) << std::left << (int) cpu.data_to_read << "  Data: 0x" << std::setw(2) << std::left << (int) cpu.nes->read(cpu.data_to_read) << "  Flags (nv_bdizc):" << cpu.getflag(0x80) << cpu.getflag(0x40) << "_" << cpu.getflag(0x10) << cpu.getflag(0x08) << cpu.getflag(0x04) << cpu.getflag(0x02) << cpu.getflag(0x01) << "  Scanline:" << std::dec << std::setw(3) << std::left << cpu.nes->ppu->get_scanline() << "  Row:" << std::setw(3) << std::left << cpu.nes->ppu->get_row() << "  vram_addr:0x" <<std::hex  << std::setw(4) << std::left << (int) cpu.nes->ppu->vmem_addr << "  t_addr:0x" << std::setw(4) << std::left << cpu.nes->ppu->addr_t << "  ctrl:0x" << std::setw(2) << std::left << (int) ppu.getPPUCTRL() << "  mask:0x" << std::setw(2) << std::left << (int) ppu.getPPUMASK() << "  status:0x" << std::setw(2) << std::left << (int) ppu.r().PPUSTATUS << "  oamddr:0x" << std::setw(2) << std::left << (int) ppu.getOAMADDR() << "  oamdata:0x" << std::setw(2) << std::left << (int) ppu.getOAMDATA() << "  scroll:0x" << std::setw(2) << std::left << (int) ppu.getPPUSCROLL() << "  oamdma:0x" << std::setw(2) << std::left << (int) ppu.getOAMDMA();
    
#warning debug
//    log << " 0xFFFD 0xFFFC: " << std::hex << (int) ((cpu.nes->read(0xc109)));
    log.close();
}

int main(){
//    initscr();
    NES nes;
    CPU *cpu = nes.cpu;
    PPU *ppu = nes.ppu;
    CARTRIDGE *cartridge = nes.cartridge;
    
    Address old_pc = 0x0000;
    
    if(!cartridge->load("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/donkeykong.nes")) {
//    if(!cartridge->load("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/nestest/nestest.nes")) {
        std::cout << "AHHHHHHHHH";
        return 0;
    }
    std::ofstream log;
    cpu->reset();
    
    
    log.open("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/nestest/lognesttest.log", std::ifstream::trunc);
    log.close();
//    logging(*cpu);
    
    
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    int a = 0;
    while(1){

        ppu->clock();
        if(a %3 == 0){
            cpu->clock();
//            if(cpu->rem_cycles == 0){
//                logging(*cpu, old_pc, *ppu);
//                old_pc = cpu->get_register_PC();
//            }

//                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
//            show_state(*cpu, *nes.ram, *ppu);
//            refresh();
        }
        
//        logging(*cpu, old_pc, *ppu);
    }
    
    
    
    return 0;
}




