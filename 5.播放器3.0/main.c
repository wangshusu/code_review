
#include "run.h"


#define TEST 1
#define FIFONAME "/root/1.fifo"

//��־mplayer�Ƿ��˳���ǰ�����Ĳ���,Ϊ0��ʾ���˳�,Ϊ1��ʾ�˳�
int mplayer_exit = 0;

//��־λstaΪ0��ʾ�ֶ�����,Ϊ1��ʾ�Զ�����
int sta = 0;

#if TEST


int main(int argc,char **argv)
{
    
    StarMplayer();

    Queue *  qu = get_dir_size("./4.mp4",".mp4");//������ǰĿ¼�µ� MP4 �ļ�
    Node * ptr = qu ->front;
    // char Epath[] = "mplayer -slave -quiet -input file=/root/1.fifo -zoom -x 630 -y 480  ";
    // char path[300] = {0};
    // sprintf(path,"%s%s",Epath,ptr ->data);
    // printf("path : %s\n",path); // ��һ�����ŵĽڵ�

    //��־�Ÿ������Ƿ��˳�,Ϊ0��ʾ���˳�,Ϊ1��ʾ�˳�
	int quit = 0;

    int ret;
    int fd = open(FIFONAME, O_RDWR);//�д򿪾�Ҫ�йر�
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
			//�ý���ȥִ�в���ָ���ĳ����ļ���
			ret = execlp("mplayer","-slave","-quiet","-input","file=/root/1.fifo","-zoom","-x","600","-y","480","&",ptr->data,NULL);
			if(ret == -1)
			{
				perror("execlp error");
				return -1;
			}
            exit(123);
		}
		else if( pid > 0 )//������  pid Ϊ�ӽ��̵� pid
		{
			mplayer_exit = 0;
			while(!mplayer_exit) //�ȴ��û�����
			{
				//fflush(stdin);
				//char ch = getchar();
				//1����һ�ף�2����һ�ף�3������ 5���˳�
				//ͨ��./mplayer -input cmdlist�鿴
				//4����ʾ��ͣ/����
                GetTouchDrop_Silther(&drp,20000);
                int i = Analyse_Drop_Dir(&drp);
                int cmd = -1;

                if(i == CLICK || i == DBCLICK || i == LongPress)
                {
                    Click_xy cl_xy;//(max,max) = (10,17)
                    Renew_Point_Data1(&cl_xy);

                    if(i == CLICK)//����
                    {
                        int x = cl_xy.x;
                        int y = cl_xy.y;
                        printf("(x,y) = (%d,%d) \n",cl_xy.x,cl_xy.y);

                        cmd = Renew_Point_Data2(&cl_xy);
                        printf("Reback cmd = %d\n",cmd);

                        switch(cmd)
                        {
                            
                            case 1: //��һ��
                                    ptr = ptr ->prev;printf("��һ��\n");
                                    mplayer_exit = 1;//next action��Ϊ0
                                break;
                            case 2: //��������
                                    write(fd,"seek -5\n",strlen("seek -5\n"));printf("��������\n");
                                break;
                            case 3:  //��ͣ/����
                                    write(fd,"pause\n",strlen("pause\n"));printf("��ͣ/����\n");
                                break;
                            case 4:  //������
                                break;

                            case 7: //��һ��
                                    ptr = ptr ->next;printf("��һ��\n");
                                    mplayer_exit = 1;//next action��Ϊ0 �������Ҫ������ǰ�Ĳ��Ž���
                                break;
                            case 8: //�������
                                    write(fd,"seek 5\n",strlen("seek 5\n"));printf("�������\n");
                                break;
                            case 9:  //��ͣ/����
                                    write(fd,"pause\n",strlen("pause\n"));printf("��ͣ/����\n");
                                break;
                            case 10:  //������
                                break;        

                            case 12:  //�˳�
                                    write(fd,"quit\n",strlen("quit\n"));printf("�˳�\n");
                                    mplayer_exit = 1;
                                    quit = 1;
                                break;
                        }

                        if( 1 == mplayer_exit ) //next action��Ϊ0
                        {
                            //�ɵ����ڲ��ŵ���Ƶ ����һ��
                            printf("�㱻�Ҹɵ���~\n");
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
		//waitpid(-1,NULL,0);//-1��ʾֻҪ�ȵ���������һ�����ӷ��ؾͿ��Է�
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
