/*
 * @Author: 王振兴 1750239165@qq.com
 * @Date: 2022-07-28 14:58:45
 * @LastEditors: 王振兴 1750239165@qq.com
 * @LastEditTime: 2022-07-28 19:05:53
 * @FilePath: \5.C++d:\Program Files\100.WorkFileFolder\6.小项目\8.tfp传输(准备工作)\9.分文件\server\inc\getpath.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: 王振兴 1750239165@qq.com
 * @Date: 2022-07-28 14:58:45
 * @LastEditors: 王振兴 1750239165@qq.com
 * @LastEditTime: 2022-07-28 19:04:40
 * @FilePath: \5.C++d:\Program Files\100.WorkFileFolder\6.小项目\8.tfp传输(准备工作)\9.分文件\server\inc\getpath.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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
/**
 * @description: 
 * @return {*}
 */
Queue *qu_filename;

/*
    get_dir_path:获取对应文件的路径名
        @path:查找的路径(可以搜索目录里面的)
        @FindFileStyle:查找文件的类型 in:".c" ".h"
*/
Queue *get_dir_path(const char *path, Queue **qu_filename);

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

