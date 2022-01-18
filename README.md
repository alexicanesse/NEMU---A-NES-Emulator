# NES-Emulator


##Compilation:
```sh
cmake -G "Unix Makefiles" -S ./
make
```

##How to run ?
```sh
./NES_Emulator --rom *file_path_to_the_nes_file* 
```

##Prerequists:
I used the librairies ncurses, sdl2 and boost. Make sure you've installed them all before trying to compile.


##Disclamer:
* A huge part of my comment has been copy-pasted from the NES dev wiki and pagetable.com. I did so because those comments do not explain how to understand *my* implementation but how to understand the hardware I'm trying to emulate. I see them as *proof* that I'm doing what I'm supposed to do.
* Donkey Kong runs fine (at a steady 60fps) on my 2019 MacBook Pro 16gb. Though, it runs at 8fps on my friend's Arch 5.16.0-arch1-1. I do not know why and I did not invastigated this issue. 


##Functionnalities:
* Runs NES roms (only mapper 0)
* Debugger: 
    * Prints the registers data
    * Prints the stack 
    * Prints the program counter linked to the interrupts
    * Prints the location of the screen where the ppu currently is 
    * Runs step by step (press any key to continue)
    * Save logs 



##Keymap:
* left   : q
* up     : z
* down   : s
* right  : d
* select : g
* start  : h
* a      : l
* b      : m



## Implementation choices:
I decided to declare `class Debugger` as a friend of many composant's classes because I did not want to create functions to access members that are fundamently private (eg. registers)  
My implementation is NOT cycle accurate: the CPU runs a full instruction at a time. It does not really matter. Only tricky to emulate game may have issue. Nonetheless, this implementation was never meant to be a perfect emulator, I wanted to make a minimalist working emulator all along .  





##How to read the code:
I recommand that you read all parts in the order they've been writen. eg. read all adressing mode's implementations from top to bottom because my comments have been written assuming they would be read in that order.
The main function (which is located at the end of the nes.cpp file) and the two loops linked to it should be read first, then the cpu class and then the ppu class (which where weird things append). 

**How to read the CPU class:**
1. Go through the hpp file to have in mind it's simple structure.
2. Read (and understand!) cycle();
3. Chose an opcode there: https://www.pagetable.com/c64ref/6502/?tab=2 and go through cycle() and read the functions it would call it the opcode fetched were the one you choosen 
4. Now that you know how the CPU works, have fun reading all instructions :)
5. Read the interupts. They're farly easy to understand when you understood how the cpu emulation works

**How to read the PPU class:**
Spend your whole life to understand how this weird piece of hardware works. Try not to kill yourself while you do not understand and then go through the ppu class starting from the clock function. 

