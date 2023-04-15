#include "getpath.h"
#include "showphoto.h"
#include "touch.h"

int main(int argc ,char *argv[])
{
    if(argc < 2)
    {
        printf("input error\n");
        return -1;
    }
    Queue * qu = get_dir_size(argv[1],".bmp");

    Node *ptr = qu ->front;
    showphoto((char *)ptr ->data,0,0,1);

    while (1)
    {
        int cmd = GetMove_Dir(ptr);
        if(cmd != DIR_ERROR)
        {
            if(cmd == 0 || cmd == 3)//�һ� �»� ��һ��
            {
                ptr = ptr ->next;
                showphoto((char *)ptr ->data,0,0,1);
                continue;
            }
            else if(cmd == 1 || cmd == 2)//�� �ϻ� ��һ��
            {
                ptr = ptr ->prev;
                showphoto((char *)ptr ->data,0,0,1);
                continue;
            }
        }
    }
    
}
    //0 RIGHT,//�һ�
    //1 LIFT,//��
    //2 UP,//�ϻ�
    //3 DOWN,//�»�
