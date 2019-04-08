//=============================================================================
//
// ブロック処理 [block.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//=============================================================================
//マクロ定義
//=============================================================================
#define	BLOCK_MAX						(146)	// ブロックの最大数
#define BLOCK_VTX_MAX					(20)	// ポリゴン枚数
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

// 障害物ブロックのポリゴン
#define BLOCK_LEFT_OUTSIDE_OBJ_VTX		(2)		// ポリゴンのナンバリング
#define BLOCK_NEAR_OUTSIDE_OBJ_VTX		(4)		// ポリゴンのナンバリング
#define BLOCK_RIGHT_OUTSIDE_OBJ_VTX		(6)		// ポリゴンのナンバリング
#define BLOCK_FEAR_OUTSIDE_OBJ_VTX		(8)		// ポリゴンのナンバリング
#define BLOCK_LEFT_INSIDE_OBJ_VTX		(10)	// ポリゴンのナンバリング
#define BLOCK_NEAR_INSIDE_OBJ_VTX		(12)	// ポリゴンのナンバリング
#define BLOCK_RIGHT_INSIDE_OBJ_VTX		(14)	// ポリゴンのナンバリング
#define BLOCK_FEAR_INSIDE_OBJ_VTX		(16)	// ポリゴンのナンバリング

//=============================================================================
// 構造体宣言
//=============================================================================
typedef struct {
	
	D3DXVECTOR3					pos;		// 現在の位置
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
