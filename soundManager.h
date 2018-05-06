#pragma once
#include "singletonBase.h"
#include <map>

#include "inc/fmod.hpp"	//fmod.hpp파일 인클루드
#pragma comment(lib, "lib/fmodex_vc.lib")	//라이브러리 파일 등록

using namespace FMOD;

#define SOUNDBUFFER 50
#define EXTRACHANNELBUFFER 5

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

enum CHANNELTYPE
{
	CH_BGM,
	CH_MENU_EFFECT,
	CH_EFFECT01,
	CH_EFFECT02,
	CH_EFFECT03,
	CH_EFFECT04,
	CH_EFFECT05,
	CH_EFFECT06,
	CH_EFFECT07,
	CH_EFFECT08,
	CH_MENUSCENE,       //MENU_SCENE  ====== WARINING ====== by won
	CH_SOKKONGGU,		//아무도 건들지 마시오! 내 채널임!!!
	CH_ENCOUNTER,

	MAX_CHANNEL
};

class soundManager : public singletonBase<soundManager>
{
private:
	typedef map<string, Sound**>				arrSounds;
	typedef map<string, Sound**>::iterator		arrSoundsIter;
	typedef map<string, Channel**>				arrChannels;
	typedef map<string, Channel**>::iterator	arrChannelsIter;

private:
	System* _system;
	Sound** _sound;
	Channel** _channel;

	arrSounds _mTotalSounds;
public:
	soundManager();
	~soundManager();

	HRESULT init();
	void release();
	void update();

	//사운드 추가 함수
	void addSound(string keyName, string soundName, bool bgm, bool loop);

	// 1.0 maximum 0.0 ~ 1.0 -> 0 ~ 255
	void play(string keyName, float volume = 1.0f);
	void play(string keyName, CHANNELTYPE channel, float volume = 1.0f);
	void stop(string keyName);
	void stop(CHANNELTYPE channel);
	void pause(string keyName);
	void pause(CHANNELTYPE channel);
	void resume(string keyName);
	void resume(CHANNELTYPE channel);

	void releaseSound(int num);
	void releaseSingleSound(string keyName);
	void releaseAllSound();
	Channel* findChannel(string keyName);

	bool isPlaySound(string keyName);
	bool isPlaySound(CHANNELTYPE channel);
	bool isPauseSound(string keyName);

	inline Channel* getChannel(CHANNELTYPE channel) { return _channel[channel]; }
};

