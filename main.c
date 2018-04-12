#include <stdio.h>
#include <sys/mman.h> //memory map
#include <sys/stat.h>
#include <string.h> 
#include <ncurses.h>  //for interface
#include <fcntl.h>    //for open
#include <unistd.h>   //for close
#include "gui.h"

typedef struct position {
  int x;
  int y;
} position;

position cursor;

char *open_file(int *file, struct stat *sb, char *fname){

  //open file 
  *file=open(fname, O_RDONLY); 

  //prepare for memory map
  char *map; 
    
  //get length
  fstat(*file, sb);            

  //perform memory map - read all values, no offset
  map=mmap(0, sb->st_size, PROT_READ, MAP_SHARED, *file, 0);  
  
  //print error status
  perror("Memory map status: "); 

  return map;
}


void test_print(char *map, struct stat *sb){


  wprintw(gui.title, "========= SXD Hex Editor ============================\n\n");

  int i;

  //print column numbers, space every 8, up to 31
  for (i=0; i<32; i++){
    if(i%8 == 0 && i>0)
      wprintw(gui.colNr, " ");
    wprintw(gui.colNr, "%.2d ", i);
  }
  wprintw(gui.colNr, "\n");

  //print column numbers, space every 8, up to 31, in hex
  for (i=0; i<32; i++){
    if(i%8 == 0 && i>0)
      wprintw(gui.colNr, " ");
    wprintw(gui.colNr, "%02X ", i);
  }
  wprintw(gui.colNr, "\n");
  wrefresh(gui.colNr);
  wrefresh(gui.title);

  //string to hold each character in the line of 32
  char tmp_data[33];

  for (i=0;i<sb->st_size;i++){

    //read char from file
    uint8_t c = *(map+i);

    //print the ascii for the current line of 32 chars
    if(i%32==0){
      if (i>0) { //we don't want this block to run immediately

        //if at the end of a line of 32, print the accumulated string
        //
        //get cursor position in hex window
        getyx(gui.hex, cursor.y, cursor.x);
        mvwprintw(gui.ascii, cursor.y + TITLE_HEIGHT, 0, "%s\n", tmp_data);
        wrefresh(gui.ascii);

        //blank the string
        strcpy(tmp_data, "                                ");
      }

      //print the new line number
      getyx(gui.hex, cursor.y, cursor.x);
      mvwprintw(gui.lineNr, cursor.y + TITLE_HEIGHT +1 , 0,"%4d\t%08x\t", i, i);
      wrefresh(gui.lineNr);
      wrefresh(gui.hex);
      wmove(gui.hex, cursor.y + 1, 0);
    }

    //print a gap after each 8 chars, like with the col nos 
    else if (i%8 == 0)
      wprintw(gui.hex, " ");
    //print the character's hex
    wprintw(gui.hex, "%02x ", c);

    //only append c to the tmp_data string if it is ascii, otherwise use a .
    if (c < 0x20)
      tmp_data[i%32] = '.';
    else {
      switch(c){
        case 0x0a:
          tmp_data[i%32] = '.';
          break;
        default:
          tmp_data[i%32] = c;
      }
    }
  }

  //prints the tmp_data string
  wprintw(gui.ascii, "%s\n", tmp_data);

  //update window
  wrefresh(gui.ascii);

  //close on button press
  getch();

  return;
}

int main (int argc, char **argv) {
  int file;

  struct stat sb;

  char *map = open_file(&file, &sb, argv[1]);

  setUpGUI();

  test_print(map, &sb);
  
  close(file);

  endwin();

  return 0;
}
