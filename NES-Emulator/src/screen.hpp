//
//  screen.hpp
//  NES-Emulator
//
//  Created by Alexi Canesse on 09/12/2021.
//

#ifndef screen_hpp
#define screen_hpp

#include <SDL2/SDL.h>
#include <array>

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
    
    void DrawPixel(int x, int y, Color); //draw a pixel but do not update the screen
    void update(); //updte screen content
};

#endif /* screen_hpp */
