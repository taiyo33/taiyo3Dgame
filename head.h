//=============================================================================
//
// モデル処理 [model.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _HEAD_H_
#define _HEAD_H_

#include "main.h"

typedef struct
{
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3			pos;				// 現在の位置
	D3DXVECTOR3			rot;				// 現在の向き
	D3DXVECTOR3			scl;				// 拡大率
	int					nIdxShadow;			// 影ID
	float				fSizeShadow;		// 影のサイズ
	D3DXCOLOR			colShadow;			// 影の色

}HEAD;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitHead(void);
void UninitHead(void);
void UpdateHead(void);
void DrawHead(void);

D3DXVECTOR3 GetPosHead(void);
D3DXVECTOR3 GetRotHead(void);
D3DXVECTOR3 GetRotDestHead(void);

#endif
