#include "Block.h"
#include<stdlib.h>




IMAGE* Block::imgs[7] = { NULL,};
int Block::size = 36;

int blocks[7][4] = {
	1,3,5,7,//l
	2,4,5,7,//z 1
	3,5,4,6,//z 2
	3,5,4,7,//T
	2,3,5,7,//L
	3,5,7,6,//j
	2,3,4,5,//田

};


Block::Block()
{
	if (imgs[0] == NULL)
	{
		IMAGE imgTmp;
		loadimage(&imgTmp, "res/tiles.png");
	
		SetWorkingImage(&imgTmp);
			for (int i = 0;i < 7;i++)
			{
				imgs[i] = new IMAGE;
				getimage(imgs[i], i * size, 0, size, size);
			}
			SetWorkingImage();//恢复工作区
	}

	//随机生成一个行，即选择一种方块类型
	blockType = 1 + rand() % 7;//rand()%7取模运算，任何数除以7余数都为0，1，2，3，4，5，6
	//初始化smallBlocks
	for (int i = 0;i < 4;i++)
	{
		int value = blocks[blockType - 1][i];//二维数组的第几行和第几个
		smallBlocks[i].row = value / 2;//小算法：用0-7表示时在刚下落时
			smallBlocks[i].col = value % 2;//方块数字÷2得到行数，余数为列数
	}
	img = imgs[blockType - 1];
}
void Block::drop()
{
	for (int i = 0;i < 4;i++)
	{
		smallBlocks[i].row++;
	}
}

void Block::moveLeftRight(int offset)
{
	for (int i = 0;i < 4;i++)
	{
		smallBlocks[i].col += offset;
	}
}

void Block::rotate()
{
	Point p = smallBlocks[1];
	for (int i = 0;i < 4;i++)
	{
		Point tmp = smallBlocks[i];
		smallBlocks[i].col = p.col - tmp.row + p.row;
		smallBlocks[i].row = p.row + tmp.col - p.col;
	}
}



void Block::draw(int leftMargin, int topMargin)
{
	for (int i = 0;i < 4;i++)
	{
		int x = leftMargin + smallBlocks[i].col*size;
		int y = topMargin + smallBlocks[i].row*size;

		putimage(x, y, img);
	}
}

IMAGE** Block::getImages()
{
	return imgs;
}

Block& Block::operator=(const Block& other)//拷贝方块
{
	if (this == &other)return*this;
	
		this->blockType = other.blockType;
	for (int i = 0;i < 4;i++)
	{
		this->smallBlocks[i] = other.smallBlocks[i];
		
	}
	return*this;
}

bool Block::blockmap(const vector<vector<int>>& map)
{
	for (int i = 0;i < 4;i++)
	{
		int rows = map.size();
			int cols = map[0].size();
			if (smallBlocks[i].col < 0 || smallBlocks[i].col >= cols ||
				smallBlocks[i].row < 0 || smallBlocks[i].row >= rows ||
				map[smallBlocks[i].row][smallBlocks[i].col])
				return false;		
	}
	return true;
}

void Block::solidify(vector<vector<int>>& map)
{
	for (int i = 0;i < 4;i++)
	{
		map[smallBlocks[i].row][smallBlocks[i].col] = blockType;//把该大方块此刻所在的位置全部变为某种类型
	}
}

int Block::getBlockType()
{
	return blockType;
}


