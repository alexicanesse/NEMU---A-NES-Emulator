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

void show_state(CPU cpu, std::array<Byte, 2048> ram){
    move(0,0);
    show_registers(cpu);
    show_ram(ram);
    
    refresh();
}

void logging(CPU cpu){
    std::ofstream log;
    log.open("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/nestest/lognesttest.log", std::ostream::app);
    log << "\n" << std::hex << cpu.get_register_PC() << "  CYC:" << std::dec << cpu.cycles << std::hex << "  A:" << (int) cpu.get_register_A() << "  X:" << (int) cpu.get_register_X() << "  Y:" << (int) cpu.get_register_Y() << " Stack:" << std::hex << (int) cpu.get_register_SP();
    
#warning debug of debug
    log << " " << std::dec << (int) cpu.get_register_SP();
    log.close();
}

int main(){
    initscr();
    CPU cpu;

    /* Exemple 1 */
//    std::array<Byte, 2048> *ram = new std::array<Byte, 2048>;
//    ram->fill(13);
//    show_ram(*ram);
//    refresh();
//    show_registers(cpu);
//    std::this_thread::sleep_for(std::chrono::milliseconds(50));
//
//
//    int i = 0;
//    while(1) {
//        i++;
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//        ram->fill(i);
//        show_state(cpu, *ram);
//    }
    /* End of exemple 1*/
    
    
    /* Exemple 2 */
    
    std::ifstream testrom;
    std::ofstream log;
    testrom.open("/Users/alexicanesse/Documents/prog/nes/NES-Emulator/NES-Emulator/tests/nestest/nestest.nes", std::ios::binary);
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
            cpu.nes.write(pos, (Byte) buffer);
            cpu.nes.write(pos1, (Byte) buffer);
//            if(pos2 < 2048) cpu.nes.write(pos2, (Byte) buffer);
            pos++;
            pos1++;
//            pos2++;
            if (testrom.eof())
                break;
            show_state(cpu, *cpu.nes.ram);
            refresh();
//            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }
    }
    
    logging(cpu);
    
    while(1){
        cpu.clock();
        show_state(cpu, *cpu.nes.ram);
        refresh();
        if(cpu.rem_cycles == 0){
            logging(cpu);
        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    testrom.close();
    /* End of exemple 2*/
    
//    addstr("\npress any key to exit...");
//
//    getch();
    

    return 0;
}

