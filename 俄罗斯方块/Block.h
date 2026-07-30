#pragma once
#include <easyx.h>
#include<vector>
using namespace std;

struct Point//方块的坐标
{
	int row;
	int col;
};

class Block
{
public:
	Block();
	void drop();
	void moveLeftRight(int offset);
	void rotate();
	void draw(int leftMargin,int topMargin);
	static IMAGE** getImages();
	Block& operator=(const Block& other);
	
	bool blockmap(const vector<vector<int>>&map);
	void solidify(vector<vector<int>>& map);
	int getBlockType();
private:
	int blockType;//方块的类型
	Point smallBlocks[4];//每个形状的4个小方块
	IMAGE*img;//实际游戏中一个画面有多个重复方块，指向指针就行，同一块内存可以重复渲染

	static IMAGE* imgs[7];
	static int size;
};

