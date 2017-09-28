#ifndef __DEMINING_H
#define __DEMINING_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <linux/types.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

/* 添加cpp支持 */
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/* 定义matrix默认大小 */
#define DEFAULT_SIZE	9
/* 定义默认 雷 的个数 */
#define DEFAULT_BOOMS	10
/* 定义debug宏 */
#define DEBUG_MATRIX 0
/* 定义帮助命令 */
#define HELP_PARAM_SHORT	"-h"
#define HELP_PARAM		"-help"
/* 定义版本命令 */
#define VERSION_PARAM_SHORT	"-v"
#define VERSION_PARAM		"-version"
/* 定义版本号 */
#define VERSION			"1.1.0"
/* 定义大小命令 */
#define SIZE_PARAM_SHORT	"-s"
#define SIZE_PARAM		"-size"
/* 定义雷数命令 */
#define BOOMS_PARAM_SHORT	"-b"
#define BOOMS_PARAM		"-booms"

/* 定义格子隐藏时的显示 */
#define HIDE_BOX		"*"
/* 定义雷标记 */
#define BOOM_MARK		"#"

/* 定义每个格子的属性元素结构体，属于类型定义 */
typedef struct element_struct {
	/* 格子的值， -1表示埋雷， 其他表示周围雷数 */
	int value;
	/* 格子是否打开 */
	int isOpen;
	/* 是否标记为雷 */
	int isMarkBoom;
} element;

/* 定义整体需要用到的属性元素结构体，属于类型定义 */
typedef struct demining_struct {
	/* 整个matrix,由n*n的格子组成 */
	element **matrix;
	/* 当前光标x位置 */
	int x;
	/* 当前光标y位置 */
	int y;
	/* 当前matrix的大小 */
	unsigned int size;
	/* 剩余雷数 */
	unsigned int booms;
} demining;

/* 定义全局扫雷变量，属于变量定义 */
demining m_demining;

/* 
 * 定义初始化函数
 * 作用：
 *    初始化m_demining并埋雷
 * 输入：
 *    size - 对应 demining结构体的size， 扫雷区域的大小
 *    booms - 设计几个地雷
 * 返回：
 *	0 - 成功，其他 - 失败
 */
int init(int size, int booms);

/*
 * 定义更新matrix的value值的函数
 * 作用：
 *    根据埋雷设置其他非雷区的value值
 * 返回：
 *    0 - 成功，其他 - 失败
 */
int update_matrix(void);

/*
 * 定义刷新函数
 * 作用：
 *    刷新显示
 */
void update();

/*
 * 定义触雷函数
 * 作用：
 *    触雷显示并结束游戏
 */
void touch_boom();

/* 定义参数枚举类型，类型定义 */
typedef struct param_data_struct
{
	int bHelp;
	int bVersion;
	int size;
	int booms;
} param_data;

/*
 * 定义参数解析函数
 * 作用：
 *    解析输入参数
 * 输入：
 *    argc - 参数个数
 *    argv - 参数指针
 *    param - 解析后的参数结构指针
 * 返回：
 *    0 - 成功，其他 - 失败
 */
int param_parse(int argc, char ** argv, param_data * param);

/*
 * 定义帮助函数
 * 作用：
 *     输出帮助信息
 */
void show_help(void);

/* 
 * 定义显示游戏完成函数
 * 作用：
 *    显示游戏完成
 */
void show_game_finish(void);

/* 添加cpp支持， 跟 extern c对应 */
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __demining_H
