#include "stdafx.h"
#include "collision.h"

namespace HEPTA_UTIL
{
	bool isCollisionReaction(const RECT& rcHold, RECT& rcMove)
	{
		//렉트끼리 충돌했을때 안에 생성되는 렉트 
		RECT rcInter;

		//충돌하지 않았다면 검출이 불필요하다!
		if (!IntersectRect(&rcInter, &rcHold, &rcMove)) return false;

		int interSectorW = rcInter.right - rcInter.left;
		int interSectorH = rcInter.bottom - rcInter.top;
		//수직으로 부딪혔다면~~~
		if (interSectorW > interSectorH)
		{
			//위에서 박았으면
			if (rcInter.top == rcHold.top)
			{
				OffsetRect(&rcMove, 0, -interSectorH);
			}
			else if (rcInter.bottom == rcHold.bottom)//아래에서 부딪혔다면
			{
				OffsetRect(&rcMove, 0, interSectorH);
			}
		}
		else
		{
			//오른쪽에서 가다 박으면
			if (rcInter.left == rcHold.left)
			{
				OffsetRect(&rcMove, -interSectorW, 0);
			}
			else if (rcInter.right == rcHold.right)
			{
				OffsetRect(&rcMove, interSectorW, 0);
			}
		}
	}

	void setWindowSize(int x, int y, int width, int height)
	{
		RECT winRect;

		winRect.left = 0;
		winRect.top = 0;
		winRect.right = width;
		winRect.bottom = height;

		//윈도우 사이즈 재조정을 해준다
		AdjustWindowRect(&winRect, WINSTYLE, false);

		//윈도우 위치를 재조정
		SetWindowPos(_hWnd, NULL, x, y, (winRect.right - winRect.left),
			(winRect.bottom - winRect.top), SWP_NOZORDER | SWP_NOMOVE);


	}

}