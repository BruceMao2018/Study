#include "create.hh"
using namespace std;

#define NUM 5

/*
void* Hello(void *argv)
{
	cout << "Hello, My process ID is: " << getpid() << " Thread ID: " << pthread_self() << endl;
}
*/

//using namespace std;

void* Hello::say_hello(void *id)
{
	int *p = (int*)id;
	cout << "New thread: " << pthread_self() << " i = " << *p << endl;
	//usleep(2000);
	int status = 100;
	pthread_exit(p);
	//pthread_exit((void*)status);
}


int main()
{
	pthread_t t[NUM];
	int i = 0;
	try{
		for (int i = 0; i < NUM; i++)
		{
			if(pthread_create(&t[i], NULL, Hello::say_hello, &i) != 0)
			{
				throw MyError("create thread error");
			}

			//sleep(1);
			
		}
	}catch (MyError &e)
	{
		cout << "Error: " << e.what() << endl;
		pthread_exit(NULL);
		exit(-1);
	}

	//usleep(5000);
	void *res;
	for (int i =0; i < NUM; i++)
	{
		pthread_join(t[i], &res);
		cout << "result: " << *((int*)res)<< endl;
		//cout << "result: " << (int)res<< endl;
	}

	cout << "Main thread, Process ID is: " << getpid() << " Thread ID: " << pthread_self() << endl;

	//pthread_exit(NULL);
	return 0;
}
