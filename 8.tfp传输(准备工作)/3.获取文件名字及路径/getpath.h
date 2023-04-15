#ifndef __HEAD_H__
#define __HEAD_H__

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

/**** 全局变量区 ***/
Queue *qu_filename;

/*
    get_dir_path:获取对应文件的路径名
        @path:查找的路径(可以搜索目录里面的)
        @FindFileStyle:查找文件的类型 in:".c" ".h"
*/
Queue *get_dir_path(const char *path);

/*
    get_FileName:获取路径下的文集名
        @path:查找的路径(可以搜索目录里面的)
        @FindFileStyle:查找文件的类型 in:".c" ".h"
*/
Queue *get_FileName(const char *path);

/* 
    getpath_Print:检测获取路径是否正确
 */
void getpath_Print(Queue *qu);

#endif

