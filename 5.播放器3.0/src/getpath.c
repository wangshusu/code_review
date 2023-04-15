#include "getpath.h"

Queue * get_dir_size(const char *path,char *FindFileStyle)
{
    //�ȴ������ argv[1] �����Ŀ¼
    DIR *dir1 = opendir(path);
    if(dir1 == NULL)
    {
        perror("��Ŀ¼ʧ��");
        return NULL;
    }

    char pathname[512] = {0};//����·��


    struct stat st;//���������ļ�����
    int dir_size = 0;//�ļ����ܳ���
    struct dirent *p = NULL;//�������� readdir ��ȡ�󷵻ص� dir


    Stack * stack = InitStack();//��ʼ��ջ
    Queue *qu = InitQueue();//��ʼ������

    if(stack == NULL || qu == NULL)//�������ʧ�ܷ��� -1
    {
        printf("st or qu can't creat\n");
        return NULL;
    }

    //��ʼ��ȡ�ļ�
    while((p = readdir(dir1)))
    {
        char *pwd = NULL;//�������·��
        memset(pathname,0,sizeof(pathname));
        sprintf(pathname,"%s/%s",path,p->d_name);//��ȡ�ļ���
        pwd = calloc(1,strlen(pathname) + 1);
        if(pwd == NULL)
        {
            printf("pwd calloc faile\n");
        }
        memcpy(pwd,pathname,strlen(pathname));//���ļ������� pwd

        stat(pwd,&st);//�� path ���ļ����Ը�ֵ ��st

        
        if(strcmp(p->d_name,".") == 0 || strcmp(p->d_name,"..") == 0)
        {

        }
        else//������ļ�
        {
            //printf("                pwd = %s\n",pwd);
            if(S_ISREG(st.st_mode))//�������ͨ�ļ�
            {
                if(form(pathname,FindFileStyle))
                {
                    InQueue(qu,(MyStackType)pwd);	
                    dir_size += 1;
                }
            }
            else//����� �ļ���
            {
                InStack(stack,(MyStackType)pwd);
            } 

        }
        
    }

    closedir(dir1);//�ر�Ŀ¼

    
    while(stack ->num != 0)//ֻҪ���滹��Ԫ�ؾͼ�����ջ
    {
        char * pwd1 = (char *)OutStack(stack);
        //printf("%s\n",pwd1);
        DIR *dir1 = opendir(pwd1);
        if(dir1 == NULL)
        {
            perror("��Ŀ¼ʧ��");
            return NULL;
        }
        while(p = readdir(dir1))
        {
            if(strcmp(p->d_name,".") == 0 || strcmp(p->d_name,"..") == 0)
            {
                //��� �� . ���� .. �ǾͲ��� ����������
            }
            else//������ļ�
            {
                char *pwd2;
                memset(pathname,0,sizeof(pathname));//���
                sprintf(pathname,"%s/%s",pwd1,p->d_name);//��ȡ�ļ���
                
                pwd2 = calloc(1,strlen(pathname) + 1);
                if(pwd2 == NULL)
                {
                    printf("pwd calloc faile\n");
                }
                memcpy(pwd2,pathname,strlen(pathname));//���ļ������� pwd2
                //printf("%s\n",pwd2);
                stat(pwd2,&st);//�� path ���ļ����Ը�ֵ ��st

                if(S_ISREG(st.st_mode))//�������ͨ�ļ�
                {
                    if(form(pathname,FindFileStyle))
                    {
                        InQueue(qu,(MyStackType)pwd2);
                    }
                }
                else//����� �ļ���
                {
                    InStack(stack,(MyStackType)pwd2);
                } 
            }

        }
        
        closedir(dir1);//�ر�Ŀ¼
        
    }

    Node * ptr = qu ->front;
    for(int i = 0 ; i < qu ->num ; i ++)
    {
        char * buf = (char *) ptr ->data;
        printf("%s\n",buf);
        ptr = ptr ->next;
    }
    printf("qu -> num = %d\n",qu ->num);


    return qu;
}


