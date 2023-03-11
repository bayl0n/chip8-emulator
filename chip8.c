#include "chip8.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

extern int errno;

/* 
 #######################
 # CHIP-8 ARCHITECTURE #
 #######################
 */

/*
 * Font set:
 * 
 **/
unsigned char font_set[80] = {
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

/*
 * Memory:
 * Contains 4096 memory locations
 **/
unsigned char memory[4096] = {0};

/*
 * Data Registers:
 * Contains 16 data registers: V0 to VF
 **/
unsigned char V[16] = {0};

/*
 * Index Register:
 * Contains a value from 0x000 to 0xFFF
 **/
unsigned short I = 0;

/*
 * Index Register:
 * Contains a value from 0x000 to 0xFFF
 * Begins at memory location 0x200
 **/
unsigned short pc = 0x200;

/*
 * Graphics system:
 * CHIP-8 has a black and white graphics system
 * which has a total for 2048 pixels (64 * 32)
 **/
unsigned char gfx[64 * 32] = {0};

/*
 * Stack:
 * Use to remember locations when
 * a jump is performed.
 **/
unsigned short stack[16];

/*
 * Stack Pointer:
 * Used to remember which level
 * the stack is used.
 **/
unsigned short sp = 0;

/*
 * Timers:
 * Registers that count at 60hz. When set
 * above 0, they decrement until they reach 0.
 **/
unsigned char delay_timer = 0;
unsigned char sound_timer = 0;


/*
 * Flags:
 * Flags for draw and sound operations.
 **/
unsigned char draw_flag = 0;
unsigned char sound_flag = 0;

/* 
 ################
 # CHIP-8 LOGIC #
 ################
 */

void init_cpu() {
    // Seed random values from time
    srand((unsigned int)time(NULL));

    // Load font set into memory
    memcpy(memory, font_set, sizeof(font_set));
}

int load_rom(char* filename) {
    FILE* fp = fopen(filename, "rb");

    if (!fp) return errno;

    struct stat st;
    stat(filename, &st);
    size_t fsize = st.st_size;

    size_t bytes_read = fread(memory + 0x200, 1, sizeof(memory) - 0x200, fp);

    if (bytes_read != fsize) {
        return -1;
    }

    fclose(fp);

    return 0;
}

void emulate_cycle() {
    draw_flag = 0;
    sound_flag = 0;

    unsigned short op = memory[pc] << 8 | memory[pc + 1];

    unsigned short x = (op & 0x0F00) >> 8;
    unsigned short y = (op & 0x00F0) >> 4;

    //TODO: switch case to handle op codes
    switch(op & 0xF000) {
        case 0x000:
            switch(op & 0x00FF) {
                case 0x00E0:
                    for(int i = 0; i < 64 * 32; i++) {
                        gfx[i] = 0;
                    }
                    pc += 2;
                    break;
            }
    }
}
