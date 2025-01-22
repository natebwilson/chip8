#include "global.h"

void initVideo(){
  setlocale(LC_ALL, "");
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
  use_default_colors();

  int i;
  for(i = 0; i < 2048; i++){
    //Init video blank
    video[i] = 0x00000000u;
  }


  //Video width 
  wrapper = newwin((VIDEO_HEIGHT + 2), (VIDEO_WIDTH + 2), 0,0);
  box(wrapper, 0, 0);
  display = derwin(wrapper, VIDEO_HEIGHT, VIDEO_WIDTH, 1, 1); 
}

int inputMonitor(){

  int ch = getch();
  // int i;
  // if(keyUsed){
  //   for(i = 0; i < KEYCOUNT; i++){
  //     //Reset keypad buffer
  //     keypada[i] = 0;
  //   }
  //   keyUsed = false;
  // }

  switch(ch){
    case '\x1b': return 1;
    case 'x': keypada[0] = 1; break;
    case '1': keypada[1] = 1; break;
    case '2': keypada[2] = 1; break;
    case '3': keypada[3] = 1; break;
    case 'q': keypada[4] = 1; break;
    case 'w': keypada[5] = 1; break;
    case 'e': keypada[6] = 1; break;
    case 'a': keypada[7] = 1; break;
    case 's': keypada[8] = 1; break;
    case 'd': keypada[9] = 1; break;
    case 'z': keypada[0xA] = 1; break;
    case 'c': keypada[0xB] = 1; break;
    case '4': keypada[0xC] = 1; break;
    case 'r': keypada[0xD] = 1; break;
    case 'f': keypada[0xE] = 1; break;
    case 'v': keypada[0xF] = 1; break;
  }

  return 0;
}



void videoUpdate(){
  int row;
  int col;
  wmove(display, 0,0);
  for(row = 0; row < VIDEO_HEIGHT; row++){
    for(col = 0; col < VIDEO_WIDTH; col++){
      if(video[(row * VIDEO_WIDTH) + col] == 0xffffffff){
        wprintw(display, "█");
      } else {
        wprintw(display, " ");
      } 
    }
  }  


  wrefresh(wrapper); 
  wrefresh(display);

}