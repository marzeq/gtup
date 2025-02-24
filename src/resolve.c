#include "resolve.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const byte* parse_mac_addr(const string saddr) {
  byte* addr = malloc(6 * sizeof(byte));
  if (!addr) {
    return NULL;
  }

  if (sscanf(saddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &addr[0], &addr[1],
             &addr[2], &addr[3], &addr[4], &addr[5]) != 6) {
    free(addr);
    return NULL;
  }

  return addr;
}
