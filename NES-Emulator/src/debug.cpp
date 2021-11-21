//
//  debug.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 19/11/2021.
//
#include <chrono>
#include <thread>


#include <stdio.h>
#include <iostream>
#include <array>
#include <ncurses.h>
#include <string>
#include <sstream>

#include "../obj/debug.hpp"
#include "../obj/cpu.hpp"




#warning TODO comment
void show_ram(std::array<Byte, 2048> ram){
    std::stringstream buffer;
    for(int i = 0; i < 2048; i++){
        buffer << "0x" << std::hex << (int) ram[i] << ", ";
        if(i% 30 == 29) buffer << "\n";
    }
    addstr(buffer.str().c_str());
}

#warning TODO comment
void show_registers(nes::CPU cpu){
    std::stringstream buffer;
    buffer << "N = " << cpu.getflag(cpu.flags.N);
//           << "\nV = " << cpu.getflag(cpu.registers::V)
//           << "\nB = " << cpu.getflag(nes::CPU::registers::B)
//           << "\nD = " << cpu.getflag(nes::CPU::D)
//           << "\nI = " << cpu.getflag(nes::CPU::I)
//           << "\nZ = " << cpu.getflag(nes::CPU::Z)
//           << "\nC = " << cpu.getflag(nes::CPU::C);
    addstr(buffer.str().c_str());
}

#warning TODO comment
int main(){
    initscr();
    
    nes::CPU cpu;
    show_registers(cpu);
    
    
    /* Exemple 1 */
    std::array<Byte, 2048> *ram = new std::array<Byte, 2048>;
    ram->fill(13);
    show_ram(*ram);
    refresh();
    show_registers(cpu);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
//    int i = 0;
//    while(1) {
//        i++;
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//        move(0,0);
//        ram->fill(i);
//        show_ram(*ram);
//        refresh();
//    }
    /* End of exemple 1*/
    
//    addstr("\npress any key to exit...");
//
//    getch();

    
    

    return 0;
}
