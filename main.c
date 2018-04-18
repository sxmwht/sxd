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

int current_window;

position cursor;
position cursor_last;

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

void move_next_byte (char dir) {
  int cursor_new_height = cursor.y;
  if (dir == 'r') {
    if (current_window == gui.ascii_width - 1) {
      if (cursor.y != gui.hex_height - 1) {
        current_window = 0;
        cursor_new_height = cursor.y + 1;
      }
    }
    else
      current_window++;
  }

  else if (dir == 'l') {
    if (current_window == 0) {
      if (cursor.y != 0) {
        current_window = gui.ascii_width - 1;
        cursor_new_height = cursor.y - 1;
      }
    }
    else
      current_window--;
    
  }

  wmove(gui.hex_cols[current_window], cursor_new_height, 0);
  wrefresh(gui.hex_cols[current_window]);

  return;
}

void move_next_char(char dir) {

  int cursor_new_y = cursor.y;
  int cursor_new_x = cursor.x;
  
  char ch;
  int i = 1;

  if (dir == 'r') {
    if (cursor.x == 0) {
      cursor_new_x = 1;
    }
    else if (cursor.x == 1) {
      if (current_window == gui.ascii_width-1) {
        if (cursor.y < gui.hex_height - 1) {
          current_window = 0;
          cursor_new_y = cursor.y + 1;
          cursor_new_x = 0;
        }
      }
      else {
        current_window++;
        cursor_new_x = 0;
      }
    }
  }

  else if (dir == 'l') {
    if (cursor.x == 1) {
      cursor_new_x = 0;
    }
    else if (cursor.x == 0) {
      if (current_window == 0) {
        if (cursor.y > 0) {
          current_window = gui.ascii_width - 1;
          cursor_new_y = cursor.y - 1;
          cursor_new_x = 1;
        }
      }
      else {
        current_window--;
        cursor_new_x = 1;
      }
    }
  }

  wmove(gui.hex_cols[current_window], cursor_new_y, cursor_new_x);

  return;

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
        getyx(gui.hex_cols[gui.ascii_width-1], cursor.y, cursor.x);
        mvwprintw(gui.ascii, cursor.y - 1, 0, "%s\n", tmp_data);
//        wrefresh(gui.ascii);

        //blank the string
        tmp_data[0] = "\0";
        //strcpy(tmp_data, "                                ");
      }

      //print the new line number
      getyx(gui.hex_cols[0], cursor.y, cursor.x);
      mvwprintw(gui.lineNr, cursor.y, 0,"%4d\t%08x\t", i, i);
//      wrefresh(gui.lineNr);
      wmove(gui.hex_cols[0], cursor.y, 0);
    }

//    //print a gap after each 8 chars, like with the col nos 
//    else if (i%8 == 0)
//      wprintw(gui.hex, " ");

    //print the character's hex
    mvwprintw(gui.hex_cols[i%gui.ascii_width], i/gui.ascii_width, 0, "%02x ", c);

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
  wrefresh(gui.lineNr);

  curs_set(2);

  for (i=0; i<gui.ascii_width; i++)
    wrefresh(gui.hex_cols[i]);

  wmove(gui.hex_cols[0], 0, 0);
  wrefresh(gui.hex_cols[0]);

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
  current_window = 0;

  bool quit = 0;

  char ch;
  char str[80];
  while (!quit) {
    while (( ch = getch()) != ':') {
      getyx(gui.hex_cols[current_window], cursor.y, cursor.x);
      wchgat(gui.ascii, 1, !A_BOLD, 0, NULL);
      switch (ch) {
        case 'h': 
          move_next_char('l');
          break;
        case 'j':
          wmove(gui.hex_cols[current_window], cursor.y + 1, cursor.x    );
          break;
        case 'k': 
          wmove(gui.hex_cols[current_window], cursor.y - 1, cursor.x    );
          break;
        case 'l':
          move_next_char('r');
          break;

        case 'w':
          move_next_byte('r');
          break;
        case 'b':
          move_next_byte('l');
          break;
      }
      getyx(gui.hex_cols[current_window], cursor.y, cursor.x);
      mvwchgat(gui.ascii, cursor.y, current_window, 1, A_BOLD, 0, NULL);
      wrefresh(gui.ascii);
      wrefresh(gui.hex_cols[current_window]);
    };

    echo();
    mvwprintw(gui.status, 0, 0, ":");
    wrefresh(gui.status);
    wgetstr(gui.status, str);

    if (strcmp(str, "q") == 0) {
      quit = 1;
    }
    else {
      wclear(gui.status);
      wrefresh(gui.status);
      wrefresh(gui.hex_cols[current_window]);
      noecho();
      raw();
    }

  }

  close(file);

  endwin();

  return 0;
}
