#include <stdio.h>
#include <stdint.h>

#include <stdlib.h>
#include <SDL2/SDL.h>


#define SCREEN_X 64
#define SCREEN_Y 32

typedef struct program {
    long length;
    uint8_t * file;
}program;
struct program* getFile();
void Run();
void drawScreen(_Bool pixels[]);
void Beep();
uint16_t getKey();
