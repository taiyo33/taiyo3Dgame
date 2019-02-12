//=============================================================================
//
// 子供モデル処理 [child.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _CHILD_H_
#define _CHILD_H_

#include "main.h"

#define CHILD_SET_MAX		(2)
#define CHILD_ONESET_MAX	(100)

typedef struct {

	D3DXVECTOR3			pos[CHILD_ONESET_MAX];		// 位置
	D3DXVECTOR3			prevPos[CHILD_ONESET_MAX];	// 前回位置
	D3DXVECTOR3			rot[CHILD_ONESET_MAX];		// 向き(回転)
	D3DXVECTOR3			scl[CHILD_ONESET_MAX];		// 大きさ(スケール)
	D3DXVECTOR3			move[CHILD_ONESET_MAX];		// 移動量
	bool				use[CHILD_ONESET_MAX];		// 使用状態
	D3DXVECTOR3			vec[CHILD_ONESET_MAX];
	D3DXVECTOR3			v1[CHILD_ONESET_MAX];
	D3DXVECTOR3			v2[CHILD_ONESET_MAX];
	D3DXVECTOR3			v3[CHILD_ONESET_MAX];
	int					cnt;						// 子供モデルの数

}CHILD;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitChild(void);
void UninitChild(void);
void UpdateChild(void);
void DrawChild(void);
CHILD *GetChild(int index);
void SetChild(D3DXVECTOR3 pos, int index);

#endif
