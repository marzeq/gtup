#include "net.h"
#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define printe(...) fprintf(stderr, __VA_ARGS__)

int main(int argc, const string* argv) {
  if (argc < 2) {
    printe("Usage: %s [mac addr]\n", argv[0]);
    return 1;
  }

  const string saddr = argv[1];
  byte* addr = parse_mac_addr(saddr);
  if (addr == NULL) {
    printe("Invalid MAC address %s\n", saddr);
    return 1;
  }

  WolSendResult res = send_wol_packet(addr);
  switch (res) {
  case WOL_SEND_OK: {
    printf("Magic packet sent to %s\n", saddr);
    return 0;
  }
  case WOL_SEND_SOCK_FAIL: {
    printe("Failed to create socket\n");
    return 1;
  }
  case WOL_SEND_SEND_FAIL: {
    printe("Failed to send magic packet\n");
    return 1;
  }
  }

  return 0;
}
