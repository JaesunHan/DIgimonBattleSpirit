#include "stdafx.h"
#include "soundManager.h"


soundManager::soundManager()
	: _system(NULL),
	_channel(NULL),
	_sound(NULL)
{
}


soundManager::~soundManager()
{

}

HRESULT soundManager::init()
{
	//FMOD 사운드엔진 사용하려면 초기화
	System_Create(&_system);

	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, NULL);

	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];

	ZeroMemory(_sound, sizeof(_sound));
	ZeroMemory(_channel, sizeof(_channel));


	return S_OK;
}

void soundManager::release()
{
	//if (*_channel != NULL || *_sound != NULL)
	//{
	//	for (int i = 0; i < TOTALSOUNDBUFFER; ++i)
	//	{
	//		if (_channel != NULL)
	//		{
	//			if (_channel[i])_channel[i]->stop();
	//		}
	//		if (_sound != NULL)
	//		{
	//			if (_sound[i] != NULL) _sound[i]->release();
	//		}
	//	}
	//}
	//

	for (int i = 0; i < TOTALSOUNDBUFFER; ++i)
	{
		_channel[i]->stop();
	}

	//arrSoundsIter iter = _mTotalSounds.begin();
	//for (iter; iter != _mTotalSounds.end();)
	//{
	//	_mTotalSounds.erase(iter);
	//}
	//for (int i = 0; i < _mTotalSounds.size(); ++i)
	//{
	//	_sound[i]->release();
	//}
	
	int tempSize = _mTotalSounds.size();
	for (int i = 0; i < tempSize; ++i)
	{
		releaseSound(0);
	}

	//메모리 지워준다
	//배열이므로
	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);
	
	//마지막으로 FMOD 사운드 시스템 닫아줌
	if (_system != NULL)
	{
		_system->release();
		_system->close();		//셧더 내립니다
	}
}

void soundManager::update()	
{
	//볼륨이 바뀌거나, 재생이 끝난 사운드를 채널에서 빼는 등
	//다양한 작업을 자동으로 해준다.
	_system->update();

}


//사운드 추가 함수
void soundManager::addSound(string keyName, string soundName, bool bgm, bool loop)
{
	arrSoundsIter iter = _mTotalSounds.find(keyName);
	if (iter != _mTotalSounds.end()) return;

	if (loop)
	{
		if (bgm)
		{
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, NULL, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, NULL, &_sound[_mTotalSounds.size()]);
		}
	}
	else
	{
		if (bgm)
		{
			_system->createStream(soundName.c_str(), FMOD_DEFAULT, NULL, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_DEFAULT, NULL, &_sound[_mTotalSounds.size()]);
		}
	}

	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}


// 1.0 maximum 0.0 ~ 1.0 -> 0 ~ 255
void soundManager::play(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	//배열 의 몇번째인지 세알리기위한 카운트 변수
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			_system->playSound(FMOD_CHANNEL_REUSE, *iter->second, false, &_channel[count]);

			_channel[count]->setVolume(volume);
			break;
		}
	}
}

void soundManager::play(string keyName, CHANNELTYPE channel, float volume)
{
 	_channel[channel]->stop();

	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter)
	{
		if (keyName == iter->first)
		{
			_system->playSound(FMOD_CHANNEL_REUSE, *iter->second, false, &_channel[channel]);

			_channel[channel]->setVolume(volume);
			break;
		}
	}
}

void soundManager::stop(string keyName)				 
{
	arrSoundsIter iter = _mTotalSounds.begin();
	//배열 의 몇번째인지 세알리기위한 카운트 변수
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void soundManager::stop(CHANNELTYPE channel)
{
	_channel[channel]->stop();
}

void soundManager::pause(string keyName)			 
{
	arrSoundsIter iter = _mTotalSounds.begin();
	//배열 의 몇번째인지 세알리기위한 카운트 변수
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void soundManager::pause(CHANNELTYPE channel)
{
	_channel[channel]->setPaused(true);
}

void soundManager::resume(string keyName)			 
{
	arrSoundsIter iter = _mTotalSounds.begin();
	//배열 의 몇번째인지 세알리기위한 카운트 변수
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

void soundManager::resume(CHANNELTYPE channel)
{
	_channel[channel]->setPaused(false);
}

bool soundManager::isPlaySound(string keyName) 
{
	bool isPlay;

	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool soundManager::isPlaySound(CHANNELTYPE channel)
{
	bool isPlay;
	_channel[channel]->isPlaying(&isPlay);
	return isPlay;
}

bool soundManager::isPauseSound(string keyName)
{
	bool isPause;

	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&isPause);
			break;
		}
	}

	return isPause;
}

void soundManager::releaseSound(int num)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter)
	{
		if (&_sound[num] == iter->second)
		{
			_mTotalSounds.erase(iter);
			break;
		}
	}
	for (int i = num; i < (_mTotalSounds.size()); ++i)
	{
		arrSoundsIter iter = _mTotalSounds.begin();
		for (iter; iter != _mTotalSounds.end(); ++iter)
		{
			if (&_sound[i + 1] == iter->second)
			{
				_mTotalSounds[iter->first] = &_sound[i];
				break;
			}
		}
		_sound[i] = _sound[i + 1];
	}

	_sound[_mTotalSounds.size()]->release();
}

void soundManager::releaseSingleSound(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	int soundNum;
	for (iter; iter != _mTotalSounds.end(); ++iter)
	{
		if (keyName == iter->first)
		{
			for (int i = 0; i < _mTotalSounds.size(); ++i)
			{
				if (&_sound[i] == iter->second)
				{
					soundNum = i;
				}
			}
			_mTotalSounds.erase(iter);
			break;
		}
	}
	for (int i = soundNum; i < (_mTotalSounds.size()); ++i)
	{
		arrSoundsIter iter = _mTotalSounds.begin();
		for (iter; iter != _mTotalSounds.end(); ++iter)
		{
			if (&_sound[i + 1] == iter->second)
			{
				_mTotalSounds[iter->first] = &_sound[i];
				break;
			}
		}
		_sound[i] = _sound[i + 1];
	}

	_sound[_mTotalSounds.size()]->release();
}

void soundManager::releaseAllSound()
{
	if (_channel != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; ++i)
		{
			_channel[i]->stop();
		}
	}
	int tempSize = _mTotalSounds.size();
	for (int i = 0; i < tempSize; ++i)
	{
		//_sound[i]->release();
		releaseSound(0);
	}
	_mTotalSounds.clear();
	SAFE_DELETE_ARRAY(_channel);
	_channel = new Channel*[TOTALSOUNDBUFFER];
	ZeroMemory(_channel, sizeof(_channel));
}

Channel* soundManager::findChannel(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			break;
		}
	}
	return _channel[count];
}