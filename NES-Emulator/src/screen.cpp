//
//  screen.cpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 09/12/2021.
//

//I know it's a weird way to include SDL but it's the only way I found to use SDL and CMake together
#include "SDL.h"

#include "screen.hpp"

/*
 Constructor and destructor
*/
GRAPHICS::GRAPHICS(float coef){
    SDL_Init(SDL_INIT_VIDEO);       // Initializing SDL as Video
    SDL_CreateWindowAndRenderer(coef * 320, coef * 240, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, coef * 8/7, coef);
    //https://wiki.nesdev.org/w/index.php?title=Overscan explains why the horizontal coefficient is multiplied by 8/7
}

GRAPHICS::~GRAPHICS(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}




void GRAPHICS::DrawPixel(int x, int y, Color c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.o);
    SDL_RenderDrawPoint(renderer, x + 12, y); //offset beacause the nes add borders to get a 280*240 image from a 256*240 image
}

void GRAPHICS::update(){
    SDL_RenderPresent(renderer);
}

void GRAPHICS::ChangeTitle(const char *s){
    SDL_SetWindowTitle(window, s);
}
