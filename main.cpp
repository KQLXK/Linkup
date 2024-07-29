#include<time.h>        //系统停止
#include<stdio.h>		//标准输入输出
#include<graphics.h>	//绘图
#include<conio.h>		//非阻塞键盘输入
#include <thread>		//创建和管理线程。
#include <chrono>		//时钟和延迟
#define MAP_SIZE 10 //每一行有多少张图片
#define IMG_SIZE 39 //每张图片的宽度
#define WIDTH (MAP_SIZE +2) * IMG_SIZE //窗口的宽度
#define PICTURE_NUM 10 //图片的总个数

int map[MAP_SIZE + 2][MAP_SIZE + 2];	//连连看游戏地图
int way[MAP_SIZE + 2][MAP_SIZE + 2];	//寻路算法辅助地图
IMAGE img_bk;							//背景图片
IMAGE img_mbk;							//主菜单背景
IMAGE img_PICTURE[PICTURE_NUM];			//素材列表

struct Index
{
	int row;
	int col;
}begin = { -1, -1 }, end = { -2, -2 };

enum Cur
{
	BEGIN,
	END,
	RESET
};
int step = 0;
Cur state = BEGIN;

int AboutInit() {
	loadimage(&img_mbk, "./ref/mbk.jpg", WIDTH, WIDTH+78);
	cleardevice(); // 清空绘图窗口

	putimage(0, 0, &img_mbk);
	
	settextstyle(30, 0, _T("微软雅黑")); // 设置字体样式
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(110, 98, _T("这是一款连连看小游戏"));
	outtextxy(187, 234, _T("返回"));
	settextstyle(20, 0, _T("微软雅黑")); // 设置字体样式
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(110, 128, _T("制作者：俞超，何一川，周晨"));
	outtextxy(110, 148, _T("制作时间：2024.5"));
	outtextxy(110, 168, _T("联系方式：1726199979@qq.com"));
	int choice = 0;
	while (true)
	{
		if (MouseHit()) // 检测鼠标点击事件
		{
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				if (msg.x > 187 && msg.x < 347)
				{
					if (msg.y > 234 && msg.y < 264)
					{
						choice = 1; // 用户选择了菜单选项3
						break;
					}
				}
			}
		}
	}

	return choice;
}

int WinInit() {
	loadimage(&img_mbk, "./ref/mbk.jpg", WIDTH, WIDTH+78);
	cleardevice(); // 清空绘图窗口

	putimage(0, 0, &img_mbk);

	settextstyle(30, 0, _T("微软雅黑")); // 设置字体样式
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(110, 98, _T("恭喜你通关了！！！"));
	outtextxy(187, 234, _T("返回"));
	//settextstyle(20, 0, _T("微软雅黑")); // 设置字体样式
	//settextcolor(BLACK);
	//setbkmode(TRANSPARENT);
	//outtextxy(110, 128, _T("制作者：俞超，何一川，周晨"));
	//outtextxy(110, 148, _T("制作时间：2024.5"));
	//outtextxy(110, 168, _T("联系方式：1726199979@qq.com"));
	int choice = 0;
	while (true)
	{
		if (MouseHit()) // 检测鼠标点击事件
		{
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				if (msg.x > 187 && msg.x < 347)
				{
					if (msg.y > 234 && msg.y < 264)
					{
						choice = 1; // 用户选择了菜单选项3
						break;
					}
				}
			}
		}
	}

	return choice;
}

int MenuInit()
{
	//initgraph(800, 600); // 初始化绘图窗口大小为800x600

	//setbkcolor(WHITE); // 设置背景颜色为白色
	loadimage(&img_mbk, "./ref/mbk.jpg", WIDTH, WIDTH+78);
	cleardevice(); // 清空绘图窗口

	putimage(0, 0, &img_mbk);

	settextstyle(30, 0, _T("微软雅黑")); // 设置字体样式
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(175, 78, _T("菜单界面")); // 绘制文本

	settextstyle(20, 0, _T("微软雅黑"));
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(187, 156, _T("1. 开始游戏")); // 绘制菜单选项1
	outtextxy(187, 195, _T("2. 关于")); // 绘制菜单选项2
	outtextxy(187, 234, _T("3. 退出")); // 绘制菜单选项3

	int choice = 0;
	while (true)
	{
		if (MouseHit()) // 检测鼠标点击事件
		{
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				if (msg.x > 187 && msg.x < 347)
				{
					if (msg.y >156  && msg.y < 186)
					{
						choice = 1; // 用户选择了菜单选项1
						break;
					}
					else if (msg.y > 195 && msg.y < 225)
					{
						choice = 2; // 用户选择了菜单选项2
						break;
					}
					else if (msg.y > 234 && msg.y < 264)
					{
						choice = 3; // 用户选择了菜单选项3
						break;
					}
				}
			}
		}
	}

	//closegraph(); // 关闭绘图窗口
	cleardevice();
	return choice;
}

void gameInit()
{
	//设置随机数种子
	srand((unsigned)time(NULL));
	//用不同的数据表示不同的图片，每种图片来10个，一共需要10种图片
	for (int i = 1; i <= MAP_SIZE; i++)
	{
		for (int k = 1; k <= MAP_SIZE; k++)
		{
			map[i][k] = i; //10个i
		}
	}
	//加载图片
	loadimage(&img_bk, "./ref/bk.jpg", WIDTH , WIDTH+100 );   //后两个参数用来调整背景图片的位置
	for (int i = 0; i < PICTURE_NUM +1; i++)
	{
		char filename[100];
		sprintf(filename, "./ref/%d.png", i); // 将参数拼接到文件名中
		loadimage(&img_PICTURE[i],filename ,IMG_SIZE,IMG_SIZE);
	}
	//SetWorkingImage();  //恢复默认工作区
	//打乱数组排列，让图片也随机排列
	for (int i = 1; i <= MAP_SIZE; i++)
	{
		for (int k = 1; k <= MAP_SIZE; k++)
		{
			//随即找到两个游戏区域的下标位置，然后交换值 随机函数 rand()
			int x = rand() % MAP_SIZE + 1;
			int y = rand() % MAP_SIZE + 1;
			int _swap = map[i][k];
			map[i][k] = map[x][y];
			map[x][y] = _swap;
		}
	}
}

void gameDraw()
{
	//map[1][1] = 10;
	putimage(0, 0, &img_bk);
	for (int i = 1; i <= MAP_SIZE; i++)
	{
		for (int k = 1; k <= MAP_SIZE; k++)
		{
			if (map[i][k] != 0)
			{
				putimage(k * IMG_SIZE, i * IMG_SIZE, &img_PICTURE[map[i][k]]);  
			}
		}
	}
}

void mouseEvent()
{
	if (MouseHit())
	{
		MOUSEMSG msg = GetMouseMsg();
		//判断鼠标点击的位置(begin,end)
		if (msg.uMsg == WM_LBUTTONDOWN && (state == BEGIN || state == RESET))
		{
			//把鼠标的坐标转成对应的数组的下标
			begin.col = msg.x / IMG_SIZE;
			begin.row = msg.y / IMG_SIZE;
			end.col = -1;
			end.row = -1;
			state = END;  //改变状态
		}
		else if (msg.uMsg == WM_LBUTTONDOWN && state == END)
		{
			end.col = msg.x / IMG_SIZE;
			end.row = msg.y / IMG_SIZE;
			state = BEGIN; //改变状态
		}
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			printf("begin(%d,%d) end(%d,%d)\n", begin.col, begin.row, end.col, end.row);
		}
		
	}
}

bool isBlock(int row, int col)
{
	if (row < 0 || col < 0 || row >= MAP_SIZE + 2 || col >= MAP_SIZE + 2)
		return 1;
	return map[row][col] + way[row][col];
}

void showMap()
{
	for (int i = 0; i < MAP_SIZE + 2; i++)
	{
		for (int k = 0; k < MAP_SIZE + 2; k++)
		{
			printf("%-2d ", map[i][k]);
		}
		printf("\n");
	}
}

bool same(int begin_x, int begin_y, int end_x, int end_y)
{
	if (begin_x < 0 || begin_y < 0 || end_x < 0 || end_y < 0 || end_x >= MAP_SIZE + 2 || end_y >= MAP_SIZE + 2 || begin_x >= MAP_SIZE + 2 || begin_y >= MAP_SIZE + 2)
		return false;
	if (begin_x == end_x && begin_y == end_y)
		return true;
	return false;
}

int can_pass(int begin_x, int begin_y, int end_x, int end_y)
{
	if (state != BEGIN || state == RESET) {  //如果还没有获取到两个坐标
		return 4;
	}
	if ( begin_x < 0 && begin_x != -1 || begin_y < 0 && begin_y != -1 || end_x < 0 && end_x != -2 || end_y < 0 && end_y != -2|| end_x >= MAP_SIZE + 2 || end_y >= MAP_SIZE + 2 || begin_x >= MAP_SIZE + 2 || begin_y >= MAP_SIZE + 2) {
		return 1;            //如果点击的是无效的位置
	}
	if (begin_x == end_x && begin_y == end_y) { //如果两次点击的是同一个位置
		return 2;
	}
	if (begin_x == -1 && begin_y == -1 && end_x == -2 && end_y == -2) { //如果是初始状态
		return 5;
	}
	if (map[begin.row][begin.col] != map[end.row][end.col])
		return 3;
	return 0;
}

bool can(int begin_x, int begin_y, int end_x, int end_y)
{
	//printf("step:%d (%d,%d)->(%d,%d)\n", step, begin_x, begin_y, end_x, end_y);
	if (step++ > 2000) //当找路太多了
		return false;
	if (begin_x < 0 || begin_y < 0 || end_x < 0 || end_y < 0 || end_x >= MAP_SIZE + 2 || end_y >= MAP_SIZE + 2 || begin_x >= MAP_SIZE + 2 || begin_y >= MAP_SIZE + 2)
		return false;
	if (same(begin_x, begin_y, end_x, end_y) || same(begin_x - 1, begin_y, end_x, end_y) || same(begin_x + 1, begin_y, end_x, end_y) || same(begin_x, begin_y - 1, end_x, end_y) || same(begin_x, begin_y + 1, end_x, end_y))
		return true;
	if (!isBlock(begin_x - 1, begin_y)) //1
	{
		way[begin_x - 1][begin_y] = 1;
		if (can(begin_x - 1, begin_y, end_x, end_y))
			return true;
		way[begin_x - 1][begin_y] = 0;
	}
	if (!isBlock(begin_x, begin_y - 1)) //3
	{
		way[begin_x][begin_y - 1] = 1;
		if (can(begin_x, begin_y - 1, end_x, end_y))
			return true;
		way[begin_x][begin_y - 1] = 0;
	}
	if (!isBlock(begin_x + 1, begin_y)) //2
	{
		way[begin_x + 1][begin_y] = 1;
		if (can(begin_x + 1, begin_y, end_x, end_y))
			return true;
		way[begin_x + 1][begin_y] = 0;
	}
	if (!isBlock(begin_x, begin_y + 1)) //4
	{
		way[begin_x][begin_y + 1] = 1;
		if (can(begin_x, begin_y + 1, end_x, end_y))
			return true;
		way[begin_x][begin_y + 1] = 0;
	}
	return false;
}

bool GameOver() {
	for (int i = 0; i < MAP_SIZE + 2; i++) {
		for (int j = 0; j < MAP_SIZE + 2; j++) {
			if (map[i][j] != 0) {
				return false;
			}
		}
	}
	return true;
}

int main()
{
	int choose = 0;
	//首先需要我们自己创建一个图形窗口
	while (choose != 3) {
		initgraph(WIDTH, WIDTH+78, SHOWCONSOLE);
		choose = MenuInit();
		switch (choose) {
		case 1:
			initgraph(WIDTH, WIDTH+78, SHOWCONSOLE);
			cleardevice();
			gameInit();
			showMap();
			while (1)
			{
				//开始双缓冲绘图
				//printf("xunhuan");
				BeginBatchDraw();
				gameDraw();
				//结束双缓冲绘图
				EndBatchDraw();
				if (can_pass(begin.row, begin.col, end.row, end.col) == 0)
				{
					memset(way, 0, sizeof(way));
					way[begin.row][begin.col] = 1;
					step = 0;
					printf("inininininin");
					if (can(begin.row, begin.col, end.row, end.col))
					{
						map[begin.row][begin.col] = 0;
						map[end.row][end.col] = 0;
						begin.row = -1;
						begin.col = -1;
						end.row = -2;
						end.col = -2;
						showMap();
					}
				}
				else if (can_pass(begin.row, begin.col, end.row, end.col) == 1) {
					state = RESET;
					printf("无效点击");
					settextstyle(30, 0, _T("微软雅黑")); // 设置字体样式
					settextcolor(WHITE);
					setbkmode(TRANSPARENT);
					outtextxy(107, 457, _T("这是一次无效点击，请重新选择图片"));
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				else if (can_pass(begin.row, begin.col, end.row, end.col) == 2) {
					printf("一样的图片");
					state = RESET;
					settextstyle(30, 0, _T("微软雅黑")); // 设置字体样式
					settextcolor(WHITE);
					setbkmode(TRANSPARENT);
					outtextxy(107, 457, _T("请不要选择同一处图片")); // 绘制文本
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				else if(can_pass(begin.row, begin.col, end.row, end.col) == 3){
					state = RESET;
					settextstyle(30, 0, _T("微软雅黑")); // 设置字体样式
					settextcolor(WHITE);
					setbkmode(TRANSPARENT);
					outtextxy(107, 457, _T("无法消除")); // 绘制文本
					printf("wu fa xiao chu");
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				mouseEvent();
				if (GameOver()) {
					choose = WinInit();
					break;
				}
				//getchar();
			}
			break;
		case 2:
			choose = AboutInit();
			break;
		case 3:
			break;
		}
		//getchar();
	}
	//getchar();
	return 0;
}