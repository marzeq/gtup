#include "net.h"
#include "resolve.h"
#include "save.h"
#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define printe(...) fprintf(stderr, __VA_ARGS__)
#define seq(a1, a2) strcmp(a1, a2) == 0

#define USAGE                                                                  \
  "Usage:\n\n"                                                                 \
                                                                               \
  "gtup [mac addr/saved name]\n\n"                                             \
                                                                               \
  "gtup --save/-s [name] [mac addr]\n"                                         \
  "gtup --edit/-e [name] [new mac addr]\n"                                     \
  "gtup --delete/-d [name]\n\n"                                                \
  "gtup --list/-l\n\n"                                                         \
                                                                               \
  "gtup --help/-h\n"                                                           \
  "gtup --version/-v\n"

#define VERSION "25.02.2"

int main(int argc, const string* argv) {
  if (argc == 1) {
    printe(USAGE);
    return 1;
  }

  if (strlen(argv[1]) >= 1 && argv[1][0] == '-') {
    if (seq(argv[1], "--save") || seq(argv[1], "-s")) {
      if (argc != 4) {
        printe("Usage: gtup --save/-s [name] [mac addr]\n");
        return 1;
      }

      SaveResult res = save_device(argv[2], argv[3]);
      if (res != SAVE_RESULT_OK) {
        printe("Failed to save device");
        if (res == SAVE_RESULT_CONFLICT) {
          printe(
              ", device %s already exists. Use --edit to change its' address",
              argv[2]);
        }
        printe("\n");
        return 1;
      }
      return 0;
    } else if (seq(argv[1], "--edit") || seq(argv[1], "-e")) {
      if (argc != 4) {
        printe("Usage: gtup --edit/-e [name] [new mac addr]\n");
        return 1;
      }

      SaveResult res = modify_saved_device(argv[2], argv[3]);
      if (res != SAVE_RESULT_OK) {
        printe("Failed to edit device\n");
        return 1;
      }
      return 0;
    } else if (seq(argv[1], "--delete") || seq(argv[1], "-d")) {
      if (argc != 3) {
        printe("Usage: gtup --delete/-d [name]\n");
        return 1;
      }

      SaveResult res = delete_saved_device(argv[2]);
      if (res != SAVE_RESULT_OK) {
        printe("Failed to delete device\n");
        return 1;
      }
      return 0;
    } else if (seq(argv[1], "--list") || seq(argv[1], "-l")) {
      string* devices = list_saved_devices();
      if (devices) {
        for (size_t i = 0; devices[i]; i++) {
          printf("%s\n", devices[i]);
          free(devices[i]);
        }
        free(devices);
      }
      return 0;
    } else if (seq(argv[1], "--help") || seq(argv[1], "-h")) {
      printf(USAGE);
      return 0;
    } else if (seq(argv[1], "--version") || seq(argv[1], "-v")) {
      printf(VERSION "\n");
      return 0;
    } else {
      printe("Unknown option %s\n", argv[1]);
      return 1;
    }
  }

  const string saddr = argv[1];
  const byte* addr = parse_mac_addr(saddr);
  if (addr == NULL) {
    const string ns = resolve_saved_device(saddr);
    if (ns == NULL) {
      printe("No such saved device %s\n", saddr);
      return 1;
    }

    addr = parse_mac_addr(ns);
    if (addr == NULL) {
      printe("Invalid MAC address %s for device %s\n", ns, saddr);
      return 1;
    }
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
