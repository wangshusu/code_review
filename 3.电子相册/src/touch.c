#include "touch.h"

/*
*    @enum Move_Dir GetMove_Dir():��ȡ��Ļ��������
*    @��������
*    ����ֵ:
*        ����enum Move_Dir����ķ����ж�
*/
enum Move_Dir GetMove_Dir(Node *ptr)
{
    int pf_mv = open(Touch_Path,O_RDONLY | O_NONBLOCK);

    if(pf_mv == -1)
    {
        perror("read Touch_Path failed\n");
        return DIR_ERROR;
    }
    // RIGHT,//�һ�
    // LIFT,//��
    // UP,//�ϻ�
    // DOWN,//�»�
    // DBCLICK,//˫��
    // CLICK,//����
    // DIR_ERROR;//����

    int x0 = -1 , y0 = -1 ;//�����¼���ʼ����
    int x1 = -1 , y1 = -1 ;//�����¼���ֹ����

    int x2 = -1 , y2 = -1 ;//�����¼���ʼ����
    int x3 = -1 , y3 = -1 ;//�����¼���ֹ����

    struct input_event ev;
    // printf("BTN_TOUCH = %#x\n",BTN_TOUCH);//0x14a
    // printf("EV_KEY = %#x\n",EV_KEY);//0x1
    // printf("EV_ABS = %#x\n",EV_ABS);//0x3
    
    int falgx = -1;
    int falgy = -1;
    int flagcircle = -1;

    time_t start_seconds = time(NULL);
    time_t end_seconds = 0;

    while(1)
    {
        int ret = read(pf_mv,&ev,sizeof(ev));
        if(ret == -1)
        {
            printf("end_seconds - start_seconds = %d\n",end_seconds - start_seconds);
            end_seconds = time(NULL);

            if((end_seconds - start_seconds) >= 10)
            {
                if((end_seconds - start_seconds) % 3 == 0)
                {
                    ptr = ptr ->next;
                    showphoto((char *)ptr ->data,0,0,2);
                }
            }

            continue;
        }

        if( ev.type = EV_ABS )//����ǻ����¼�
        {
            if(ev.code == ABS_X && ev.value != 0)//x��������¼�
            {
                if(-1 == falgx)
                {
                    x0 = ev.value;
                    falgx = 1;
                }
                else
                {
                    x1 = ev.value;
                }
                
            }
            if(ev.code == ABS_Y && ev.value != 0)//y��������¼�
            {
                if(-1 == falgy)
                {
                    y0 = ev.value;
                    falgy = 1;
                }
                else
                {
                    y1 = ev.value;
                }
                
            }
            if(ev.code == 0x14a && ev.value == 0)
            {
                //printf("tanqi\n");
                break;
            }
        }

    }
    Decide_Dir(x0,y0,x1,y1);  
}

/*
    @enum Move_Dir Decide_Dir:�жϷ�����
    @���� ��ʼ �� ��ֹ ��(x,y)����
    ����ֵ:
        ����enum Move_Dir����ķ����ж�
*/
enum Move_Dir Decide_Dir(int start_x,int start_y,int end_x,int end_y)
{
    if((end_y - start_y) >= 0 && abs(end_x - start_x) <= abs(end_y - start_y))//�»� ������ y���� > x���� 
    {
        printf("down\n");
        return DOWN;
    }
    else if((end_y - start_y) < 0 && abs(end_x - start_x) <= abs(end_y - start_y))//�ϻ� ������ y���� > x���� 
    {
        printf("up\n");
        return UP;
    }
    else if((end_x - start_x) > 0 && abs(end_x - start_x) > abs(end_y - start_y))//��  ������ x���� > y����
    {
       printf("right\n");
        return  RIGHT;
    }
    else if((end_x - start_x) <= 0 && abs(end_x - start_x) > abs(end_y - start_y))//�һ�  ������ x���� > y����
    {
        
         printf("lift\n");
        return LIFT;
    }
    printf("\n");
}

