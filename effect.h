//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 scl;		// スケール
	D3DXVECTOR3 move;		// 移動量
	float		fWidth;			// 幅
	float		fHeight;			// 高さ
	int			IndxShadow;			// 影ID
	bool		bUse;				// 使用しているかどうか
	float		angle;
	float		Alpha;
	int			time;

} EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(int type);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
bool SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest);
#endif
