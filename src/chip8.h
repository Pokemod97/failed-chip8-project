#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "main.h" 

#define SCREEN_X 64
#define SCREEN_Y 32

void initChip(uint8_t *File);
void runOpcode();
void setPc(uint16_t address);
void clearDisplay();
void drawSprite(uint8_t x, uint8_t y, uint8_t nibble);
uint8_t  getRegister( uint8_t opcode ,uint8_t position);
