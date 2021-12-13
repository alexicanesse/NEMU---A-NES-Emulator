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




//Show ram value
void show_ram(std::array<Byte, 2048> ram){
    std::stringstream buffer;
    buffer << "\n\nRam:\n";
    for(int i = 0; i < 2048; i++){
        buffer << "0x" << std::hex << (int) ram[i] << ", ";
        if(i% 30 == 29) buffer << "\n";
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
    buffer << std::hex << "PPUDATA:   0x" << (int) ppu.getPPUDATA(); addstr(buffer.str().c_str()); move(9,30); buffer.str("");
    buffer << std::hex << "OAMDMA:    0x" << (int) ppu.getOAMDMA(); addstr(buffer.str().c_str());
}

void show_state(CPU cpu, std::array<Byte, 2048> ram, PPU ppu){
    move(0,0);
    show_registers(cpu);
    show_ram(ram);
    show_ppu_register(ppu);
    refresh();
}

void logging(CPU cpu){
    std::ofstream log;
    log.open("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/nestest/lognesttest.log", std::ostream::app);
    log << "\n" << std::hex << cpu.get_register_PC() << "  CYC:" << std::dec << cpu.cycles << std::hex << "  A:" << (int) cpu.get_register_A() << "  X:" << (int) cpu.get_register_X() << "  Y:" << (int) cpu.get_register_Y() << "  Stack:" << std::hex << (int) cpu.get_register_SP() << "  opcode: 0x" << (int) cpu.opcode;
    
#warning debug
//    log << " 0xFFFD 0xFFFC: " << std::hex << (int) ((cpu.nes->read(0xFFFD) << 8) + cpu.nes->read(0xFFFC));
    log.close();
}

int main(){
//    initscr();
    NES nes;
    CPU *cpu = nes.cpu;
    PPU *ppu = nes.ppu;
    CARTRIDGE *cartridge = nes.cartridge;
    
    
    if(!cartridge->load("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/donkeykong.nes")) {
//    if(!cartridge->load("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/nestest/nestest.nes")) {
        std::cout << "AHHHHHHHHH";
        return 0;
    }
    std::ofstream log;

    log.open("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/nestest/lognesttest.log", std::ifstream::trunc);
    log.close();
    logging(*cpu);

    cpu->reset();
    logging(*cpu);
//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        for(int x= 0; x<500000; x++){
            for(int y = 0; y <1; y++){

                ppu->clock();
                if((x+y) %3 == 0){
                    cpu->clock();
//                    show_state(*cpu, *nes.ram, *ppu);
//                    refresh();
//                    if(cpu->rem_cycles == 0){
//                        logging(*cpu);
//                    }
//                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
            }
    }
    
    
    
    return 0;
}




