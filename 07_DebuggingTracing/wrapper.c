#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>

int unlinkat(int dirfd, const char *pathname, int flags) {
  printf("Called unlinkat!\n");
  if (strstr(pathname, "FIX") != NULL) {
    printf("Found FIX in %s\n", pathname);
    return EPERM;
  }
  int (*original_unlink)(int dirfd, const char *pathname, int flags);
  original_unlink = dlsym(RTLD_NEXT, "unlinkat");
  return original_unlink(dirfd, pathname, flags);
}
