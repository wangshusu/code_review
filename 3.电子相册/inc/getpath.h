#ifndef __GETPATH_H__
#define __GETPATH_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include "LinkedListStack.h"
#include "LinkedListQueue.h"
#include "form.h"

/*
    ����:��ȡ�涨���ļ�·��,������һ��������
    @const char *path:������·�������з��ϸ�ʽ��ͼƬ
    @char *FindFileStyle:�ļ���ʽ
    ����ֵ:
        ����ͷ(Queue *)
*/
Queue * get_dir_size(const char *path,char *FindFileStyle);

#endif

