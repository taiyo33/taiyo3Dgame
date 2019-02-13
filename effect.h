//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"


#define DELETEFFECT_ONESET_MAX		(10)

typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos[DELETEFFECT_ONESET_MAX];		// 位置
	float			dif_mi[DELETEFFECT_ONESET_MAX];
	bool			use[DELETEFFECT_ONESET_MAX];				// 使用しているかどうか
	int				time[DELETEFFECT_ONESET_MAX];

} EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(int type);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
bool SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest, int index);

#endif
