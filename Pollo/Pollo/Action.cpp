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
	yLocation	= 500;
	Speed_Pollo.xSpeed		= 1;
	Speed_Pollo.ySpeed		= -80;
	preCursor.x = 0;
	preCursor.y = 0;
	nowCursor.x = 0;
	nowCursor.y = 0;
	InCircleFlag = FALSE;
}


Action::~Action(void)
{
}

void Action::CalSpeed()
{
	double dGravity = GRAVITY;
	double dFrequency = (double)TIMER_CLK/TPROPOR;

	if(yLocation < DIAMETER || yLocation > cyClient - DIAMETER)
	{
		Speed_Pollo.ySpeed = -(Speed_Pollo.ySpeed / ELASTICLOSS);
	}

	if(xLocation < DIAMETER || xLocation > cxClient - DIAMETER)
	{
		Speed_Pollo.xSpeed = -(Speed_Pollo.xSpeed / ELASTICLOSS);
	}

	Speed_Pollo.ySpeed += dGravity * dFrequency;	//当前速度等于 t*a+v

	if(Speed_Pollo.ySpeed > 0 && Speed_Pollo.ySpeed < 3.2)
	{
		Speed_Pollo.xSpeed -= FRICTION * Speed_Pollo.xSpeed;
	}
	//用鼠标来提供碰撞
	double Distance = sqrt(double(pow((double)(nowCursor.x - currpt.x), 2) + pow((double)(nowCursor.y - currpt.y), 2)));

	double Speed = sqrt(pow(Speed_Pollo.xSpeed, 2) + pow(Speed_Pollo.ySpeed, 2));

	if(MouseLBFlag)	//如果是鼠标拖动
	{
		Speed_Pollo.ySpeed = Speed_Cursor.ySpeed;
		Speed_Pollo.xSpeed = Speed_Cursor.xSpeed;

		xLocation = nowCursor.x;
		yLocation = nowCursor.y;
	}

	else if(Distance <= DIAMETER)
	{
		double CursorAngle = atan((double)(nowCursor.y - currpt.y) / (nowCursor.x - currpt.x + 0.0001));
		//arctan值域为-2/PI到2/PI，无法表达所有角度，因此要根据相对位置来加减角度得到真实角度
		if (nowCursor.x - currpt.x < 0 && nowCursor.y - currpt.y >= 0)
		{
			CursorAngle = CursorAngle + PI;
		}
		else if (nowCursor.x - currpt.x < 0 && nowCursor.y - currpt.y < 0)
		{
			CursorAngle = CursorAngle - PI;
		}

		double CircleAngle = atan((double)(Speed_Pollo.ySpeed / (Speed_Pollo.xSpeed + 0.0001)));//为了防止除以0
		if (Speed_Pollo.xSpeed < 0 && Speed_Pollo.ySpeed >= 0)
		{
			CircleAngle = CircleAngle + PI;
		}
		else if (Speed_Pollo.xSpeed < 0 && Speed_Pollo.ySpeed < 0)
		{
			CircleAngle = CircleAngle - PI;
		}

		//计算角度时，根据相对大小来计算夹角
		if(CursorAngle >= 0)
		{
			ResultAngle = CursorAngle * 2 - CircleAngle - PI;
		}
		else
		{
			ResultAngle = CursorAngle * 2 - CircleAngle + PI;
		}

		//使ResultAngle的值在-PI到PI之间
		if(ResultAngle > PI)
		{
			ResultAngle = 2 * PI - ResultAngle;
		}
		else if (ResultAngle < -PI)
		{
			ResultAngle = 2 * PI + ResultAngle;
		}

		if(InCircleFlag == FALSE)
		{
			//if((Speed_Pollo.ySpeed >= 0 && nowCursor.y - currpt.y < 0) || (Speed_Pollo.ySpeed < 0 && nowCursor.y - currpt.y >= 0))	//不同号取正
			if(ResultAngle >= 0)
			{
				Speed_Pollo.ySpeed = fabs(sin(ResultAngle)) * Speed + Speed_Cursor.ySpeed;
			}
			else
			{
				Speed_Pollo.ySpeed = -fabs(sin(ResultAngle)) * Speed + Speed_Cursor.ySpeed;
			}

			//if((Speed_Pollo.xSpeed < 0 && nowCursor.x - currpt.x > 0) || (Speed_Pollo.xSpeed > 0 && nowCursor.x - currpt.x < 0))//不同号取正
			if(ResultAngle >= -2/PI && ResultAngle < 2/PI)
			{
				Speed_Pollo.xSpeed = fabs(cos(ResultAngle)) * Speed + Speed_Cursor.xSpeed;
			}
			else
			{
				Speed_Pollo.xSpeed = -fabs(cos(ResultAngle)) * Speed + Speed_Cursor.xSpeed;
			}
			InCircleFlag = TRUE;
		}
	}

	if(Distance > DIAMETER)
	{
		InCircleFlag = FALSE;
	}
}

void Action::CirCleMove(HBITMAP hBitMap)
{
	double dFrequency = (double)TIMER_CLK/TPROPOR;


	xLocation += Speed_Pollo.xSpeed * dFrequency;
	yLocation += Speed_Pollo.ySpeed * dFrequency;

	CalSpeed();

	if(yLocation > cyClient - DIAMETER)
	{
		yLocation = cyClient - DIAMETER;
	}

	if(xLocation > cxClient - DIAMETER)
	{
		xLocation = cxClient - DIAMETER;
	}

	if(yLocation < DIAMETER)
	{
		yLocation = DIAMETER;
	}

	if(xLocation < DIAMETER)
	{
		xLocation = DIAMETER;
	}

	currpt.x = (int)xLocation;
	currpt.y = (int)yLocation;

	DrawCirCle(hBitMap, currpt);
}

void Action::DrawCirCle(HBITMAP hBitMap, POINT pt)
{
	POINT OutPoint[PTNUM];		//用于计算的点
	POINT InPoint[PTNUM];
	int ptNum;
	HDC hdcBuffer, hdcEyes, hdcShadow;
	HBRUSH hBrush;
	static int i = 1000;

	hdcBuffer = CreateCompatibleDC(NULL);
	hdcEyes = CreateCompatibleDC(NULL);
	hdcShadow = CreateCompatibleDC(NULL);

	SelectObject(hdcBuffer, hBitMap);
	PatBlt(hdcBuffer, 0, 0, cxClient, cyClient, WHITENESS);
	for(ptNum=0; ptNum<PTNUM; ptNum++)
	{
		OutPoint[ptNum].x = (int)(DIAMETER * cos(2 * PI * ((double)ptNum+1) / (double)PTNUM)) + pt.x;
		OutPoint[ptNum].y = (int)(DIAMETER * sin(2 * PI * ((double)ptNum+1) / (double)PTNUM)) + pt.y;
		InPoint[ptNum].x = (int)((DIAMETER - 1) * cos(2 * PI * ((double)ptNum+1) / (double)PTNUM)) + pt.x;
		InPoint[ptNum].y = (int)((DIAMETER - 1) * sin(2 * PI * ((double)ptNum+1) / (double)PTNUM)) + pt.y;
		if(InPoint[ptNum].y < pt.y)
		{
			InPoint[ptNum].y = pt.y;
		}
	}
	SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
	SelectObject(hdcBuffer,GetStockObject(NULL_BRUSH));
	Polygon(hdcBuffer, OutPoint, PTNUM);
	SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
	hBrush = CreateSolidBrush(RGB(100, 255, 0));
	//SelectObject(hdcBuffer, GetStockObject(LTGRAY_BRUSH));
	SelectObject(hdcBuffer, hBrush);
	Polygon(hdcBuffer, InPoint, PTNUM);
	SelectObject(hdcBuffer, GetStockObject(BLACK_PEN));
	//CalBezierPoint(hdcBuffer, 1, 10, 1, pt);

	SelectObject(hdcEyes, hBitEyes);

	HDC hdcRotateBuff;
	Rotate(hBitEyes, 0, hdcBuffer, hdcRotateBuff);

	//BitBlt(hdcBuffer, pt.x + 7, pt.y - 18, 14, 17, hdcEyes, 0, 0, SRCAND);
	//BitBlt(hdcBuffer, pt.x + 23, pt.y - 28, 14, 17, hdcEyes, 0, 0, SRCAND);

	BitBlt(hdcBuffer, pt.x + 7, pt.y - 18, 14, 17, hdcRotateBuff, 0, 0, SRCAND);


	SelectObject(hdcShadow, hBitShadow);
	BLENDFUNCTION Blendfunction;
	Blendfunction.BlendOp = AC_SRC_OVER;
	Blendfunction.BlendFlags = 0;
	Blendfunction.SourceConstantAlpha = 50;
	Blendfunction.AlphaFormat = 0x00;

	AlphaBlend(hdcBuffer, pt.x - DIAMETER, pt.y - DIAMETER, DIAMETER * 2, DIAMETER * 2, hdcShadow, 0, 0, DIAMETER * 2, DIAMETER * 2, Blendfunction);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcEyes);
	DeleteDC(hdcShadow);
}

void Action::SurfaceChange(int txClient, int tyClient)
{
	cxClient = txClient;
	cyClient = tyClient;
}

void Action::GetBitMap(HINSTANCE hInstance)
{
	hBitEyes = LoadBitmap(hInstance, TEXT("Eyes"));
	hBitShadow = LoadBitmap(hInstance, TEXT("Shadow"));
}

void Action::GetCurrCursor(POINT currCursor)
{
	preCursor = nowCursor;
	nowCursor = currCursor;

	Speed_Cursor.xSpeed = nowCursor.x - preCursor.x;
	Speed_Cursor.ySpeed = nowCursor.y - preCursor.y;
}

void Action::GetMouseLButton(bool LBStatus)
{
	MouseLBFlag = LBStatus;
}

void Action::Rotate(HBITMAP hBmpSrc, float angle, HDC hdcSrc, HDC &hdcDst)	//按角度旋转图案
{
	HBITMAP hBmpDst;

	hdcDst = CreateCompatibleDC(hdcSrc);
	hBmpDst = CreateCompatibleBitmap(hdcDst, 14, 17);
	SelectObject(hdcDst, hBmpDst);

	pBGR src, dst, dstLine;
	src = MyGetDibBits(hdcSrc, hBmpSrc, 14, 17);
	dst = MyGetDibBits(hdcDst, hBmpDst, 14, 17);

	dstLine = dst;
	for(int i = 0; i < 14 * 17; ++i)
	{
		dstLine[i] = src[i];
	}

	// Set the new Bitmap	
	BITMAPINFO bi;
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = 14;
	bi.bmiHeader.biHeight = 17;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = 14 * 4 * 17;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;
	SetDIBits(hdcDst, hBmpDst, 0, 100, dst, &bi, DIB_RGB_COLORS);
	DeleteObject(hBmpDst);

	free(dst);
	free(src);
}

pBGR Action::MyGetDibBits(HDC hdcSrc, HBITMAP hBmpSrc, int nx, int ny)
{
	BITMAPINFO bi;
	BOOL bRes;
	pBGR buf;

	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = nx;
	bi.bmiHeader.biHeight = - ny;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = nx * 4 * ny;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;

	buf = (pBGR) malloc(nx * 4 * ny);
	bRes = GetDIBits(hdcSrc, hBmpSrc, 0, ny, buf, &bi, DIB_RGB_COLORS);
	if (!bRes) {
		free(buf);
		buf = 0;
	}
	return buf;
}
//******iStatus 表示在目前震动的状态，数值为1-50***************//
//******iStrength表示目前震动的强度，数值范围待定**************//
//int Action::CalBezierPoint(HDC hdcBuffer, int xPoint, int iStrength, int iStatus, POINT pt)
//{
//	POINT CtrlPoint[4];
//	static int i = 10;
//	static int flag = 0;
//	static int k = 80;
//
//	CtrlPoint[0].x = pt.x - DIAMETER;
//	CtrlPoint[0].y = pt.y;
//
//	CtrlPoint[1].x = pt.x - DIAMETER + iStrength;
//	CtrlPoint[1].y = pt.y - i;
//	
//	CtrlPoint[2].x = pt.x + DIAMETER - iStrength;
//	CtrlPoint[2].y = pt.y + i;
//
//	CtrlPoint[3].x = pt.x + DIAMETER;
//	CtrlPoint[3].y = pt.y;
//
//	PolyBezier(hdcBuffer, CtrlPoint, 4);
//
//	/*
//	double t = 0.3;
//	int y = (int)((pt.x - DIAMETER) * pow((1-t),3) + 
//			3 * CtrlPoint[0].x * t * pow((1-t),2) +
//			3 * CtrlPoint[1].x * pow(t, 2)*(1-t) +
//			pt.x + DIAMETER * pow(t,3));
//	*/
//
//	if(flag == 1)
//	{
//		i += 6;
//		if(i >= k)
//		{
//			flag = 0;
//			k -= 2;
//			return 0;
//		}
//	}
//
//	if(flag == 0)
//	{
//		i -= 6;
//		if(i <= -k)
//		{
//			flag = 1;
//			k -= 2;
//		}
//	}
//	return 0;
//}
