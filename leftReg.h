//=============================================================================
//
// 左足モデル処理 [leftReg.h]
// Author : GP11A_341_22_田中太陽 GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _LEFTREG_H_
#define _LEFTREG_H_

#include "main.h"

typedef struct {

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3			pos;					// 現在の位置
	D3DXVECTOR3			rot;					// 現在の向き

	static int					nIdxShadow;				// 影ID
	static float				fSizeShadow;				// 影のサイズ
	static D3DXCOLOR			colShadow;				// 影の色

}LEFTREG;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLeftReg(void);
void UninitLeftReg(void);
void UpdateLeftReg(void);
void DrawLeftReg(void);


#endif
