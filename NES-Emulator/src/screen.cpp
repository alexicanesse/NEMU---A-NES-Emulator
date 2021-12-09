//
//  screen.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 09/12/2021.
//

#include "screen.hpp"

/*
 Constructor and destructor
*/
GRAPHICS::GRAPHICS(int width, int height){
    SDL_Init(SDL_INIT_VIDEO);       // Initializing SDL as Video
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    
    /*
    define the palette
    https://wiki.nesdev.org/w/index.php/PPU_palettes#Palettes
    */
    this->palette->at(0x00).r = 84;
    this->palette->at(0x00).g = 84;
    this->palette->at(0x00).b = 84;
    
    this->palette->at(0x10).r = 152;
    this->palette->at(0x10).g = 150;
    this->palette->at(0x10).b = 152;
    
    this->palette->at(0x20).r = 236;
    this->palette->at(0x20).g = 238;
    this->palette->at(0x20).b = 236;
    
    this->palette->at(0x30).r = 236;
    this->palette->at(0x30).g = 238;
    this->palette->at(0x30).b = 236;
    
    this->palette->at(0x11).r = 0;
    this->palette->at(0x11).g = 30;
    this->palette->at(0x11).b = 116;
    
    this->palette->at(0x11).r = 8;
    this->palette->at(0x11).g = 76;
    this->palette->at(0x11).b = 196;
    
    this->palette->at(0x21).r = 76;
    this->palette->at(0x21).g = 154;
    this->palette->at(0x21).b = 236;
    
    this->palette->at(0x31).r = 168;
    this->palette->at(0x31).g = 204;
    this->palette->at(0x31).b = 236;
    
    this->palette->at(0x02).r = 8;
    this->palette->at(0x02).g = 16;
    this->palette->at(0x02).b = 144;
    
    this->palette->at(0x12).r = 48;
    this->palette->at(0x12).g = 50;
    this->palette->at(0x12).b = 236;
    
    this->palette->at(0x22).r = 120;
    this->palette->at(0x22).g = 124;
    this->palette->at(0x22).b = 236;
    
    this->palette->at(0x32).r = 188;
    this->palette->at(0x32).g = 188;
    this->palette->at(0x32).b = 236;
    
    this->palette->at(0x03).r = 48;
    this->palette->at(0x03).g = 0;
    this->palette->at(0x03).b = 136;
    
    this->palette->at(0x13).r = 92;
    this->palette->at(0x13).g = 30;
    this->palette->at(0x13).b = 228;
    
    this->palette->at(0x23).r = 176;
    this->palette->at(0x23).g = 98;
    this->palette->at(0x23).b = 236;
    
    this->palette->at(0x33).r = 212;
    this->palette->at(0x33).g = 178;
    this->palette->at(0x33).b = 236;
    
    this->palette->at(0x04).r = 68;
    this->palette->at(0x04).g = 0;
    this->palette->at(0x04).b = 100;
    
    this->palette->at(0x14).r = 136;
    this->palette->at(0x14).g = 20;
    this->palette->at(0x14).b = 176;

    this->palette->at(0x24).r = 228;
    this->palette->at(0x24).g = 84;
    this->palette->at(0x24).b = 236;
    
    this->palette->at(0x34).r = 236;
    this->palette->at(0x34).g = 174;
    this->palette->at(0x34).b = 236;
    
    this->palette->at(0x05).r = 92;
    this->palette->at(0x05).g = 0;
    this->palette->at(0x05).b = 48;
    
    this->palette->at(0x15).r = 160;
    this->palette->at(0x15).g = 20;
    this->palette->at(0x15).b = 100;

    this->palette->at(0x25).r = 236;
    this->palette->at(0x25).g = 88;
    this->palette->at(0x25).b = 180;
    
    this->palette->at(0x35).r = 236;
    this->palette->at(0x35).g = 174;
    this->palette->at(0x35).b = 212;
    
    this->palette->at(0x06).r = 84;
    this->palette->at(0x06).g = 4;
    this->palette->at(0x06).b = 0;
    
    this->palette->at(0x16).r = 152;
    this->palette->at(0x16).g = 34;
    this->palette->at(0x16).b = 32;

    this->palette->at(0x26).r = 236;
    this->palette->at(0x26).g = 106;
    this->palette->at(0x26).b = 100;
    
    this->palette->at(0x36).r = 236;
    this->palette->at(0x36).g = 180;
    this->palette->at(0x36).b = 176;
    
    this->palette->at(0x07).r = 60;
    this->palette->at(0x07).g = 24;
    this->palette->at(0x07).b = 0;
    
    this->palette->at(0x17).r = 120;
    this->palette->at(0x17).g = 60;
    this->palette->at(0x17).b = 0;

    this->palette->at(0x27).r = 212;
    this->palette->at(0x27).g = 136;
    this->palette->at(0x27).b = 32;
    
    this->palette->at(0x37).r = 228;
    this->palette->at(0x37).g = 196;
    this->palette->at(0x37).b = 144;
    
    this->palette->at(0x08).r = 32;
    this->palette->at(0x08).g = 42;
    this->palette->at(0x08).b = 0;
    
    this->palette->at(0x18).r = 84;
    this->palette->at(0x18).g = 90;
    this->palette->at(0x18).b = 0;
    
    this->palette->at(0x28).r = 160;
    this->palette->at(0x28).g = 170;
    this->palette->at(0x28).b = 0;
    
    this->palette->at(0x38).r = 204;
    this->palette->at(0x38).g = 210;
    this->palette->at(0x38).b = 120;
    
    this->palette->at(0x09).r = 8;
    this->palette->at(0x09).g = 58;
    this->palette->at(0x09).b = 0;
    
    this->palette->at(0x19).r = 40;
    this->palette->at(0x19).g = 114;
    this->palette->at(0x19).b = 0;
    
    this->palette->at(0x29).r = 116;
    this->palette->at(0x29).g = 196;
    this->palette->at(0x29).b = 0;
    
    this->palette->at(0x39).r = 180;
    this->palette->at(0x39).g = 222;
    this->palette->at(0x39).b = 120;
    
    this->palette->at(0x0A).r = 0;
    this->palette->at(0x0A).g = 64;
    this->palette->at(0x0A).b = 0;
    
    this->palette->at(0x1A).r = 8;
    this->palette->at(0x1A).g = 124;
    this->palette->at(0x1A).b = 0;
    
    this->palette->at(0x2A).r = 76;
    this->palette->at(0x2A).g = 208;
    this->palette->at(0x2A).b = 32;
    
    this->palette->at(0x3A).r = 168;
    this->palette->at(0x3A).g = 226;
    this->palette->at(0x3A).b = 144;
    
    this->palette->at(0x0B).r = 0;
    this->palette->at(0x0B).g = 60;
    this->palette->at(0x0B).b = 0;
    
    this->palette->at(0x1B).r = 0;
    this->palette->at(0x1B).g = 118;
    this->palette->at(0x1B).b = 40;
    
    this->palette->at(0x2B).r = 56;
    this->palette->at(0x2B).g = 204;
    this->palette->at(0x2B).b = 108;
    
    this->palette->at(0x3B).r = 152;
    this->palette->at(0x3B).g = 226;
    this->palette->at(0x3B).b = 180;
    
    this->palette->at(0x0C).r = 0;
    this->palette->at(0x0C).g = 50;
    this->palette->at(0x0C).b = 60;
    
    this->palette->at(0x1C).r = 0;
    this->palette->at(0x1C).g = 102;
    this->palette->at(0x1C).b = 120;
    
    this->palette->at(0x2C).r = 56;
    this->palette->at(0x2C).g = 180;
    this->palette->at(0x2C).b = 204;
    
    this->palette->at(0x3C).r = 160;
    this->palette->at(0x3C).g = 214;
    this->palette->at(0x3C).b = 228;
    
    this->palette->at(0x0D).r = 0;
    this->palette->at(0x0D).g = 0;
    this->palette->at(0x0D).b = 0;
    
    this->palette->at(0x1D).r = 0;
    this->palette->at(0x1D).g = 0;
    this->palette->at(0x1D).b = 0;
    
    this->palette->at(0x2D).r = 60;
    this->palette->at(0x2D).g = 60;
    this->palette->at(0x2D).b = 60;
    
    this->palette->at(0x3D).r = 160;
    this->palette->at(0x3D).g = 162;
    this->palette->at(0x3D).b = 160;
    
    this->palette->at(0x0E).r = 0;
    this->palette->at(0x0E).g = 0;
    this->palette->at(0x0E).b = 0;
    
    this->palette->at(0x1E).r = 0;
    this->palette->at(0x1E).g = 0;
    this->palette->at(0x1E).b = 0;
    
    this->palette->at(0x2E).r = 0;
    this->palette->at(0x2E).g = 0;
    this->palette->at(0x2E).b = 0;
    
    this->palette->at(0x3E).r = 0;
    this->palette->at(0x3E).g = 0;
    this->palette->at(0x3E).b = 0;
    
    this->palette->at(0x0F).r = 0;
    this->palette->at(0x0F).g = 0;
    this->palette->at(0x0F).b = 0;
    
    this->palette->at(0x1F).r = 0;
    this->palette->at(0x1F).g = 0;
    this->palette->at(0x1F).b = 0;
    
    this->palette->at(0x2F).r = 0;
    this->palette->at(0x2F).g = 0;
    this->palette->at(0x2F).b = 0;
    
    this->palette->at(0x3F).r = 0;
    this->palette->at(0x3F).g = 0;
    this->palette->at(0x3F).b = 0;
}

GRAPHICS::~GRAPHICS(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}




void GRAPHICS::DrawPixel(int x, int y, Color c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.o);
    SDL_RenderDrawPoint(renderer, 2*x, 2*y);
    SDL_RenderDrawPoint(renderer, 2*x, 2*y + 1);
    SDL_RenderDrawPoint(renderer, 2*x + 1, 2*y);
    SDL_RenderDrawPoint(renderer, 2*x + 1, 2*y + 1);
}

void GRAPHICS::update(){
    SDL_RenderPresent(renderer);
}
