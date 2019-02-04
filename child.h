//=============================================================================
//
// 子供モデル処理 [child.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _CHILD_H_
#define _CHILD_H_

#include "main.h"


#define CHILD_MAX			(30)
#define CHILD_SET_MAX		(2)
#define CHILD_ONESET_MAX	(30)

typedef struct {

D3DXVECTOR3			pos[CHILD_ONESET_MAX];				// モデルの位置
D3DXVECTOR3			rot[CHILD_ONESET_MAX];				// モデルの向き(回転)
D3DXVECTOR3			scl[CHILD_ONESET_MAX];				// モデルの大きさ(スケール)
bool				use[CHILD_ONESET_MAX];
int					life[CHILD_ONESET_MAX];

}CHILD;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitChild(void);
void UninitChild(void);
void UpdateChild(void);
void DrawChild(void);
CHILD *GetChild(int index);

#endif
