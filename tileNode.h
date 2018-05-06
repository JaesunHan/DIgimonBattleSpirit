#pragma once
#include "gameNode.h"

//�� Ÿ���� �԰��� 16�� �Ѵ�
#define TILESIZE 16

#define TILENUMX	30
#define TILENUMY	20
#define TILETOTALNUM	TILENUMX*TILENUMY


#define ATTR_UNMOVE 1		//��
#define ATTR_SWAMP	2		//�̼� �������� Ÿ��
#define ATTR_POISON 4		//�� Ÿ��
#define ATTR_FROZEN 8		//���� Ÿ��

//���������� ���� ����
enum STAGE {
	STAGE_NONE = -1,
	STAGE_TOY,			
	STAGE_SAND,
	STAGE_ICE,
	STAGE_END
};
//������ ���� ����
enum TERRAIN
{
	TERRAIN_NONE=-1,
	TR_WALL,			//�Ϲ�����
	TR_SPECIAL,			//Ư������(STAGE �� ���� ����Ǵ°� �ٸ�\
						 ex. toy ���� ���� ����!, ice �� �� ���� �̲�����,\
							 sand �� �� ���� ������)
	TR_END
};


//������ (���� �ε��� ������ ������ �Ҵ��� �� �����ǿ���)
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
	
	int _left, _top;		//Ÿ���� �� ��ġ (left, top)
	int _startFrameX;		//�ִϸ��̼�Ÿ���� ��� �����ؾ��ϴ� Ÿ���ε���
	int _terrainFrameX;		//Ÿ�� ���� �ε��� X
	int _terrainFrameY;		//Ÿ�� ���� �ε��� Y
	
	int _cnt;				//������ ������Ʈ��
	bool _isGround;			//Ÿ���� �� �ٴڿ� �� ��� true
public:
	tileNode();
	~tileNode();

	void frameUpdate();

	STAGE getStage() { return _stage; }			void setStage(STAGE stage) { _stage = stage; }
	TERRAIN getTerrain() { return _terrain; }	void setTerrain(TERRAIN terrain) { _terrain = terrain; }
	void setCount(int cnt) { _cnt = cnt; }
	//Ÿ���� ��ġ - left, top ����
	int getLeft() { return _left; }		void setLeft(int l) { _left = l; }
	int getTop() { return _top; }		void setTop(int t) { _top = t; }
	int getStartFrameX() { return _startFrameX; }
	int getTerrainFrameX() { return _terrainFrameX; }	void setTerrainFrameX(int frameX) { _terrainFrameX = frameX; _startFrameX = frameX; }
	int getTerrainFrameY() { return _terrainFrameY; }	void setTerrainFrameY(int frameY) { _terrainFrameY = frameY; }
	RECT getRect() { return RectMake(_left, _top, TILESIZE, TILESIZE); }

	bool getIsGround() { return _isGround; } void setIsGround(bool b) { _isGround = b; }
};
