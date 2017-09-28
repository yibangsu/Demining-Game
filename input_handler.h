#ifndef __INPUT_HANDLER_H
#define __INPUT_HANDLER_H

#include "demining.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/* 回车键值 */
#define NORMAL_KEY_ENTER	(10)
/* 空格键值 */
#define NORMAL_KEY_SPACE	(32)
/* c键值 */
#define NORMAL_KEY_C		(99)

/* 定义按键处理返回值，枚举变量类型定义 */
typedef enum input_result
{
	ret_nop,
	ret_click,
	ret_end
} input_ret;

/*
 * 定义处理按键输入
 * 作用：
 *    根据按键实际，更改matrix
 * 输入：
 *    c - 按键值
 *    p_demining - demining指针
 * 返回：
 *    input_ret - 是否打开操作
 */
input_ret handle_input(int c, demining * p_demining);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __INPUT_HANDLER_H
