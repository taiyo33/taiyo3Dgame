#ifndef ___SOUND_H___
#define ___SOUND_H___

#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>

// マクロ定義
enum
{	// サウンド通しナンバー
	BGM_00,
	BGM_GAME01,
	BGM_GAME02,
	SE_SPACE,
	SE_SELECT,
	SE_STARTCALL,
	SE_FINISH,
	SE_BULLET,
	SE_REFLECT,
	SE_CHILDDAMAGE,
	SE_HIT,
	SOUND_MAX
};

enum
{	// 再生用フラグ
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};



HRESULT					InitSound( HWND hWnd );	// 初期化
void					UninitSound();			// 後片付け
LPDIRECTSOUNDBUFFER8	LoadSound( int no );	// サウンドのロード
void					PlaySound( LPDIRECTSOUNDBUFFER8 pBuffer, int flag = 0 );	// 音ごとに再生
bool					IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer );					// 再生中かどうか
void					SetVolumeSound(LPDIRECTSOUNDBUFFER8 pBuffer);				// 音量の調整
void					StopSound(LPDIRECTSOUNDBUFFER8 pBuffer);					// 停止

#endif