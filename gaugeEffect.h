//=============================================================================
//
// 爆発エフェクト処理 [explosion.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _GAUGEEFFECT_H_
#define _GAUGEEFFECT_H_

#include "main.h"

#define TEXTURE_GAUGEEFFECT_SIZE_X	 (200)	 // テクスチャサイズ
#define TEXTURE_GAUGEEFFECT_SIZE_Y	 (150)	 // 

#define GAUGEEFFECT_POS_X_01				 (428)	 // 初期X座標 
#define GAUGEEFFECT_POS_Y_01				 (0)	 // 初期Y座標

#define GAUGEEFFECT_POS_X_02				 (754)	 // 初期X座標 
#define GAUGEEFFECT_POS_Y_02				 (0)	 // 初期Y座標


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGaugeEffect(int type);
void UninitGaugeEffect(void);
void UpdateGaugeEffect(void);
void DrawGaugeEffect(void);

#endif

