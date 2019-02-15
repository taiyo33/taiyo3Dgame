#ifndef ___SOUND_H___
#define ___SOUND_H___

#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>

// �}�N����`
enum
{	// �T�E���h�ʂ��i���o�[
	BGM_00,
	BGM_GAME01,
	BGM_GAME02,
	SE_SPACE,
	SE_SELECT,
	SE_STARTCALL,
	SE_FINISH,
	SE_BULLET,
	SOUND_MAX
};

enum
{	// �Đ��p�t���O
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};



HRESULT					InitSound( HWND hWnd );	// ������
void					UninitSound();			// ��Еt��
LPDIRECTSOUNDBUFFER8	LoadSound( int no );	// �T�E���h�̃��[�h
void					PlaySound( LPDIRECTSOUNDBUFFER8 pBuffer, int flag = 0 );	// �����ƂɍĐ�
bool					IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer );					// �Đ������ǂ���
void					SetVolumeSound(LPDIRECTSOUNDBUFFER8 pBuffer);				// ���ʂ̒���
void					StopSound(LPDIRECTSOUNDBUFFER8 pBuffer);					// ��~

#endif