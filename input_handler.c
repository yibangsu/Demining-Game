#include "input_handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

/*
 * 处理按键输入
 * 作用：
 *    根据按键实际，更改matrix
 * 输入：
 *    c - 按键值
 *    p_demining - demining指针
 * 返回：
 *    input_ret - 是否打开操作
 */
input_ret handle_input(int c, demining * p_demining)
{
	input_ret ret = ret_nop;
	switch(c)
	{
	/* 方向键下，移动光标 */
	case KEY_DOWN:
		p_demining->y += 1;
		if (p_demining->y >= p_demining->size) p_demining->y = p_demining->size - 1;
		break;
	/* 方向键上，移动光标 */
	case KEY_UP:
		p_demining->y -= 1;
		if (p_demining->y < 0) p_demining->y = 0;
		break;
	/* 方向键左，移动光标 */
	case KEY_LEFT:
		p_demining->x -= 1;
		if (p_demining->x < 0) p_demining->x = 0;
		break;
	/* 方向键右，移动光标 */
	case KEY_RIGHT:
		p_demining->x += 1;
		if (p_demining->x >= p_demining->size) p_demining->x = p_demining->size - 1;
		break;
	/* 空格或回车键，打开格子 */
	case NORMAL_KEY_ENTER:
	case NORMAL_KEY_SPACE:
		p_demining->matrix[p_demining->x][p_demining->y].isOpen = 1;
		ret = ret_click;
		break;
	/* C键，标记雷 */
	case NORMAL_KEY_C:
		p_demining->matrix[p_demining->x][p_demining->y].isMarkBoom = !p_demining->matrix[p_demining->x][p_demining->y].isMarkBoom;
		if (p_demining->matrix[p_demining->x][p_demining->y].value < 0) // 确实是雷
		{
			if (p_demining->matrix[p_demining->x][p_demining->y].isMarkBoom) // 标记了雷
			{
				p_demining->booms --;
			}
			else // 取消了标记
			{
				p_demining->booms ++;
			}
		}
		break;
	default:
		break;
	}

	return ret;
}
