#include "landmine.h"
#include "lcd.h"

//ȫ�ֱ�����
int V1 = 16;//X��
int V2 = 11;//Y��
int LandMineNum;
int LandNullNum;
int FindLandNullNum;
char buf[16][11];
int Line[16][11];//1Ϊ��
int Mine[16][11];//��Χ�׵ĸ���

/*
    InitLindMine:��ʼ������
    @int DegDiff :�׵ĸ���  (1 = 15   , 2 = 20   , 3 = 25  )
    ����ֵ:��
*/
void InitLindMine(int DegDiff)
{
    switch (DegDiff)
    {
    case 1:
        DegDiff = 10;
        break;
    case 2:
        DegDiff = 18;
        break;
    case 3:
        DegDiff = 25;
        break;
    default:
        printf("LandSize error!\n");
        break;
    }
    
    //1.�����ĳ�ʼ��
    memset(Line,0,sizeof(Line));//��ʼ��Ϊ0
    for(int i = 0 ; i < DegDiff ; i++)//���������� ��ֵ
    {
        int x = rand() % (V1 );
        int y = rand() % (V2 );
        while(1 == Line[x][y])//�����ظ�
        {
            x = rand() % (V1 );
            y = rand() % (V2 );
        }
        Line[x][y] = 1;
    }

    //memset(buf,0,sizeof(buf));//�����ӱ�ǵ������ʼ��Ϊ "0"  ------> ���� ԭ��:

    for(int i = 0; i < V1 ; i++)
    {
        for(int j = 0 ; j < V2 ; j++)
        {
            buf[i][j] = '0';
        }
    }

    LandMineNum = DegDiff;
    LandNullNum = V1*V2 - DegDiff;
    FindLandNullNum = 0;
    printf("%d\n",LandMineNum);
    printf("%d\n",LandNullNum);
    printf("%d\n",LandMineNum + LandNullNum);
    printf("%d\n",V1 * V2);
    return;
}

/*
    Count_LandMine_Num:������Χ�׸�������
    ����ֵ:��
*/
void Count_LandMine_Num(void)
{
    memset(Mine,0,sizeof(Mine)); //���Mine ���������
    for(int i = 0 ; i < V1 ; i++)
    {
        for(int j = 0 ; j < V2 ; j++)
        {
            //�����������Χ����  ��ô������ +1  Line Mine
            //j - 1 >= 0
            //j + 1 < V2
            //i - 1 >= 0
            //i + 1 < V1
            if(j - 1 >= 0 && Line[i][j - 1] == 1){Mine[i][j]++;}//��
            if(j + 1 < V2 && Line[i][j + 1] == 1){Mine[i][j]++;}//��
            if(i - 1 >= 0 && Line[i - 1][j] == 1){Mine[i][j]++;}//��
            if(i + 1 < V1 && Line[i + 1][j] == 1){Mine[i][j]++;}//��
            if(j - 1 >= 0 && i - 1 >= 0 && Line[i - 1][j - 1] == 1){Mine[i][j]++;}//����
            if(j - 1 >= 0 && i + 1 < V1 && Line[i + 1][j - 1] == 1){Mine[i][j]++;}//����
            if(j + 1 < V2 && i - 1 >= 0 && Line[i - 1][j + 1] == 1){Mine[i][j]++;}//����
            if(j + 1 < V2 && i + 1 < V1 && Line[i + 1][j + 1] == 1){Mine[i][j]++;}//����
        }
    }
    return;
}

/*
    FindAndShowNoLandMine:�ҵ����ұ��������
    @int i,j:Ҫ�ҵĵ������
*/
void FindAndShowNoLandMine(int x, int y)
{
    if(2 == Line[x][y] || buf[x][y] == '1') //����Ѿ����� ��ô��ֹͣ����
    {
        return;//������ǰ�ݹ�
    }
    
    else if(0 == Line[x][y])//����� ������ ����û����ʾ��
    {
        
        FindLandNullNum++;  

        printf("�հ״�+1\n");

        Line[x][y] = 2;//���������Ϊ�Ѿ�����
        //printf("already show (x,y) = (%d,%d)\n",x,y);
        
        if(Mine[x][y] != 0)//�������Χ���� ��ô����ʾ��һ����
        {
            clect_bmp(x,y);
            return;
        }
        else
        {
            clect_bmp(x,y);
            if(x - 1 >= 0){FindAndShowNoLandMine(x - 1, y );}//��
            if(x + 1 < V1){FindAndShowNoLandMine(x + 1, y );}//��
            if(y - 1 >= 0){FindAndShowNoLandMine(x ,y - 1 );}//��
            if(y + 1 < V2){FindAndShowNoLandMine(x ,y + 1 );}//��
            if(x - 1 >= 0 && y - 1 >= 0){FindAndShowNoLandMine(x - 1, y - 1 );}//����
            if(x + 1 < V1 && y - 1 >= 0){FindAndShowNoLandMine(x + 1, y - 1 );}//����
            if(x - 1 >= 0 && y + 1 < V2){FindAndShowNoLandMine(x - 1, y + 1 );}//����
            if(x + 1 < V1 && y + 1 < V2){FindAndShowNoLandMine(x + 1, y + 1 );}//����
            return;//����������Χ��û�е���,��ô�ͽ��������
        }
    }
    else
    {
        printf("error = %d\n  %d  %d ",Line[x][y],x,y);
        printf("FindAndShowNoLandMine was error\n");
        return;
    }
    
}

/*
    @Print_Arry:�����������
*/
void Print_Arry(void)
{
    int i,j;
    printf("\n");
    for(i = 0 ; i < V2 ; i++)
    {
        for(j = 0 ; j < V1 ; j++)
        {
            if(Line[j][i] == 0)
                printf(". ");
            else
                printf("%d ",Line[j][i]);
        }
        printf("\n");
    }
    printf("\n");

    printf("\n");
    for(i = 0 ; i < V2 ; i++)
    {
        for(j = 0 ; j < V1 ; j++)
        {   
            if(Mine[j][i] == 0)
                printf(". ");
            else
                printf("%d ",Mine[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
    clect_bmp:����С������ʾ
*/
void clect_bmp(int x,int y)
{
    switch (Mine[x][y])
    {
    case 0:
        display_bmp("./1.landminebmp/0.bmp",arr_x[x],arr_y[y],0);
        break;
    case 1:
        display_bmp("./1.landminebmp/1.bmp",arr_x[x],arr_y[y],0);
        break;
    case 2:
        display_bmp("./1.landminebmp/2.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 3:
        display_bmp("./1.landminebmp/3.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 4:
        display_bmp("./1.landminebmp/4.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 5:
        display_bmp("./1.landminebmp/5.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 6:
        display_bmp("./1.landminebmp/6.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 7:
        display_bmp("./1.landminebmp/7.bmp",arr_x[x],arr_y[y],0);
        break; 
    case 8:
        display_bmp("./1.landminebmp/8.bmp",arr_y[y],arr_x[x],0);
        break;   
    case 9:
        display_bmp("./1.landminebmp/9.bmp",arr_y[y],arr_x[x],0);
        break; 
    default:
        break;
    }
}