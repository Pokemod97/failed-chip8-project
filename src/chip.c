#include "chip.h"

// memory
uint8_t memory[4096];
//Registers
//general
uint8_t V[16];
// memory address register
uint16_t I;
// program counter
uint16_t pc;

uint16_t stack[16];
//Stack pointer
uint8_t sp;

//Timers
uint8_t delay = 0;
uint8_t sound = 0;


//screen
_Bool screen[SCREEN_X * SCREEN_Y];
// Draw flag
_Bool draw = 0;
//Font
uint8_t chip8_fontset[80] =
        {
                0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                0x20, 0x60, 0x20, 0x20, 0x70, // 1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

_Bool  quit = 0;

void Run() {
    initChip(getFile());
    while (1) {
        if(pc > 4096)
            pc = 0x200;
        runOpcode();
        if (delay > 0)
            delay--;
        if (sound > 0) {
            Beep();
            sound--;
        }

            if(draw) {
                drawScreen(screen);
                draw = 0;
            }

    }

}

void initChip( program* file) {
    // insert chip 8 font set
    for (int i = 0; i < 80; i++) {
        memory[i] = chip8_fontset[i];
    }
    // load program into memory
    for (int i = 0; i < file->length; i++) {
        memory[i + 0x200] = file->file[i];
    }
    //free(file);
    pc = 0x200;
    srand((unsigned) time(NULL));
}

void runOpcode() {
    uint16_t opcode = memory[pc] << 8 | memory[pc + 1];
    uint8_t x = getRegister(opcode, 1);
    uint8_t y = getRegister(opcode, 2);
    uint16_t key = getKey();
    printf("%04X\n", opcode);
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                // CLS
                case 0x00E0:
                    clearDisplay();
                    break;
                    //RET
                case 0x00EE:
                    setPc(stack[sp] & 0x0FFF);
                    sp--;
                    break;
                case 0x0000:
                    quit =1;
                    break;

            }
            break;
            //JP addr
        case 0x1000:
            setPc(opcode & 0x0FFF);
            break;
            //CALL addr
        case 0x2000:
            sp++;
            stack[sp] = (opcode & 0x0FFF);
            setPc(stack[sp]);
            break;
            //SE Vx, byte
        case 0x3000:
            // if( register == byte) skip next instruction
            if (V[x] == (opcode & 0x00FF))
                pc += 2;
            break;
            //SNE Vx, byte
        case 0x4000:
            // if(register != byte) skip next instruction
            if (V[x] != (opcode & 0x00FF))
                pc += 2;
            break;
            //SE Vx, Vy
        case 0x5000:
            //if(register == register2) skip next instruction
            if (V[x] == V[y])
                pc += 2;
            break;
            //LD Vx, byte
        case 0x6000:
            V[x] = opcode & 0x00FF;
            break;
            // ADD Vx, byte
        case 0x7000:
            V[x] += opcode & 0x00FF;
            break;
            // Register operations
            //empty expression to allow for creation of variables
        case 0x8000:

            switch (opcode & 0x000F) {

                //LD Vx, Vy
                case 0x0:
                    V[x] = V[y];
                    break;
                    //OR Vx, Vy
                case 0x1:
                    V[x] = V[x] | V[y];
                    break;
                    // And Vx, Vy
                case 0x2:
                    V[x] = V[x] & V[y];
                    break;
                    // Xor Vx, Vy
                case 0x3:
                    V[x] = V[x] ^ V[y];
                    break;
                    // Add Vx, Vy
                case 0x4:
                    if (V[x] + V[y] > 255)
                        V[0xF] = 1;
                    V[x] += V[y];
                    break;
                    // Sub Vx, Vy
                case 0x5:
                    if (V[x] > V[y])
                        V[0xF] = 1;
                    V[x] -= V[y];
                    //SHR Vx {, Vy}
                case 0x6:
                    if (V[y] & 1)
                        V[0xF] = 1;
                    V[x] = V[y] >> 1;
                    break;
                    // SUBN Vx, Vy
                case 0x7:
                    V[x] = V[y] - V[x];
                    break;
                    //SHL Vx {, Vy}
                case 0xE:
                    if ((V[y] << 1) & 1)
                        V[0xF] = 1;
                    V[x] = V[y] << 1;
                    break;
            }
            //SNE
        case 0x9000:
            if (V[x] != V[y])
                pc += 1;
            break;
            //Ld I, Addr
        case 0xA000:
            I = (opcode & 0x0FFF);
            break;
            // JP V0, addr
        case 0xB000:
            setPc((opcode & 0x0FFF) + V[0]);
            break;
            //RND Vx, byte
        case 0xC000:
            V[x] = (rand() % 256) & (opcode & 0x00FF);
            break;
            // DRW Vx, Vy, nibble
        case 0xD000:
            drawSprite(x, y, opcode & 0x000F);
            break;
            //skips
        case 0xE000:
            switch (opcode & 0x00FF) {
                //SKP Vx
                case 0x9E:
                    if (key == V[x])
                        pc += 2;
                    break;
                    //SKNP Vx
                case 0xA1:
                    if (key != V[x])
                        pc += 2;
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {

                //LD Vx, DT
                case 0x07:
                    V[x] = delay;
                    break;
                    // Fx0A - LD Vx, K
                case 0x0A:


                    if (key != 0x10) {
                        V[x] = key;
                    } else {
                        pc -= 2;
                    }
                    break;
                    // LD DT, Vx
                case 0x15:
                    delay = V[x];
                    break;
                    // LD ST, Vx
                case 0x18:
                    sound = V[x];
                    break;
                    //ADD I, Vx
                case 0x1E:
                    I += V[x];
                    break;
                    //LD F, Vx
                case 0x29:
                    I = memory[(V[x] * 4)];
                    break;
                case 0x33:
                    memory[I + 2] = V[x] % 10;
                    memory[I + 1] = V[x] % 100 - memory[I + 2];
                    memory[I] = V[x] - (memory[I + 1] + memory[I + 2]);
                    break;
                case 0x55:
                    for (uint8_t i = 0; i < 16; i++) {
                        memory[I + i] = V[i];
                    }
                    break;
                case 0x65:
                    for (uint8_t i = 0; i < 16; i++) {
                        V[i] = memory[I + i];
                    }
                    break;
            }
            break;


    }
    pc += 2;

}

void setPc(uint16_t address) {
    pc = address - 2;
}

// position: 1 for x or 2 for
uint8_t getRegister(uint16_t opcode, uint8_t position) {
    if (position == 1) {
        return (opcode & 0x0F00) >> 8;
    } else if (position == 2) {
        return (opcode & 0x00F0) >> 4;
    }
    return 0xF;
}

void clearDisplay() {
    draw = 1;
    for (int i = 0; i < sizeof(screen); i++) {
        screen[i] = 0;
    }
}

void drawSprite(uint8_t x, uint8_t y, uint8_t nibble) {
    draw = 1;
    nibble >>= 4;
    V[0xF] = 0;
    for (int i = 0; i <= (nibble); i++) {
        for (int t = 0; t < 8; t++) {
            uint16_t index = (uint16_t) (i * SCREEN_X + V[y]) * (t + V[x]);
            if (screen[index])
                V[0xF] = 1;
            screen[index] ^= memory[I + i];
        }
    }
}
