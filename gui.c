#include <ncurses.h>  //for interface
#include "gui.h"
#include <math.h>


/*
 *===========================================================================
 *  title 
 *
 *===========================================================================
 *        |                                                   |
 *        |       colNr                                       |
 *        |                                                   |
 *        |===================================================|
 * lineNr |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |      hex                                          |   ascii
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *        |                                                   |
 *===========================================================================
 *              status                                        
 *===========================================================================
 */       

void setUpGUI() {
  //set up curses
  
  initscr();

  //chars are sent as soon as they are pressed
  raw();

  //enable F keys etc 
  keypad(stdscr, TRUE);

  //do not echo when using getch() etc
  noecho();

  refresh();

  gui.title_height=3;
  gui.title_width=COLS;
  gui.status_height=2;
  gui.colNr_height=2;
  gui.lineNr_width=20;
  gui.lineNr_height=(LINES - gui.title_height - gui.status_height);
  gui.status_width=COLS;
  gui.ascii_height=gui.lineNr_height;
  gui.hex_height=(LINES - gui.title_height - gui.colNr_height - gui.status_height);

  gui.chars_per_line=((COLS - gui.lineNr_width) * 8 / 33);

  int i = 0;

  while (gui.chars_per_line >>= 1) {i++;};

  gui.ascii_width = pow(2, i);

  gui.hex_width = gui.ascii_width * 25 / 8;
  gui.colNr_width = gui.hex_width;

  //newwin(height, width, starty, startx)
  gui.title  = newwin(gui.title_height, gui.title_width, 0, 0);
  gui.lineNr = newwin(gui.lineNr_height, gui.lineNr_width, gui.title_height + gui.colNr_height +1, 0);
  gui.colNr  = newwin(gui.colNr_height, gui.colNr_width, gui.title_height, gui.lineNr_width);
  gui.status = newwin(gui.status_height, gui.status_width, LINES-gui.status_height, 0);
  gui.hex    = newwin(gui.hex_height, gui.hex_width, gui.colNr_height + gui.title_height, gui.lineNr_width);
  gui.ascii  = newwin(gui.ascii_height, gui.ascii_width, gui.title_height + gui.colNr_height + 1, gui.lineNr_width + gui.hex_width);


  gui.hex_cols = (WINDOW *) malloc (gui.ascii_width * sizeof(WINDOW*));

  for (i = 0; i < gui.ascii_width; i++) {
    gui.hex_cols[i] = newwin(gui.hex_height, 2, gui.title_height + gui.colNr_height +1,
        gui.lineNr_width + i*3 + i/8);
    //box(gui.hex_cols[i], 0, 0);
    //wrefresh(gui.hex_cols[i]);
  }

  wattrset(gui.title,  A_BOLD);
  wattrset(gui.lineNr, A_BOLD);
  wattrset(gui.colNr,  A_BOLD);
 
////outline gui elements for debugging
//  box(gui.ascii, 0,0);
////  box(gui.hex, 0, 0);
//  box(gui.status, 0, 0);
//  box(gui.colNr, 0, 0);
//  box(gui.lineNr, 0, 0);
//  box(gui.title, 0, 0);

  wrefresh(gui.ascii);
  wrefresh(gui.title);
  wrefresh(gui.status);
  wrefresh(gui.lineNr);
  wrefresh(gui.colNr);
 // wrefresh(gui.hex);

  for (i = 0; i < gui.ascii_width; i++)
    wrefresh(gui.hex_cols[0]);


  return;
}
