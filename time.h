//=============================================================================
//
// �������ԏ��� [score.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_



#define BULLET_HIT			(10)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTime(int type);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);

int GetTime(void);
void AddTime( int add );
LPDIRECTSOUNDBUFFER8 GetGameBGM02(void);

#endif
