//=============================================================================
//
// バレット処理 [bullet.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"


#define BULLET_MAX				(60)

#define BULLET_SET_MAX			(30)

typedef struct
{
	D3DXMATRIX		mtxWorld;	 // ワールドマトリックス
	LPD3DXMESH		D3DXMesh;	 // ID3DXMeshインターフェイスへのポインタ
	LPD3DXBUFFER	D3DXBuffMat; // メッシュのマテリアル情報を格納
	DWORD			NumMat;		 // 属性情報の総数
	D3DXVECTOR3		pos[BULLET_MAX];		 // 位置
	D3DXVECTOR3		prevPos[BULLET_MAX];	 // 前回の位置
	D3DXVECTOR3		rot[BULLET_MAX];		 // 回転量
	D3DXVECTOR3		scl[BULLET_MAX];		 // スケール
	float			mass[BULLET_MAX];		 // 質量
	bool			use[BULLET_MAX];		 // 使用しているかどうか
	int				time[BULLET_MAX];		 // 寿命
	
} BULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(int type);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
BULLET *GetBullet(int bno);
bool SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest, int index);

#endif
