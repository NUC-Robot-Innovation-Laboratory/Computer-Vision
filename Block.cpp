#include "Block.h"
#include<stdlib.h>

IMAGE* Block::imgs[7] = { NULL, };
int Block::size = 36;


//构造函数：首次创建方块时加载并分割方块图片资源
Block::Block()
{
	if (imgs[0] == NULL) {
		IMAGE imgTmp;
		loadimage(&imgTmp, "res/tiles.png");

		SetWorkingImage(&imgTmp);
		for (int i = 0; i < 7; i++) {
			imgs[i] = new IMAGE;
			getimage(imgs[i], i * size, 0, size, size);
		}
		SetWorkingImage();//恢复工作区
		//srand(time(NULL));
	}
	//定义7种方块类型
	int blocks[7][4] = {
		1,3,5,7,//I
		2,4,5,7,//Z 1
		3,5,4,6,//Z 2
		3,5,4,7,//T
		2,3,5,7,//L
		3,5,7,6,//J
		2,3,4,5,//田
	};

	//随机生成一种方块
	blockType = 1+rand() % 7;//0..6

	//初始化 smallBlocks
	for (int i = 0; i < 4; i++) {
		int value = blocks[blockType - 1][i];
		smallBlocks[i].row = value / 2;
		smallBlocks[i].col = value % 2;
	}

	img = imgs[blockType - 1];

}

//方块下落：所有小方块的row坐标+1
void Block::drop()
{
	for (int i = 0; i < 4; i++) {
		smallBlocks[i].row++;
	}
}
//左右移动：所有小方块的col坐标增加偏移量
void Block::moveLeftRight(int offset)
{
	for (int i = 0; i < 4; i++) {
		smallBlocks[i].col += offset;
	}
}
//旋转方块：以中心点为基准进行坐标变换
void Block::rotate()
{
	Point p = smallBlocks[1];// 取中心点为旋转基准

	for (int i = 0; i < 4; i++) {
		Point tmp = smallBlocks[i];
		// 旋转坐标变换公式
		smallBlocks[i].col = p.col - tmp.row + p.row;
		smallBlocks[i].row = p.row + tmp.col - p.col;
	}
}
//绘制方块：计算每个小方块的实际屏幕坐标并绘制
void Block::draw(int leftMargin, int topMargin)
{
	for (int i = 0; i < 4; i++) {
		int x = leftMargin + smallBlocks[i].col * size;
		int y = topMargin + smallBlocks[i].row * size;
		putimage(x, y, img);
	}
}

IMAGE** Block::getImages()
{
	return imgs;
}
//运算符重载
Block& Block::operator=(const Block& other) {
	if (this == &other)
		return *this;
	this->blockType = other.blockType;

	for (int i = 0; i < 4; i++) {

		this->smallBlocks[i] = other.smallBlocks[i];
	}
	return *this;
}
//检查方块是否在地图范围内且不与已有方块重叠
bool Block::blockInMap(const vector<vector<int>>& map) {
	int rows = map.size();

	int cols = map[0].size();

	for (int i = 0; i < 4; i++) {
		if (smallBlocks[i].col < 0 || smallBlocks[i].col >= cols ||
			smallBlocks[i].row < 0 || smallBlocks[i].row >= rows ||
			map[smallBlocks[i].row][smallBlocks[i].col]) {
			return false;
		}
	}
	return true;
}

void Block::solidify(vector<vector<int>>& map)
{
	for (int i = 0; i < 4; i++) {
		//设计标记，固化对应位置
		map[smallBlocks[i].row][smallBlocks[i].col] = blockType;
	}
}

int Block::getBlockType()
{
	return blockType;
}

