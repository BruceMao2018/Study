#pragma warning(disable:4996)  
  
#include <iostream>  
#include <conio.h>  
#include <time.h>  
  
#include "Graphic.h"  
#include "MainTank.h"  
  
using namespace std;  
  
void main()  
{  
    Graphic::Create();  
  
    MainTank mainTank;  
  
    bool loop = true;  
    bool skip = false;  
    while (loop)  
    {  
        if (kbhit())  
        {  
            int key = getch();  
  
            switch (key)  
            {  
		//UP
		case 72:  
                mainTank.SetDir(Dir::UP);  
                break;

		case 80:   
                mainTank.SetDir(Dir::DOWN);  
                break;

		case 75:   
                mainTank.SetDir(Dir::LEFT);  
                break; 
	
		case 77:   
                mainTank.SetDir(Dir::RIGHT);  
                break;  

            	case 224:
		break;
		case 27:  
                loop = false;  
                break;

		case 32:  
                break;

		case 13:  
                if (skip)  
                    skip = false;  
                else  
                    skip = true;  
                break;  
            default:   
                break;  
            }  
        }  
  
        if (!skip)  
        {  
            cleardevice();  
  
            mainTank.Move();  
            mainTank.Display();  
        }  
  
        Sleep(200);  
    }  
  
    Graphic::Destroy();  
}
