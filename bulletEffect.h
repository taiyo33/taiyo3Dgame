//=============================================================================
//
// バレットエフェクト処理 [smoke.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _BULLETEFFECT_H_
#define _BULLETEFFECT_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define BULLETEFFECT_SET_MAX			(6)		// 組の最大数
#define BULLETEFFECT_ONESET_MAX			(30)	// 一組の最大数

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct
{
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 pos[BULLETEFFECT_ONESET_MAX];	// 位置
	D3DXVECTOR3 scl[BULLETEFFECT_ONESET_MAX];	// スケール
	bool		use[BULLETEFFECT_ONESET_MAX];	// 使用状態
	int			time[BULLETEFFECT_ONESET_MAX];	// エフェクト寿命
	float		dif[BULLETEFFECT_ONESET_MAX];	// 透過値

} BULLETEFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBulletEffect(int type);
void UninitBulletEffect(void);
void UpdateBulletEffect(void);
void DrawBulletEffect(void);
void SetBulletEffect(D3DXVECTOR3 pos, int index);

#endif
