#include "stdafx.h"
#include "guilmon.h"


guilmon::guilmon()
{
	//길몬이 사용할 불렛

	_bullet = new bullet;
	_bullet->init("guilmon_bullet", ".\\image\\digimon\\guilmon\\guilmon_jump_bullet.bmp", 32, 32, 2, 2, 4.0f, 0.0f, 100, 14);

	_statusImgInfo[STATUS_BOUNCE] = { 180, 76, 5, 2 , false };
	_statusImgInfo[STATUS_ATTACK] = { 336, 64, 6, 2 , false };
	_statusImgInfo[STATUS_ATTACK_UP] = { 324, 60, 9, 2 , false };
	//	_statusImgInfo[STATUS_BURNED] = { 480, 96, 10, 2 , true};
	_statusImgInfo[STATUS_FALL] = { 136, 80, 4, 2, true };

	_statusImgInfo[STATUS_GAMEOVER] = { 144, 60, 4, 2 , true };
	//_statusImgInfo[STATUS_GRAB] = { 336, 96, 7, 2 , false };
	//_statusImgInfo[STATUS_GRAB_FAIL] = { 96, 96, 2, 2 , false };
	//_statusImgInfo[STATUS_GRAB_JUMP] = { 432, 96, 9, 2 , false };
	//_statusImgInfo[STATUS_GRAB_WALk] = { 384, 96, 8, 2 , true};


	_statusImgInfo[STATUS_GUARD] = { 140, 60, 5, 2 , false };
	_statusImgInfo[STATUS_GUARD_IN_AIR] = { 140, 60, 5, 2, false };
	//_statusImgInfo[STATUS_SPECIAL] = { 192, 96, 4, 2, false };
	_statusImgInfo[STATUS_HURT] = { 180, 72, 5, 2, false };
	_statusImgInfo[STATUS_HURT2] = { 180, 72, 5, 2, false };

	_statusImgInfo[STATUS_IDLE] = { 512, 64, 16, 2, true };
	_statusImgInfo[STATUS_JUMP] = { 180, 76, 5, 2, false };
	//_statusImgInfo[STATUS_KNOCK_BACK] = { 672, 96, 14, 2, false };
	_statusImgInfo[STATUS_LAND] = { 320, 80, 8, 2, false };
	_statusImgInfo[STATUS_LOSE] = { 144, 60, 4, 2 , false };

	//_statusImgInfo[STATUS_RECOVER_FROM_KNOCKBACK] = { 528, 96, 11, 2 , false };
	_statusImgInfo[STATUS_RUN] = { 272, 60, 8, 2 , false };
	//_statusImgInfo[STATUS_SHOCKED] = { 336, 96, 7, 2 , false };
	_statusImgInfo[STATUS_STOP] = { 160, 64, 5, 2 , false };
	//_statusImgInfo[STATUS_STOP_IN_AIR] = { 192, 96, 3, 2, false };

	//_statusImgInfo[STATUS_STOP_IN_GROUND] = { 128, 96, 2, 2 , false };
	_statusImgInfo[STATUS_JUMP_ATTACK] = { 360, 88, 9, 2 , false };
	_statusImgInfo[STATUS_RUN_SKILL] = { 528, 88, 8, 2 , false };
	_statusImgInfo[STATUS_JUMP_SKILL] = { 280, 84, 7, 2 , false };
	//_statusImgInfo[STATUS_THROW] = { 528, 96, 11, 2 , false };
	_statusImgInfo[STATUS_WALK] = { 272, 64, 8, 2 , true };
	_statusImgInfo[STATUS_WIN] = { 680, 76, 16, 2 , true };

	//	_hitWidth = 14, _hitHeight = 24;

	//애니렌더할 때 애니메이셔 재생 속도 초기화
	_playAnimSpeed[STATUS_BOUNCE] = 14;
	_playAnimSpeed[STATUS_ATTACK] = 14;
	_playAnimSpeed[STATUS_ATTACK_UP] = 12;
	//_playAnimSpeed[STATUS_BURNED] = 10;
	_playAnimSpeed[STATUS_FALL] = 10;

	_playAnimSpeed[STATUS_GAMEOVER] = 8;
	//_playAnimSpeed[STATUS_GRAB] = 14;
	//_playAnimSpeed[STATUS_GRAB_FAIL] = 9;
	//_playAnimSpeed[STATUS_GRAB_JUMP] = 10;
	//_playAnimSpeed[STATUS_GRAB_WALk] = 10;

	_playAnimSpeed[STATUS_GUARD] = 11;
	_playAnimSpeed[STATUS_GUARD_IN_AIR] = 12;
	//_playAnimSpeed[STATUS_SPECIAL] = 10;
	_playAnimSpeed[STATUS_HURT] = 11;
	_playAnimSpeed[STATUS_HURT2] = 11;

	_playAnimSpeed[STATUS_IDLE] = 10;
	_playAnimSpeed[STATUS_JUMP] = 12;
	//_playAnimSpeed[STATUS_KNOCK_BACK] = 14;
	_playAnimSpeed[STATUS_LAND] = 10;
	_playAnimSpeed[STATUS_LOSE] = 8;

	//_playAnimSpeed[STATUS_RECOVER_FROM_KNOCKBACK] = 10;
	_playAnimSpeed[STATUS_RUN] = 11;
	//_playAnimSpeed[STATUS_SHOCKED] = 10;
	_playAnimSpeed[STATUS_STOP] = 10;
	//_playAnimSpeed[STATUS_STOP_IN_AIR] = 10;

	//_playAnimSpeed[STATUS_STOP_IN_GROUND] = 10;
	_playAnimSpeed[STATUS_JUMP_ATTACK] = 16;
	_playAnimSpeed[STATUS_RUN_SKILL] = 10;
	_playAnimSpeed[STATUS_JUMP_SKILL] = 16;
	//_playAnimSpeed[STATUS_THROW] = 10;
	_playAnimSpeed[STATUS_WALK] = 10;
	_playAnimSpeed[STATUS_WIN] = 11;
	_cnt = 0;
	wsprintf(_digimonName, "%s", "guilmon");
	setAllImage();
	_direct = RIGHT;
	_status = STATUS_IDLE;


	//타격이 들어갈 이미지프레임 설정
	//============= attack ================
	vector<int> atkFrameIdx;
	atkFrameIdx.push_back(0);
	atkFrameIdx.push_back(2);
	
	_hitFrameIdx.insert(make_pair(_statusFileNameKey[STATUS_ATTACK], atkFrameIdx));
	//============= attack_up ================
	vector<int> atkUpFrameIdx;
	atkUpFrameIdx.push_back(0);
	atkUpFrameIdx.push_back(3);
	_hitFrameIdx.insert(make_pair(_statusFileNameKey[STATUS_ATTACK_UP], atkUpFrameIdx));
	//============= run_skill ================
	vector<int> runSkillFrameIdx;
	runSkillFrameIdx.push_back(0);
	runSkillFrameIdx.push_back(3);
	_hitFrameIdx.insert(make_pair(_statusFileNameKey[STATUS_RUN_SKILL], runSkillFrameIdx));
	//============= jump_attack ================
	vector<int> jumpAtkFrameIdx;
	jumpAtkFrameIdx.push_back(0);
	jumpAtkFrameIdx.push_back(2);
	jumpAtkFrameIdx.push_back(5);
	_hitFrameIdx.insert(make_pair(_statusFileNameKey[STATUS_JUMP_ATTACK], jumpAtkFrameIdx));




}


guilmon::~guilmon()
{

}


HRESULT guilmon::init(char name[MAXNAME])
{
	digimon::init(name);
	
	return S_OK;
}
void guilmon::update()					 
{
	digimon::update();

	


	//공격 상태가 아닐때는 어택 박스를 잠시 차가운 화면 밖으로 세팅
	if (_status != STATUS_ATTACK
		& _status != STATUS_ATTACK_UP
		&& _status != STATUS_JUMP_SKILL
		&& _status != STATUS_JUMP_ATTACK
		&& _status != STATUS_RUN_SKILL)
		setAtkBox(-100, -100, 0, 0);

	//if (_status == STATUS_ATTACK_UP)
	//{
	//	if (!_statusAnim[_direct][_status]->isPlay())
	//	{
	//		_bullet->setIsFire(true);
	//		_bullet->setPos(PointMake(_x, _y));
	//		_bullet->setStartPos(PointMake(_x, _y));
	//		switch (_direct)
	//		{
	//		case LEFT:
	//			_bullet->setAngle(PI);
	//			break;
	//		case RIGHT:
	//			_bullet->setAngle(PI2);
	//			break;
	//		}
	//	}
	//}

	switch (_status)
	{
	case STATUS_BOUNCE:
		if (_direct == RIGHT)
			setHitBox(22, 13, 10, 20);
		else if (_direct == LEFT)
			setHitBox(16, 13, 10, 20);
		break;
	case STATUS_ATTACK:

		if (_direct == RIGHT)
		{
			setHitBox(22, 10, 10, 20);
			setAtkBox(_hitBox.right + 2, _hitBox.top +6, 24, 12);
		}
		else if (_direct == LEFT)
		{
			setHitBox(32, 10, 10, 20);
			setAtkBox(_hitBox.left - 24 - 1, _hitBox.top +6, 24, 12);
		}
		//공격 애니메이션 한 번 재생하고 상태를 IDLE 로 바꾼다
		if (!_statusAnim[_direct][_status]->isPlay())
		{
			_status = STATUS_IDLE;
		}
		break;
	case STATUS_ATTACK_UP:
		/*if (KEYMANAGER->isStayKeyDown('X'))
		{
			_status = STATUS_ATTACK_UP;
			if (_statusAnim[_direct][_status]->getNowPlayIndex() >= 8 && _cnt <= 1)
			{
				_cnt++;
				_statusAnim[_direct][_status]->start();
				_statusAnim[_direct][_status]->setNowPlayIndex(4);
			}
			else
			{
				if (!_statusAnim[_direct][_status]->isPlay())
				{
					_statusAnim[_direct][_status]->setNowPlayIndex(0);
					_status = STATUS_IDLE;
					_cnt = 0;
				}
			}
		}
		else
		{
			_cnt = 0;
			if (!_statusAnim[_direct][_status]->isPlay())
			{
				_statusAnim[_direct][_status]->setNowPlayIndex(0);
				_status = STATUS_IDLE;
			}
		}*/
		if (_statusAnim[_direct][_status]->getNowPlayIndex() == 3)
		{
			_bullet->setIsFire(true);
			_bullet->setPos(PointMake(_x, _y-3));
			_bullet->setStartPos(PointMake(_x, _y-3));
			switch (_direct)
			{
			case LEFT:
				_bullet->setAngle(PI);
				break;
			case RIGHT:
				_bullet->setAngle(PI2);
				break;
			}
		}
		if (!_statusAnim[_direct][_status]->isPlay())
		{
			_statusAnim[_direct][_status]->setNowPlayIndex(0);
			_status = STATUS_IDLE;
		}
		if (_direct == RIGHT)
		{
			setHitBox(18 + 30, 10, 10, 20);
			setAtkBox(_hitBox.right - 1, _hitBox.top +2, 64, 10);
		}
		else if (_direct == LEFT)
		{
			setHitBox(72 - 28, 10, 10, 20);
			setAtkBox(_hitBox.left + 1 - 64, _hitBox.top +2, 64, 10);
		}
		break;
		
	case STATUS_FALL:
		if (_direct == RIGHT)
			setHitBox(10,10, 10, 22);
		else if (_direct == LEFT)
			setHitBox(72 - 61, 10, 10, 22);
		break;
	case STATUS_GAMEOVER:
		if (_direct == RIGHT)
			setHitBox(18, 28, 14, 20);
		else if (_direct == LEFT)
			setHitBox(72 - 57, 28, 14, 20);
		break;
		
	case STATUS_GUARD:
		if (_statusAnim[_direct][_status]->getNowPlayIndex() >4)
		{
			_cnt++;
			_statusAnim[_direct][_status]->start();
			_statusAnim[_direct][_status]->setNowPlayIndex(4);

		}

		if (_direct == RIGHT)
			setHitBox(17, 14, 14, 18);
		else if (_direct == LEFT)
			setHitBox(72 - 54, 14, 14, 18);
		break;
	case STATUS_GUARD_IN_AIR:
		if (_statusAnim[_direct][_status]->getNowPlayIndex() > 4)
		{
			_cnt++;
			_statusAnim[_direct][_status]->start();
			_statusAnim[_direct][_status]->setNowPlayIndex(4);
			
		}

		if (_direct == RIGHT)
			setHitBox(19, 14, 16, 18);
		else if (_direct == LEFT)
			setHitBox(72 - 58, 14, 16, 18);
		break;
		
	case STATUS_HURT:
		if (!_statusAnim[_direct][_status]->isPlay())
			_status = STATUS_IDLE;
		if (_direct == RIGHT)
			setHitBox(19, 14, 12, 18);
		else if (_direct == LEFT)
			setHitBox(72 - 55, 14, 12, 18);
		break;
	case STATUS_HURT2:
		if (_direct == RIGHT)
			setHitBox(17, 16, 15, 15);
		else if (_direct == LEFT)
			setHitBox(72 - 56, 16, 15, 15);
		break;
	case STATUS_IDLE:
		if (_direct == RIGHT)
			setHitBox(10, 12, 12, 20);
		else if (_direct == LEFT)
			setHitBox(72 - 65, 12, 12, 20);
		break;
	case STATUS_JUMP:
		if (_direct == RIGHT)
			setHitBox(16, 13, 18, 18);
		else if (_direct == LEFT)
			setHitBox(72 - 56, 13, 18, 18);
		break;
		
	case STATUS_LAND:
		if (!_statusAnim[_direct][_status]->isPlay())
		{
			_status = STATUS_IDLE;
		}
		if (_direct == RIGHT)
		{
			_x += 0.2f;
			setHitBox(23, 18, 12, 17);
		}
		else if (_direct == LEFT)
		{
			_x -= 0.2f;
			setHitBox(72 - 58, 18, 12, 17);
		}
		break;
	case STATUS_LOSE:
		if (_direct == RIGHT)
			setHitBox(19, 16, 12, 15);
		else if (_direct == LEFT)
			setHitBox(72 - 56, 16, 12, 15);
		break;
		
	case STATUS_RUN:
		if (_direct == RIGHT)
			setHitBox(22, 13, 15, 16);
		else if (_direct == LEFT)
			setHitBox(72 - 62, 13, 15, 16);
		break;
		
	case STATUS_STOP:
		if (!_statusAnim[_direct][_status]->isPlay())
		{
			_cnt++;
			_statusAnim[_direct][_status]->start();
			_statusAnim[_direct][_status]->setNowPlayIndex(0);
		}
		else if (_cnt >= 4) {
			_status = STATUS_IDLE;
			_cnt = 0;
		}
		if (_direct == RIGHT)
		{
			_x += 0.5;
			setHitBox(18, 14, 15, 18);
		}
		else if (_direct == LEFT)
		{
			_x -= 0.5;
			setHitBox(72 - 58, 14, 15, 18);
		}
		break;
		
	case STATUS_JUMP_ATTACK:
		if (_statusAnim[_direct][_status]->getNowPlayIndex() < 6)
		{
			_playAnimSpeed[_status] = 16;
		}
		else {
			_playAnimSpeed[_status] = 19;
		}

		if (_direct == RIGHT) {
			_x += (_speed);
			_y -= _jumpPower;
			_jumpPower -= GRAVITY;
			setHitBox(19, 9, 23, 22);
			setAtkBox(_x - 30 / 2, _hitBox.top - 2, 30, 30);
		}
		else if (_direct == LEFT)
		{
			_x -= (_speed);
			_y -= _jumpPower;
			_jumpPower -= GRAVITY;
			setHitBox(72 - 50, 9, 23, 22);
			setAtkBox(_x - 30 / 2, _hitBox.top - 2, 30, 30);
		}
		break;
	case STATUS_RUN_SKILL:
		//_y -= 2;
		if (!_statusAnim[_direct][_status]->isPlay())
		{
			_cnt++;
			_statusAnim[_direct][_status]->setNowPlayIndex(1);
			_statusAnim[_direct][_status]->start();
			
		}
		if (_cnt >= 2) {
			_status = STATUS_IDLE;
			_cnt = 0;
		}

		if (_direct == RIGHT) {
			setHitBox(18, 14, 18, 22);
			setAtkBox(_x - 30 / 2, _hitBox.top - 2, 30, 30);
		}
		else if (_direct == LEFT)
		{
			setHitBox(72 - 50 + 8, 14, 18, 22);
			setAtkBox(_x - 30 / 2, _hitBox.top - 2, 30, 30);
		}
		break;
	case STATUS_JUMP_SKILL:
		digimonSearchBakcTile((DIGIMONDIRECTION)(1 - _direct));
		if (_direct == RIGHT) {
			//회오리 날리면서 반동이 있다.
			_x -= 0.7;
			setHitBox(19, 9, 24, 24);
		}
		else if (_direct == LEFT)
		{
			_x += 0.7;
			setHitBox(72 - 50, 9, 24, 24);
		}
		break;
	
	case STATUS_WALK:
		if (_direct == RIGHT)
			setHitBox(15, 10, 14, 22);
		else if (_direct == LEFT)
			setHitBox(72 - 64, 10, 14, 22);
		break;
	case STATUS_WIN:
		if (_direct == RIGHT)
			setHitBox(19, 12, 14, 20);
		else if (_direct == LEFT)
			setHitBox(72 - 60, 12, 14, 20);
		break;

	}

	digimonSearchDownTile();
	digimonSearchLeftRightTile();

	_bullet->update();

}

void guilmon::draw(HDC hdc)
{
	//_statusImg[_status]->aniRender(hdc,
	//	_x - _statusImg[_status]->getFrameWidth() / 2,
	//	_y - _statusImg[_status]->getFrameHeight() / 2 + 1,
	//	_statusAnim[_direct][_status]);
	
	if (_status == STATUS_RUN_SKILL)
	{
		if (_direct == RIGHT) {
			_statusImg[_status]->aniRender(hdc,
				_x -30,
				_y - 25,
				_statusAnim[_direct][_status]);
		}
		else if (_direct == LEFT)
		{
			_statusImg[_status]->aniRender(hdc,
				_x -30 ,
				_y - 25,
				_statusAnim[_direct][_status]);
		}
	}
	if (_status == STATUS_LAND)
	{
		_statusImg[_status]->aniRender(hdc,
			_x - _statusImg[_status]->getFrameWidth() / 2,
			_y -13,
			_statusAnim[_direct][_status]);
	}
	//다른 상태는 그냥 출력해도 괜춘할듯
	else {
		_statusImg[_status]->aniRender(hdc,
			_x - _statusImg[_status]->getFrameWidth() / 2,
			_y - _statusImg[_status]->getFrameHeight() / 2 + 1,
			_statusAnim[_direct][_status]);
	}


	if (_isDebug)
	{

		//for (int i = 0; i < TILENUMX*TILENUMY; ++i)
		//{
		//	char str[64];
		//	wsprintf(str, "%d", _tiles[i].getIsGround());
		//
		//}
		//
		////Rectangle(hdc, _tiles[testIdx0].getRect().left, _tiles[testIdx0].getRect().top, _tiles[testIdx0].getRect().right, _tiles[testIdx0].getRect().bottom);
		//for (int i = 0; i < 3; ++i)
		//{
		//	Rectangle(hdc,
		//		_tiles[idx[i]].getRect().left,
		//		_tiles[idx[i]].getRect().top,
		//		_tiles[idx[i]].getRect().right,
		//		_tiles[idx[i]].getRect().bottom);
		//	char str[64];
		//	wsprintf(str, "%d", i);
		//	DrawText(hdc, str, strlen(str), &_tiles[idx[i]].getRect(), DT_SINGLELINE | DT_VCENTER);
		//}
	}
	//Rectangle(hdc, _hitBox.left, _hitBox.top, _hitBox.right, _hitBox.bottom);
	if (_status != STATUS_RUN_SKILL)
		_bullet->render(hdc);

	if (_isDebug)
	{
		Rectangle(hdc, _hitBox.left, _hitBox.top, _hitBox.right, _hitBox.bottom);
		//Rectangle(hdc, _atkBox.left, _atkBox.top, _atkBox.right, _atkBox.bottom);

		TextOut(hdc, _hitBox.left, _hitBox.top + 2, "hitBox", strlen("hitBox"));
		//TextOut(hdc, _atkBox.left, _atkBox.top + 2, "atkBox", strlen("atkBox"));

	}

}