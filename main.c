#include <stdio.h>
#include <sys/mman.h> //memory map
#include <sys/stat.h>
#include <string.h>
#include <ncurses.h>  //for interface
#include <fcntl.h>    //for open
#include <unistd.h>   //for close
#include "gui.h"
#include <math.h>

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


  wprintw(gui.title, "========= SXD Hex Editor ==========================\n\n");

  int i;

  //print column numbers, space every 8, up to gui.ascii_width -1
  for (i=0; i<gui.ascii_width; i++){
    if(i%8 == 0 && i>0)
      wprintw(gui.colNr, " ");
    wprintw(gui.colNr, "%.2d ", i);
  }
  wprintw(gui.colNr, "\n");

  //print column numbers, space every 8, up to gui.ascii_width -1, in hex
  for (i=0; i<gui.ascii_width; i++){
    if(i%8 == 0 && i>0)
      wprintw(gui.colNr, " ");
    wprintw(gui.colNr, "%02X ", i);
  }
  wprintw(gui.colNr, "\n");
  wrefresh(gui.colNr);
  wrefresh(gui.title);

  //string to hold each character in the line of gui.ascii_width
  char tmp_data[gui.ascii_width+1];

  for (i=0;i<sb->st_size;i++){

    //read char from file
    uint8_t c = *(map+i);

    //print the ascii for the current line of gui.ascii_width chars
    if(i%gui.ascii_width==0){
      if (i>0) { //we don't want this block to run immediately

        //if at the end of a line of gui.ascii_width, print the accumulated
        //string

        //get cursor position in hex window
        getyx(gui.hex, cursor.y, cursor.x);
        mvwprintw(gui.ascii, cursor.y, 0, "%s\n", tmp_data);
        wrefresh(gui.ascii);

        //blank the string
        tmp_data[0] = "\0";
        //strcpy(tmp_data, "                                ");
      }

      //print the new line number
      getyx(gui.hex, cursor.y, cursor.x);
      mvwprintw(gui.lineNr, cursor.y + 1 , 0,"%4d\t%08x\t", i, i);
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
      tmp_data[i%gui.ascii_width] = '.';
    else {
      switch(c){
        case 0x0a:
          tmp_data[i%gui.ascii_width] = '.';
          break;
        default:
          tmp_data[i%gui.ascii_width] = c;
      }
    }
  }

  //prints the tmp_data string
  wprintw(gui.ascii, "%s\n", tmp_data);

  //update window
  wrefresh(gui.ascii);

  curs_set(2);

  wmove(gui.hex, 1, 0);
  wrefresh(gui.hex);


  getch();

  return;
}

int main (int argc, char **argv) {
  int file;

  struct stat sb;

  char *map = open_file(&file, &sb, argv[1]);

  setUpGUI();

  ////pause before file load
  //getch();

  test_print(map, &sb);

  char ch;
  while (( ch = getch()) != 'q') {
    getyx(gui.hex, cursor.y, cursor.x);
    switch (ch) {
      case 'h': 
        wmove(gui.hex, cursor.y    , cursor.x - 1);
        break;
      case 'j': 
        wmove(gui.hex, cursor.y + 1, cursor.x    );
        break;
      case 'k': 
        wmove(gui.hex, cursor.y - 1, cursor.x    );
        break;
      case 'l': 
        wmove(gui.hex, cursor.y    , cursor.x + 1);
        break;

    }
        wrefresh(gui.hex);
  };

  close(file);

  endwin();

  return 0;
}
