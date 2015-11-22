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
	POINT point[PTNUM];		//用于计算的点
	int ptNum;
	HDC hdcBuffer, hdcEyes;
	HBRUSH hBrush;
	static int i = 1000;

	hdcBuffer = CreateCompatibleDC(NULL);
	hdcEyes = CreateCompatibleDC(NULL);

	hBrush = CreateSolidBrush(RGB(rand()%255, rand()%255, rand()%255));
	SelectObject(hdcBuffer,hBrush);
	//SelectObject(hdcBuffer,GetStockObject(LTGRAY_BRUSH));
	//SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
	SelectObject(hdcBuffer, hBitMap);
	PatBlt(hdcBuffer, 0, 0, cxClient, cyClient, WHITENESS);
	for(ptNum=0; ptNum<PTNUM; ptNum++)
	{
		point[ptNum].x = (int)(DIAMETER * cos(2 * PI * ((double)ptNum+1) / (double)PTNUM)) + pt.x;
		point[ptNum].y = (int)(DIAMETER * sin(2 * PI * ((double)ptNum+1) / (double)PTNUM)) + pt.y;
	}
	Polygon(hdcBuffer, point, PTNUM);
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