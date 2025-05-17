#pragma once
#include<graphics.h>
#include<vector>
#include"Block.h"
using namespace std;

class Tetris
{
public:
	//���캯��
	Tetris(int rows, int cols, int left, int top, int blockSize);
	void init(); //��ʼ��
	void play(); //��ʼ��Ϸ
private:
	void keyEvent();
	void updateWindow();
	//���ؾ�����һ�ε��øú���������˶೤ʱ�䣨ms��
	//��һ�η���0
	int getDelay();//��ȡʱ����
	void drop();
	void clearLine();
	void moveLeftRight(int offset);
	void rotate();//��ת
	void drawScore();//��ʾ����
	void checkOver();//�����Ϸ�Ƿ����
	void saveScore();//�������
	void displayOver();//������Ϸ��������

private:
	int delay;
	bool update;//�Ƿ����

	//int map[20][10];
	//0��û�з���
	//5����5��
	vector<vector<int>> map;
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;
	//��̬�ڴ����
	Block* curBlock;
	Block* nextBlock;//Ԥ�淽��
	Block bakBlock;//��ǰ���齵������У�����������һ���Ϸ�λ�õ�

	int score;
	int highestScore;//��߷�
	int level;//��ǰ���ٹ�
	int lineCount;//��ǰ�Ѿ������˶�����
	bool gameOver;//��Ϸ�Ƿ��Ѿ�����

	IMAGE imgOver;
	IMAGE imgWin;
};

