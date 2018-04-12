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

  //newwin(height, width, starty, startx)
  gui.title  = newwin(TITLE_HEIGHT, TITLE_WIDTH, 0, 0);
  gui.lineNr = newwin(LINENR_HEIGHT, LINENR_WIDTH, TITLE_HEIGHT, 0);
  gui.colNr  = newwin(COLNR_HEIGHT, COLNR_WIDTH, TITLE_HEIGHT, LINENR_WIDTH);
  gui.status = newwin(STATUS_HEIGHT, STATUS_WIDTH, LINES-STATUS_HEIGHT, 0);
  gui.hex    = newwin(HEX_HEIGHT, HEX_WIDTH, COLNR_HEIGHT + TITLE_HEIGHT, LINENR_WIDTH);
  gui.ascii  = newwin(ASCII_HEIGHT, ASCII_WIDTH, TITLE_HEIGHT, LINENR_WIDTH + HEX_WIDTH);

  wattrset(gui.title, A_BOLD);
  wattrset(gui.lineNr, A_BOLD);
  wattrset(gui.colNr, A_BOLD);

  box(gui.ascii, 0,0);

  wrefresh(gui.ascii);
  wrefresh(gui.title);
  wrefresh(gui.status);
  wrefresh(gui.lineNr);
  wrefresh(gui.colNr);
  wrefresh(gui.hex);

  printf("%d", COLNR_WIDTH);

  return;
}
