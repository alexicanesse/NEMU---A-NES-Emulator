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


#warning TODO
//There are two ways to emulate the pixel aspect ratio of the NES: scale before padding and pad before scaling. The NES PPU hardware performs the padding first, adding 24 pixels of border to form a 280x240 pixel picture that can be resized to 320x240, 640x480, or 960x720 square pixels, or to 352x240 or 704x480 if your SDTV output circuit produces non-square pixels at 13.5 MHz (Rec. 601/DVD dot clock, 132/35*colorburst) or 13.423 MHz (PlayStation dot clock, 15/4*colorburst). But as a slight optimization, you can scale first (256 * 8/7 = 292) and then pad: stretch the 256x240 pixels to 292x240, 584x480, 876x720, or 1168x960 square pixels or 320x240 or 640x480 non-square pixels. Or when enlarging 4.5x to fit a 1080p screen, output 256*(1080/240)*(8/7) = 1316 pixels. Then you can emulate the overscan by drawing a TV bezel on top of the edges of the emulated picture.


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
    
//    void DrawPixel(int x, int y, Color); //draw a pixel but do not update the screen
    void update(); //updte screen content
    
    
    
#warning TEST
    void DrawPixel(int x, int y, Color);
};

#endif /* screen_hpp */
