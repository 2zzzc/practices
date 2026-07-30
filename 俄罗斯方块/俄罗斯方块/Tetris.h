#pragma once
#include<vector>
#include<graphics.h>
#include"Block.h"

using namespace std;
class Tetris
{
public:
	Tetris(int rows, int cols, int left, int top, int blockSize);//游戏的界面尺寸

	  void init();//初始化
	  void play();//开始游戏
	  

private:
	void keyEvent();
	void updateWindow();
	
	int getDelay();//返回距离上一次调用该函数间隔时间（ms）
	void drop();
	void clearLine();
	void moveleftright(int offset);
	void rotate();
	void drawScore();
	void checkover();
	void saveScore();
	void displayover();

private:
	int delay; 
	bool update;//是否更新
	//0:没有方块   5：第五种
	vector<vector<int>> map;//map：初始化了一个动态的全0矩阵
							//来表示各个位置的状态
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;

	IMAGE imgBg;

	Block* curblock;
	Block* nextblock;
	Block bakblock;//当前方块下落过程，备用上一个合法位置
	int score;
	int level;
	int lineCount;
	int highestScore;
	bool gameover;
	IMAGE imgover;
	IMAGE imgwin;
};

