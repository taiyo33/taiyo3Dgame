//=============================================================================
//
// 子供モデル処理 [ball.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _BALL_H_
#define _BALL_H_

#include "main.h"

#define BALL_SET_MAX	(2)
#define BALL_ONESET_MAX	(100)

typedef struct {

	D3DXVECTOR3				pos[BALL_ONESET_MAX];		// 位置
	D3DXVECTOR3				prevPos[BALL_ONESET_MAX];	// 前回位置
	D3DXVECTOR3				rot[BALL_ONESET_MAX];		// 向き(回転)
	D3DXVECTOR3				scl[BALL_ONESET_MAX];		// 大きさ(スケール)
	D3DXVECTOR3				move[BALL_ONESET_MAX];		// 移動量
	bool					use[BALL_ONESET_MAX];		// 使用状態
	D3DXVECTOR3				vec[BALL_ONESET_MAX];
	D3DXVECTOR3				v1[BALL_ONESET_MAX];
	D3DXVECTOR3				v2[BALL_ONESET_MAX];
	D3DXVECTOR3				v3[BALL_ONESET_MAX];
	int						cnt;						// 子供モデルの数
	LPDIRECTSOUNDBUFFER8	damageSE;					// 消滅SE
}BALL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBall(void);
void UninitBall(void);
void UpdateBall(void);
void DrawBall(void);
BALL *GetBall(int index);
void ComprareBall(void);
void SetBall(int index);
void SetInitPosBall(void);

#endif
