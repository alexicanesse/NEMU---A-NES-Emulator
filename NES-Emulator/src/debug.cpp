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
    log << "\n" << std::hex << cpu.get_register_PC() << "  CYC:" << std::dec << cpu.cycles << std::hex << "  A:" << (int) cpu.get_register_A() << "  X:" << (int) cpu.get_register_X() << "  Y:" << (int) cpu.get_register_Y() << "  Stack:" << std::hex << (int) cpu.get_register_SP();
    log.close();
}

int main(){
    initscr();
    NES nes;
    CPU *cpu = nes.cpu;
    PPU *ppu = nes.ppu;
    
    
    /* Exemple */
    
    std::ifstream testrom;
    std::ofstream log;
    testrom.open("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/color_test.nes", std::ios::binary);
//    testrom.open("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/nestest/nestest.nes", std::ios::binary);
    log.open("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/nestest/lognesttest.log", std::ifstream::trunc);
    log.close();
    if(testrom.fail() | log.fail())
        std::cout << "pb";
    else{
        char buffer = 0x00;
        Address pos = 0x8000;
        Address pos1 = 0xC000;
//        Address pos2 = 0x0000;
        for(int i = 0; i < 0x0010; i++) testrom.read(&buffer,1); //skip header
        while (pos < (0xBFFF)) {
            testrom.read(&buffer,1);
            cpu->nes->write(pos, (Byte) buffer);
            cpu->nes->write(pos1, (Byte) buffer);
//            if(pos2 < 2048) cpu.nes.write(pos2, (Byte) buffer);
            pos++;
            pos1++;
//            pos2++;
            if (testrom.eof())
                break;
            show_state(*cpu, *nes.ram, *ppu);
            refresh();
//            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }
    }
    
    logging(*cpu);
    
    while(1){
        cpu->clock();
        show_state(*cpu, *nes.ram, *ppu);
        refresh();
        if(cpu->rem_cycles == 0){
            logging(*cpu);
        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    testrom.close();
    /* End of exemple */
    
    

    return 0;
}

