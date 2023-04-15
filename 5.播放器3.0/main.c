
#include "run.h"


#define TEST 1
#define FIFONAME "/root/1.fifo"

//标志mplayer是否退出当前歌曲的播放,为0表示不退出,为1表示退出
int mplayer_exit = 0;

//标志位sta为0表示手动播放,为1表示自动播放
int sta = 0;

#if TEST


int main(int argc,char **argv)
{
    
    StarMplayer();

    Queue *  qu = get_dir_size("./4.mp4",".mp4");//检索当前目录下的 MP4 文件
    Node * ptr = qu ->front;
    // char Epath[] = "mplayer -slave -quiet -input file=/root/1.fifo -zoom -x 630 -y 480  ";
    // char path[300] = {0};
    // sprintf(path,"%s%s",Epath,ptr ->data);
    // printf("path : %s\n",path); // 第一个播放的节点

    //标志着父进程是否退出,为0表示不退出,为1表示退出
	int quit = 0;

    int ret;
    int fd = open(FIFONAME, O_RDWR);//有打开就要有关闭
    if(fd == -1)
    {
        printf("\n\n");
        perror("Open fifo fail\n");
        printf("\n\n");
        return -1;
    }

    while(1)
	{
		pid_t pid = fork();
        if(pid == -1)
        {
            printf("error !\n");
        }
		if( 0 == pid)
		{
			//让进程去执行参数指定的程序文件。
			ret = execlp("mplayer","-slave","-quiet","-input","file=/root/1.fifo","-zoom","-x","600","-y","480","&",ptr->data,NULL);
			if(ret == -1)
			{
				perror("execlp error");
				return -1;
			}
            exit(123);
		}
		else if( pid > 0 )//父进程  pid 为子进程的 pid
		{
			mplayer_exit = 0;
			while(!mplayer_exit) //等待用户输入
			{
				//fflush(stdin);
				//char ch = getchar();
				//1：下一首，2：上一首，3：结束 5：退出
				//通过./mplayer -input cmdlist查看
				//4：表示暂停/播放
                GetTouchDrop_Silther(&drp,20000);
                int i = Analyse_Drop_Dir(&drp);
                int cmd = -1;

                if(i == CLICK || i == DBCLICK || i == LongPress)
                {
                    Click_xy cl_xy;//(max,max) = (10,17)
                    Renew_Point_Data1(&cl_xy);

                    if(i == CLICK)//单击
                    {
                        int x = cl_xy.x;
                        int y = cl_xy.y;
                        printf("(x,y) = (%d,%d) \n",cl_xy.x,cl_xy.y);

                        cmd = Renew_Point_Data2(&cl_xy);
                        printf("Reback cmd = %d\n",cmd);

                        switch(cmd)
                        {
                            
                            case 1: //上一首
                                    ptr = ptr ->prev;printf("上一个\n");
                                    mplayer_exit = 1;//next action不为0
                                break;
                            case 2: //后退五秒
                                    write(fd,"seek -5\n",strlen("seek -5\n"));printf("后退五秒\n");
                                break;
                            case 3:  //暂停/继续
                                    write(fd,"pause\n",strlen("pause\n"));printf("暂停/继续\n");
                                break;
                            case 4:  //音量加
                                break;

                            case 7: //下一首
                                    ptr = ptr ->next;printf("下一个\n");
                                    mplayer_exit = 1;//next action不为0 这里就需要结束当前的播放进程
                                break;
                            case 8: //快进五秒
                                    write(fd,"seek 5\n",strlen("seek 5\n"));printf("快进五秒\n");
                                break;
                            case 9:  //暂停/继续
                                    write(fd,"pause\n",strlen("pause\n"));printf("暂停/继续\n");
                                break;
                            case 10:  //音量减
                                break;        

                            case 12:  //退出
                                    write(fd,"quit\n",strlen("quit\n"));printf("退出\n");
                                    mplayer_exit = 1;
                                    quit = 1;
                                break;
                        }

                        if( 1 == mplayer_exit ) //next action不为0
                        {
                            //干掉正在播放的视频 放下一个
                            printf("你被我干掉了~\n");
                            kill(pid, 9 );// SIGKILL - 9
                        }
                        if(quit)
                        {
                            exit(0);
                        }
                    }
                }
				
					
			}
			
		}
		//waitpid(-1,NULL,0);//-1表示只要等到它的任意一个孩子返回就可以返
	}
    close(fd);
    return 0;
}
#else
void Show_Tool_picture()
{
    display_bmp("./2.mplayerbmp/1.bmp", arr_x[0],arr_y[0], 0);
    display_bmp("./2.mplayerbmp/2.bmp", arr_x[0],arr_y[1], 0);
    display_bmp("./2.mplayerbmp/3.bmp", arr_x[0],arr_y[2], 0);

    display_bmp("./2.mplayerbmp/7.bmp", arr_x[1],arr_y[0], 0);
    display_bmp("./2.mplayerbmp/8.bmp", arr_x[1],arr_y[1], 0);
}

int main(int argc , char ** argv)
{
    lcd_open();

    Divide_rectangle();
    Show_Tool_picture();

	lcd_close();
}

#endif
