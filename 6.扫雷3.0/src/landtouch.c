#include "landtouch.h"

/*
    touch_Open:�����ļ��򿪺���
*/
void touch_Open(void)
{
    /* �ȴ���Ļ�ļ� --> �򿪾�Ҫ�ر� */
    fp_open = open(TouchPath,O_RDONLY | O_NONBLOCK);    /* ����ʹ�õ��Ƿ����� */
    if(fp_open == -1)
    {
        perror("open TouchPath failed");
        return;
    }
}

/*
    touch_Close:�����ļ��رպ���
*/
void touch_Close(void)
{
    close(fp_open);
}


/*
    @GetTouchDrop_Silther():��ȡ��ĺ���
        @Dorp * drp:ȫ�ֱ��� drp �ĵ�ַ
        @int while_PassTime :��ȡ���� ���� (MAX = u32 /1000)
        ����ֵ:
            (int) -1:����
            (int)  0:�ɹ�
*/
int GetTouchDrop_Silther(Dorp *drp, int while_PassTime)
{
    /* ��ʼ�� ȫ��Ϊ -1 */
    drp ->end_x = drp ->start_x = -1; //x��
    drp ->end_y = drp ->start_y = -1; //y��
    drp ->end_time = -1;
    drp ->start_time = -1;
    drp ->pass_time = -1;
    
    int flag_while = -1;
    int flag_x = -1;
    int flag_y = -1;
    struct input_event ev;      //���� ��Ļ���ݵ�
    struct timeval ti;          //��ȡʱ������
    struct timeval star_time;   //���濪ʼʱ��
    struct timeval end_time;    //��������ʱ��
    
    /* -----��ʼ��ȡ----- */
    while (1)
    {
        if(-1 == flag_while)    /* ����ǵ�һ�ν��� */
        {
            flag_while = 1;     /* ��־�޸� */
            gettimeofday(&ti, NULL);     /* ��ȡ��ʼʱ�� */
            while_StartTime = while_EndTime = ti.tv_usec / 1000 +  ti.tv_sec * 1000;
        }

        //3.��ȡ��Ļ������<��ȡ�� ev ��ȥ>
        int fp_rd = read(fp_open,&ev,sizeof(ev));

        if(fp_rd == -1 && while_EndTime - while_StartTime <= while_PassTime)//���û�ж����ļ�
        {
            usleep(50);
            gettimeofday(&ti,NULL);     /* ��ȡʱ�� */
            while_EndTime = ti.tv_usec / 1000 +  ti.tv_sec * 1000;
            Show_Time();
            continue;       /* һֱ�� ֱ�������ļ�����������Ϊֹ */
        }
        else if(while_EndTime - while_StartTime > while_PassTime)
        {
            printf("read is down!\n");
            return -1;
        }   
        else if(fp_rd != -1)    /* ��������ļ���,���ǾͿ�ʼ�ж� */
        {
                    
            if(ev.type = EV_ABS)//<1>�����ǻ����ж�
            {
                if(ev.code == ABS_X && ev.value > 0)    /* �ȴ�x������ >0 ���ҲŽ��� һ�㲻�ᴥ�������� */
                {
                    if(-1 == flag_x)//����Ǹտ�ʼ������
                    {
                        gettimeofday(&star_time,NULL);//��ȡ��ʼʱ��
                        drp ->start_time = star_time.tv_usec / 1000 +  star_time.tv_sec * 1000;
                        drp ->start_x = ev.value * 800 / 1024;
                        flag_x = 0;
                    }
                    else
                    {
                        drp ->end_x = ev.value * 800 / 1024; 
                    }
                }
                else if(ev.code == ABS_Y && ev.value > 0)
                {
                    if(-1 == flag_y)//����Ǹտ�ʼ������
                    {
                        
                        drp ->start_y = ev.value * 480 /600;
                        flag_y = 0;
                    }
                    else
                    {
                        drp ->end_y = ev.value * 480 /600;
                    }
                }
                else if(ev.code == BTN_TOUCH && ev.value == 0)//����뿪��Ļ
                {
                    gettimeofday(&end_time,NULL);//��ȡ����ʱ��
                    drp ->end_time = end_time.tv_usec / 1000 +  end_time.tv_sec * 1000;
                    drp ->pass_time = drp ->end_time - drp ->start_time;
                    break;
                }
            }
        }
    

    }
    return 0;
}

/*
    Analyse_Drop_Dir(Dorp * drp):������������ ����� ����/˫�� ��ȫ�ֱ��� cl �б��� (x,y)
        @Dorp * drp:struct Drop(�ṹ��)
        ����ֵ: ����enum Move_Dir����ķ����ж�
*/
enum Move_Dir Analyse_Drop_Dir(Dorp * drp)
{
    last_ms = 9999999;
    int dire = -1;  /* ������ ��󷵻� */

    /* ----��ʼ�ж�----- */
    if (drp ->end_x != -1 && 
       drp ->end_y != -1 &&
       drp ->start_x != -1 &&
       drp ->start_y != -1 )    /* ������ݲ����������� */
    {
        if ((drp ->end_y - drp ->start_y) >= 0 
        && abs(drp ->end_x - drp ->start_x) <= abs(drp ->end_y - drp ->start_y))//�»� ������ y���� > x���� 
        {
            printf("down\n");
            dire = DOWN;
        }
        else if ((drp ->end_y - drp ->start_y) < 0 
        && abs(drp ->end_x - drp ->start_x) <= abs(drp ->end_y - drp ->start_y))//�ϻ� ������ y���� > x���� 
        {
            printf("up\n");
            dire = UP;
        }
        else if ((drp ->end_x - drp ->start_x) > 0 
        && abs(drp ->end_x - drp ->start_x) > abs(drp ->end_y - drp ->start_y))//��  ������ x���� > y����
        {
            printf("right\n");
            dire =  RIGHT;
        }
        else if ((drp ->end_x - drp ->start_x) <= 0 
        && abs(drp ->end_x - drp ->start_x) > abs(drp ->end_y - drp ->start_y))//�һ�  ������ x���� > y����
        {
            printf("lift\n");
            dire = LIFT;
        }
        else
        {
            printf("Analyse_Drop_Dir ERROR\n");
        }
    }
    else if(drp ->end_x == -1 &&
            drp ->end_y == -1 &&
            drp ->start_x != -1 &&
            drp ->start_y != -1 &&
            drp ->pass_time <= 300)     /* �����ж� */
    {
        printf("CLICK\n");
        dire = CLICK;
        cl.dorp_x = drp ->start_x;
        cl.dorp_y = drp ->start_y;
        
        last_ms = drp ->end_time;
        GetTouchDrop_Silther(drp,500); /* ˫���ж� */

        if(drp ->start_time - last_ms < 500)
        {
            printf("DBCLICK\n");
            dire = DBCLICK;
        }
    }
    else if(drp ->end_x == -1 && 
            drp ->end_y == -1 &&
            drp ->start_x != -1 &&
            drp ->start_y != -1 &&
            drp ->pass_time > 500)  /* �����ж� */
    {
        cl.dorp_x = drp ->start_x;
        cl.dorp_y = drp ->start_y;
        printf("LongPress\n");
        dire = LongPress;
    }

    /* ������Ҫ��� */
    drp ->end_x = -1;
    drp ->end_y = -1;
    drp ->start_x = -1;
    drp ->start_y = -1;
    drp ->pass_time = -1;

    printf("\n");
    return dire;

}