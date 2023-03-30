#ifndef CHIP8_H
#define CHIP8_H

extern unsigned char font_set[80];
extern unsigned char memory[4096];
extern unsigned char V[16];
extern unsigned short I;
extern unsigned char gfx[64 * 32];
extern unsigned short pc;
extern unsigned short stack[16];
extern unsigned char keypad[16];
extern unsigned short sp;
extern unsigned char delay_timer;
extern unsigned char sound_timer;
extern unsigned char draw_flag;
extern unsigned char sound_flag;

void init_cpu();
int load_rom(char*);
void emulate_cycle();

#endif
