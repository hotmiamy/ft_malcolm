#include "ft_malcolm.h"

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int validate_ip(const char *ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip, &(sa.sin_addr));
}

int validate_mac(const char *mac) {
    int i = 0, s = 0;
    while (*mac) {
        if (isxdigit(*mac)) {
            i++;
        } else if (*mac == ':' && (i == 2 || i == 5 || i == 8 || i == 11 || i == 14)) {
            i++;
            s++;
        } else {
            return 0;
        }
        mac++;
    }
    return (i == 17 && s == 5);
}

void capture_arp_packets(int sockfd) {
    struct sockaddr_storage src_addr;
    socklen_t src_addr_len = sizeof(src_addr);
    uint8_t buffer[BUFF_SIZE];

    while (1) {
        ssize_t num_bytes = recvfrom(sockfd, buffer, BUFF_SIZE, 0, NULL, NULL);
        if (num_bytes == -1) {
            perror("recvfrom");
        }

        struct ethhdr *eth_header = (struct ethhdr *)buffer;
        if (ntohs(eth_header->h_proto) == ETH_P_ARP) {
            struct arp_header *arp = (struct arp_header *)(buffer + sizeof(struct ethhdr));
            if (ntohs(arp->oper) == ARPOP_REQUEST) {
                // Process ARP request
                /* printf("An ARP request has been broadcast.\n");
                printf("ARP request: who has %d.%d.%d.%d? Tell %d.%d.%d.%d\n",
                       arp->tpa[0], arp->tpa[1], arp->tpa[2], arp->tpa[3],
                       arp->spa[0], arp->spa[1], arp->spa[2], arp->spa[3]); */
            }
            if (ntohs(arp->oper) == ARPOP_REPLY) {
                printf("Mac address of request: %02x:%02x:%02x:%02x:%02x:%02x\n",
                        arp->sha[0], arp->sha[1], arp->sha[2], arp->sha[3], arp->sha[4], arp->sha[5]);
                printf("IP address of request: %d.%d.%d.%d\n", 
                        arp->spa[0], arp->spa[1], arp->spa[2], arp->spa[3]);
            }
        }
    }
}

void send_arp_reply(int sockfd, const char *src_ip, const char *src_mac, const char *target_ip, const char *target_mac) {
    uint8_t buffer[ETH_FRAME_LEN];
    struct ethhdr *eth_header = (struct ethhdr *)buffer;
    struct arp_header *arp = (struct arp_header *)(buffer + sizeof(struct ethhdr));

    // Preencher o cabeçalho Ethernet
    sscanf(target_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &eth_header->h_dest[0], &eth_header->h_dest[1], &eth_header->h_dest[2],
           &eth_header->h_dest[3], &eth_header->h_dest[4], &eth_header->h_dest[5]);
    sscanf(src_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &eth_header->h_source[0], &eth_header->h_source[1], &eth_header->h_source[2],
           &eth_header->h_source[3], &eth_header->h_source[4], &eth_header->h_source[5]);
    eth_header->h_proto = htons(ETH_P_ARP);

    // Preencher o cabeçalho ARP
    arp->htype = htons(ARPHRD_ETHER);
    arp->ptype = htons(ETH_P_IP);
    arp->hlen = 6;
    arp->plen = 4;
    arp->oper = htons(ARPOP_REPLY);
    memcpy(arp->sha, eth_header->h_source, 6);
    inet_pton(AF_INET, src_ip, arp->spa);
    sscanf(target_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &arp->tha[0], &arp->tha[1], &arp->tha[2], &arp->tha[3], &arp->tha[4], &arp->tha[5]);
    inet_pton(AF_INET, target_ip, arp->tpa);

    // Enviar o pacote ARP
    struct sockaddr_ll device;
    memset(&device, 0, sizeof(device));
    device.sll_ifindex = if_nametoindex("eth0");
    device.sll_halen = ETH_ALEN;
    memcpy(device.sll_addr, eth_header->h_dest, 6);

    if (sendto(sockfd, buffer, ETH_FRAME_LEN, 0, (struct sockaddr *)&device, sizeof(device)) == -1) {
        perror("sendto");
    } else {
        printf("ARP reply sent.\n");
    }
}

char *find_interface() {
    struct ifaddrs *ifaddr, *ifa;
    char *interface = NULL;

    if (getifaddrs(&ifaddr) == -1) {
        handle_error("getifaddrs");
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_PACKET) {
            if ((ifa->ifa_flags & IFF_UP) && (ifa->ifa_flags & IFF_RUNNING)){
                if (!(ifa->ifa_flags & IFF_LOOPBACK))
                    interface = ifa->ifa_name;
                    break;
            }
        }
    }

    freeifaddrs(ifaddr);
    return interface;
}

int main(int argc, char *argv[]) {
    char *interface = NULL;

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <source IP> <source MAC> <target IP> <target MAC>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!validate_ip(argv[1])) {
        fprintf(stderr, "Invalid source IP address: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (!validate_mac(argv[2])) {
        fprintf(stderr, "Invalid source MAC address: %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    if (!validate_ip(argv[3])) {
        fprintf(stderr, "Invalid target IP address: %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    if (!validate_mac(argv[4])) {
        fprintf(stderr, "Invalid target MAC address: %s\n", argv[4]);
        return EXIT_FAILURE;
    }

    interface = find_interface();

    if (!interface) {
        fprintf(stderr, "No interface found.\n");
        return EXIT_FAILURE;
    }

    /* int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd == -1) {
        handle_error("socket");
    }


    printf("Valid interface found: %s\n", interface);

    printf("Capturing ARP packets...\n");
    capture_arp_packets(sockfd);

    // Enviar resposta ARP depois de capturar a solicitação ARP
    send_arp_reply(sockfd, argv[1], argv[2], argv[3], argv[4]);

    close(sockfd); */
    return EXIT_SUCCESS;
}