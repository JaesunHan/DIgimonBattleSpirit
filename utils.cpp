#include "stdafx.h"
#include "utils.h"

namespace HEPTA_UTIL
{

	float getDistance(float startX, float startY, float endX, float endY)
	{
		float x = endX - startX;
		float y = endY - startY;

		return sqrtf(x * x + y * y);
	}

	float getAngle(float x1, float y1, float x2, float y2)
	{
		float x = x2 - x1;
		float y = y2 - y1;

		float distance = sqrtf(x * x + y * y);

		float angle = acosf(x / distance);

		if (y2 > y1)
		{
			angle = 2 * PI - angle;
			if (angle >= 2 * PI) angle -= 2 * PI;
		}

		return angle;
	}

	//                      입력할텍스트           텍스트위치 XY		 폰트SIZE       폰트굵기       폰트명          텍스트색상(기본흰색)  텍스트 중앙정렬 
	void textPrint(HDC hdc, const char* textInput, int textX, int textY, int fontWidth, int fontThick, char* fontName, COLORREF RGB, BOOL textCenter)
	{
		//----------------------------------- 폰트셋팅
		HFONT font, oldFont;
		SetBkMode(hdc, TRANSPARENT);                  //문자 투명배경
		if (textCenter) SetTextAlign(hdc, TA_CENTER); //문자 중앙정렬
		SetTextColor(hdc, RGB);                       //문자 색상변경
		font = CreateFont(fontWidth, 0, 0, 0, fontThick, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT(fontName));
		oldFont = (HFONT)SelectObject(hdc, font);
		//----------------------------------- 문자출력
		TCHAR str[256];
		ZeroMemory(&str, sizeof(str));
		wsprintf(str, "%s", textInput);
		TextOut(hdc, textX, textY, str, strlen(str));
		//----------------------------------- 이전 설정으로 복구
		SelectObject(hdc, oldFont);
		DeleteObject(font);
		SetTextColor(hdc, RGB(0, 0, 0));
		SetTextAlign(hdc, TA_LEFT);
	}

	//							  텍스트위치 X, Y	텍스트				텍스트색상			외곽선색상				현재폰트색상					현재폰트사이즈
	void outlineTextOut(HDC hdc, int left, int top, const char * str, COLORREF fontColor, COLORREF outlineColor, COLORREF currentFontColor, int curFontSize)
	{
		SetTextColor(hdc, outlineColor);

		if (curFontSize < 40) curFontSize = 40;

		TextOut(hdc, left + curFontSize / 40, top, str, strlen(str));
		TextOut(hdc, left - curFontSize / 40, top, str, strlen(str));
		TextOut(hdc, left, top + curFontSize / 40, str, strlen(str));
		TextOut(hdc, left, top - curFontSize / 40, str, strlen(str));

		SetTextColor(hdc, fontColor);
		TextOut(hdc, left, top, str, strlen(str));

		SetTextColor(hdc, currentFontColor);
	}
	//숫자를 TCHAR 로 반환!		=> ex) 5 -> "5"
	TCHAR* changeIntToTChar(int num)
	{
		TCHAR str[512];
		wsprintf(str, "%d", num);
		return str;
	}
	//문자와 숫자를 조합하여 문자열로 변환 => ex) "Player", 2 -> "Player2"
	TCHAR* changeStringNIntToTChar(char strChange[512], int num)
	{
		TCHAR str[512];
		wsprintf(str, "%s%d", strChange, num);
		return str;
	}
	
}