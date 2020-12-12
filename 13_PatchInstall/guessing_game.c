#include <stdio.h>
#include <libintl.h>
#include <locale.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

#define _(STRING) gettext(STRING)

int main(int argc, char **argv) {
  #ifdef LOCALEDIR
  char *dir = LOCALEDIR;
  #else
  char *dir = dirname(realpath(argv[0], NULL));
  #endif
  setlocale(LC_ALL, "");
  bindtextdomain("guess", dir);
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