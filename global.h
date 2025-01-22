#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <stdarg.h>
#include <string.h>


/** HARDWARD CONSTANTS */

#define MEMORY_START_ADDRESS 0x200
#define FONTSET_START_ADDRESS 0x50
#define MEMORY_SIZE 4096
#define FONTSET_SIZE 80
#define STACK_SIZE 16
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32
#define REGISTERS 16
#define KEYCOUNT 16



/** VIDEO */

void initVideo();
int inputMonitor();
void videoUpdate();
extern int videoScale;
extern int cycleDelay;

/** DATA */

extern uint8_t memory[MEMORY_SIZE];
extern int pc;
extern uint16_t stack[STACK_SIZE];
extern int sp;
extern uint16_t opcode;
extern uint8_t registers[REGISTERS];
extern uint16_t memindex;
extern uint8_t keypada[KEYCOUNT];
extern bool keyUsed;
extern uint8_t delayTimer;
extern uint8_t soundTimer;
typedef void (*Chip8Func)();
extern Chip8Func table[0xf + 1];
extern Chip8Func table0[0xe + 1];
extern Chip8Func table8[0xe + 1];
extern Chip8Func tableE[0xe + 1];
extern Chip8Func tableF[0x65 + 1];
extern WINDOW *wrapper, *display;
extern FILE *logfile;
void initTables();
int loadRom(const char *filename);
void logput(const char *fmt, ...);
void logopcode();
extern int logStarted;


//Though this is less memory efficient, but makes video a little easier
//0xFFFFFFFF = ON
//0X00000000 = OFF
extern uint32_t video[VIDEO_HEIGHT*VIDEO_WIDTH];

/** FONTSET */

extern uint8_t fontset[FONTSET_SIZE];




// Functions
void initTables();
void Table0();
void Table8();
void TableE();
void TableF();