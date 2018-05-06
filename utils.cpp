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

	//                      �Է����ؽ�Ʈ           �ؽ�Ʈ��ġ XY		 ��ƮSIZE       ��Ʈ����       ��Ʈ��          �ؽ�Ʈ����(�⺻���)  �ؽ�Ʈ �߾����� 
	void textPrint(HDC hdc, const char* textInput, int textX, int textY, int fontWidth, int fontThick, char* fontName, COLORREF RGB, BOOL textCenter)
	{
		//----------------------------------- ��Ʈ����
		HFONT font, oldFont;
		SetBkMode(hdc, TRANSPARENT);                  //���� ������
		if (textCenter) SetTextAlign(hdc, TA_CENTER); //���� �߾�����
		SetTextColor(hdc, RGB);                       //���� ���󺯰�
		font = CreateFont(fontWidth, 0, 0, 0, fontThick, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT(fontName));
		oldFont = (HFONT)SelectObject(hdc, font);
		//----------------------------------- �������
		TCHAR str[256];
		ZeroMemory(&str, sizeof(str));
		wsprintf(str, "%s", textInput);
		TextOut(hdc, textX, textY, str, strlen(str));
		//----------------------------------- ���� �������� ����
		SelectObject(hdc, oldFont);
		DeleteObject(font);
		SetTextColor(hdc, RGB(0, 0, 0));
		SetTextAlign(hdc, TA_LEFT);
	}

	//							  �ؽ�Ʈ��ġ X, Y	�ؽ�Ʈ				�ؽ�Ʈ����			�ܰ�������				������Ʈ����					������Ʈ������
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
	//���ڸ� TCHAR �� ��ȯ!		=> ex) 5 -> "5"
	TCHAR* changeIntToTChar(int num)
	{
		TCHAR str[512];
		wsprintf(str, "%d", num);
		return str;
	}
	//���ڿ� ���ڸ� �����Ͽ� ���ڿ��� ��ȯ => ex) "Player", 2 -> "Player2"
	TCHAR* changeStringNIntToTChar(char strChange[512], int num)
	{
		TCHAR str[512];
		wsprintf(str, "%s%d", strChange, num);
		return str;
	}
	
}