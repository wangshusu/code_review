#include <iostream>
using namespace std;
#include "dianhua.h"

int main()
{
    phonebook *PB = phonebook_Init();
    while(1)
    {
        help_choice();
        int cmd_one;
        cout << "ÇëÊäÈëÃüÁî£º";
        cin >> cmd_one;
        switch (cmd_one)
        {
        case 1:
            Add_Linkman(PB,PB ->phone_num);
            break;
        case 2:
            Show_All_Linkman(PB);
            break;
        case 3:
            break;
        case 4:
            break;
        
        default:
            break;
        }
        system("pause");
        system("cls");
    }
    
}//