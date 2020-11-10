#include<iostream>
#include<conio.h>
#include<Windows.h>
using namespace std;
int main()
{
    //Clear screen
    system("CLS");
    //start the timer at zero
    int MS = 0;
    cout << "\n\t\t Score";
    cout << "\n\t\t milliseconds:";
    cout << "\n\t\t " << MS;
    cout << "\n\t\t press any key to start";
    //checks when a key is pressed
    _getch();
    //while key is not pressed keep looping
    while (!_kbhit())
    {
        MS++;
        Sleep(100);
       //keep clearing the screen and update
        system("CLS");
        cout << "\n\t\t Score";
        cout << "\n\t\t seconds";
        cout << "\n\t\t " << MS;
        cout << "\n\t\t press any key to stop";
    }
    
    _getch();
    cout << "\n\t\t the score after pausing is";
    cout << "\n\t\t " << MS;
    _getch();
    return 0;
    //once key is pressed show score
}
