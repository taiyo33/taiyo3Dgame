//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _HITEFFECT_H_
#define _HITEFFECT_H_

#include "main.h"

#define HITEFFECT_MAX				(255)
#define HITEFFECT_SET_MAX			(20)
#define HITEFFECT_ONESET_MAX		(24)

typedef struct
{
	D3DXMATRIX		mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3		pos[HITEFFECT_ONESET_MAX];		// 位置
	D3DXVECTOR3		move[HITEFFECT_ONESET_MAX];		// 移動量
	float			dif_mi[HITEFFECT_ONESET_MAX];	// 透過度
	bool			use;							// 使用しているかどうか
	float			angle[HITEFFECT_ONESET_MAX];	// 移動角度
	float			speed[HITEFFECT_ONESET_MAX];	// 移動速度

} HITEFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitHitEffect(int type);
void UninitHitEffect(void);
void UpdateHitEffect(void);
void DrawHitEffect(void);
void SetHitEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest);

#endif
