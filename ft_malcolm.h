#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

#define ARPOP_REQUEST 1    // Requisição ARP
#define ARPOP_REPLY 2      // Resposta ARP
#define ARPHRD_ETHER 1     // Tipo Ethernet
#define BUFF_SIZE 42

void handle_error(const char *msg);
int validate_ip(const char *ip);
int validate_mac(const char *mac);
void capture_arp_packets(int sockfd, char *target_ip, char *target_mac);

struct arp_packet {
    struct ether_header ether_header;
    struct ether_arp arp;
};

#endif