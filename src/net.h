#ifndef _NET_H
#define _NET_H 1

#include "shared.h"

typedef enum {
  WOL_SEND_OK,
  WOL_SEND_SOCK_FAIL,
  WOL_SEND_SEND_FAIL
} WolSendResult;

#define WOL_PORT 7
#define PACK_SZ 102 // 6b + 16 * 6b
#define BROADCAST_IP "255.255.255.255"

WolSendResult send_wol_packet(const byte* mac_addr);

#endif // _NET_H
