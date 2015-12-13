#pragma once
#include <atlimage.h>
#include <windows.h>
#include <math.h>

#define PTNUM		100					//绘画的点的数量
#define DIAMETER	50					//半径
#define PI			3.1415926			//派
#define AREA        DIAMETER*DIAMETER*PI//面积
#define GRAVITY     10					//重力
#define TIMER_CLK   16					//定时器周期
#define TPROPOR		41					//与真实时间的比例，由于像素点与真实距离差别大，因此提高时间比例来提高速度
#define ELASTICLOSS 1.3					//弹性损失

class Action
{
public:
	Action(void);
	~Action(void);
	void DrawCirCle(HBITMAP, POINT);	//画Pollo~
	void CirCleMove(HBITMAP);	//Pollo移动
	void SurfaceChange(int, int);	//界面边界变化
	void CalSpeed();
	void GetBitMap(HINSTANCE);
	void GetCurrCursor(POINT);

	double ResultAngle;

	//int CalBezierPoint(HDC, int, int, int, POINT);	//计算贝塞尔曲线，根据强度来决定振幅，通过比例来决定状态
private:
	POINT currpt;		//当前位置
	POINT preCursor;	//鼠标前一个位置
	POINT nowCursor;	//鼠标当前位置
	double xLocation;	//由于POINT按照整数移动，用double来计算实际位置
	double yLocation;
	int cxClient;		//界面大小
	int cyClient;
	double xSpeed;		//速度
	double ySpeed;
	double xCursorSpeed;//鼠标速度
	double yCursorSpeed;
	HBITMAP hBitEyes;	//眼睛
	HBITMAP hBitShadow;
	char InCircleFlag;
};

