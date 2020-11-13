#include <stdio.h>
#include <libintl.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define _(STRING) gettext(STRING)

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