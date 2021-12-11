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
//    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

GRAPHICS::~GRAPHICS(){
//    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
