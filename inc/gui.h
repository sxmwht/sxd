#ifndef GUI_h
#define GUI_h

#define TITLE_HEIGHT 3
#define TITLE_WIDTH LINES
#define STATUS_HEIGHT 2
#define ASCII_WIDTH 32
#define COLNR_HEIGHT 3
#define LINENR_WIDTH 20
#define LINENR_HEIGHT (LINES - TITLE_HEIGHT - STATUS_HEIGHT)
#define STATUS_WIDTH COLS
#define ASCII_HEIGHT LINENR_HEIGHT
#define COLNR_WIDTH (((COLS - LINENR_WIDTH) * 8 / 33))
#define HEX_WIDTH COLNR_WIDTH
#define HEX_HEIGHT (LINES - TITLE_HEIGHT - COLNR_HEIGHT - STATUS_HEIGHT)

void setUpGUI();

struct GUI {
  WINDOW* title;
  WINDOW* colNr;
  WINDOW* lineNr;
  WINDOW* hex;
  WINDOW* ascii;
  WINDOW* status;
} gui;

#endif
