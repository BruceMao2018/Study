#include <string.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	int loop = 10;
	for(int i = 0; i < 10; i++)
	{
		cout << "i = " << i << endl;
		if(i == 5)
		{
		cout << "inside i = " << i << endl;
			continue;
		}
		cout << "finished " << i << endl;
	}

	return 0;
}	
