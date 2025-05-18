#pragma comment(lib,"Winmm.lib")
#include<windows.h>
#include"Defination.h"//宏定义头文件
#include<graphics.h>//提供图形窗口支持
#include<conio.h>//提供一些用于控制台输入输出操作的函数
#include<mmsystem.h>
#include<time.h>
#include<mciapi.h>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <array>
using namespace std;

//图片数据				 						     
IMAGE   top, mid_up, mid_down, bottom;                //四张背景图原图
IMAGE   top_y, mid_up_y, mid_down_y, bottom_y;        //遮罩图
IMAGE   little_mouse, mid_mouse, big_mouse;           //老鼠原图
IMAGE   little_mouse_y, mid_mouse_y, big_mouse_y;     //遮罩图
IMAGE   img_hammer, img_hammer_down;                  //锤子原图
IMAGE   img_hammer_mask, img_hammer_down_mask;        //锤子遮罩图
IMAGE   img_hitted, img_hitted_mask;                  //击中图与遮罩图
IMAGE   img_win;                                      //获胜图片
IMAGE   img_lose;                                     //失败图片
HWND    hWnd;                                         //窗口句柄

//全局变量				
bool    isOver;                                       //判断是否结束
bool    isWin = false;                                //是否获胜
bool    isLose = false;                               //是否失败
bool    menu_exit = false;                            //是否在菜单页面退出
int     score;                                        //初始分数
int     target;                                       //设置目标
int     missed;                                       //成功逃离的地鼠
int     max_miss;                                     //最多可miss的地鼠数量
int     life;                                         //剩余生命
int     m;                                            //记录当前是哪个地鼠动
int     speed;                                        //地鼠的速度，不同难度下不同 
array<int, 10> tops;                                  //排名前三位的分数
array<int, 10> top_life;                              //排名前三位的分数
//全局变量初始化
void set() {		        
	isOver = false;         //没有结束
	isWin = false;          //没有获胜
	isLose = false;         //没有失败
	score = 0;              //初始分数为0
	missed = 0;             //成功逃离的地鼠数是0
	life = max_miss;        //剩余生命是最多可错过的地鼠
	srand(time(0));         //将当前时间作为随机数的种子
	m = rand() % 9;         //m是0~9十个整数中的一个随机整数
}	
//基类
class Base
{
public:
	virtual void draw() = 0;
	void setXY(int newX, int newY);
	int showX() const { return x; }
	int showY() const { return y; }
	void setImages(const IMAGE& newImg, const IMAGE& newImgY);
protected:
	int x, y;
	IMAGE img, img_y;
};
//地鼠类
class Mouse :public Base
{
public:
	int showDir()const { return dir; }
	IMAGE showimg() const { return img; }
	IMAGE showimg_y() const { return img_y;}
	bool isHitted()const { return hitted; }

	void setDir(int newDir) { dir = newDir; }
	void setHitted(bool newHitted) { hitted = newHitted; }

	void draw();
	void moveUp(){ y -= speed; }
	void moveDown() { y += speed; }

private:
	int dir;
	bool hitted;
};
Mouse ds[9];
//锤子类
class Hammer :public Base
{
public:
	bool isHiting() const { return hiting; }

	void setHit(bool newHiting) { hiting = newHiting; }
	void setImggesHit(const IMAGE& newimghit, const IMAGE& newimghity);
	void draw();

private:
	bool hiting;
	IMAGE img_hit, img_hit_y;
};
Hammer hammer;
//函数声明					                         
void drawRules();                                     //显示游戏规则以及关闭规则界面
void choose();                                        //选择难度，显示菜单
void intiGame();                                      //游戏框架初始化
bool canNotMoveDown();                                //检查往上是否到顶了
bool canNotMoveDown();                                //检查往下是否到底了
void ctolGame();                                      //控制游戏，模拟被击打后下降效果
void refreshMouse();                                  //更新地鼠
void logic();                                         //通过计算，判断锤子有效击中区域是否与地鼠重叠，以此判断游戏是否胜利
void drawGame();                                      //游戏画面显示,通过每一帧的函数调用，游戏界面会被重新绘制，确保数据始终反映在屏幕上。
void control();                                       //获取并处理鼠标信息
void showResult();                                    //显示输赢
void ReadRecordFile();                                //读档
void WriteRecordFile();                               //存档
void drawRank();      

//主函数
int main() {
	drawRules();
	choose();
	if (menu_exit == true) return 0;

	set();
	intiGame();
	SetTimer(hWnd, 8, SPEED, (TIMERPROC)ctolGame);//定时器每隔SPEED毫秒触发一次。每次定时器触发时，调用ctolGame函数来处理定时器事件。

	while (!isOver) {
		logic();
		drawGame();
		control();
	}
	showResult();

	drawRank();

	return 0;
}

//函数定义
void drawRules()
{
	IMAGE img_rules;
	initgraph(RULE_WIDTH, RULE_HEIGH);
	loadimage(&img_rules, "D:\\vs studio 2022 practice\\地鼠也疯狂！\\picture\\rules.jpg");
	putimage(0, 0, &img_rules);
	
	ExMessage em;
	bool exit = false;
	while (true) 
	{
		while (peekmessage(&em, EM_MOUSE)) 
		{
			em = getmessage();
			if (em.message == WM_KEYDOWN                   // 用户按下某个键盘键
				|| em.message == WM_LBUTTONDOWN            // 按下鼠标左键
				|| em.message == WM_RBUTTONDOWN)           // 按下鼠标右键
			{
				exit = true;
				closegraph();                              // 清空缓冲区，防止键盘跳过规则的情况下键盘缓冲区中存留一个字符导致选择难度出错的问题
				while (_kbhit())                           // _kbhit检查键盘缓冲区中是否有按键被按下。如果有键被按下，返回非零值，否则返回零。
					_getch();                              // _getch读取一个字符并返回，不会回显（即字符不会显示在屏幕上）
				break;
			}
		}
		if (exit == true) { break; }
	}
}
void choose()
{
	mciSendString("open \"D:\\vs studio 2022 practice\\c++c项目\\music\\HitCtrl - Happy Pixie Town.mp3\" alias bgm", NULL, 0, NULL);
	mciSendString("play bgm repeat", NULL, 0, NULL);
	IMAGE img_choose;
	initgraph(RULE_WIDTH, RULE_HEIGH);
	loadimage(&img_choose, "picture\\choose.jpg");
	putimage(0, 0, &img_choose);

	// 创建难度选择按钮区域
	RECT easyRect = { 200, 200, 400, 250 };
	RECT mediumRect = { 200, 270, 400, 320 };
	RECT hardRect = { 200, 340, 400, 390 };
	RECT hellRect = { 200, 410, 400, 460 };
	RECT exitRect = { 200, 480, 400, 530 };

	// 设置文本样式
	settextstyle(30, 0, "微软雅黑");
	setbkmode(TRANSPARENT);

	// 绘制按钮
	setfillcolor(WHITE);
	fillrectangle(easyRect.left, easyRect.top, easyRect.right, easyRect.bottom);
	fillrectangle(mediumRect.left, mediumRect.top, mediumRect.right, mediumRect.bottom);
	fillrectangle(hardRect.left, hardRect.top, hardRect.right, hardRect.bottom);
	fillrectangle(hellRect.left, hellRect.top, hellRect.right, hellRect.bottom);
	fillrectangle(exitRect.left, exitRect.top, exitRect.right, exitRect.bottom);

	// 绘制按钮文字
	settextcolor(BLACK);
	outtextxy(easyRect.left + 50, easyRect.top + 15, "1. 简单");
	outtextxy(mediumRect.left + 50, mediumRect.top + 15, "2. 中等");
	outtextxy(hardRect.left + 50, hardRect.top + 15, "3. 困难");
	outtextxy(hellRect.left + 50, hellRect.top + 15, "4. 地狱");
	outtextxy(exitRect.left + 50, exitRect.top + 15, "5. 退出游戏");

	// 主循环
	ExMessage msg;
	bool selected = false;
	while (!selected) {
		if (peekmessage(&msg, EM_MOUSE)) 
		{
			if (msg.message == WM_LBUTTONDOWN) 
			{
				// 检查点击了哪个按钮
				if (msg.x >= easyRect.left && msg.x <= easyRect.right &&
					msg.y >= easyRect.top && msg.y <= easyRect.bottom) {
					target = SIMPLE_TARGET;
					max_miss = SIMPLE_MAXMISS;
					speed = SIMPLE_SPEED;
					selected = true;
				}
				else if (msg.x >= mediumRect.left && msg.x <= mediumRect.right &&
					msg.y >= mediumRect.top && msg.y <= mediumRect.bottom) {
					target = MIDDLE_TARGET;
					max_miss = MIDDLE_MAXMISS;
					speed = MIDDLE_SPEED;
					selected = true;
				}
				else if (msg.x >= hardRect.left && msg.x <= hardRect.right &&
					msg.y >= hardRect.top && msg.y <= hardRect.bottom) {
					target = HARD_TARGET;
					max_miss = HARD_MAXMISS;
					speed = HARD_SPEED;
					selected = true;
				}
				else if (msg.x >= hellRect.left && msg.x <= hellRect.right &&
					msg.y >= hellRect.top && msg.y <= hellRect.bottom) {
					target = HEEL_TARGET;
					max_miss = HEEL_MAXMISS;
					speed = HEEL_SPEED;
					selected = true;
				}
				else if (msg.x >= exitRect.left && msg.x <= exitRect.right &&
					msg.y >= exitRect.top && msg.y <= exitRect.bottom) {
					closegraph();
					menu_exit = true;
					selected = true;
				}
				// 关闭音乐
				mciSendString("stop bgm", NULL, 0, NULL);
				mciSendString("close bgm", NULL, 0, NULL);

			}
		}
	}
	closegraph();
}
void intiGame()
{
	//1.设置窗口
	hWnd = initgraph(WINDOW_WIDTH, WINDOW_HEITH);
	//2.图片资源引入,背景音乐资源引入
	loadimage(&top, "picture\\top.jpg");
	loadimage(&mid_up, "picture\\mid_up.bmp");
	loadimage(&mid_down, "picture\\mid_down.bmp");
	loadimage(&bottom, "picture\\bottom.bmp");

	loadimage(&top_y, "picture\\top_y.bmp");
	loadimage(&mid_up_y, "picture\\mid_up_y.bmp");
	loadimage(&mid_down_y, "picture\\mid_down_y.bmp");
	loadimage(&bottom_y, "picture\\bottom_y.bmp");

	loadimage(&little_mouse, "picture\\little_mouse.bmp");
	loadimage(&mid_mouse, "picture\\mid_mouse.bmp");
	loadimage(&big_mouse, "picture\\big_mouse.bmp");

	loadimage(&little_mouse_y, "picture\\little_mouse_y.bmp");
	loadimage(&mid_mouse_y, "picture\\mid_mouse_y.bmp");
	loadimage(&big_mouse_y, "picture\\big_mouse_y.bmp");

	loadimage(&little_mouse_y, "picture\\little_mouse_y.bmp");
	loadimage(&mid_mouse_y, "picture\\mid_mouse_y.bmp");
	loadimage(&big_mouse_y, "picture\\big_mouse_y.bmp");

	loadimage(&img_hammer, "picture\\hammer.bmp");
	loadimage(&img_hammer_mask, "picture\\hammer_y.bmp");

	loadimage(&img_hammer_down, "picture\\hammer_down.bmp");
	loadimage(&img_hammer_down_mask, "picture\\hammer_down_y.bmp");

	loadimage(&img_hitted, "picture\\died .png");
	loadimage(&img_hitted_mask, "picture\\died_mask .png");

	mciSendString("open music\\happy_adveture.mp3 alias bkmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);
	mciSendString("open music\\hammerhit.mp3 alias hammerhit", NULL, 0, NULL);

	//3.类数据初始化
	for (int i = 0;i < 9;i++) {
		if (i >= 0 && i < 3) {
			ds[i].setImages(little_mouse, little_mouse_y);
			ds[i].setXY(LITTLE_MOUSE_BEGIN_X + i * LITTLE_MOUSE_SPACE, LITTLE_MOUSE_BEGIN_Y);
		}
		if (i >= 3 && i < 6) {
			ds[i].setImages(mid_mouse, mid_mouse_y);
			ds[i].setXY(MID_MOUSE_BEGIN_X + (i - 3) * MID_MOUSE_SPACE, MID_MOUSE_BEGIN_Y);
		}
		if (i >= 6 && i < 9) {
			ds[i].setImages(big_mouse, big_mouse_y);
			ds[i].setXY(BIG_MOUSE_BEGIN_X + (i - 6) * BIG_MOUSE_SPACE, BIG_MOUSE_BEGIN_Y);
		}

		ds[i].setDir(1);//1-up   0-down
		ds[i].setHitted(false);//地鼠是否被击过中赋值为否
	}
	hammer.setImages(img_hammer, img_hammer_mask);
	hammer.setImggesHit(img_hammer_down, img_hammer_down_mask);
	hammer.setXY(HAMMER_BEGIN_X,HAMMER_BEGIN_Y);
	hammer.setHit(false);
}
bool canNotMoveUp()
{
	if (m >= 0 && m < 3) {
		if (ds[m].showY()<= LITTLE_MOUSE_BEGIN_Y - LITTLE_MOUSE_HEIGH)
			return true;
	}
	if (m >= 3 && m < 6) {
		if (ds[m].showY() <= MID_MOUSE_BEGIN_Y - MID_MOUSE_HEIGH)
			return true;
	}
	if (m >= 6 && m < 9) {
		if (ds[m].showY() <= BIG_MOUSE_BEGIN_Y - BIG_MOUSE_HEIGH)
			return true;
	}
	return false;
}
bool canNotMoveDown()
{
	if (m >= 0 && m < 3) {
		if (ds[m].showY() >= LITTLE_MOUSE_BEGIN_Y)
			return true;
	}
	if (m >= 3 && m < 6) {
		if (ds[m].showY() >= MID_MOUSE_BEGIN_Y)
			return true;
	}
	if (m >= 6 && m < 9) {
		if (ds[m].showY() >= BIG_MOUSE_BEGIN_Y)
			return true;
	}
	return false;
}
void ctolGame()
{
	if (ds[m].showDir() == UP) {
		ds[m].moveUp();
		if (canNotMoveUp()) {
			ds[m].setDir(DOWN);
		}
	}
	else {
		ds[m].moveDown();
		if (canNotMoveDown()) {
			ds[m].setDir(UP);
			if (ds[m].isHitted() == false) {
				missed++;
				life--;
			}
			else {
				ds[m].setHitted(false);
			}
			int tmp = m;
			while (tmp == m)//这个while循环的作用是：若刷新的数与之前的数相同，则继续刷新，直到不同，跳出循环
				m = rand() % 9;
		}
	}
}
void refreshMouse()
{
	if (m >= 0 && m < 3)
		ds[m].setXY(LITTLE_MOUSE_BEGIN_X,LITTLE_MOUSE_BEGIN_Y + 1);
	else if (m >= 3 && m < 6)
		ds[m].setXY(MID_MOUSE_BEGIN_X,MID_DOWN_BEGIN_Y + 1);
	else if (m >= 6 && m < 9)
		ds[m].setXY(BIG_MOUSE_BEGIN_X,BIG_MOUSE_BEGIN_Y + 1);
}
void logic()
{
	int effectiveZoneX, effectiveZoneY, effectiveZoneHeight, effectiveZoneWidth;//锤子有效击中区域的中心坐标(x,y)、锤子有效击中区域的高度和宽度
	//计算锤子的有效击中区域
	effectiveZoneX = hammer.showX() + EFFECTIVE_ZONE_X;
	effectiveZoneY = hammer.showY() + EFFECTIVE_ZONE_Y;
	effectiveZoneHeight = EFFECTIVE_ZONE_HEIGHT;
	effectiveZoneWidth = EFFECTIVE_ZONE_WIDTH;
	//abs()是取绝对值，计算锤子有效击中区域与地鼠之间的水平和垂直距离。如果锤子的有效区域和地鼠的图片重叠（即它们在水平方向和垂直方向上的距离小于它们的半宽度和半高度之和），则认为锤子击中了地鼠。
	if (hammer.isHiting() == true && ds[m].isHitted()== false) //检查锤子是否击中地鼠
	{
		int mouseWidth = ds[m].showimg().getwidth();
		int mouseHeight = ds[m].showimg().getheight();
		if (abs(effectiveZoneX - ds[m].showX()) < effectiveZoneWidth / 2 +mouseWidth / 2 &&
			abs(effectiveZoneY - ds[m].showY()) < effectiveZoneHeight / 2 + mouseHeight / 2)//检查锤子的有效击中区域是否与地鼠重叠
			
		{
			mciSendString("play hammerhit from 0", NULL, 0, NULL);
			mciSendString("setaudio hammerhit volume to 300", NULL, 0, NULL); // 30% 音量
			score++;
			ds[m].setHitted(true);
			ds[m].setDir(DOWN);
			refreshMouse();
		}
	}

	if (score == target) {
		isWin = true;
	}
	if (life == 0) {
		isLose = true;
	}
	if (isWin == true || isLose == true) {
		isOver = true;
	}
}
void drawGame()
{
	BeginBatchDraw();

	//用基类指针数组管理所有可绘制对象（按绘制顺序存放）
	Base* drawOrder[] = 
	{
		// 上背景
		nullptr,    // 背景没有继承Base，用nullptr占位
		&ds[0], &ds[1], &ds[2],  // 小地鼠
		// 中上背景
		nullptr,
		&ds[3], &ds[4], &ds[5],  // 中地鼠
		// 中下背景
		nullptr,
		&ds[6], &ds[7], &ds[8],  // 大地鼠
		// 下背景
		nullptr,
		&hammer                  // 锤子
	};

	//遍历数组统一绘制
	int bgIndex = 0;
	for (auto obj : drawOrder) 
	{
		if (obj == nullptr) 
		{// 绘制背景（保持原有顺序）
			switch (bgIndex++) 
			{
			case 0:
				putimage(0, 0, &top_y, SRCAND);
				putimage(0, 0, &top, SRCPAINT);
				break;
			case 1:
				putimage(0, MID_UP_BEGIN_Y, &mid_up_y, SRCAND);
				putimage(0, MID_UP_BEGIN_Y, &mid_up, SRCPAINT);
				break;
			case 2:
				putimage(0, MID_DOWN_BEGIN_Y, &mid_down_y, SRCAND);
				putimage(0, MID_DOWN_BEGIN_Y, &mid_down, SRCPAINT);
				break;
			case 3:
				putimage(0, BOTTOM_BEGIN_Y, &bottom_y, SRCAND);
				putimage(0, BOTTOM_BEGIN_Y, &bottom, SRCPAINT);
				break;
			}
		}
		else 
		{// 多态调用：自动执行 Mouse::draw() 或 Hammer::draw()
			obj->draw();
		}
	}

	//显示生命值和分数
	settextstyle(20, 0, "Arial");
	settextcolor(GREEN);
	TCHAR s1[] = "Current score:";
	outtextxy(SCORE_WORDS_X, SCORE_Y, s1);
	TCHAR s_12[5];//定义一个字符串 ，表示要显示的文本“Current score：”
	sprintf_s(s_12, "%d", score);//将当前得分score格式化为字符串并存储到s_12中。
	outtextxy(SCORE_NUMBER_X+20, SCORE_Y, s_12);//在指定位置(SCORE_NUMBER_X, SCORE_Y)显示格式化后的得分。

	settextcolor(YELLOW);
	TCHAR s2[] = "Target score:";
	outtextxy(TARGET_WORDS_X, TARGET_Y, s2);
	sprintf_s(s_12,"%d", target);
	outtextxy(TARGET_NUMBER_X+10, TARGET_Y, s_12);

	settextcolor((0, 100, 240));
	TCHAR s3[] = "Remaining lives:";
	outtextxy(MAX_MISSED_WORDS_X, MAX_MISSED_Y, s3);
	sprintf_s(s_12, "%d", life);
	outtextxy(MAX_MISSED_NUMBER_X, MAX_MISSED_Y, s_12);

	settextcolor(YELLOW);
	TCHAR s4[] = "Initial blood volume:";
	outtextxy(MISSED_WORDS_X-5, MISSED_Y, s4);
	sprintf_s(s_12,"%d", max_miss);
	outtextxy(MAX_MISSED_NUMBER_X, MISSED_Y, s_12);

	EndBatchDraw();
}
void control()
{
	ExMessage m;
	while (peekmessage(&m, EM_MOUSE, PM_NOREMOVE)) //PM_NOREMOVE表示仅检索消息，但不移除它
	{
		m = getmessage();
		if (m.message == WM_MOUSEMOVE)//如果鼠标移动，则锤子跟着鼠标移动
		{
			hammer.setXY((m.x - (img_hammer.getwidth() / 2)), (m.y - (img_hammer.getheight() / 2)));//m.x是鼠标的横坐标,m.y是鼠标的竖坐标
		}
		if (m.message == WM_LBUTTONDOWN && hammer.isHiting() == false) //m.message用来区分鼠标消息类型
		{
			hammer.setHit(true);
		}
	}
}
void showResult()
{
	// 关闭音乐
	mciSendString("stop bkmusic", NULL, 0, NULL);
	mciSendString("close bkmusic", NULL, 0, NULL);

	closegraph();
	initgraph(RULE_WIDTH, RULE_HEIGH);
	if (isWin == true) {
		mciSendString("open music\\Happy8Bit.mp3 alias happyend", NULL, 0, NULL);
		mciSendString("play happyend repeat", NULL, 0, NULL);
		mciSendString("setaudio happyend volume to 200", NULL, 0, NULL); // 20% 音量
		loadimage(&img_win, "picture\\win.jpg");
		putimage(0, 0, &img_win);
	}
	else {
		mciSendString("open \"D:\\vs studio 2022 practice\\c++c项目\\music\\Sad Piano.mp3\" alias badend", NULL, 0, NULL);
		mciSendString("play badend repeat", NULL, 0, NULL);
		loadimage(&img_lose, "picture\\lose.jpg");
		putimage(0, 0, &img_lose);
	}
	Sleep(3000);
	cleardevice();//关闭画面
}
void ReadRecordFile() {
	// 默认初始化
	tops.fill(0);
	top_life.fill(0);

	ifstream inFile(".\\record.dat");
	if (!inFile.is_open()) {
		return;
	}

	for (int i = 0; i < 3; ++i) {
		if (!(inFile >> tops[i] >> top_life[i])) {
			// 文件内容错误，恢复默认值
			tops.fill(0);
			top_life.fill(0);
			break;
		}
	}
}
void WriteRecordFile() {
	// 创建新记录
	struct Record {
		int score;
		int life;
		bool operator<(const Record& other) const {
			return score > other.score || (score == other.score && life > other.life);
		}
	};

	// 读取现有记录
	array<Record, 4> records; // 3个旧记录 + 1个新记录
	for (int i = 0; i < 3; ++i) {
		records[i] = { tops[i], top_life[i] };
	}
	records[3] = { score, life };

	// 排序并保留前3名
	sort(records.begin(), records.end());

	// 更新全局变量
	for (int i = 0; i < 3; ++i) {
		tops[i] = records[i].score;
		top_life[i] = records[i].life;
	}

	// 写入文件
	ofstream outFile(".\\record.dat");
	if (!outFile.is_open()) {
		return;
	}

	for (int i = 0; i < 3; ++i) {
		outFile << tops[i] << " " << top_life[i];
		if (i < 2) outFile << " ";
	}
}
void drawRank()
{
	ReadRecordFile();
	WriteRecordFile();
	//设置大小
	settextstyle(60, 0, "Arial");

	//排名"Rank!!!"
	settextcolor(YELLOW);
	TCHAR s0[] = "Rank!!!";
	outtextxy(SCORE_WORDS_X + 120, SCORE_Y - 30 + 50, s0);
	TCHAR s_12[5];

	//第一名
	settextcolor(GREEN);
	TCHAR s1[] = "Top1 score:      life:";
	outtextxy(SCORE_WORDS_X + 120, SCORE_Y + 100, s1);
	settextcolor((0, 100, 240));
	sprintf_s(s_12, "%d", tops[0]);
	outtextxy(SCORE_NUMBER_X + 310, SCORE_Y + 100, s_12);//展示得分
	sprintf_s(s_12, "%d", top_life[0]);
	outtextxy(SCORE_NUMBER_X + 300 + 200, SCORE_Y + 100, s_12);//展示生命

	//第二名
	settextcolor(GREEN);
	TCHAR s2[] = "Top2 score:      life:";
	outtextxy(SCORE_WORDS_X + 120, SCORE_Y + 50 + 100, s2);
	settextcolor((0, 100, 240));
	sprintf_s(s_12, _T("%d"), tops[1]);
	outtextxy(SCORE_NUMBER_X + 310, SCORE_Y + 50 + 100, s_12);
	sprintf_s(s_12, _T("%d"), top_life[1]);
	outtextxy(SCORE_NUMBER_X + 300 + 200, SCORE_Y + 50 + 100, s_12);
	//第三名
	settextcolor(GREEN);
	TCHAR s3[] = "Top3 score:      life:";
	outtextxy(SCORE_WORDS_X + 120, SCORE_Y + 100 + 100, s3);
	settextcolor((0, 100, 240));
	sprintf_s(s_12, _T("%d"), tops[2]);
	outtextxy(SCORE_NUMBER_X + 310, SCORE_Y + 100 + 100, s_12);
	sprintf_s(s_12, _T("%d"), top_life[2]);
	outtextxy(SCORE_NUMBER_X + 300 + 200, SCORE_Y + 100 + 100, s_12);

	Sleep(5000);//显示5秒排行榜
	closegraph();
}
//类成员函数定义
void Base:: setXY(int newX, int newY)
{
	x = newX;
	y = newY;
}
void Base:: setImages(const IMAGE& newImg, const IMAGE& newImgY)
{
	img = newImg;
	img_y = newImgY;
}
void Mouse::draw()
{
	putimage(x, y, &img_y, SRCAND);
	putimage(x, y, &img, SRCPAINT);
}
void Hammer::setImggesHit(const IMAGE& newimghit, const IMAGE& newimghity)
{
	img_hit = newimghit;
	img_hit_y = newimghity;
}
void Hammer::draw() {
	bool flag = false;
	//flag确保在目标被击中后，显示相应的击中效果，直到一定时间后再重置状态。
	if (hiting == false)
	{
		putimage(x, y, &img_y, SRCAND);
		putimage(x, y, &img, SRCPAINT);
	}
	else {
		static int hitx, hity;/*静态变量在函数调用之间保持其值。对于hitx和hity,这使得在不同的帧之间能保存撞击位置的坐标，防止每次调用时都重新计算。*/
		if (ds[m].isHitted())
		{
			flag = true;
			hitx = x - 5;
			hity = y + 40;
		}
		if (flag == true)
		{
			putimage(hitx, hity, &img_hitted_mask, NOTSRCERASE);
			putimage(hitx, hity, &img_hitted, SRCINVERT);
		}
		putimage(x, y, &img_hit_y, SRCAND);
		putimage(x, y, &img_hit, SRCPAINT);
		static int count = 0;/*静态变量在多次调用之间不会被销毁或重新初始化。对于count变量，程序需要跟踪函数调用的次数，因此必须保持其值，直到特定条件满足后才会重置。*/
		count++;
		if (count == FRAME) //当count=FRAME（预设帧数值）时，击中效果结束，状态被重置为hit，清除标志flag，恢复正常状态；
		{
			count = 0;
			hiting = false;
			flag = false;
		}
	}
}
