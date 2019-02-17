//=============================================================================
//
// チャージエフェクト処理 [chargeEffect.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _CHARGEEFFECT_H_
#define _CHARGEEFFECT_H_

#include "main.h"

#define CHARGEEFFECT_MAX			(50)
#define CHARGEEFFECT_SET_MAX		(2)
#define CHARGEEFFECT_ONESET_MAX		(4)

typedef struct
{
	D3DXMATRIX		mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3		pos[CHARGEEFFECT_ONESET_MAX];		// 位置
	D3DXVECTOR3		scl[CHARGEEFFECT_ONESET_MAX];
	float			dif_mi[CHARGEEFFECT_ONESET_MAX];	// 透過度
	bool			use[CHARGEEFFECT_ONESET_MAX];							// 使用しているかどうか

} CHARGEEFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitChargeEffect(int type);
void UninitChargeEffect(void);
void UpdateChargeEffect(void);
void DrawChargeEffect(void);
void SetChargeEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest, int index);

#endif
