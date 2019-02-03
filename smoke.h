//=============================================================================
//
// スモークエフェクト処理 [smoke.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _SMOKE_H_
#define _SMOKE_H_

#include "main.h"

#define SMOKE_ONESET_MAX			(30)
#define SMOKE_SET_MAX				(2)

typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos[SMOKE_ONESET_MAX];		// 位置
	D3DXVECTOR3 scl[SMOKE_ONESET_MAX];		// スケール
	bool		use[SMOKE_ONESET_MAX];				// 使用しているかどうか
	int			time[SMOKE_ONESET_MAX];
	float		Alpha[SMOKE_ONESET_MAX];
	float		angle;

} SMOKE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSmoke(int type);
void UninitSmoke(void);
void UpdateSmoke(void);
void DrawSmoke(void);
bool SetSmoke(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest, float sizeX, float sizeY, int index);
#endif
