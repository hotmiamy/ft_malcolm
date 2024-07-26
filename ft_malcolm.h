#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <errno.h>
#include <net/if_arp.h> 
#include <ctype.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

void handle_error(const char *msg);
int validate_ip(const char *ip);
int validate_mac(const char *mac);
void capture_arp_packets(int sockfd);

struct arp_header {
    uint16_t htype;    // Hardware type
    uint16_t ptype;    // Protocol type
    uint8_t hlen;      // Hardware address length
    uint8_t plen;      // Protocol address length
    uint16_t oper;     // Operation (request or reply)
    uint8_t sha[6];    // Sender hardware address (MAC)
    uint8_t spa[4];    // Sender protocol address (IP)
    uint8_t tha[6];    // Target hardware address (MAC)
    uint8_t tpa[4];    // Target protocol address (IP)
};

#endif