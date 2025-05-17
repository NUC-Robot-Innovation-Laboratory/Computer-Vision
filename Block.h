#pragma once
#include<graphics.h>
#include<vector>
using namespace std;

//定义Point结构体，表示方块小格的坐标(row行,col列)
struct Point {
	int row;
	int col;
};

//声明Block类的公共成员函数，包括方块的基本操作和辅助功能
class Block
{
public:
	Block();
	void drop();
	void moveLeftRight(int offset);
	void rotate();//旋转
	void draw(int leftMargin,int topMargin);
	static IMAGE** getImages();
	Block& operator=(const Block& other);

	bool blockInMap(const vector<vector<int>>&map);

	void solidify(vector<vector<int>>& map);
	int getBlockType();

private:
	int x;
	int y;
	int blockType; //方块的类型
	Point smallBlocks[4];
	IMAGE *img;

private:
	static IMAGE* imgs[7];
	static int size;
};

