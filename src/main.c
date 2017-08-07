#include "main.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
char *path;
struct program  file;

SDL_Event e;

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "*** Unable to init SDL: %s ***\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);
    window = SDL_CreateWindow("Chip 8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                               SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(renderer, SCREEN_X, SCREEN_Y);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, SCREEN_X, SCREEN_Y);
    if (argc >= 2)
        path = argv[1];
    Run();
}

void drawScreen(_Bool screen[]) {
    uint8_t pixels[SCREEN_X * SCREEN_Y];
    for (int i = 0; i < SCREEN_X * SCREEN_Y; i++) {
        if (screen[i]){
            pixels[i] = 255;
        }else{
            pixels[i] = 0;

        }
    }

    SDL_UpdateTexture(texture, NULL, pixels, SCREEN_X * sizeof(uint8_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

}

uint16_t getKey() {
    while (SDL_PollEvent(&e) != 0) {
        //User requests quit
        if (e.type == SDL_QUIT) {
            SDL_Quit();

            exit(0);
        } else if (e.type == SDL_KEYDOWN) {

            switch (e.key.keysym.sym) {
                case SDLK_1:
                    return 0x1;
                case SDLK_2:
                    return 0x2;
                case SDLK_3:
                    return 0x3;
                case SDLK_4:
                    return 0xC;
                case SDLK_q:
                    return 0x4;
                case SDLK_w:
                    return 0x5;
                case SDLK_e:
                    return 0x6;
                case SDLK_r:
                    return 0xD;
                case SDLK_a:
                    return 0x7;
                case SDLK_s:
                    return 0x8;
                case SDLK_d:
                    return 0x9;
                case SDLK_f:
                    return 0xE;
                case SDLK_z:
                    return 0xA;
                case SDLK_x:
                    return 0;
                case SDLK_c:
                    return 0xB;
                case SDLK_v:
                    return 0xF;
                default:
                    return 0x10;
            }
        }
    }
    return 0x10;

}

void Beep() {
    printf("Beep!");
}

struct program * getFile() {
    FILE *fileptr;
    uint8_t *buffer;
    long filelen;

    fileptr = fopen(path, "rb");  // Open the file in binary mode
    if(fileptr == NULL)
        exit(3);
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (uint8_t *)malloc((filelen)*sizeof(uint8_t)); // Enough memory for file
    fread(buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file
    file.length =filelen;
    file.file = buffer;
    return &file;
}