#include<time.h>        //ϵͳֹͣ
#include<stdio.h>		//��׼�������
#include<graphics.h>	//��ͼ
#include<conio.h>		//��������������
#include <thread>		//�����͹����̡߳�
#include <chrono>		//ʱ�Ӻ��ӳ�
#define MAP_SIZE 10 //ÿһ���ж�����ͼƬ
#define IMG_SIZE 39 //ÿ��ͼƬ�Ŀ��
#define WIDTH (MAP_SIZE +2) * IMG_SIZE //���ڵĿ��
#define PICTURE_NUM 10 //ͼƬ���ܸ���

int map[MAP_SIZE + 2][MAP_SIZE + 2];	//��������Ϸ��ͼ
int way[MAP_SIZE + 2][MAP_SIZE + 2];	//Ѱ·�㷨������ͼ
IMAGE img_bk;							//����ͼƬ
IMAGE img_mbk;							//���˵�����
IMAGE img_PICTURE[PICTURE_NUM];			//�ز��б�

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
	cleardevice(); // ��ջ�ͼ����

	putimage(0, 0, &img_mbk);
	
	settextstyle(30, 0, _T("΢���ź�")); // ����������ʽ
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(110, 98, _T("����һ��������С��Ϸ"));
	outtextxy(187, 234, _T("����"));
	settextstyle(20, 0, _T("΢���ź�")); // ����������ʽ
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(110, 128, _T("�����ߣ��ᳬ����һ�����ܳ�"));
	outtextxy(110, 148, _T("����ʱ�䣺2024.5"));
	outtextxy(110, 168, _T("��ϵ��ʽ��1726199979@qq.com"));
	int choice = 0;
	while (true)
	{
		if (MouseHit()) // ���������¼�
		{
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				if (msg.x > 187 && msg.x < 347)
				{
					if (msg.y > 234 && msg.y < 264)
					{
						choice = 1; // �û�ѡ���˲˵�ѡ��3
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
	cleardevice(); // ��ջ�ͼ����

	putimage(0, 0, &img_mbk);

	settextstyle(30, 0, _T("΢���ź�")); // ����������ʽ
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(110, 98, _T("��ϲ��ͨ���ˣ�����"));
	outtextxy(187, 234, _T("����"));
	//settextstyle(20, 0, _T("΢���ź�")); // ����������ʽ
	//settextcolor(BLACK);
	//setbkmode(TRANSPARENT);
	//outtextxy(110, 128, _T("�����ߣ��ᳬ����һ�����ܳ�"));
	//outtextxy(110, 148, _T("����ʱ�䣺2024.5"));
	//outtextxy(110, 168, _T("��ϵ��ʽ��1726199979@qq.com"));
	int choice = 0;
	while (true)
	{
		if (MouseHit()) // ���������¼�
		{
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				if (msg.x > 187 && msg.x < 347)
				{
					if (msg.y > 234 && msg.y < 264)
					{
						choice = 1; // �û�ѡ���˲˵�ѡ��3
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
	//initgraph(800, 600); // ��ʼ����ͼ���ڴ�СΪ800x600

	//setbkcolor(WHITE); // ���ñ�����ɫΪ��ɫ
	loadimage(&img_mbk, "./ref/mbk.jpg", WIDTH, WIDTH+78);
	cleardevice(); // ��ջ�ͼ����

	putimage(0, 0, &img_mbk);

	settextstyle(30, 0, _T("΢���ź�")); // ����������ʽ
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(175, 78, _T("�˵�����")); // �����ı�

	settextstyle(20, 0, _T("΢���ź�"));
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(187, 156, _T("1. ��ʼ��Ϸ")); // ���Ʋ˵�ѡ��1
	outtextxy(187, 195, _T("2. ����")); // ���Ʋ˵�ѡ��2
	outtextxy(187, 234, _T("3. �˳�")); // ���Ʋ˵�ѡ��3

	int choice = 0;
	while (true)
	{
		if (MouseHit()) // ���������¼�
		{
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				if (msg.x > 187 && msg.x < 347)
				{
					if (msg.y >156  && msg.y < 186)
					{
						choice = 1; // �û�ѡ���˲˵�ѡ��1
						break;
					}
					else if (msg.y > 195 && msg.y < 225)
					{
						choice = 2; // �û�ѡ���˲˵�ѡ��2
						break;
					}
					else if (msg.y > 234 && msg.y < 264)
					{
						choice = 3; // �û�ѡ���˲˵�ѡ��3
						break;
					}
				}
			}
		}
	}

	//closegraph(); // �رջ�ͼ����
	cleardevice();
	return choice;
}

void gameInit()
{
	//�������������
	srand((unsigned)time(NULL));
	//�ò�ͬ�����ݱ�ʾ��ͬ��ͼƬ��ÿ��ͼƬ��10����һ����Ҫ10��ͼƬ
	for (int i = 1; i <= MAP_SIZE; i++)
	{
		for (int k = 1; k <= MAP_SIZE; k++)
		{
			map[i][k] = i; //10��i
		}
	}
	//����ͼƬ
	loadimage(&img_bk, "./ref/bk.jpg", WIDTH , WIDTH+100 );   //����������������������ͼƬ��λ��
	for (int i = 0; i < PICTURE_NUM +1; i++)
	{
		char filename[100];
		sprintf(filename, "./ref/%d.png", i); // ������ƴ�ӵ��ļ�����
		loadimage(&img_PICTURE[i],filename ,IMG_SIZE,IMG_SIZE);
	}
	//SetWorkingImage();  //�ָ�Ĭ�Ϲ�����
	//�����������У���ͼƬҲ�������
	for (int i = 1; i <= MAP_SIZE; i++)
	{
		for (int k = 1; k <= MAP_SIZE; k++)
		{
			//�漴�ҵ�������Ϸ������±�λ�ã�Ȼ�󽻻�ֵ ������� rand()
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
		//�ж��������λ��(begin,end)
		if (msg.uMsg == WM_LBUTTONDOWN && (state == BEGIN || state == RESET))
		{
			//����������ת�ɶ�Ӧ��������±�
			begin.col = msg.x / IMG_SIZE;
			begin.row = msg.y / IMG_SIZE;
			end.col = -1;
			end.row = -1;
			state = END;  //�ı�״̬
		}
		else if (msg.uMsg == WM_LBUTTONDOWN && state == END)
		{
			end.col = msg.x / IMG_SIZE;
			end.row = msg.y / IMG_SIZE;
			state = BEGIN; //�ı�״̬
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
	if (state != BEGIN || state == RESET) {  //�����û�л�ȡ����������
		return 4;
	}
	if ( begin_x < 0 && begin_x != -1 || begin_y < 0 && begin_y != -1 || end_x < 0 && end_x != -2 || end_y < 0 && end_y != -2|| end_x >= MAP_SIZE + 2 || end_y >= MAP_SIZE + 2 || begin_x >= MAP_SIZE + 2 || begin_y >= MAP_SIZE + 2) {
		return 1;            //������������Ч��λ��
	}
	if (begin_x == end_x && begin_y == end_y) { //������ε������ͬһ��λ��
		return 2;
	}
	if (begin_x == -1 && begin_y == -1 && end_x == -2 && end_y == -2) { //����ǳ�ʼ״̬
		return 5;
	}
	if (map[begin.row][begin.col] != map[end.row][end.col])
		return 3;
	return 0;
}

bool can(int begin_x, int begin_y, int end_x, int end_y)
{
	//printf("step:%d (%d,%d)->(%d,%d)\n", step, begin_x, begin_y, end_x, end_y);
	if (step++ > 2000) //����·̫����
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
	//������Ҫ�����Լ�����һ��ͼ�δ���
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
				//��ʼ˫�����ͼ
				//printf("xunhuan");
				BeginBatchDraw();
				gameDraw();
				//����˫�����ͼ
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
					printf("��Ч���");
					settextstyle(30, 0, _T("΢���ź�")); // ����������ʽ
					settextcolor(WHITE);
					setbkmode(TRANSPARENT);
					outtextxy(107, 457, _T("����һ����Ч�����������ѡ��ͼƬ"));
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				else if (can_pass(begin.row, begin.col, end.row, end.col) == 2) {
					printf("һ����ͼƬ");
					state = RESET;
					settextstyle(30, 0, _T("΢���ź�")); // ����������ʽ
					settextcolor(WHITE);
					setbkmode(TRANSPARENT);
					outtextxy(107, 457, _T("�벻Ҫѡ��ͬһ��ͼƬ")); // �����ı�
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				else if(can_pass(begin.row, begin.col, end.row, end.col) == 3){
					state = RESET;
					settextstyle(30, 0, _T("΢���ź�")); // ����������ʽ
					settextcolor(WHITE);
					setbkmode(TRANSPARENT);
					outtextxy(107, 457, _T("�޷�����")); // �����ı�
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