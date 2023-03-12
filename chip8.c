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
        case 0x0000:
            switch(op & 0x00FF) {
                // 0x00E0: Clear screan
                case 0x00E0:
                    for(int i = 0; i < 64 * 32; i++) {
                        gfx[i] = 0;
                    }
                    pc += 2;
                    break;
            }
        // 0x1NNN: Jump
        case 0x1000:
            pc = op & 0x0FFF;
            break;
        // 0x6XNN: Set Register V[x]
        case 0x6000:
            V[x] = (op & 0x00FF);
            pc += 2;
            break;
        // 0x7XNN: Add value to Register V[x]
        case 0x7000:
            V[x] += (op & 0x00FF);
            pc += 2;
            break;
        // 0xANNN: Set index register I
        case 0xA000:
            I = (op & 0x0FFF);
        // 0x00E0: Clear screan
        case 0xD000:
            draw_flag = 1;

            unsigned short height = op & 0x000F;
            unsigned short px;

            V[0xF] = 0;

            for (int yline = 0; yline < height; y++) {
                px = memory[I + yline];

                for (int xline = 0; xline < 8; xline++) {
                    if ((px & (0x80 >> xline)) != 0) {
                        if (gfx[(V[x] + xline + ((V[y] + yline) * 64))] == 1) {
                            V[0xF] = 1;
                        }
                        gfx[V[x] + xline + ((V[y] + yline) * 64)] ^= 1;
                    }
                }
            }

            pc += 2;
            break;
    }
}
