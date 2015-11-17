#pragma once
#include <windows.h>
#include <math.h>

#define PTNUM		100					//绘画的点的数量
#define DIAMETER	50					//半径
#define PI			3.1415926			//派
#define AREA        DIAMETER*DIAMETER*PI//面积
#define GRAVITY     10					//重力
#define TIMER_CLK   10					//定时器周期
#define TPROPOR		41					//与真实时间的比例，由于像素点与真实距离差别大，因此提高时间比例来提高速度
#define ELASTICLOSS 1.3					//弹性损失

class Action
{
public:
	Action(void);
	~Action(void);
	void DrawCirCle(HBITMAP, POINT);	//画Pollo~
	void CirCleMove(HBITMAP, POINT);	//Pollo移动
	void SurfaceChange(int, int);	//界面边界变化
	void CalSpeed();
	void GetEyesBitMap(HINSTANCE);
private:
	POINT currpt;		//当前位置
	double xLocation;	//由于POINT按照整数移动，用double来计算实际位置
	double yLocation;
	int cxClient;		//界面大小
	int cyClient;
	double xSpeed;		//速度
	double ySpeed;
	HBITMAP hBitEyes;	//眼睛
};

