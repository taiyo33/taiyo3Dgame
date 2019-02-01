//=============================================================================
//
// モデル処理 [rightReg.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _RIGHTREG_H_
#define _RIGHTREG_H_

#include "main.h"

typedef struct {

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3			pos;					// 現在の位置
	D3DXVECTOR3			rot;					// 現在の向き

	 int				nIdxShadow;				// 影ID
	 float				fSizeShadow;				// 影のサイズ
	 D3DXCOLOR			colShadow;				// 影の色

}RIGHTREG;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRightReg(void);
void UninitRightReg(void);
void UpdateRightReg(void);
void DrawRightReg(void);

#endif
