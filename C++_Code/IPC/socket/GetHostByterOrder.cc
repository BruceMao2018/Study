/* 0 - Little endinan, 1 - Big endinan
 * Little endinan, A low address stored at low address in memory, and a high address stored in memory.
 */

#include <stdlib.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;
int dGetHostByteOrder()
{
    uint32_t a = 0x12345678;
    uint8_t *p = (uint8_t *)(&a);  
    if(*p==0x78)
    {
	cout << "This is Little endinan" << endl;
    	return 0;
    }
    else
    {
	cout << "This is Big endinan" << endl;
    	return 1;
    }
}

int main()
{
	dGetHostByteOrder();

	return 0;
}
