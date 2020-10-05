#include <stdio.h>
#include <ncurses.h>      /* ncurses.h includes stdio.h */  
#include <string.h> 
#include <locale.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define SPACE ' '
#define EOL '\n'
#define TAB '\t'
#define RET '\r'

// #define LINES 25
// #define COLS 80
#define BX 3
#define BRD 1
#define NUMS 4
#define PUNCT 4

FILE *log_file = NULL;

typedef struct Lines {
  char **lines;
  int *lines_len;
  int n_lines;
} Lines;

void fill_wnd(char **wnd, Lines lines, int top_line, int left_offset);

void init_curses() {
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  cbreak();
}

void end_curses() {
  endwin();
}

void curses_stuff(char **win, Lines lines) {
  int box_rows = LINES - 2*BX;
  int box_cols = COLS - 2*BX;
  int box_row_offset = BX;
  int box_col_offset = BX;
  int text_row_offset = BX + BRD;
  int text_col_offset = BX + BRD + NUMS + PUNCT;
  int text_rows = box_rows - 2*BRD;
  int text_cols = box_cols - 2*BRD - NUMS - PUNCT;
  int nums_rows = text_rows;
  int nums_cols = NUMS + PUNCT;
  int nums_row_offset = text_row_offset;
  int nums_col_offset = BX + BRD;

  WINDOW *box_wnd = newwin(box_rows, box_cols, box_row_offset, box_col_offset);
  WINDOW *text_wnd = newwin(text_rows, text_cols, text_row_offset, text_col_offset);
  WINDOW *nums_wnd = newwin(nums_rows, nums_cols, nums_row_offset, nums_col_offset);

  keypad(text_wnd, TRUE);
  keypad(box_wnd, TRUE);

  int c = 0;
  int top_offset = 0;
  int left_offset = 0;
  int cursor_row = 0;
  int cursor_col = 0;
  int cursor_wants_col = 0;


  do {
    fprintf(log_file, "New cycle: %d\n", top_offset);
    fprintf(log_file, "Key: %d, Name: %s\n", c, keyname(c));
    int redraw = 0;

    int new_left_offset = left_offset;
    int new_top_offset = top_offset;

    switch (c) {
      case KEY_NPAGE:
      cursor_row += text_rows;
      break;
      case KEY_PPAGE:
      cursor_row -= text_rows;
      break;
      case KEY_HOME:
      cursor_col = 0;
      break;
      case KEY_END:
      cursor_col = lines.lines_len[cursor_row];
      break;
      case KEY_UP:
      cursor_row--;
      break;
      case EOL:
      case KEY_DOWN:
      cursor_row++;
      break;
      case KEY_LEFT:
      cursor_col--;
      break;
      case KEY_RIGHT:
      cursor_col++;
      break;
    } 

    fprintf(log_file, "before modification: cursor_col %d, cursor row %d\n", cursor_col, cursor_row);
    fprintf(log_file, "before modification: top_offset %d, left_offset %d\n", top_offset, left_offset);
    fprintf(log_file, "before modification: new_top_offset %d, new_left_offset %d\n", new_top_offset, new_left_offset);
    fprintf(log_file, "before modification: cursor_wants_col %d\n", cursor_wants_col);

    if (cursor_row < 0) {
      cursor_row = 0;
    } else if (cursor_row >= lines.n_lines) {
      cursor_row = lines.n_lines - 1;
      if (cursor_row < 0) {
        cursor_row = 0;
      }
    }

    int available_max_col = lines.lines_len[cursor_row];

    if (cursor_col < 0) {
      if (cursor_row > 0) {
        cursor_row--;
        cursor_col = lines.lines_len[cursor_row];
      } else {
        cursor_col = 0;
      }
    } else if (cursor_col > available_max_col) {
      if (c == KEY_RIGHT && cursor_row < lines.n_lines - 1) {
        cursor_row++;
        cursor_col = 0;
      } else {
        cursor_col = available_max_col;
      }
    }

    if (c == KEY_RIGHT || c == KEY_LEFT || c == KEY_HOME || c == KEY_END) {
      cursor_wants_col = cursor_col;
    } else if ((c == KEY_UP || c == KEY_DOWN || c == KEY_NPAGE || c == KEY_PPAGE) && available_max_col >= cursor_wants_col) {
      cursor_col = cursor_wants_col;
    } else if ((c == KEY_UP || c == KEY_DOWN || c == KEY_NPAGE || c == KEY_PPAGE)) {
      cursor_col = available_max_col;
    }

    int rel_cursor_row = cursor_row - top_offset;
    int rel_cursor_col = cursor_col - left_offset;

    if (rel_cursor_row >= text_rows) {
      new_top_offset += text_rows;
      rel_cursor_row = cursor_row - new_top_offset;
    } else if (rel_cursor_row < 0) {
      new_top_offset -= text_rows;
      rel_cursor_row = cursor_row - new_top_offset;
    }
    if (rel_cursor_col >= text_cols) {
      new_left_offset += text_cols;
      rel_cursor_col = cursor_col - new_left_offset;
    } else if (rel_cursor_col < 0) {
      new_left_offset -= text_cols;
      rel_cursor_col = cursor_col - new_left_offset;
    }

    if (new_top_offset >= lines.n_lines) {
      new_top_offset = lines.n_lines-1;
    }
    if (new_left_offset < 0) {
      new_left_offset = 0;
    }
    if (new_top_offset < 0) {
      new_top_offset = 0;
    }

    if (new_left_offset != left_offset) {
      left_offset = new_left_offset;
      redraw = 1;
    }
    if (new_top_offset != top_offset) {
      top_offset = new_top_offset;
      redraw = 1;
    }

    if (c != 0) {
      c = 0;
      fill_wnd(win, lines, top_offset, left_offset);
    } else {
      redraw = 1;
    }

    fprintf(log_file, "top_offset %d, left_offset %d\n", top_offset, left_offset);
    fprintf(log_file, "cursor_wants_col %d\n", cursor_wants_col);
    if (redraw) {
      for (int row = 0; row < text_rows; row++) {
        for (int col = 0; col < text_cols; col++) {
          mvwaddch(text_wnd, row, col, win[row][col]);
        }
      }
      box(box_wnd, 0, 0);
      for (int row = 0; row < text_rows; row++) {
        int line_no = row + top_offset;
        if (line_no >= lines.n_lines) {
          break;
        }
        mvwprintw(nums_wnd, row, 0, "%4d:   ", line_no + 1);
      }
      // box(nums_wnd, 0, 0);
      wrefresh(box_wnd);
      wrefresh(nums_wnd);
      wrefresh(text_wnd);
    }
    if (rel_cursor_row < 0) 
      rel_cursor_row = 0;
    if (rel_cursor_col < 0) 
      rel_cursor_col = 0;
    fprintf(log_file, "cursor row: %d, cursor col: %d\n", cursor_row, cursor_col);
    fprintf(log_file, "relative cursor row: %d, cursor col: %d\n", rel_cursor_row, rel_cursor_col);
    wmove(text_wnd, rel_cursor_row, rel_cursor_col);

    fprintf(log_file, "===========\n\n");
  } while((c = wgetch(text_wnd)) != 'q');

  delwin(box_wnd);
  delwin(text_wnd);
}

int buflen = 0;
char *read_file(const char *fname) {
  FILE *f = fopen(fname, "rb");
  fseek(f, 0, SEEK_END);
  int fsize = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *buf = malloc(fsize + 1);
  fread(buf, 1, fsize, f);
  fclose(f);

  buf[fsize] = 0;
  buflen = fsize;
  return buf;
}

Lines alloc_lines(const char *buf, int buflen) {
  int n_lines = 25;
  int line_no = 0;
  char **lines = (char **)malloc(n_lines * sizeof(char *));
  int col_no = 0;
  int n_cols = 0;
  int *lines_len = (int *)malloc(n_lines * sizeof(int));
  lines_len[0] = 0;
  int i = 0;

  while (i < buflen) {
    fprintf(log_file, "=====================\n\n");
    fprintf(log_file, "while loop start, i=%d, buflen=%d\n", i, buflen);
    n_cols = 100;
    lines[line_no] = (char *)malloc(n_cols);
    if (lines[line_no] == NULL) {
      perror("line malloc failed");
      exit(4);
    }
    col_no = 0;
    fprintf(log_file, "we are at line %d, i=%d, buf[%d] = %c\n", line_no, i, i, buf[i]);
    if (i > 0) {
      fprintf(log_file, "last line %d ended in %c\n", line_no-1, buf[i-1]);
    }
    while (i < buflen && buf[i] != EOL) {
      lines[line_no][col_no++] = buf[i++];
      if (col_no == n_cols) {
        int n_cols_tmp = n_cols << 1;
        fprintf(log_file, "Line limit reached, col_no = %d, increasing to %d\n", n_cols, n_cols_tmp);
        char *col_tmp = (char *)realloc(lines[line_no], n_cols_tmp);
        if (col_tmp == NULL) {
          perror("line realloc failed");
          exit(5);
        }
        lines[line_no] = col_tmp;
        n_cols = n_cols_tmp;
      }
    }
    fprintf(log_file, "putting final symbol into line %d:%d\n", line_no, col_no);
    lines[line_no][col_no] = '\0';
    fprintf(log_file, "got a line: <%s>\n", lines[line_no]);
    fprintf(log_file, "filled line %d, number of columns is %d\n", line_no, col_no);
    lines_len[line_no++] = col_no;
    if (++i >= buflen) {
      // file ended
      break;
    }
    if (line_no == n_lines) {
      fprintf(log_file, "!!!!! line limit reached, allocating more lines\n");
      int n_lines_tmp = n_lines * n_lines;
      fprintf(log_file, "n_lines = %d, new_n_lines = %d\n", n_lines, n_lines_tmp);
      char **lines_tmp = (char **)realloc(lines, n_lines_tmp * sizeof(char *));
      if (lines_tmp == NULL) {
        perror("realloc failed");
        exit(3);
      }
      lines = lines_tmp;
      n_lines = n_lines_tmp;
      lines_len = (int *)realloc(lines_len, n_lines * sizeof(int));
    }
  }

  Lines lns;
  lns.lines = lines;
  lns.lines_len = lines_len;
  lns.n_lines = line_no;

  return lns;
}

void dealloc_lines(Lines lns) {
  for (int i = 0; i < lns.n_lines; i++) {
    free(lns.lines[i]);
  }
  free(lns.lines_len);
}

char **alloc_wnd() {
  char **wnd = (char **)malloc(LINES * sizeof(char *));
  for (int row = 0; row < LINES; row++) {
    wnd[row] = (char *)malloc(COLS);
  }
  return wnd;
}

void dealloc_wnd(char **wnd) {
  for (int row = 0; row < LINES; row++) {
    free(wnd[row]);
  }
  free(wnd);
}

void fill_wnd(char **wnd, Lines lines, int top_line, int left_offset) {
  for (int row = 0; row < LINES; row++) {
    for (int col = 0; col < COLS; col++) {
      int text_col = left_offset + col;
      int text_row = top_line + row;
      if (text_row < lines.n_lines) {
        if (text_col < lines.lines_len[text_row])
          wnd[row][col] = lines.lines[text_row][text_col];
        else
          wnd[row][col] = SPACE;
      } else {
        wnd[row][col] = SPACE;
      }
    }
  }
}

void print_wnd(char **wnd) {
  for (int row = 0; row < LINES; row++) {
    for (int col = 0; col < COLS; col++) {
      fprintf(log_file, "%c", wnd[row][col]);
    }
    fprintf(log_file, "\n");
  }
}

int main(int argc, char **argv) {
  char *fn = "/dev/null";

  if (argc < 2) {
    fprintf(stderr, "No file given\n");
    return 1;
  }

  if (argc > 2)
    fn = argv[2];
  log_file = fopen(fn, "w");

  if (log_file == NULL) {
    perror("Failed to open log file");
    return 2;
  }

  char *buf = read_file(argv[1]);

  Lines lines = alloc_lines(buf, buflen);

  init_curses();
  char **wnd = alloc_wnd();
  fill_wnd(wnd, lines, 0, 0);
  printw("File: %s; size: %d", argv[1], lines.n_lines);
  refresh();
  curses_stuff(wnd, lines);
  dealloc_wnd(wnd);
  fclose(log_file);
  dealloc_lines(lines);
  end_curses();
}