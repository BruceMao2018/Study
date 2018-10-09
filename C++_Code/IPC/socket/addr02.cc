#include <arpa/inet.h>
#include <stdio.h>
int main(void)
{
        char dotaddr[] = "192.168.8.128";
        struct in_addr ipaddr;

        inet_pton(AF_INET, dotaddr, (void*)&ipaddr);

        ipaddr.s_addr = ntohl(ipaddr.s_addr); //convert the network address to host address
        printf("addr = %x\n", ipaddr.s_addr);

        ipaddr.s_addr = htonl(ipaddr.s_addr);
        inet_ntop(AF_INET, (void*)&ipaddr, dotaddr, 16); //convert network address to char * address
        printf("addr = %s\n", dotaddr);

        return 0;
}        
