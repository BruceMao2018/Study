#include <arpa/inet.h>
#include <stdio.h>

int main(void)
{
        unsigned int bytes = 0x12345678;
        const char* p = (const char*)&bytes;
        printf("%x_%x_%x_%x\n", p[0], p[1], p[2], p[3]);

        unsigned int netbytes = htonl(bytes);
        p = (const char*)&netbytes;
        printf("%x_%x_%x_%x\n", p[0], p[1], p[2], p[3]);
        
        return 0;
}
