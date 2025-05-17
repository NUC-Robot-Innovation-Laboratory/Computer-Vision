#include "Tetris.h"
#include<time.h>
#include<stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<iostream>
#include<fstream>
#include"Block.h"

#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define MAX_LEVEL 5
#define RECORDER_FILE "recorder.txt"

//const int SPEED_NORMAL = 500;//ms
const int SPEED_NORMAL[MAX_LEVEL] = { 500,450,400,350,300 };// 各级下落间隔(ms)
const int SPEED_QUICK = 30;

//初始化游戏地图和基本参数
Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)
{
	this->rows = rows;
	this->cols = cols;
	this->leftMargin = left;
	this->topMargin = top;
	this->blockSize = blockSize;

	for (int i = 0; i < rows; i++) {
		vector<int> mapRow;
		for (int j = 0; j < cols; j++) {
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}
}

void Tetris::init()
{
	mciSendString("play res/bg.mp3 repeat",0,0,0);
	delay = SPEED_NORMAL[0];

	//配置随机种子
	srand(time(NULL));

	//创建游戏窗口
	initgraph(938,896);

	//加载背景图片
	loadimage(&imgBg, "res/bg2.png");

	loadimage(&imgWin, "res/win.png");

	loadimage(&imgOver, "res/Over.png");


	//初始化游戏区中的数据
	char data[20][10];
	for (int i = 0; i <rows; i++) {
		for (int j = 0; j < cols; j++) {
			map[i][j] = 0;
		}
	}

	score = 0;
	lineCount = 0;
	level = 1;

	//初始化最高分
	ifstream file(RECORDER_FILE);
	if (!file.is_open()) {
		cout << RECORDER_FILE << "打开失败" << endl;
		highestScore = 0;// 文件不存在时默认0
	}
	else {
		file >> highestScore; // 读取文件中的最高分
	}
	file.close();//关闭文件

	gameOver = false;
}

//游戏主循环：处理输入、下落、渲染、游戏逻辑
void Tetris::play()
{ 
	init();

	nextBlock = new Block;
	curBlock = nextBlock;
	nextBlock = new Block;


	int timer = 0;
	while (1) {
		//接受用户的输入
		keyEvent();

		timer += getDelay();
		if (timer > delay) {
			timer = 0;
			drop();
			//渲染游戏画面
			update = true;
			
		}
		if (update) {
			update = false;
			//更改游戏画面
			updateWindow();

			//更改游戏数据
			clearLine();

		}

		if (gameOver) {
			//保存分数
			saveScore();

			//显示游戏更新界面
			displayOver();

			system("pause");
			init();//重新开局
		}
	}
}

void Tetris::keyEvent()
{
	unsigned char ch;
	bool rotateFlag = false;
	int dx = 0;

	if (_kbhit()) {
		ch = _getch();


		//按下 向上键     224 72
		//       下       224 80
		//       左       224 75
		//       右       224 77
		if (ch == 224) {
			ch = _getch();
			switch (ch) {
			case 72:
				rotateFlag = true;
				break;
			case 80:
				delay = SPEED_QUICK;
				break;
			case 75:
				dx = -1;
				break;
			case 77:
				dx = 1;
				break;
			default:
				break;
			}
		}
	}

	if (rotateFlag) {
		rotate();
		update = true;
	}

	if (dx != 0) {
		moveLeftRight(dx);
		update = true;
	}
}

void Tetris::updateWindow()
{
	putimage(0, 0, &imgBg);//绘制背景图片

	IMAGE** imgs = Block::getImages();
	BeginBatchDraw();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (map[i][j] == 0)continue;

			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}

		curBlock->draw(leftMargin, topMargin);
		nextBlock->draw(689, 150);

		drawScore();

		EndBatchDraw();
	}
}


int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}
	


void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();

	if (!curBlock->blockInMap(map)) {
		//把这个方块固化
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;

		//检查游戏是否结束
		checkOver();
	}

	delay = SPEED_NORMAL[level-1];
}

void Tetris::clearLine()
{
	int lines = 0;
	int k = rows - 1;//储存数据的行数
	for (int i = rows - 1; i >= 0; i--) {
		//检查第i行是否满行
		int count = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j]) {
				count++;
			}
			map[k][j] = map[i][j];//一边扫描，一边储存
		}

		if (count < cols) {//不是满行
			k--;
		}
		else {//count==cols 满行
			lines++;
		}
	}

	if (lines > 0) {
		//计算得分
		int addScore[4] = { 10,30,60,80 };
		score += addScore[lines - 1];

		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;

		//每100分一个级别 0—100第1关 101-200第2关
		level = (score + 29) / 30;
		if (level > MAX_LEVEL) {
			gameOver = true;
		}//// 通关

		lineCount+=lines;

		

	}
}

void Tetris::moveLeftRight(int offset) {
	bakBlock = *curBlock;
	curBlock->moveLeftRight(offset);

	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::rotate() {

	if (curBlock->getBlockType() == 7)
	{
		return;
		}
	bakBlock = *curBlock;
	curBlock->rotate();

	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}

}

void Tetris::drawScore() {

	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);

	//改颜色
	setcolor(RGB(180, 180, 180));

	//改字体
	LOGFONT f;
	gettextstyle(&f);//获取当前的字体
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("微软雅黑"));
	settextstyle(&f);

	setbkmode(TRANSPARENT);//透明

	outtextxy(670, 727, scoreText);

	//绘制消除了多少行
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);

	gettextstyle(&f);

	int xPos = 224 - f.lfWidth * strlen(scoreText);

	outtextxy(xPos, 817, scoreText);

	//绘制当前是第几关
	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	outtextxy(224-30, 727, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(670, 817, scoreText);
}

void Tetris::checkOver()
{
	 gameOver=(curBlock->blockInMap(map) == false);

}

void Tetris::saveScore()
{
	if (score > highestScore) {
		highestScore = score;// 更新最高分

		ofstream file(RECORDER_FILE);
		file << highestScore;// 写入文件
		file.close();
	}
}

void Tetris::displayOver()
{
	mciSendString("stop res/bg.mp3", 0, 0, 0);

	//胜利结束，还是失败结束
	if (level <= MAX_LEVEL) {
		putimage(262, 361, &imgOver);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else {
		putimage(262, 361, &imgWin);
		mciSendString("play res/Win.mp3", 0, 0, 0);
	}
}
