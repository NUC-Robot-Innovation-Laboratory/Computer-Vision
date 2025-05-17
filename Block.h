#pragma once
#include<graphics.h>
#include<vector>
using namespace std;

//����Point�ṹ�壬��ʾ����С�������(row��,col��)
struct Point {
	int row;
	int col;
};

//����Block��Ĺ�����Ա��������������Ļ��������͸�������
class Block
{
public:
	Block();
	void drop();
	void moveLeftRight(int offset);
	void rotate();//��ת
	void draw(int leftMargin,int topMargin);
	static IMAGE** getImages();
	Block& operator=(const Block& other);

	bool blockInMap(const vector<vector<int>>&map);

	void solidify(vector<vector<int>>& map);
	int getBlockType();

private:
	int x;
	int y;
	int blockType; //���������
	Point smallBlocks[4];
	IMAGE *img;

private:
	static IMAGE* imgs[7];
	static int size;
};

