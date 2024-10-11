#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int is_valid_ipv6(const char *ip_address) {
    struct in6_addr addr;
    return inet_pton(AF_INET6, ip_address, &addr) == 1;
}

int main() {
    const char *ipv6_address = "2000::100";
    
    if (is_valid_ipv6(ipv6_address)) {
        printf("%s 是有效的 IPv6 地址\n", ipv6_address);
    } else {
        printf("%s 不是有效的 IPv6 地址\n", ipv6_address);
    }

    return 0;
}
