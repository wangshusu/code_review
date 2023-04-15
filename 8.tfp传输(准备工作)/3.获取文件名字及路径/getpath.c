#include "getpath.h"

/*
    get_dir_path:获取对应文件的路径名
        @path:查找的路径(可以搜索目录里面的)
        @FindFileStyle:查找文件的类型 in:".c" ".h"
*/
Queue *get_dir_path(const char *path)
{
printf("\n%s %d\n",__FUNCTION__,__LINE__);
    /* 打开 path 这个目录 */
    DIR *dir1 = opendir(path);
    if(dir1 == NULL)
    {
        perror("打开目录失败");
        return NULL;
    }
printf("\n%s %d\n",__FUNCTION__,__LINE__);
    struct stat st;     /* 用来保存文件属性 */
    int dir_size = 0;   /* 文件的总数量 */
    struct dirent *p = NULL;    /* 用来保存 readdibr 读取后返回的 dir */
printf("\n%s %d\n",__FUNCTION__,__LINE__);
    Stack *stack = stack_Init();    /* 初始化栈 */
printf("\n%s %d\n",__FUNCTION__,__LINE__);
    Queue *qu    = queue_Init();    /* 初始化队列 */
printf("\n%s %d\n",__FUNCTION__,__LINE__); 
    qu_filename = queue_Init();    /* 初始化队列 */

printf("\n%s %d\n",__FUNCTION__,__LINE__);
    if(stack == NULL || qu == NULL || qu_filename == NULL) /* 如果创建失败 */
    {
        printf("st or qu can't creat\n");
        return NULL;
    }
printf("\n%s %d\n",__FUNCTION__,__LINE__);
    while((p = readdir(dir1)))  /* 开始读取文件 */
    {
        char *pwd = NULL;       /* 保存相对路径 */
        char *pwd1 = NULL;      /* 保存文件名 */
        char pathname[512] = {0};   /* 用来存储路径 */

        sprintf(pathname,"%s/%s",path,p->d_name);   /* 获取文件名 */
        pwd =  calloc(1,strlen(pathname) + 1);
        
        if(pwd == NULL)
        {
            printf("pwd calloc faile\n");
        }
        memcpy(pwd,pathname,strlen(pathname));  /* 将文件名传给 pwd */

        stat(pwd,&st);  /* 将 path 的文件属性赋值 给st */

        if(strcmp(p->d_name,".") == 0 || strcmp(p->d_name,"..") == 0)
        {
            continue;
        }
        else    /* 如果是文件 */
        {
            if(S_ISREG(st.st_mode)) /* 如果是普通文件 */
            {
                pwd1 = calloc(1,strlen(p->d_name) + 1);
                memcpy(pwd1,p->d_name,strlen(p->d_name));  /* 将文件名传给 pwd */

                queue_In(qu_filename, (MyStackType)pwd1); /* 入队 */
                queue_In(qu, (MyStackType)pwd); /* 入队 */
                dir_size += 1;
            }
            else//如果是 文件夹
            {
                stack_In(stack, (MyStackType)pwd);  /* 入栈 */
            } 

        }
        
    }

    closedir(dir1);//关闭目录

    
    while(stack ->num != 0) /* 只要里面还有元素就继续出栈 */
    {
        char *pwd1 = (char *)stack_Out(stack);
        DIR *dir1 = opendir(pwd1);
        if(dir1 == NULL)
        {
            perror("打开目录失败");
            return NULL;
        }

        while(p = readdir(dir1))    /* 开始读取文件 */
        {
            char pathname[512] = {0};   /* 用来存储路径 */
            if(strcmp(p->d_name,".") == 0 || strcmp(p->d_name,"..") == 0)
            {
                continue;
            }
            else    /* 如果是文件 */
            {
                char *pwd2 = NULL;
                char *pwd3 = NULL;      /* 保存文件名 */
                memset(pathname,0,sizeof(pathname));
                sprintf(pathname,"%s/%s",pwd1,p->d_name);   /* 获取文件名 */
                
                pwd2 = calloc(1,strlen(pathname) + 1);
                if(pwd2 == NULL)
                {
                    printf("pwd calloc faile\n");
                }
                memcpy(pwd2,pathname,strlen(pathname));

                stat(pwd2,&st);  /* 将 path 的文件属性赋值 给st */
                if(S_ISREG(st.st_mode))
                {
                    pwd3 = calloc(1,strlen(p->d_name) + 1);
                    memcpy(pwd3,p->d_name,strlen(p->d_name));  /* 将文件名传给 pwd */
                    queue_In(qu_filename, (MyStackType)pwd3); /* 入队 */

                    queue_In(qu,(MyStackType)pwd2); /* 入队 */
                    printf("filename = %s\n",p->d_name);
                    dir_size += 1;
                }
                else//如果是 文件夹
                {
                    stack_In(stack,(MyStackType)pwd2);  /* 入栈 */
                } 
            }
        }
        
        closedir(dir1);//关闭目录
    }
    return qu;
}



/* 
    getpath_Print:检测获取路径是否正确
 */
void getpath_Print(Queue *qu)
{
    Node * ptr = qu ->front;
    while(ptr)
    {
        char * buf = (char *) ptr ->data;
        printf("%s\n",buf);
        ptr = ptr ->next;
    }

        // ptr = qu ->rear;
    // int i = 0;
    // while(ptr)
    // {
    //     char * buf = (char *) ptr ->data;
    //     printf("%s\n",buf);
    //     //ptr = ptr ->prev;
    //     i++;
    // }
    // printf("qu -> num = %d\n",i);
    printf("qu -> num = %d\n",qu -> num);
}