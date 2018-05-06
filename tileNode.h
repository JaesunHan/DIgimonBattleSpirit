#pragma once
#include "gameNode.h"

//한 타일의 규격은 16로 한다
#define TILESIZE 16

#define TILENUMX	30
#define TILENUMY	20
#define TILETOTALNUM	TILENUMX*TILENUMY


#define ATTR_UNMOVE 1		//벽
#define ATTR_SWAMP	2		//이속 느려지는 타일
#define ATTR_POISON 4		//독 타일
#define ATTR_FROZEN 8		//빙결 타일

//스테이지에 대한 정의
enum STAGE {
	STAGE_NONE = -1,
	STAGE_TOY,			
	STAGE_SAND,
	STAGE_ICE,
	STAGE_END
};
//지형에 대한 정의
enum TERRAIN
{
	TERRAIN_NONE=-1,
	TR_WALL,			//일반지형
	TR_SPECIAL,			//특수지형(STAGE 에 따라 적용되는게 다름\
						 ex. toy 맵일 때는 점핑!, ice 맵 일 때는 미끄러짐,\
							 sand 맵 일 때는 느려짐)
	TR_END
};


//포지션 (맵을 로드해 왔을때 실제로 할당이 될 포지션영역)
enum POS
{
	POS_FLAG1, POS_FLAG2, POS_TANK1, POS_TANK2
};



static string _stageKey[STAGE_END] =
{
	"toyTile",
	"sandTile",
	"iceTile"
};

static string _stageSpecialLandKey[STAGE_END] =
{
	"cloud",
	"sand",
	"ice"
};

static string _sampleSpecialKey[STAGE_END] =
{
	"special_toy_sampleImg",
	"special_sand_sampleImg",
	"special_ice_sampleImg"
};

static string _backgroundKey[STAGE_END] =
{
	"toyBackground",
	"sandBackground",
	"iceBackground"
};

class tileNode : public gameNode
{
private:
	STAGE	_stage;
	TERRAIN _terrain;
	
	int _left, _top;		//타일이 깔릴 위치 (left, top)
	int _startFrameX;		//애니메이션타일일 경우 시작해야하는 타일인덱스
	int _terrainFrameX;		//타일 셋의 인덱스 X
	int _terrainFrameY;		//타일 셋의 인덱스 Y
	
	int _cnt;				//프레임 업데이트용
	bool _isGround;			//타일이 맨 바닥에 깔린 경우 true
public:
	tileNode();
	~tileNode();

	void frameUpdate();

	STAGE getStage() { return _stage; }			void setStage(STAGE stage) { _stage = stage; }
	TERRAIN getTerrain() { return _terrain; }	void setTerrain(TERRAIN terrain) { _terrain = terrain; }
	void setCount(int cnt) { _cnt = cnt; }
	//타일의 위치 - left, top 기준
	int getLeft() { return _left; }		void setLeft(int l) { _left = l; }
	int getTop() { return _top; }		void setTop(int t) { _top = t; }
	int getStartFrameX() { return _startFrameX; }
	int getTerrainFrameX() { return _terrainFrameX; }	void setTerrainFrameX(int frameX) { _terrainFrameX = frameX; _startFrameX = frameX; }
	int getTerrainFrameY() { return _terrainFrameY; }	void setTerrainFrameY(int frameY) { _terrainFrameY = frameY; }
	RECT getRect() { return RectMake(_left, _top, TILESIZE, TILESIZE); }

	bool getIsGround() { return _isGround; } void setIsGround(bool b) { _isGround = b; }
};
