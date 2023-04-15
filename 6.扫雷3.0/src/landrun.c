#include "landrun.h"

/*************** �ⲿ������ **************/
extern unsigned char SchName1[4][40*35/8];
extern int LandMineNum;
extern int LandNullNum;
extern int FindLandNullNum;

/*************** ȫ�ֱ����� **************/
struct timeval sta_tv;//��Ϸ��ʼ��ʱ���ʱ��
struct timeval cur_tv;//��Ϸ��ǰ��ʱ��
Click_xy cl_xy;//����ר��
int game_over;

/*
    Run_Start:��������ǰ����׼������
*/
void Run_Start(void)
{
    game_over = 1;
    touch_Open();
    srand((unsigned int)time(NULL)); //�������ø���ʱ��ı����
    lcd_open();
    
    /********* �����ָ� **********/
    Divide_rectangle();//����ָ�
    Show_Blank();//��ʾBlank�������

    /********* �������ָ� **********/
    Show_FindLandNullNum(0);//
    lcd_draw_word(724,433,32,40,SchName1[0],0xffffff);//������ "��"
    lcd_draw_word(761,433,32,40,SchName1[1],0xffffff);//������ "��"
    lcd_draw_word(724,388,32,40,SchName1[2],0xffffff);//������ "��"
    lcd_draw_word(761,388,32,40,SchName1[3],0xffffff);//������ "��"
    
    Show_Face(1);//Ц����

    gettimeofday(&sta_tv,NULL);//��ȡ��ʼʱ��
    //printf("��ǰ����Ϊ : %d\n\n",sta_tv.tv_sec);
}

/*
    Run_End:��Ϸ�˳���β����
*/
void Run_End(void)
{
    lcd_close();
    touch_Close();
    return;
}

/*
    Failure_Game:�㵽�� ��Ϸʧ�ܺ���
*/
void Failure_Game(Click_xy cl_xy)
{
    Show_Face(3);//Ц����
    printf("game over!\n");
    for(int i = 0 ; i < 3 ; i ++)
    {
        display_bmp("./1.landminebmp/mine1.bmp",arr_x[cl_xy.x],arr_y[cl_xy.y],0);
        GetTouchDrop_Silther(&drp,300);
        display_bmp("./1.landminebmp/blank.bmp",arr_x[cl_xy.x],arr_y[cl_xy.y],0);
        GetTouchDrop_Silther(&drp,300);
    }

    display_bmp("./1.landminebmp/mine1.bmp",arr_x[cl_xy.x],arr_y[cl_xy.y],0);
    for(int i = 0 ; i < V1 ; i++)
    {
        for(int j = 0 ; j < V2 ; j++)
        {
            if(i == cl_xy.x && j == cl_xy.y)
            {
                //��²������ʲô?
            }
            else if(1 == Line[i][j] )
            {
                display_bmp("./1.landminebmp/mine2.bmp",arr_x[i],arr_y[j],0);
            }
            
            //usleep(300);
        }
    }

    lcd_draw_rectangle(arr_x[4], arr_y[4],334,120,0xffffff);
    Show_Str_Mid(arr_x[2],arr_y[4],GameOver0,48,400,0x000000);
    GetTouchDrop_Silther(&drp,600);
    Show_Str_Mid(arr_x[5],arr_y[6],GameOver1,24,300,0x000000); 

    // ������ʾ �����������
    GetTouchDrop_Silther(&drp,20000);

}

/*
    Success_Game:���سɹ�����
*/
void Success_Game(void)
{
    Show_Face(4);//Ц����
    printf("���سɹ�!\n");
    //��ʾ���е��׵ĵص�
    for(int i = 0 ; i < V1 ; i++)
    {
        for(int j = 0 ; j < V2 ; j++)
        {
            if(i == cl_xy.x && j == cl_xy.y)
            {
                //��²������ʲô?
            }
            else if(1 == Line[i][j] )
            {
                display_bmp("./1.landminebmp/mine2.bmp",arr_x[i],arr_y[j],0);
            }
            
            //usleep(300);
        }
    }
    lcd_draw_rectangle(arr_x[4], arr_y[4],334,120,0xffffff);
    Show_Str_Mid(arr_x[2],arr_y[4],GameOver2,48,400,0x000000);
    GetTouchDrop_Silther(&drp,600);
    Show_Str_Mid(arr_x[5],arr_y[6],GameOver1,24,300,0x000000); 

    GetTouchDrop_Silther(&drp,20000);
    return;
}

/*
    Open_Cell_CLICK:�����򿪸���
    @Click_xy cl_xy:�ṹ�� (�Ѿ��Ǹ��ӵ���)
    @����ֵ:
        ������� 0  --->�Ǿ���������
        ������� -1 --->��Ϸ����,��Ҫ������һ��
*/
int Open_Cell_CLICK(Click_xy cl_xy)
{
    //���������Ѿ�����������,��ô�ҾͲ�׼�����������
    if('1' == buf[cl_xy.x][cl_xy.y])
    {
        printf("�����Ѿ�����������!!\n");
        return 0;
    }
    else if(0 == Line[cl_xy.x][cl_xy.y])//��cell�ɹ�
    {
        FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
        //FindLandNullNum = Judge_LandNum();
        Show_FindLandNullNum(1);
        printf("FindLandNullNum = %d\n",FindLandNullNum);
    }
    else if(1 == Line[cl_xy.x][cl_xy.y])//����ʧ��
    {
        Failure_Game(cl_xy);
        return -1;
    }
    if(FindLandNullNum == LandNullNum)//���سɹ�
    {
       Success_Game();
       return -1;
    }
    return 0;
}

/*
    Open_Cell_LongPress:����ִ�в����Ӻ���
    @Click_xy cl_xy:�ṹ�� (�Ѿ��Ǹ��ӵ���)
*/
void Open_Cell_LongPress(Click_xy cl_xy)
{
    printf("�Ѿ������˳�����Χ!\n");
    //����� 2 �ľͲ���ʾ
    // printf("�����ĸ����� (%d,%d)\n",cl_xy.x,cl_xy.y);
    // printf("line�ĸ����� %d\n",Line[cl_xy.x][cl_xy.y]);
    if(2 != Line[cl_xy.x][cl_xy.y])
    {
        printf("�õ�û����ʾ��(%d,%d)!\n",cl_xy.x,cl_xy.y);
        if('0' == buf[cl_xy.x][cl_xy.y])//�����û����ʾ�� �Ǿ���ʾ
        {
            buf[cl_xy.x][cl_xy.y] = '1';
            display_bmp("./1.landminebmp/flag.bmp",arr_x[cl_xy.x],arr_y[cl_xy.y],0);
        }
        else if('1' == buf[cl_xy.x][cl_xy.y])
        {
            buf[cl_xy.x][cl_xy.y] = '0';
            display_bmp("./1.landminebmp/blank.bmp",arr_x[cl_xy.x],arr_y[cl_xy.y],0);
        }
        
    }
}

/*
    Open_Cell_DBCLICK:˫������ ��
    @Click_xy cl_xy:�ṹ�� (�Ѿ��Ǹ��ӵ���)
    @����ֵ:
        ������� 0  --->�Ǿ���������
        ������� -1 --->��Ϸ����,��Ҫ������һ��
*/
int Open_Cell_DBCLICK(Click_xy cl_xy)
{
    //1.���ھ�Ҫ���ж�  ��˫��������� ��Χ�������� == Mine[x][y]
    int sum_line = 0;
    printf("˫���ĵ�����:(%d,%d)\n",cl_xy.x,cl_xy.y);
    
    int i = cl_xy.x;
    int j = cl_xy.y;
    //�����������Χ������  ��ô������ +1  Line Mine
    if(j - 1 >= 0 && buf[i][j - 1] == '1'){sum_line++;}//��
    if(j + 1 < V2 && buf[i][j + 1] == '1'){sum_line++;}//��
    if(i - 1 >= 0 && buf[i - 1][j] == '1'){sum_line++;}//��
    if(i + 1 < V1 && buf[i + 1][j] == '1'){sum_line++;}//��
    if(j - 1 >= 0 && i - 1 >= 0 && buf[i - 1][j - 1] == '1'){sum_line++;}//����
    if(j - 1 >= 0 && i + 1 < V1 && buf[i + 1][j - 1] == '1'){sum_line++;}//����
    if(j + 1 < V2 && i - 1 >= 0 && buf[i - 1][j + 1] == '1'){sum_line++;}//����
    if(j + 1 < V2 && i + 1 < V1 && buf[i + 1][j + 1] == '1'){sum_line++;}//����


    //2.������ ��ô���Ǿʹ��������
    if(sum_line == Mine[cl_xy.x][cl_xy.y])
    {
        //������� ��� ���� ����� Ҳ����˫���ĵ���Χ ����û�б���ȫ���������� ��ô�����Ϸ��ʧ����
        //�ж���Χ�ĵ� �����λ�� �Ƿ�����
        //�����������Χ����  ��ô������ +1  Line Mine
        //FindAndShowNoLandMine(cl_xy.x,cl_xy.y);//�ȴ��Լ�
                                                         //�����ȸ���һ�´��׵ĵ���                                  //��Ϸʧ�� ����-1
        if(j - 1 >= 0 && buf[i][j - 1] == '0' && Line[i][j - 1] == 1 )//&& Line[i][j - 1] != 2)//��
        {
            cl_xy.x = i; 
            cl_xy.y = j - 1 ;
            Failure_Game(cl_xy);
            return -1;
        }
        else if(j - 1 >= 0)//�������ʹ������
        {
            cl_xy.x = i; 
            cl_xy.y = j - 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
        }

        if(j + 1 < V2 && buf[i][j + 1] == '0' && Line[i][j + 1] == 1 )//&& Line[i][j + 1] != 2)//��
        {
            cl_xy.x = i; 
            cl_xy.y = j + 1 ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(j + 1 < V2)
        {
            cl_xy.x = i; 
            cl_xy.y = j + 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(i - 1 >= 0 && buf[i - 1][j] == '0' && Line[i - 1][j] == 1 )//&& Line[i - 1][j] != 2)//��
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(i - 1 >= 0)
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(i + 1 < V1 && buf[i + 1][j] == '0' && Line[i + 1][j] == 1 )//&& Line[i + 1][j] != 2)//��
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(i + 1 < V1)//�������ʹ������
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(j - 1 >= 0 && i - 1 >= 0 && buf[i - 1][j - 1] == '0' && Line[i - 1][j - 1] == 1 )//&& Line[i - 1][j - 1] != 2)//����
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j - 1 ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(j - 1 >= 0 && i - 1 >= 0)//�������ʹ������
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j - 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(j - 1 >= 0 && i + 1 < V1 && buf[i + 1][j - 1] == '0' && Line[i + 1][j - 1] == 1 )//&& Line[i + 1][j - 1] != 2)//����
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j - 1 ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(j - 1 >= 0 && i + 1 < V1)//�������ʹ������
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j - 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(j + 1 < V2 && i - 1 >= 0 && buf[i - 1][j + 1] == '0' && Line[i - 1][j + 1] == 1 )//&& Line[i - 1][j + 1] != 2)//����
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j + 1 ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(j + 1 < V2 && i - 1 >= 0)//�������ʹ������
        {
            cl_xy.x = i - 1; 
            cl_xy.y = j + 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }

        if(j + 1 < V2 && i + 1 < V1 && buf[i + 1][j + 1] == '0' && Line[i + 1][j + 1] == 1 )//&& [i + 1][j + 1] != 2)//����
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j + 1 ;
            Failure_Game(cl_xy);
            return -1;
            }
        else if(j + 1 < V2 && i + 1 < V1)//�������ʹ������
        {
            cl_xy.x = i + 1; 
            cl_xy.y = j + 1 ;
            FindAndShowNoLandMine(cl_xy.x,cl_xy.y);
            }
    }



    if(FindLandNullNum == LandNullNum)//���سɹ�
    {
       Success_Game();
       return -1;
    }
        
    
    else
    {
        printf("��������� �� ����������� �����!!!\n");
        printf("���������:%d\n",Mine[cl_xy.x][cl_xy.y]);
        printf("���������:%d\n",sum_line);
    }

    //�����˳��Ļ�  ��ô�ͷ��� 0 
    return 0;
}

/*  
    Renew_Point_Data:������������ ת��Ϊ ���ӵ���  ����ר��
    @Click_xy cl_xy:�ṹ�� (���Ǵ�������)
*/
void Renew_Point_Data(Click_xy *cl_xy)
{
    printf("�����ĵ�����:(%d,%d)",cl.dorp_x,cl.dorp_y);
    if(cl.dorp_x < 720 && cl.dorp_x > 15)
    {
        cl_xy ->x = (cl.dorp_x - 10)/42;
        cl_xy ->y = (cl.dorp_y - 23)/42;
        if(cl_xy ->x > 17){cl_xy ->x = 17;}
        if(cl_xy ->y > 10){cl_xy ->y = 10;}
    }
    else if(cl.dorp_x > 720)
    {
        if(cl.dorp_x >= 724 && cl.dorp_x <= 793
        && cl.dorp_y >= 385 && cl.dorp_y <= 425)
        {
            cl_xy ->x = 77;
            cl_xy ->y = 77;
            printf("�ؿ�����!\n");
        }
        else if(cl.dorp_x >= 724 && cl.dorp_y >= 430 )
        {
            cl_xy ->x = 88;
            cl_xy ->y = 88;
            printf("�˳�����!\n");
            
        }
    }
    

    printf("����ĸ�����:(%d,%d)\n",cl_xy ->x,cl_xy ->y);
}

/*
    Show_Blank:��ʾδ����������
*/
void Show_Blank(void)
{
    for(int i = 0 ; i < X_Max ; i++)
    {
        for(int j = 0 ; j < Y_Max ; j++)
        {
            display_bmp("./1.landminebmp/blank.bmp",arr_x[i],arr_y[j],0);
        }
    }
}

/*
    Show_Face:Ц����ʾ����
        @int num:ѡ����ʾ�ĸ�Ц��
*/
void Show_Face(int num)
{
    switch (num)
    {
    case 1:
        display_bmp("./1.landminebmp/Face1.bmp",724,206,0);
        break;
    case 2:
        display_bmp("./1.landminebmp/Face2.bmp",724,206,0);
        break;
    case 3:
        display_bmp("./1.landminebmp/Face3.bmp",724,206,0);
        break;
    case 4:
        display_bmp("./1.landminebmp/Face4.bmp",724,206,0);
        break;
    
    default:
        break;
    }
 
}

/*
    Show_Time:ʱ�书������ʾ����
*/
void Show_Time(void)
{
    
    gettimeofday(&cur_tv,NULL);//��ȡ��ʼʱ��
    int time = cur_tv.tv_sec - sta_tv.tv_sec;
    time = time % 1000;
    int  ge = time%10;
    int shi = time/10%10;
    int bai = time/100;

    // printf("Gewei  = %d\n",ge);
    // printf("Shiwei = %d\n",shi);
    // printf("Baiwei = %d\n",bai);

    Selext_Num(bai,724,10);
    Selext_Num(shi,747,10);
    Selext_Num(ge,770,10);
}

/*
    Show_FindLandNullNum:�ҳ��Ŀհ����ĸ���
*/
void Show_FindLandNullNum(int cmd)
{
    if(cmd)
    {
        int num = FindLandNullNum;
        int  ge = num%10;
        int shi = num/10%10;
        int bai = num/100;

        // printf("Gewei  = %d\n",ge);
        // printf("Shiwei = %d\n",shi);
        // printf("Baiwei = %d\n",bai);

        Selext_Num(bai,724,55);
        Selext_Num(shi,747,55);
        Selext_Num(ge,770,55);
    }
    else
    {
        Selext_Num(0,724,55);
        Selext_Num(0,747,55);
        Selext_Num(0,770,55);
    }
    
}

/*
    Selext_Num:ѡ����ʾ����ͼƬ
        @int num:�ĸ�����
        @int x  :x���λ��
        @int y  :y���λ��
*/
void Selext_Num(int num,int x,int y)
{
    switch (num)
    {
    case 0:
        display_bmp("./1.landminebmp/00.bmp",x,y,0);//�� 1 ��λ��
        break;
    case 1:
        display_bmp("./1.landminebmp/01.bmp",x,y,0);//�� 1 ��λ��
        break;
    case 2:
        display_bmp("./1.landminebmp/02.bmp",x,y,0);//�� 1 ��λ��
        break;
    case 3:
        display_bmp("./1.landminebmp/03.bmp",x,y,0);//�� 1 ��λ��
        break;
    case 4:
        display_bmp("./1.landminebmp/04.bmp",x,y,0);//�� 1 ��λ��
        break;
    case 5:
        display_bmp("./1.landminebmp/05.bmp",x,y,0);//�� 1 ��λ��
        break;
    case 6:
        display_bmp("./1.landminebmp/06.bmp",x,y,0);//�� 1 ��λ��
        break;
    case 7:
        display_bmp("./1.landminebmp/07.bmp",x,y,0);//�� 1 ��λ��
        break;
    case 8:
        display_bmp("./1.landminebmp/08.bmp",x,y,0);//�� 1 ��λ��
        break;
    case 9:
        display_bmp("./1.landminebmp/09.bmp",x,y,0);//�� 1 ��λ��
        break;
    default:
        break;
    }
}


/*
    Judge_LandNum:�жϵ㿪�˶��ٸ�����
        @����ֵ:(int)�㿪�׵ĸ���
*/
int Judge_LandNum()
{   
    int num = 0;
    for(int i = 0; i < V1; i++)
    {
        for(int j = 0; j < V2; j++)
        {
            if(Line[i][j] == 2)
            {
                num++;
            }
        }
    }
    return num;
}

/*
    Run_LandMine:������������!
*/
int Run_LandMine(void)
{
    Run_Start();//��������׼������
    InitLindMine(2);
    Count_LandMine_Num();
    Print_Arry();
    //Show_Face(1);

    //������Ҫһ��ѡ���ѶȺ���
    
    while(1)
    {
        Show_Time();
        GetTouchDrop_Silther(&drp,20000);
        int i = Analyse_Drop_Dir(&drp);


        if(i == CLICK || i == LongPress || i == DBCLICK)//���� ���� ˫�� ����
        {
            Renew_Point_Data(&cl_xy);

            /**************** ��������ť ***************/
            if(cl_xy.x == 88 && cl_xy.y == 88)//�˳�
            {
                Run_End();
                return -1;
            }
            else if(cl_xy.x == 77 && cl_xy.y == 77)//��Ϸˢ��
            {
                Run_End();
                return 0;
            }
            /**************** ��Ϸ����ť ***************/
            if(i == CLICK)//�����򿪸ø���
            {
                int op = Open_Cell_CLICK(cl_xy);
                if(-1 == op){break;}
            }
            else if (i == LongPress)//����������
            {
                Open_Cell_LongPress(cl_xy);
            }
            else if ( i == DBCLICK)//˫�����ٴ�
            {
                int op = Open_Cell_DBCLICK(cl_xy);
                if(-1 == op){break;}//�����Ϸ�˳���
                    Show_FindLandNullNum(1);
                    printf("FindLandNullNum = %d\n",FindLandNullNum);
            }
        }
        Print_Arry();
    }
    Run_End();
    return 0;
}
