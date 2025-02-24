#include "net.h"
#include "shared.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

WolSendResult send_wol_packet(const byte* mac_addr) {
  int sock;
  struct sockaddr_in server;
  byte packet[PACK_SZ];

  memset(packet, 0xFF, 6);
  for (int i = 0; i < 16; i++) {
    memcpy(packet + 6 + i * 6, mac_addr, 6);
  }

  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0) {
    return WOL_SEND_SOCK_FAIL;
  }

  int broadcast_enable = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_enable,
                 sizeof(broadcast_enable)) < 0) {
    close(sock);
    return WOL_SEND_SOCK_FAIL;
  }

  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(WOL_PORT);
  server.sin_addr.s_addr = inet_addr(BROADCAST_IP);

  if (sendto(sock, packet, PACK_SZ, 0, (struct sockaddr*)&server,
             sizeof(server)) < 0) {
    close(sock);
    return WOL_SEND_SEND_FAIL;
  }

  close(sock);
  return WOL_SEND_OK;
}
