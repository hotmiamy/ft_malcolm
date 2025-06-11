#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <ifaddrs.h>
#include "libft/libft.h"

#define ARPOP_REQUEST 1    // Requisição ARP
#define ARPOP_REPLY 2      // Resposta ARP
#define ARPHRD_ETHER 1     // Tipo Ethernet
#define BUFF_SIZE 42

void handle_error();
int validate_ip(const char *ip);
int validate_mac(const char *mac);
int parse_mac_safe(const char *mac_str, unsigned char *output);
void capture_arp_packets(int sockfd, char *source_ip, char *target_ip);
void send_arp_reply(int sockfd, const char *src_ip, const char *src_mac, const char *target_ip, const char *target_mac, const char *iface_name);


typedef struct {
    struct ether_header ether_header;
    struct ether_arp arp;
    uint8_t padding[18];
} arp_packet;

#endif