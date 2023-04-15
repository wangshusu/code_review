#include "touch.h"

/************** ȫ�ֱ����� **********/
int fp_open;//��Ļ�ļ�ָ��
struct Dorp drp;
int movedir = 0;

//touch ����׼������
void Star_touch(void)
{
    //1.�ȴ���Ļ�ļ� --> �򿪾�Ҫ�ر�
    fp_open = open(TouchPath,O_RDONLY | O_NONBLOCK);//����ʹ�õ��Ƿ�����
    if(fp_open == -1)
    {
        perror("open TouchPath failed");
        close(fp_open);
        return;//������NULLֵ
    }
}

//touch ��Դ����
void End_touch(void)
{
    close(fp_open);//û�ص�����ϵͳ�����ǹذ�
}



/*
    @GetTouchDrop_Silther():��ȡ��ĺ���
    @Dorp * drp:ȫ�ֱ��� drp �ĵ�ַ
    @int while_PassTime :��ȡ���� ���� (MAX = u32 /1000)
    ����ֵ:
        (int) -1:����
        (int)  0:�ɹ�
*/
int GetTouchDrop_Silther(Dorp * drp , int while_PassTime)
{
    
    //2.��ʼ��
    //memset(drp,0,sizeof(Dorp));
    drp ->end_x = drp ->start_x = -1; //x��
    drp ->end_y = drp ->start_y = -1; //y��
    drp ->end_time = -1;
    drp ->start_time = -1;
    drp ->pass_time = -1;
    
    int flag_while = -1;
    int flag_x = -1; //�ж��Ƿ��� start
    int flag_y = -1;
    struct input_event ev; //����ṹ������������ ��Ļ���ݵ�
    struct timeval while_time;//�������� ѭ����ʼ��ʱ��
    struct timeval star_time; //= calloc(1,sizeof(struct timeval));//����ṹ������������ ʱ�����ݵ�
    struct timeval end_time; //= calloc(1,sizeof(struct timeval));//����
    int while_EndTime = 0;

    //����ʹ�õ� timeval �ṹ����
    //tv_sec ���������
    //tv_usec �������΢�� 1000000 ΢�� = 1��
    //���ﴫ��� while_PassTime �ĵ�λ �Ǻ���
    
    while (1)
    {
        if(-1 == flag_while)//����ֻ�ڽ��뺯����ʱ���ȡһ�� ��֤���ǵ� while_StartTime ��ȡ�ɹ� ���һ�ȡһ��
        {
            flag_while = 1;//��־�޸�

            gettimeofday(&while_time,NULL);//��ȡ��ʼʱ��
            while_StartTime = while_EndTime = while_time.tv_usec / 1000 +  while_time.tv_sec * 1000;
        }

        //3.��ȡ��Ļ������<��ȡ�� ev ��ȥ> --- ����ʹ�õ��Ƿ������ķ�ʽ
        int fp_rd = read(fp_open,&ev,sizeof(ev));
        if(fp_rd == -1 && while_EndTime - while_StartTime <= while_PassTime)//�ڹ涨��ʱ���ڳ����Ķ��ļ�
        {
            usleep(50);//��λ�� ΢�� = ǧ��֮һ����  //������ͣ 2 ����

            gettimeofday(&while_time,NULL);//��ȡʱ��
            while_EndTime = while_time.tv_usec / 1000 +  while_time.tv_sec * 1000;//���� ������ʱ��

            continue;//�������һֱ�� ֪�������ļ�����������Ϊֹ
        }
        else if(while_EndTime - while_StartTime > while_PassTime)//����ڹ涨��ʱ����û�ж����ļ�
        {
            //printf("read is down!\n");
            //close(fp_open);//----------------------------------------< �����̵߳Ļ� ���ǾͲ��ر���  ���ر�
            return -1;
        }   
        else if(fp_rd != -1)
        {
                    //4.��������ļ���,���ǾͿ�ʼ�ж�
            if(ev.type = EV_ABS)//<1>�����ǻ����ж�
            {
                if(ev.code == ABS_X && ev.value > 0)//�ȴ�x������ >0 ���ҲŽ��� һ�㲻�ᴥ��������
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
                    //printf("start_time = %d\n",drp ->start_time);
                    //printf("end_time   = %d\n",drp ->end_time);
                    //printf("pass_time  = %d\n",drp ->pass_time);
                    break;
                }
            }
        }
    

    }

    //close(fp_open);
    return 0;
}

/*
    @Analyse_Drop_Dir(Dorp * drp):������������ ����� ����/˫�� ��ȫ�ֱ��� cl �б��� (x,y)
    @Dorp * drp:struct Drop(�ṹ��)
    ����ֵ: 
        ����enum Move_Dir����ķ����ж�
*/
enum Move_Dir Analyse_Drop_Dir(Dorp * drp)
{
    int dire = -1;//enum ������ ��󷵻�
    if(drp ->end_x != -1 && 
       drp ->end_y != -1 &&
       drp ->start_x != -1 &&
       drp ->start_y != -1 )//������ݲ�����������<�жϻ���>
    {
        if((drp ->end_y - drp ->start_y) >= 0 && abs(drp ->end_x - drp ->start_x) <= abs(drp ->end_y - drp ->start_y))//�»� ������ y���� > x���� 
        {
            printf("--------------------------------------down\n");
            dire = DOWN;
        }
        else if((drp ->end_y - drp ->start_y) < 0 && abs(drp ->end_x - drp ->start_x) <= abs(drp ->end_y - drp ->start_y))//�ϻ� ������ y���� > x���� 
        {
            printf("--------------------------------------up\n");
            dire = UP;
        }
        else if((drp ->end_x - drp ->start_x) > 0 && abs(drp ->end_x - drp ->start_x) > abs(drp ->end_y - drp ->start_y))//��  ������ x���� > y����
        {
            printf("--------------------------------------right\n");
            dire =  RIGHT;
        }
        else if((drp ->end_x - drp ->start_x) <= 0 && abs(drp ->end_x - drp ->start_x) > abs(drp ->end_y - drp ->start_y))//�һ�  ������ x���� > y����
        {
            printf("--------------------------------------lift\n");
            dire = LIFT;
        }
        else
        {
            printf("--------------------------------------Analyse_Drop_Dir ERROR\n");
        }
    }
    else if(drp ->end_x == -1 && 
            drp ->end_y == -1 &&
            drp ->start_x != -1 &&
            drp ->start_y != -1 &&
            drp ->pass_time <= 500)
    {
        printf("CLICK\n");
        dire = CLICK;
        cl.dorp_x = drp ->start_x;
        cl.dorp_y = drp ->start_y;

        last_ms = drp ->end_time;
        // GetTouchDrop_Silther(drp,800);//�ڶ��λ�ȡ�ж�

        // if(drp ->start_time - last_ms < 800)
        // {
        //     printf("DBCLICK\n");
        //     dire = DBCLICK;
        // }
    }
    // else if(drp ->end_x == -1 && 
    //         drp ->end_y == -1 &&
    //         drp ->start_x != -1 &&
    //         drp ->start_y != -1 &&
    //         drp ->pass_time > 500)
    // {
    //     cl.dorp_x = drp ->start_x;
    //     cl.dorp_y = drp ->start_y;
    //     printf("LongPress\n");
    //     dire = LongPress;

    // }
    
    //��������� ��ô���Ǿͽ�����������ݵ���������
    drp ->end_x = -1;
    drp ->end_y = -1;
    drp ->start_x = -1;
    drp ->start_y = -1;


    return dire;

}


//�жϴ���ĵ� �Ƿ������ jr ��������
int JudgeRegion(Judge_Region jr ,Dorp_Click dc)
{
    //����ж� (x,y) ���� �������
    if(dc.dorp_x < jr.end_x &&
       dc.dorp_y < jr.end_y &&
       dc.dorp_y > jr.start_y &&
       dc.dorp_y > jr.start_y )return 1;
    else return -1;
}