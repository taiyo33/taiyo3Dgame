//=============================================================================
//
// バレットエフェクト処理 [smoke.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _BULLETEFFECT_H_
#define _BULLETEFFECT_H_

#include "main.h"

#define BULLETEFFECT_ONESET_MAX			(30)
#define BULLETEFFECT_SET_MAX				(6)

typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos[BULLETEFFECT_ONESET_MAX];		// 位置
	D3DXVECTOR3 scl[BULLETEFFECT_ONESET_MAX];		// スケール
	bool		use[BULLETEFFECT_ONESET_MAX];		// 使用しているかどうか
	int			time[BULLETEFFECT_ONESET_MAX];		// エフェクト寿命
	float		Alpha[BULLETEFFECT_ONESET_MAX];
	float		angle;

} BULLETEFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBulletEffect(int type);
void UninitBulletEffect(void);
void UpdateBulletEffect(void);
void DrawBulletEffect(void);
bool SetBulletEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, float Dest, float sizeX, float sizeY, int index);
#endif
