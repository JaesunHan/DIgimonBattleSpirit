#pragma once

#define PI 3.141592654f
#define PI2 PI * 2

#define PI8		float(PI / 8.0f)
#define PI16	float(PI / 16.0f)
#define PI32	float(PI / 32.0f)
#define PI64	float(PI / 64.0f)
#define PI128	float(PI / 128.0f)

namespace HEPTA_UTIL
{
	float getDistance(float startX, float startY, float endX, float endY);

	float getAngle(float x1, float y1, float x2, float y2);

	void textPrint(HDC hdc, const char* textInput, int textX, int textY, int fontWidth = 20, int fontThick = 20, char* fontName = "HY°ß°íµñ", COLORREF RGB = RGB(255, 255, 255), BOOL textCenter = TRUE);
	void outlineTextOut(HDC hdc, int left, int top, const char* str, COLORREF fontColor, COLORREF outlineColor, COLORREF currentFontColor, int curFontSize);

	TCHAR* changeIntToTChar(int num);
	TCHAR* changeStringNIntToTChar(char str[512], int num);


}