//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define	BLOCK_MAX		(84)	// ブロックの最大数


typedef struct {
	
	D3DXVECTOR3					pos;		// 現在の位置
	int							life;		// ブロックの寿命
	int							textureNum; // テクスチャーの番号
	bool						use;		// 使用状態

}BLOCK;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBlock(int type);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

BLOCK *GetBlock(int bno);
void BlockDamageManager(int bno);
bool HitCheckBlock(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

#endif
