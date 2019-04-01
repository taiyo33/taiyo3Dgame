//=============================================================================
//
// チャージエフェクト処理 [chargeEffect.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _CHARGEEFFECT_H_
#define _CHARGEEFFECT_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CHARGEEFFECT_SET_MAX		(2)		 // 組の最大数
#define CHARGEEFFECT_ONESET_MAX		(4)		 // 一組の最大数
#define MAX_CHARGEEFFECT			(CHARGEEFFECT_SET_MAX * CHARGEEFFECT_ONESET_MAX) // 総数

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	D3DXMATRIX		mtxWorld;							// ワールドマトリックス
	D3DXVECTOR3		pos[CHARGEEFFECT_ONESET_MAX];		// 位置
	D3DXVECTOR3		scl[CHARGEEFFECT_ONESET_MAX];		// スケール
	float			dif[CHARGEEFFECT_ONESET_MAX];		// 透過度
	bool			use[CHARGEEFFECT_ONESET_MAX];		// 使用状態

} CHARGEEFFECT;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitChargeEffect(int type);
void UninitChargeEffect(void);
void UpdateChargeEffect(void);
void DrawChargeEffect(void);
void SetChargeEffect(D3DXVECTOR3 pos, int index);

#endif
