#include <stdio.h>
#include <libintl.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define _(STRING) gettext(STRING)

char *romans[] = ["I",
"II",
"III",
"IV",
"V",
"VI",
"VII",
"VIII",
"IX",
"X",
"XI",
"XII",
"XIII",
"XIV",
"XV",
"XVI",
"XVII",
"XVIII",
"XIX",
"XX",
"XXI",
"XXII",
"XXIII",
"XXIV",
"XXV",
"XXVI",
"XXVII",
"XXVIII",
"XXIX",
"XXX",
"XXXI",
"XXXII",
"XXXIII",
"XXXIV",
"XXXV",
"XXXVI",
"XXXVII",
"XXXVIII",
"XXXIX",
"XL",
"XLI",
"XLII",
"XLIII",
"XLIV",
"XLV",
"XLVI",
"XLVII",
"XLVIII",
"XLIX",
"L",
"LI",
"LII",
"LIII",
"LIV",
"LV",
"LVI",
"LVII",
"LVIII",
"LIX",
"LX",
"LXI",
"LXII",
"LXIII",
"LXIV",
"LXV",
"LXVI",
"LXVII",
"LXVIII",
"LXIX",
"LXX",
"LXXI",
"LXXII",
"LXXIII",
"LXXIV",
"LXXV",
"LXXVI",
"LXXVII",
"LXXVIII",
"LXXIX",
"LXXX",
"LXXXI",
"LXXXII",
"LXXXIII",
"LXXXIV",
"LXXXV",
"LXXXVI",
"LXXXVII",
"LXXXVIII",
"LXXXIX",
"XC",
"XCI",
"XCII",
"XCIII",
"XCIV",
"XCV",
"XCVI",
"XCVII",
"XCVIII",
"XCIX",
"C"];

int main() {
  setlocale(LC_ALL, "");
  bindtextdomain("guess", ".");
  textdomain("guess");
  char buf[1024];
  int left = 1;
  int right = 100;
  printf(_("Think of a number between %d and %d\n"), left, right);
  int middle = 0;
  while (left < right) {
    middle = (left+right)/2;
    printf(_("Is your number greater than %d (%s/%s): "), middle, _("yes"), _("no"));
    scanf("%s", buf);
    if (!strcmp(buf, _("yes")) || !strcmp(buf, _("y"))) {
      // yes
      left = middle+1;
    } else {
      right = middle;
    }
  }
  printf(_("Your number is %d\n"), right);
}