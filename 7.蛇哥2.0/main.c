#include "run.h"

#define TEST 1

int main()
{
        #if TEST
        Run_Snake();
        #else
        Star_En();
        while (1)
        {
                GetTouchDrop_Silther(&drp,10000);
                Analyse_Drop_Dir(&drp);
        }
        End_En();
        
        #endif
    
}

