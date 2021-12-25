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
#warning TODO curstom size
    SDL_RenderSetScale(renderer, 4. * 8/7,4);
}

GRAPHICS::~GRAPHICS(){
//    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
