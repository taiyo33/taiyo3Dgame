//=============================================================================
//
// 地面処理 [field.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

#define FIELD_SIZE_X	(400.0f)
#define FIELD_SIZE_Y	(0.0f)
#define FIELD_SIZE_Z	(300.0f)					


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitField(int type);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

#endif
