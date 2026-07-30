#include "Tetris.h"
#include<time.h>
#include<stdlib.h>
#include"Block.h"
#include<conio.h>
#include<fstream>
#include<iostream>

#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define MAX_LEVEL 5
#define record_file "record.txt"

//const int SPEED_NORMAL = 500;//ms、
const int SPEED_NORMAL[MAX_LEVEL] = { 500,300,150,100,80 };
const int SPEED_QUICK = 50;//ms

Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)
{
	this->rows = rows;   //此rows并非彼rows，第一个rows是this对象的属性的rows，第二个rows是该函数形参的rows
	this->cols = cols;
	this->leftMargin = left;
	this->topMargin = top;
	this->blockSize = blockSize;

	for (int i = 0;i < rows;i++)
	{
		vector<int>mapRow;
		for (int j = 0;j < cols;j++)
		{
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}
}

void Tetris::init()
{

	mciSendString("play res/bg.mp3 repeat", 0, 0,0);
	delay = SPEED_NORMAL[0];

	//配置随机种子
	srand((unsigned)time(NULL));//time(NULL)获取当前时间的秒数（从1970。1。1开始）
	
	initgraph(938, 896);//创建窗口的大小

	//加载背景图片
	loadimage(&imgBg, "res/bg2.png");

	loadimage(&imgwin, "res/win.png");
	loadimage(&imgover, "res/over.png");

	//初始化游戏区的数据
	char data[20][10];
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			map[i][j] = 0;
		}
	}

	score = 0;
	lineCount = 0;
	level = 1;

	ifstream file(record_file);
	if (!file.is_open())
	{
		std::cout<< record_file << "filed to open" << std::endl;
	}
	else {
		file >> highestScore;
	}


	file.close();


	gameover = false;

}						//srand以该种子作为配方，之后rand从其中获取随机数

void Tetris::play()//游戏主循环
{
	init();
	nextblock = new Block();
	curblock = nextblock;
	nextblock = new Block();

	int timer = 0;
	while (1)
	{
		
		//接收用户输入
		keyEvent();
		timer += getDelay();
		if (timer > delay)
		{
			timer = 0;
			drop();
			update = true;
		}
		if (update)
		{
			update = false;
			//渲染游戏画面
			updateWindow();

			//更换游戏数据
			clearLine();

		}
		if (gameover)
		{//保存分数
			saveScore();

			//更新游戏画面
			displayover();

			system("pause");
				init();
		}
	}
}

void Tetris::keyEvent()
{
	unsigned char ch;
	bool rotateFlag = false;
	int dx =0;

	if (_kbhit())
	{
		ch =_getch();//上224，72，下224，80
					//左224，75，右224，77
		if (ch == 224)
		{
			ch =_getch();
			switch (ch)
			{
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
		if (rotateFlag)
		{
			rotate();
			update = true;
			 
		}
		if (dx != 0)//left right
		{
			printf("hello");
			moveleftright(dx);
			update = true;
		}

	}

}

void Tetris::updateWindow()
{
	

	IMAGE** imgs = Block::getImages();
	BeginBatchDraw();//一次全部刷新完再显示
	putimage(0, 0, &imgBg);//绘制背景图片
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			if (map[i][j] ==0)continue;
			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}
	
	curblock->draw(leftMargin, topMargin);
	nextblock -> draw(689, 150);
	drawScore();
	EndBatchDraw();//结束批量模式

}

int Tetris::getDelay()
{
	static unsigned long long LastTime = 0;
	unsigned long long currentTime = GetTickCount();
	
	if (LastTime == 0)
	{
		LastTime = currentTime;
		return 0;
	}
	else
	{
		int ret = currentTime - LastTime;
		LastTime = currentTime;
		return ret;
	}
}

void Tetris::drop()
{
	bakblock = *curblock;
	curblock->drop();

	if (!curblock->blockmap(map))
	{
		bakblock.solidify(map);
		delete curblock;
		curblock = nextblock;
		nextblock = new Block();
		checkover();
	}
	
	delay = SPEED_NORMAL[level-1];

}

void Tetris::clearLine()
{
	int line = 0;
	int k = rows - 1;
	for (int i = rows - 1;i > 0;i--)//扫描行
	{
		int count = 0;
		for (int j = 0;j < cols;j++)//扫描列
		{
			if (map[i][j])//如果对于一行中每一列都有方块（为真）
			{
				count++;
			}
			map[k][j] = map[i][j];//扫描后将扫描的这一行复制转入存储行
		}
			if (count < cols)//不是满的行
			{
				k--;//存储行存的目标行移动
			}
		else{
				line++;//存储行存的目标行不移动，新复制进的行将已经满的行覆盖掉
			}
	}
	if (line > 0)
	{
		int addScore[4] ={ 10,30,60,80 };
		score += addScore[line - 1];

		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;
		
		level = (score + 99) / 100;//100 1 level
		if (level > MAX_LEVEL)
		{
			gameover = true;
		}
		lineCount += line;
	}

	
}

void Tetris::moveleftright(int offset)
{
	bakblock = *curblock;
	curblock->moveLeftRight(offset);
	if (!curblock->blockmap(map))
	{
		*curblock = bakblock;
	}
}

void Tetris::rotate()
{
	if (curblock->getBlockType() == 7)return;
	
	bakblock = *curblock;
	curblock->rotate();
	if (!curblock->blockmap(map))
	{
		*curblock = bakblock;
	}

}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);//获取当前字体
	
	setcolor(RGB(180, 180, 180));
	
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);

	setbkmode(TRANSPARENT);

	outtextxy(670, 727, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);
	int xPos = 224 - f.lfWidth * strlen(scoreText);
	outtextxy(xPos, 817, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	outtextxy(224 - 30, 727, scoreText);
	
	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(670, 817, scoreText);
}

void Tetris::checkover()
{
	
	gameover = (curblock->blockmap(map) == false);
}

void Tetris::saveScore()
{
	if (score > highestScore)
	{
		highestScore = score;
		ofstream file(record_file);
		file << highestScore;
		file.close();
	}
}

void Tetris::displayover()
{
	mciSendString("stop res/bg.mp3", 0, 0, 0);
	if (level <= MAX_LEVEL)
	{
		putimage(262, 361, &imgover);
		mciSendString("play res/over.mp3",0,0,0);
	}
	else
	{
		putimage(262, 361, &imgwin);
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}
