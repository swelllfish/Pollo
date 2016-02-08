#pragma once
#include <atlimage.h>
#include <windows.h>
#include <math.h>

#define PTNUM		100					//绘画的点的数量
#define DIAMETER	50					//半径
#define PI			3.1415926			//派
#define AREA        DIAMETER*DIAMETER*PI//面积
#define GRAVITY     1					//重力
#define TIMER_CLK   16					//定时器周期
#define TPROPOR		41					//与真实时间的比例，由于像素点与真实距离差别大，因此提高时间比例来提高速度
#define ELASTICLOSS 3					//弹性损失
#define FRICTION	0.1					//表面摩擦

typedef struct			//移动速度
{
	double xSpeed;
	double ySpeed;
}SPEED;

typedef struct ssBGR {	//每一个像素点的参数
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char pad;
}sBGR, *pBGR;	

class Action
{
public:
	Action(void);
	~Action(void);
	void DrawCirCle(HBITMAP, POINT);	//画Pollo~	
	void SurfaceChange(int, int);		//界面边界变化
	void CirCleMove(HBITMAP);			//Pollo移动
	void CalSpeed();					//进行新的速度的计算
	double CalVectorAngle(SPEED);		//计算向量与水平线的夹角
	SPEED CalVectorSpeed();				//碰撞结果计算
	void GetBitMap(HINSTANCE);
	void GetCurrCursor(POINT);
	void GetMouseLButton(bool);			//获取鼠标左键状态
	void Rotate(HBITMAP, float, HDC, HDC &);
	pBGR MyGetDibBits(HDC hdcSrc, HBITMAP hBmpSrc, int nx, int ny);
	//int CalBezierPoint(HDC, int, int, int, POINT);	//计算贝塞尔曲线，根据强度来决定振幅，通过比例来决定状态
private:
	SPEED Speed_Pollo;	//移动速度
	SPEED Speed_Cursor;	//鼠标速度

	POINT currpt;		//当前位置
	POINT preCursor;	//鼠标前一个位置
	POINT nowCursor;	//鼠标当前位置
	double xLocation;	//由于POINT按照整数移动，用double来计算实际位置
	double yLocation;
	int cxClient;		//界面大小
	int cyClient;
	HBITMAP hBitEyes;	//眼睛
	HBITMAP hBitShadow;	//阴影
	bool InCircleFlag;
	bool MouseLBFlag;	//记录鼠标LB状态 1为按下，0为释放
};

