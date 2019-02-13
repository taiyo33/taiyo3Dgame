//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _ICON_H_
#define _ICON_H_

#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================

#define ICON_MAX				(2)
#define ICON_LIFE_MAX			(100.0f)
#define ICON_FALL_SPEED		(5.0f)

#define ICON01_INITPOS_X		(-300.0f)
#define ICON01_INITPOS_Y		(10.0f)
#define ICON01_INITPOS_Z		(200.0f)

#define ICON02_INITPOS_X		(300.0f)
#define ICON02_INITPOS_Y		(10.0f)
#define ICON02_INITPOS_Z		(-200.0f)

typedef struct {
	
	D3DXMATRIX					mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3					pos;				// 現在の位置
	bool						use;				// 使用状態
}ICON;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitIcon(int type);
void UninitIcon(void);
void UpdateIcon(void);
void DrawIcon(void);


#endif
