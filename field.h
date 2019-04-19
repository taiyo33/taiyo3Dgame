//=============================================================================
//
// 地面処理 [field.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitField(int type);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
void SetCollarField(int fno, float colR, float colB, float colG);

#endif
