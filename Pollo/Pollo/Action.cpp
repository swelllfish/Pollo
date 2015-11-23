/* Version: v1.0
   Owner: Swell_Fish
   Remarks: Now I'm learning the Palette.But don't wantted stop
			commit new thing.So modify some Notes to make it looks better
*/

#include "StdAfx.h"
#include "Action.h"

Action::Action(void)
{
	xLocation	= 500;
	yLocation	= DIAMETER;
	xSpeed		= 0;
	ySpeed		= 0;
}


Action::~Action(void)
{
}

void Action::CalSpeed()
{
	double dGravity = GRAVITY;
	double dFrequency = (double)TIMER_CLK/TPROPOR;

	if(currpt.y == 0 || currpt.y == cyClient - DIAMETER)
	{
		ySpeed = -(ySpeed / ELASTICLOSS);
	}

	if(currpt.x == 0 || currpt.x == cxClient - DIAMETER)
	{
		xSpeed = -(xSpeed / ELASTICLOSS);
	}

	ySpeed += dGravity * dFrequency;	//当前速度等于 t*a+v

}

void Action::CirCleMove(HBITMAP hBitMap, POINT despt)
{
	double dFrequency = (double)TIMER_CLK/TPROPOR;

	CalSpeed();

	xLocation += xSpeed * dFrequency;
	yLocation += ySpeed * dFrequency;

	currpt.x = (int)xLocation;
	currpt.y = (int)yLocation;

	if(currpt.y > cyClient - DIAMETER)
	{
		currpt.y = cyClient - DIAMETER;
	}

	if(currpt.x > cyClient - DIAMETER)
	{
		currpt.x = cxClient - DIAMETER;
	}

	DrawCirCle(hBitMap, currpt);
}

void Action::DrawCirCle(HBITMAP hBitMap, POINT pt)
{
	POINT OutPoint[PTNUM];		//用于计算的点
	POINT InPoint[PTNUM];
	int ptNum;
	HDC hdcBuffer, hdcEyes;
	HBRUSH hBrush;
	static int i = 1000;

	hdcBuffer = CreateCompatibleDC(NULL);
	hdcEyes = CreateCompatibleDC(NULL);

	//hBrush = CreateSolidBrush(RGB(rand()%255, rand()%255, rand()%255));
	//SelectObject(hdcBuffer,hBrush);
	SelectObject(hdcBuffer, hBitMap);
	PatBlt(hdcBuffer, 0, 0, cxClient, cyClient, WHITENESS);
	for(ptNum=0; ptNum<PTNUM; ptNum++)
	{
		OutPoint[ptNum].x = (int)(DIAMETER * cos(2 * PI * ((double)ptNum+1) / (double)PTNUM)) + pt.x;
		OutPoint[ptNum].y = (int)(DIAMETER * sin(2 * PI * ((double)ptNum+1) / (double)PTNUM)) + pt.y;
		InPoint[ptNum].x = (int)((DIAMETER - 1) * cos(2 * PI * ((double)ptNum+1) / (double)PTNUM)) + pt.x;
		InPoint[ptNum].y = (int)((DIAMETER - 1)* sin(2 * PI * ((double)ptNum+1) / (double)PTNUM)) + pt.y;
		if(InPoint[ptNum].y < pt.y)
		{
			InPoint[ptNum].y = pt.y;
		}
	}
	SelectObject(hdcBuffer,GetStockObject(NULL_BRUSH));
	Polygon(hdcBuffer, OutPoint, PTNUM);
	SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
	SelectObject(hdcBuffer, GetStockObject(LTGRAY_BRUSH));
	Polygon(hdcBuffer, InPoint, PTNUM);
	SelectObject(hdcBuffer, GetStockObject(BLACK_PEN));
	CalBezierPoint(hdcBuffer, 1, 10, 1, pt);

	SelectObject(hdcEyes, hBitEyes);
	BitBlt(hdcBuffer, pt.x + 7, pt.y - 18, 14, 17, hdcEyes, 0, 0, SRCAND);
	BitBlt(hdcBuffer, pt.x + 23, pt.y - 28, 14, 17, hdcEyes, 0, 0, SRCAND);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcEyes);
}

void Action::SurfaceChange(int txClient, int tyClient)
{
	cxClient = txClient;
	cyClient = tyClient;
}

void Action::GetEyesBitMap(HINSTANCE hInstance)
{
	hBitEyes = LoadBitmap(hInstance, TEXT("Eyes"));
}

//******iStatus 表示在目前震动的状态，数值为1-50***************//
//******iStrength表示目前震动的强度，数值范围待定**************//
int Action::CalBezierPoint(HDC hdcBuffer, int xPoint, int iStrength, int iStatus, POINT pt)
{
	POINT CtrlPoint[4];
	static int i = 10;
	static int flag = 0;
	static int k = 80;

	CtrlPoint[0].x = pt.x - DIAMETER;
	CtrlPoint[0].y = pt.y;

	CtrlPoint[1].x = pt.x - DIAMETER + iStrength;
	CtrlPoint[1].y = pt.y - i;
	
	CtrlPoint[2].x = pt.x + DIAMETER - iStrength;
	CtrlPoint[2].y = pt.y + i;

	CtrlPoint[3].x = pt.x + DIAMETER;
	CtrlPoint[3].y = pt.y;

	PolyBezier(hdcBuffer, CtrlPoint, 4);

	/*
	double t = 0.3;
	int y = (int)((pt.x - DIAMETER) * pow((1-t),3) + 
			3 * CtrlPoint[0].x * t * pow((1-t),2) +
			3 * CtrlPoint[1].x * pow(t, 2)*(1-t) +
			pt.x + DIAMETER * pow(t,3));
	*/

	if(flag == 1)
	{
		i += 6;
		if(i >= k)
		{
			flag = 0;
			k -= 2;
			return 0;
		}
	}

	if(flag == 0)
	{
		i -= 6;
		if(i <= -k)
		{
			flag = 1;
			k -= 2;
		}
	}
	return 0;
}