//=============================================================================
//
// スモークエフェクト処理 [smoke.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _SMOKE_H_
#define _SMOKE_H_

#include "main.h"

typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 scl;		// スケール
	bool		use;				// 使用しているかどうか
	float		angle;
	float		Alpha;
	int			time;

} SMOKE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSmoke(int type);
void UninitSmoke(void);
void UpdateSmoke(void);
void DrawSmoke(void);
bool SetSmoke(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest);
#endif
