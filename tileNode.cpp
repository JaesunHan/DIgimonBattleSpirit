#include "stdafx.h"
#include "tileNode.h"


tileNode::tileNode()
	: _stage(STAGE_NONE),
	_terrain(TERRAIN_NONE),
	_terrainFrameX(0),
	_terrainFrameY(0),
	_isGround(false)
{
	_cnt = 0;
	
}


tileNode::~tileNode()
{
}

//스페셜 타일은 애니메이션이 적용 되어야 한다.
void tileNode::frameUpdate()
{
	_cnt++;
	if (_cnt % 15 == 0)
	{
		_terrainFrameX = _terrainFrameX + 3;
		if (_terrainFrameX > IMAGEMANAGER->findImage(_stageSpecialLandKey[_stage])->getMaxFrameX())
			_terrainFrameX = _startFrameX;

		_cnt = 0;
	}

}