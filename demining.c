#include "demining.h"
#include "input_handler.h"
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * 帮助函数
 * 作用：
 *     输出帮助信息
 */
void show_help(void)
{
	fprintf(stdout, "usage:\n");
	fprintf(stdout, "\tdemining [command]\n");
	fprintf(stdout, "\t\t -s|-size n[set size, n must be a positive int string]\n");
	fprintf(stdout, "\t\t -b|-booms n[set booms, n must be a positive int string]\n");
	fprintf(stdout, "\t\t -v|-version[get software version]\n");
	fprintf(stdout, "\t\t -h|-help[get help information]\n");
}

/*
 * 字符转整数函数，只能输入 0-9的字符串
 * 作用：
 *    把字符数组转换成对应的int，注意不要越界
 *    主要用来转换运行时输入的size 和 booms参数
 */
int strtoint(char* str)
{
	int ret = 0;
	while ( *str >= '0' && *str <= '9')
	{
		ret = ret * 10 + (*str - '0');
		str++;
	}
	return ret;
}

/*
 * 更新matrix的value值的函数
 * 作用：
 *    根据埋雷设置其他非雷区的value值
 * 返回：
 *    0 - 成功，其他 - 失败
 */
int update_matrix(void)
{
	/* 如果没有申请matrix, 提示错误并返回 */
	if (!m_demining.matrix)
	{
		fprintf(stdout, "update_matrix : matrix err\n");
		return -1;
	}

	/* 参数定义 */
	int i=0 /* 遍历扫雷matrix的行 */, j /* 遍历扫雷matrix的列 */, x /* 记录matrix的行 */, y /* 记录matrix的列 */;
	/* 用i,j记录当前要赋值的格子； 用x,y记录周围正在判断的格子 */

	/* 用for循环遍历扫雷matrix的行 */
	for (; i < m_demining.size; i++)
	{
		/* 用for循环遍历扫雷matrix的列 */
		j = 0; // 每次j重新归0
		for (; j < m_demining.size; j++)
		{
			/* 如果当前格子埋雷，不需要处理 */
			if (m_demining.matrix[i][j].value < 0)
				continue;
			
			int ii = 0 /* 遍历周围格子的行 */, jj /* 遍历周围格子的列 */;
			/* 用for循环遍历周围的行 */
			for (; ii < 3; ii++)
			{
				/* 记录当前的行 */
				x = i + ii - 1;
				/* 如果行超出范围，继续下一行 */
				if (x < 0 || x >= m_demining.size)
					continue;
				/* 用for循环遍历周围的列 */
				jj = 0; // 每次jj重新归0
				for (; jj < 3; jj++)
				{
					/* 记录当前的列 */
					y = j + jj - 1;
					/* 如果列超出范围，继续下一列 */
					if (y < 0 || y >= m_demining.size)
						continue;

					/* 如果周围存在一个雷， 要赋值的格子加1 */
					if (m_demining.matrix[x][y].value < 0) // 这个格子是雷
						m_demining.matrix[i][j].value ++; // 要赋值的格子加1
				}
			}
		}
	}

	return 0;
}

/* 
 * 初始化函数
 * 作用：
 *    初始化m_demining并埋雷
 * 输入：
 *    size - 对应 demining结构体的size， 扫雷区域的大小
 *    booms - 设计几个地雷
 * 返回：
 *	0 - 成功，其他 - 失败
 */
int init(int size, int booms)
{
	// 如果输入的参数有问题，返回错误
	if (size <= 0 || size >= RAND_MAX || booms < 0 || booms >= size*size)
		return -1;

	/* 申请内存并初始化matrix -- begin */
	/* 申请size个行 */
	m_demining.matrix = malloc(sizeof(element *) * size);
	if (!m_demining.matrix)
	{
		fprintf(stdout, "init : matrix err\n");
		return -1;
	}
	
	/* 每行有size个格子 */
	int i=0;
	for (; i<size; i++)
	{
		/* 申请size个格子 */
		m_demining.matrix[i] = malloc(sizeof(element) * size);
		if (!m_demining.matrix[i])
		{
			fprintf(stdout, "matrix[%d] err\n", i);
			return -1;
		}
	}
	/* 申请内存并初始化matrix -- end */

	// record the size
	// 记录matrix的size
	m_demining.size = size;
	// 记录雷数
	m_demining.booms = booms;

	/* 随机埋雷 */
	srand((unsigned int)time(NULL)); //初始化随机种子
	/* 埋雷 */
	while (booms > 0)
	{
		/* 随机埋雷的坐标 */
		int x = rand() % size;
		int y = rand() % size;

		// set a boom
		// 埋雷
		if (m_demining.matrix[x][y].value >= 0) // 如果当前坐标没有埋雷
		{
			m_demining.matrix[x][y].value = -1; // 埋雷
			booms --; // 剩余雷数减1
			#if (DEBUG_MATRIX == 1)
			fprintf(stdout, "set boom at [%d][%d], booms left %d\n", x, y, booms); // 调试语句
			#endif
		}
	}

	// 更新其他非雷格子值
	update_matrix();

	return 0;
}

/*
 * 刷新函数
 * 作用：
 *    刷新显示
 * 输入：
 *    game_state - 游戏状态
 */
void update(game_ret game_state)
{
	/* 如果没有申请matrix, 提示错误并返回 */
	if (!m_demining.matrix || m_demining.size <= 0)
	{
		fprintf(stdout, "update: no matrix!\n");
		return;
	}

	/* 清屏 */
	erase();

	/* 刷新每一个格子 */
	int i=0, j;
	// 每行
	for (; i<m_demining.size; i++)
	{
		// 每列，每次j归0
		j=0;
		for (; j<m_demining.size; j++)
		{
#if (DEBUG_MATRIX == 1)
			mvwprintw(stdscr, j, i, "%d", m_demining.matrix[i][j].value); // 调试语句，只有在宏DEBUG_MATRIX 为1时有用
#else
			if (game_continue == game_state) 
			{
				/* 显示每一个格子 */
				if (m_demining.matrix[i][j].isOpen) // 如果打开
				{
					mvwprintw(stdscr, j, i, "%d", m_demining.matrix[i][j].value); // 显示周围雷数
				}
				else if (m_demining.matrix[i][j].isMarkBoom) // 标记为雷
				{
					mvwprintw(stdscr, j, i, BOOM_MARK); // 显示雷标
				}
				else // 如果没打开
				{
					mvwprintw(stdscr, j, i, HIDE_BOX); // 显示隐藏
				}
			}
			else
			{
				if (m_demining.matrix[i][j].value < 0 && m_demining.matrix[i][j].isOpen) // 踩到雷
				{
					mvwprintw(stdscr, j, i, BOOM_ON); // 显示踩到的雷
				} 
				else if (m_demining.matrix[i][j].value < 0) // 雷
				{
					mvwprintw(stdscr, j, i, BOOM); // 显示雷标
				}
				else if (m_demining.matrix[i][j].isOpen || game_win == game_state) //打开或赢了
				{
					mvwprintw(stdscr, j, i, "%d", m_demining.matrix[i][j].value); // 显示周围雷数
				}
				else
				{
					mvwprintw(stdscr, j, i, HIDE_BOX); // 显示隐藏
				}
			}
#endif
		} 
		wprintw(stdscr, "\r"); // 换行
	}

	if (game_win == game_state)
	{
		mvwprintw(stdscr, m_demining.size, 0, "You have finish the game, press y to exit.\r");
	}
	else if (game_lose == game_state)
	{
		mvwprintw(stdscr, m_demining.size, 0, "You have touch a boom, press y to exit.\r");
	}

	wmove(stdscr, m_demining.y, m_demining.x); // 显示光标
}

/*
 * 主函数，程序入口
 * 可输入 size值和booms值
 */
int main(int argc, char ** argv)
{
	/* 解析参数 */
	param_data param;
	int result = param_parse(argc, argv, &param);
	if (result)
	{
		fprintf(stdout, "parsing params err: %d\n", result);
		show_help();
		return result;
	}
	if (param.booms < 0 || param.booms > param.size*param.size)
	{
		fprintf(stdout, "set booms err: booms = %d\n", param.booms);
		show_help();
		return -4;
	}
	
	if (param.bHelp)
	{
		show_help();
		return 1;
	}
	else if (param.bVersion)
	{
		fprintf(stdout, "version: %s\n", VERSION);
		return 1;
	}

	/* 初始化 */
	if (init(param.size, param.booms))
	{
		fprintf(stdout, "init error\n");
		return -1;
	}
	
	/* 使能nurses窗口设置 */
	initscr(); // 初始化标准输出窗
	noecho(); // 设置不回显
	keypad(stdscr, TRUE); // 捕捉特殊按键，包括方向键

	/* 刷新显示 */
	game_ret game_state = game_continue;
	update(game_state); // 刷新显示
	refresh(); // 刷新屏幕

	int c;
	/* 死循环刷新并捕捉输入 */
	do
	{
		c = getch(); // 捕捉按键
		input_ret ret = handle_input(c, &m_demining); // 处理按键事件
		
		/* 打开格子 */
		if (ret == ret_click)
		{
			/* 判断是否踩雷 */
			if (m_demining.matrix[m_demining.x][m_demining.y].value < 0)
			{
				game_state = game_lose;
			}
			/* 判断是否中奖 */
			if (0 == m_demining.matrix[m_demining.x][m_demining.y].value)
			{
				open_around(m_demining.x, m_demining.y);
			}
		}
		/* 判断游戏是否完成 */
		if (!m_demining.booms) // 游戏完成
		{
			game_state = game_win;
		}
		
		
		update(game_state); // 刷新显示
		refresh(); // 刷新屏幕

	} while(game_continue == game_state || NORMAL_KEY_Y != c);

	endwin(); // 终止nurses
	return 0;
}

/*
 * 内联函数，判断是否在扫雷范围
 * 输入:
 * 	x或y的坐标值
 * 返回:
 * 	1 - 不在范围内
 * 	0 - 在范围内
 */
inline int check_out_of_arrange(int v)
{
	if (v < 0 || v >= m_demining.size) return 1;

	return 0;
}

/*
 * 迭代函数，打开格子值为0的周围格子
 * 输入:
 * 	dx - x的坐标值
 * 	dy - y的坐标值
 */
void open_around(int dx, int dy)
{
	int x, y;

	if (check_out_of_arrange(dx) || check_out_of_arrange(dy) 
		|| 0 != m_demining.matrix[dx][dy].value) return;

	for (x = dx - 1; x <= dx + 1; x++)
	{
		if (check_out_of_arrange(x)) continue;
		for (y = dy - 1; y <= dy + 1; y++)
		{
			if (check_out_of_arrange(y)) continue;
			if (1 == m_demining.matrix[x][y].isOpen) continue;

			m_demining.matrix[x][y].isOpen = 1;

			if (0 == m_demining.matrix[x][y].value)
			{
				open_around(x, y);
			}
		}
	}
}

/*
 * 参数解析函数
 * 作用：
 *    解析输入参数
 * 输入：
 *    argc - 参数个数
 *    argv - 参数指针
 *    param - 解析后的参数结构指针
 * 返回：
 *   
 */
int param_parse(int argc, char ** argv, param_data * param)
{
	/* 重置参数缓存 */
	memset(param, 0, sizeof(param_data));
	param->size = DEFAULT_SIZE;
	param->booms = DEFAULT_BOOMS;

	/* 解析输入参数 */
	while (argc > 0)
	{
		if (!strcmp(*argv, HELP_PARAM_SHORT) || !strcmp(*argv, HELP_PARAM))
		{
			param->bHelp = 1;
		}
		else if (!strcmp(*argv, VERSION_PARAM_SHORT) || !strcmp(*argv, VERSION_PARAM))
		{
			param->bVersion = 1;
		}
		else if (!strcmp(*argv, SIZE_PARAM_SHORT) || !strcmp(*argv, SIZE_PARAM)) // 一次处理2个参数
		{
			/* 预期下一个参数是表示大小的字符串 */
			argc--; argv++;
			if (argc <= 0) return -1;
			param->size = strtoint(*argv);
			if (param->size <= 0 || param->size >= RAND_MAX) return -2;
		}
		else if (!strcmp(*argv, BOOMS_PARAM_SHORT) || !strcmp(*argv, BOOMS_PARAM)) // 一次处理2个参数
		{
			argc--; argv++;
			if (argc <= 0) return -3;
			param->booms = strtoint(*argv);
			// if (param->booms < 0 || param->booms > param->size*param->size) return -4; // 处理完最后再判断
		}
		else
		{

		}
		/* 下一个参数 */
		argc--;
		argv++;
	}
	return 0;
}
