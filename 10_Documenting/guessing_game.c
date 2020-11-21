#include <stdio.h>
#include <libintl.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#define _(STRING) gettext(STRING)

#define MAXNUM 100
#define FALSE 0
#define TRUE 1

const char *romans[] = {"I","II","III","IV","V","VI","VII","VIII","IX","X","XI","XII","XIII","XIV","XV","XVI","XVII","XVIII","XIX","XX","XXI","XXII","XXIII","XXIV","XXV","XXVI","XXVII","XXVIII","XXIX","XXX","XXXI","XXXII","XXXIII","XXXIV","XXXV","XXXVI","XXXVII","XXXVIII","XXXIX","XL","XLI","XLII","XLIII","XLIV","XLV","XLVI","XLVII","XLVIII","XLIX","L","LI","LII","LIII","LIV","LV","LVI","LVII","LVIII","LIX","LX","LXI","LXII","LXIII","LXIV","LXV","LXVI","LXVII","LXVIII","LXIX","LXX","LXXI","LXXII","LXXIII","LXXIV","LXXV","LXXVI","LXXVII","LXXVIII","LXXIX","LXXX","LXXXI","LXXXII","LXXXIII","LXXXIV","LXXXV","LXXXVI","LXXXVII","LXXXVIII","LXXXIX","XC","XCI","XCII","XCIII","XCIV","XCV","XCVI","XCVII","XCVIII","XCIX","C"};

const char *to_roman(int x) {
  return romans[x-1];
}

int from_roman(const char *roman) {
  for (int i = 0; i < MAXNUM; i++)
    if (!strcmp(roman, romans[i]))
      return i+1;
  return -1;
}

void show_help() {
  printf("Hello, world!\n");
  exit(0);
}

void show_version() {
  printf("%s\n", PACKAGE_STRING);
  exit(0);
}

int is_roman;
char buf[1024];

const char *to_s(int x) {
  if (is_roman)
    return to_roman(x);
  else {
    snprintf(buf, 10, "%d", x);
    return buf;
  }
}

int main(int argc, char **argv) {

  is_roman = FALSE;

  if (argc > 1) {
    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
      show_help();
    } else if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) {
      show_version();
    } else if (!strcmp(argv[1], "-r")) {
      is_roman = TRUE;
    }
  }

  setlocale(LC_ALL, "");
  bindtextdomain("guess", ".");
  textdomain("guess");
  char buf[1024];
  int left = 1;
  int right = MAXNUM;
  printf(_("Think of a number between %s "), to_s(left));
  printf(_("and %s\n"), to_s(right));
  int middle = 0;
  while (left < right) {
    middle = (left+right)/2;
    printf(_("Is your number greater than %s (%s/%s): "), to_s(middle), _("yes"), _("no"));
    scanf("%s", buf);
    if (!strcmp(buf, _("yes")) || !strcmp(buf, _("y"))) {
      // yes
      left = middle+1;
    } else {
      right = middle;
    }
  }
  printf(_("Your number is %s\n"), to_s(right));
}