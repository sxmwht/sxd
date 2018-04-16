#ifndef GUI_h
#define GUI_h

void setUpGUI();

struct GUI {
  int chars_per_line;
  WINDOW* title;
  int title_height;
  int title_width;
  WINDOW* colNr;
  int colNr_height;
  int colNr_width;
  WINDOW* lineNr;
  int lineNr_height;
  int lineNr_width;
  WINDOW* hex;
  int hex_height;
  int hex_width;
  WINDOW* ascii;
  int ascii_height;
  int ascii_width;
  WINDOW* status;
  int status_height;
  int status_width;
  WINDOW** hex_cols;
} gui;


#endif

