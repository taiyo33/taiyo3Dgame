//=============================================================================
//
// 制限時間処理 [time.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTime(int type);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);

int GetTime(void);
void AddTime( int add );
LPDIRECTSOUNDBUFFER8 GetGameBGM02(void);

#endif
