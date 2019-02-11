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
#define	BLOCK_MAX						(146)	// ブロックの最大数
#define BLOCK_VTX_MAX					(122)	// ポリゴン枚数
#define BLOCK_NUM_WALL					(106)	// 外枠ブロックの数


#define BLOCK_NUM_CORNER				(4)		// ４つ角
#define BLOCK_NUM_LEFTSIDE				(24)	// ポリゴンのナンバリング
#define BLOCK_NUM_NEARSIDE				(53)	// 
#define BLOCK_NUM_RIGHTSIDE				(77)	// 
#define BLOCK_NUM_FEARSIDE				(106)	// 
#define BLOCK_NUM_LEFTSIDE_OBJ			(116)	// 障害物ブロックのナンバリング
#define BLOCK_NUM_NEARSIDE_OBJ			(126)	// 
#define BLOCK_NUM_RIGHTSIDE_OBJ			(136)	// 
#define BLOCK_NUM_FEARSIDE_OBJ			(146)	// 
#define BLOCK_LEFT_OUTSIDE_OBJ_VTX		(108)	// ポリゴンのナンバリング
#define BLOCK_NEAR_OUTSIDE_OBJ_VTX		(110)	// ポリゴンのナンバリング
#define BLOCK_RIGHT_OUTSIDE_OBJ_VTX		(112)	// ポリゴンのナンバリング
#define BLOCK_FEAR_OUTSIDE_OBJ_VTX		(114)	// ポリゴンのナンバリング
#define BLOCK_LEFT_INSIDE_OBJ_VTX		(116)	// ポリゴンのナンバリング
#define BLOCK_NEAR_INSIDE_OBJ_VTX		(118)	// ポリゴンのナンバリング
#define BLOCK_RIGHT_INSIDE_OBJ_VTX		(120)	// ポリゴンのナンバリング
#define BLOCK_FEAR_INSIDE_OBJ_VTX		(122)	// ポリゴンのナンバリング



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
bool HitCheckBlock(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, int max);
bool HitCheckCornerBlock(D3DXVECTOR3 pos0);

#endif
