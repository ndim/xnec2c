#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "i18n.h"

typedef struct {
  char *locale;
  char *expected;
} pair;

/* TRANSLATORS: This is just the abstract word "yes" to test the
 * translation framework with a few selected locales like C,
 * en_US.UTF-8, de_DE.UTF-8. */
char *untranslated = N_("yes");

pair pairs[] = {
  {"C",           "yes"},
  {"en_US.UTF-8", "yes"},
  /* {"en_US.utf8",  "yes"},   does not work on macOS  */
  /* {"en_US",       "yes"},   does not work on Ubuntu */
  /* {"en",          "yes"},   does not work on Fedora */

  {"de_DE.UTF-8", "ja" },
  /* {"de_DE.utf8",  "ja" },   does not work on macOS  */
  /* {"de_DE",       "ja" },   does not work on Ubuntu */
  /* {"de",          "ja" },   does not work on Fedora */

  /* {"xy_ZT.UTF-8", "xyz"},   shall not work anywhere */
  {NULL, NULL},
};

int main(void)
{
#ifndef ENABLE_NLS

  /* exit code 77 means "we have skipped this test" */
  return 77;

#else

  if (bindtextdomain(PACKAGE, LOCALEDIR) == NULL) {
    perror("bindtextdomain(\"" PACKAGE "\", \"" LOCALEDIR "\")");
    exit(EXIT_FAILURE);
  }
  printf("localedir for %s set to %s\n", PACKAGE, LOCALEDIR);

  if (textdomain(PACKAGE) == NULL) {
    perror("textdomain(\""PACKAGE"\")");
    exit(EXIT_FAILURE);
  }

#define FMTSTR "  %-6s  %-12s  %-6s  %-6s  %-8s  %-7s\n"
  printf(FMTSTR, "result", "locale", "orig", "expect", "dgettext", "gettext");

  int total = 0;
  int setlocale_errors = 0;
  int dgettext_errors = 0;
  int gettext_errors = 0;

  for (pair *p=pairs; p->locale; p++) {
    total++;

    if (NULL == setlocale(LC_ALL, p->locale)) {
      printf("       failed to setlocale(LC_MESSAGES, \"%s\")\n", p->locale);
      setlocale_errors++;
      continue;
    }

    char *dgettext_is = dgettext(PACKAGE, untranslated);
    char *gettext_is  = gettext(untranslated);

    char *result = "good";
    if (strcmp(p->expected, dgettext_is) != 0) {
      result = "FAIL";
      dgettext_errors++;
    }
    if (strcmp(p->expected, gettext_is) != 0) {
      result = "FAIL";
      gettext_errors++;
    }

    printf(FMTSTR,
	   result, p->locale, untranslated, p->expected, dgettext_is, gettext_is);
  }

  printf("total: %d, setlocale_errors: %d, dgettext_errors: %d, gettext_errors: %d\n",
	 total, setlocale_errors, dgettext_errors, gettext_errors);

  if (setlocale_errors > 0) {
    return 3;
  } else if (dgettext_errors > 0) {
    return 2;
  } else if (gettext_errors > 0) {
    return 1;
  } else {
    return 0;
  }
#endif
}
