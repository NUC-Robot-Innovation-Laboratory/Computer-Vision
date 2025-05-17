#pragma once
#include<graphics.h>
#include<vector>
#include"Block.h"
using namespace std;

class Tetris
{
public:
	//构造函数
	Tetris(int rows, int cols, int left, int top, int blockSize);
	void init(); //初始化
	void play(); //开始游戏
private:
	void keyEvent();
	void updateWindow();
	//返回距离上一次调用该函数，间隔了多长时间（ms）
	//第一次返回0
	int getDelay();//获取时间间隔
	void drop();
	void clearLine();
	void moveLeftRight(int offset);
	void rotate();//旋转
	void drawScore();//显示分数
	void checkOver();//检查游戏是否结束
	void saveScore();//保存分数
	void displayOver();//更新游戏结束画面

private:
	int delay;
	bool update;//是否更新

	//int map[20][10];
	//0：没有方块
	//5：第5种
	vector<vector<int>> map;
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;
	//动态内存分配
	Block* curBlock;
	Block* nextBlock;//预告方块
	Block bakBlock;//当前方块降落过程中，用来备份上一个合法位置的

	int score;
	int highestScore;//最高分
	int level;//当前多少关
	int lineCount;//当前已经消除了多少行
	bool gameOver;//游戏是否已经结束

	IMAGE imgOver;
	IMAGE imgWin;
};

