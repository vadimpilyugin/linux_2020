#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#endif

#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <pcre2.h>
#include "config.h"

void init_curses() {
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
}

void end_curses() {
  endwin();
}

typedef struct windows {
  WINDOW *regex_input;
  WINDOW *string_input;
  WINDOW *output;
} windows;

windows draw_window() {
  printw("Input:\n");
  refresh();
  int DX = 3;
  int center_left_col = COLS / 2;
  int center_right_col = center_left_col + 1;
  int first_col = DX;
  int last_col = COLS - DX;
  int top_input_row = 2;
  int bottom_input_row = top_input_row + 2;
  int input_rows = bottom_input_row - top_input_row + 1;
  int first_input_cols = center_left_col - first_col + 1;
  int second_input_cols = last_col - center_right_col + 1;

  printw("left_input: (%d, %d) rows %d cols %d\n", top_input_row, first_col, input_rows, first_input_cols);
  printw("right_input: (%d, %d) rows %d cols %d\n", top_input_row, center_right_col, input_rows, second_input_cols);

  int top_output_row = bottom_input_row + 1;
  int bottom_output_row = LINES - DX;
  int output_rows = bottom_output_row - top_output_row + 1;
  int output_cols = last_col - first_col + 1;

  WINDOW *left_input_box = newwin(input_rows, first_input_cols, top_input_row, first_col);
  WINDOW *left_input = newwin(input_rows-2, first_input_cols-2, top_input_row+1, first_col+1);
  WINDOW *right_input_box = newwin(input_rows, second_input_cols, top_input_row, center_right_col);
  WINDOW *right_input = newwin(input_rows-2, second_input_cols-2, top_input_row+1, center_right_col+1);
  WINDOW *output_box = newwin(output_rows, output_cols, top_output_row, first_col);
  WINDOW *output = newwin(output_rows-2, output_cols-2, top_output_row+1, first_col+1);

  scrollok(output, TRUE);

  box(left_input_box, 0, 0);
  box(right_input_box, 0, 0);
  box(output_box, 0, 0);

  wrefresh(left_input_box);
  wrefresh(right_input_box);
  wrefresh(output_box);

  delwin(left_input_box);
  delwin(right_input_box);
  delwin(output_box);

  windows wnd;
  wnd.regex_input = left_input;
  wnd.string_input = right_input;
  wnd.output = output;

  wmove(left_input, 0, 0);

  return wnd;
}

void del_windows(windows wnd) {
  delwin(wnd.regex_input);
  delwin(wnd.string_input);
  delwin(wnd.output);
}

#define BUFLEN 1024
char regex_input[BUFLEN];
char string_input[BUFLEN];

void accept_input(windows wnd) {
  mvwgetnstr(wnd.regex_input, 0, 0, regex_input, BUFLEN);
  mvwgetnstr(wnd.string_input, 0, 0, string_input, BUFLEN);
}

void output_result(windows wnd) {

  PCRE2_SPTR pattern = (PCRE2_SPTR)regex_input;
  PCRE2_SPTR string = (PCRE2_SPTR)string_input;
  PCRE2_SIZE string_len = (PCRE2_SIZE)strlen((char *)string);
  int errnum;
  PCRE2_SIZE erroffs;

  pcre2_code *re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 
#ifdef UTF
    PCRE2_UCP,
#else
    PCRE2_NEVER_UCP,
#endif
    &errnum, &erroffs, NULL);

  if (re == NULL) {
    PCRE2_UCHAR buffer[256];
    pcre2_get_error_message(errnum, buffer, sizeof(buffer));
    wprintw(wnd.output, "%s\n", regex_input);
    for (int i = 0; i < (int)erroffs; i++)
      waddch(wnd.output, ' ');
    wprintw(wnd.output, "^\n");
    wprintw(wnd.output, "PCRE2 compilation failed at offset %d: %s\n", (int)erroffs, buffer);
    wprintw(wnd.output, "\n");
    wrefresh(wnd.output);
    return;
  }

  pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);
  int rc = pcre2_match(re, string, string_len, 0, 0, match_data, NULL);

  if (rc < 0) {
    switch (rc) {
    case PCRE2_ERROR_NOMATCH:
      wprintw(wnd.output, "No match\n");
      break;
    default:
      wprintw(wnd.output, "Matching error %d\n", rc);
      break;
    }
    wprintw(wnd.output, "\n");
    wrefresh(wnd.output);
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
    return;
  }

  PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);

  wprintw(wnd.output, "`%s`: `%s`\n", regex_input, string_input);
  for (int i = 0; i < rc; i++){
    wprintw(wnd.output, "%2ld: %.*s\n", ovector[2*i], (int)(ovector[2*i+1] - ovector[2*i]), string + ovector[2*i]);
  }
  wprintw(wnd.output, "\n");

  pcre2_match_data_free(match_data);
  pcre2_code_free(re);

  wrefresh(wnd.output);
}

void erase_input(windows wnd) {
  wclear(wnd.regex_input);
  wclear(wnd.string_input);
  wrefresh(wnd.regex_input);
  wrefresh(wnd.string_input);
}

int main() {
  init_curses();
  windows wnd = draw_window();
  for (;;) {
    accept_input(wnd);
    output_result(wnd);
    erase_input(wnd);
  }
  del_windows(wnd);
  end_curses();
}