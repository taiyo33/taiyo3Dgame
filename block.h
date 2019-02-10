//=============================================================================
//
// ブロック処理 [block.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define	BLOCK_MAX				(106)	// ブロックの最大数
#define VTX_MAX					(104)	// ポリゴン枚数
#define BLOCK_NUM_CORNER		(4)		// ４つ角
#define BLOCK_NUM_LEFTSIDE		(24)	// ポリゴンのナンバリング
#define BLOCK_NUM_NEARSIDE		(53)	// 
#define BLOCK_NUM_RIGHTSIDE		(77)	// 
#define BLOCK_NUM_FEARSIDE		(106)	// 

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
bool HitCheckCornerBlock(D3DXVECTOR3 pos0);

#endif
