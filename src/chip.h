#include <time.h>

#include "main.h" 
void initChip( struct program* file);
void runOpcode();
void setPc(uint16_t address);
void clearDisplay();
void drawSprite(uint8_t x, uint8_t y, uint8_t nibble);
uint8_t  getRegister( uint16_t opcode ,uint8_t position);
