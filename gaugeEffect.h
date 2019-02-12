//=============================================================================
//
// 爆発エフェクト処理 [explosion.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _GAUGEEFFECT_H_
#define _GAUGEEFFECT_H_

#include "main.h"

#define TEXTURE_GAUGEEFFECT_SIZE_X	 (100)	 // テクスチャサイズ
#define TEXTURE_GAUGEEFFECT_SIZE_Y	 (120)	 // 

#define GAUGEEFFECT_POS_X_01				 (550)	 // 初期X座標 
#define GAUGEEFFECT_POS_Y_01				 (-10)	 // 初期Y座標

#define GAUGEEFFECT_POS_X_02				 (610)	 // 初期X座標 
#define GAUGEEFFECT_POS_Y_02				 (-10)	 // 初期Y座標


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGaugeEffect(int type);
void UninitGaugeEffect(void);
void UpdateGaugeEffect(void);
void DrawGaugeEffect(void);

#endif

