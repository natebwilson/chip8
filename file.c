#include "global.h"

void logopcode(){
  if(opcode == 0x0000u){
    return;
  }

  if(logStarted == 0){
    logfile = fopen("log.txt", "w");
    fprintf(logfile, "Executing opcode: 0x%x\n", opcode);
    fclose(logfile);
    logStarted = 1;
  } else {
    logfile = fopen("log.txt", "a");
    fprintf(logfile, "Executing opcode: 0x%x\n", opcode);
    fclose(logfile);
  }
}

void logput(const char *fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  if(logStarted == 0){
    logfile = fopen("log.txt", "w");
    vfprintf(logfile, fmt, ap);
    fclose(logfile);
    logStarted = 1;
  } else {
    logfile = fopen("log.txt", "a");
    vfprintf(logfile, fmt, ap);
    fclose(logfile);
  }
}

int loadRom(const char *filename){
  #ifdef __linux__
    FILE *file = fopen(filename, "r");
  #else
    FILE *file = fopen(filename, "rb");
  #endif

  if(!file){
    //Fail
    return 1;
  }
  
  //Get size of loaded ROM and allocate buffer
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  char *buffer = malloc(size + 1);

  if(!buffer){
    fclose(file);
    return 1;
  }

  //Read in ROM
  rewind(file);
  size_t bytesRead = fread(buffer, 1, size, file);
  fclose(file);
  if(bytesRead != size){
    return 1;
  }

  int i;
  for(i = 0; i < size; i++){
    memory[MEMORY_START_ADDRESS + i] = buffer[i];
  }

  free(buffer);
  return 0;
}