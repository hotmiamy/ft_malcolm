#include "ft_malcolm.h"

static unsigned char hex_char_to_byte(char c) {
    c = ft_tolower((unsigned char)c);
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0; // Caractere inválido (opcional: tratar erro)
}

int parse_mac_safe(const char *mac_str, unsigned char *output) {
    for (int i = 0; i < 6; i++) {
        if (!ft_isxdigit((unsigned char)mac_str[0]) || 
            !ft_isxdigit((unsigned char)mac_str[1])) {
            return -1;
        }
        
        output[i] = (hex_char_to_byte(mac_str[0]) << 4) | 
                     hex_char_to_byte(mac_str[1]);
        mac_str += 2;
        
        if (i < 5) {
            if (*mac_str != ':') return -1;
            mac_str++;
        }
    }
    return 0;
}

void handle_error() {
    printf("Error: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

int validate_ip(const char *ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip, &(sa.sin_addr));
}

int validate_mac(const char *mac) {
    int i = 0, s = 0;
    while (*mac) {
        if (ft_isxdigit(*mac)) {
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