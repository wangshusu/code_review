#include "landrun.h"
//                                      printf("%s %d\n",__FUNCTION__,__LINE__);


int main()
{
    int GAME = 0;//管理游戏退出
    while(0 == GAME)
    {
         GAME = Run_LandMine();
    }
    return 0;
}
