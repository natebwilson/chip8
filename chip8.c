#include "global.h"


uint8_t randByte() {
  return (uint8_t)(rand() % 256);
}

void init(){

  logStarted = 0;

  //Init program counter to start address
  pc = MEMORY_START_ADDRESS;

  //Jump to memory start address at end of memory for now
  //0x1nnn jumps to memory location nnn, MEMORY_START_ADDRESS = 0x200
  //Comment these lines out if actually loading ROM, otherwise this causes seg fault.
  memory[MEMORY_SIZE - 1] = 0x08;
  memory[MEMORY_SIZE - 2] = 0x12;

  //Init stack pointer
  sp = 0;  

  //Init fontset
  int i;
  for(i = 0; i < FONTSET_SIZE; i++){
    memory[FONTSET_START_ADDRESS+i] = fontset[i];
  }

  //Init RNG
  srand(time(NULL));

  initTables();
  initVideo();

  //Init keyUsed flag
  keyUsed = false;
}

void cycle(){
  opcode = (memory[pc] << 8u) | memory[pc+1]; //Retrieves the next two bytes, stores lower address in most significant byte

  logput("Instruction Ox%x executing at memory location 0x%x\n", opcode, pc);

  pc += 2; //Increment program counter

  //Decode and execute
  //logopcode();
  (table[(opcode & 0xF000u) >> 12u])(); 

  if(delayTimer > 0){
    delayTimer--;
  }

  if(soundTimer > 0){
    soundTimer--;
  }

}

int main(int argc, char *argv[]){

  if(argc < 3 || argc > 4){
    printf("Usage: %s <Delay> <ROM> (Debug)\n", argv[0]);
    return(1);
  }

  cycleDelay = atoi(argv[1]);
  char const *fileName = argv[2];
  if(argc == 4){
    debug = atoi(argv[3]);
  } else {
    debug = false;
  }

  init();
  if(loadRom(fileName) == 1){
    videoUpdate();
    echo();
    endwin();
    printf("Error: %s did not load properly or could not be found\n", fileName);
    return(1);
  }

  int quit = 0;
  struct timespec currentTime;
  struct timespec lastCycleTime;
  clock_gettime(CLOCK_MONOTONIC, &lastCycleTime);
  while(!quit){
    quit = inputMonitor();

    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    float dt = (currentTime.tv_sec - lastCycleTime.tv_sec) * 1000.0f + (currentTime.tv_nsec - lastCycleTime.tv_nsec) / 1000000.0f;  
    if(dt > cycleDelay){
      lastCycleTime = currentTime;

      cycle();
      videoUpdate();
      keyUsed = true;
    }
  }
  logput("Final video[64] value: %x\n", video[64]);
  endwin();
  return 0;
}