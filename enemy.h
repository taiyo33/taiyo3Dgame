//=============================================================================
//
// 敵モデル処理 [enemy.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"


#define ENEMY_MAX			(1)

typedef struct {

D3DXVECTOR3			pos;				// モデルの位置
D3DXVECTOR3			rot;				// モデルの向き(回転)
D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
bool				use;
int					life;

}ENEMY;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int index);

#endif
