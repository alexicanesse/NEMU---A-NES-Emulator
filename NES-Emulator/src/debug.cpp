//
//  debug.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 19/11/2021.
//
#include <chrono>
#include <thread>


#include <iostream>
#include <array>
#include <ncurses.h>
#include <string>
#include <sstream>

#include "debug.hpp"
#include "cpu.hpp"



//Show ram value
void show_ram(std::array<Byte, 2048> ram){
    std::stringstream buffer;
    buffer << "\n\n Ram:\n";
    for(int i = 0; i < 2048; i++){
        buffer << "0x" << std::hex << (int) ram[i] << ", ";
        if(i% 30 == 29) buffer << "\n";
    }
    addstr(buffer.str().c_str());
}

//Show the register flags statue
void show_registers(nes::CPU cpu){
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
           << "\nProgram counter : 0x" << std::hex << (int) cpu.get_register_PC();
    addstr(buffer.str().c_str());
}

void show_state(nes::CPU cpu, std::array<Byte, 2048> ram){
    move(0,0);
    
    show_registers(cpu);
    show_ram(ram);
    
    refresh();
}

int main(){
    initscr();
    nes::CPU cpu;

    /* Exemple 1 */
    std::array<Byte, 2048> *ram = new std::array<Byte, 2048>;
    ram->fill(13);
    show_ram(*ram);
    refresh();
    show_registers(cpu);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    
    int i = 0;
    while(1) {
        i++;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ram->fill(i);
        show_state(cpu, *ram);
    }
    /* End of exemple 1*/
    
//    addstr("\npress any key to exit...");
//
//    getch();


    return 0;
}

