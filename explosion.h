//=============================================================================
//
// 爆発エフェクト処理 [explosion.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 scl;		// スケール
	VERTEX_3D	*Vtx;
	D3DXVECTOR3 rot;		// 移動量
	bool		use;				// 使用しているかどうか
	int			time;
	int			patternAnim;
	int			cntAnim;

} EXPLOSION;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitExplosion(int type);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest);

#endif
