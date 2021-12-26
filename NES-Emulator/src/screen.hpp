//
//  screen.hpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 09/12/2021.
//

#ifndef screen_hpp
#define screen_hpp

//I know it's a weird way to include SDL but it's the only way I found to use SDL and CMake together
#include "SDL.h"
#include <array>
#include <sstream>>


class GRAPHICS{
private:
        int height;     // Height of the window
        int width;      // Width of the window
        SDL_Renderer *renderer = NULL;      // Pointer to the renderer
        SDL_Window *window = NULL;      // Pointer to the window
public:
    /*
     Constructor and destructor
    */
    GRAPHICS(int width, int height);
    ~GRAPHICS();
    
    
    struct Color{
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t o = 255; //opacity
    };
    
    void update(); //updte screen content
    
    void ChangeTitle(const char *);
    void DrawPixel(int x, int y, Color);
};

#endif /* screen_hpp */
