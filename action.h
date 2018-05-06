#pragma once
#include "gameNode.h"

class action : public gameNode
{
private:
	image* _image;

	float _startX, _startY;			//시작 지점
	float _endX, _endY;				//도착 지점

	float _angle;
	float _travelRange;				//여행거리( 도달할 거리)
	float _worldTimeCount;			
	float _time;

	bool _isMoving;					//움직였느냥

public:
	action();
	~action();

	HRESULT init();
	void release();
	void update();
	void render();

	void moveTo(image* image, float endX, float endY, float time);

	void moving(void);

	bool getIsMoving() { return _isMoving; }
};

