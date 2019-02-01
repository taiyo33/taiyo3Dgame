//=============================================================================
//
// チュートリアル処理 [LifeGauge.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _LIFEGAUGE_H_
#define _LIFEGAUGE_H_

// マクロ定義
#define TEXTURE_LIFEGAUGE_OUTER_SIZE_X	 (500)	 // テクスチャサイズ
#define TEXTURE_LIFEGAUGE_OUTER_SIZE_Y	 (100)	 // 
#define TEXTURE_LIFEGAUGE_INNER_SIZE_X	 (480)	 // テクスチャサイズ
#define TEXTURE_LIFEGAUGE_INNER_SIZE_Y	 (80)	 // 
#define TEXTURE_LIFEICON_SIZE_X			 (100)	 // テクスチャサイズ
#define TEXTURE_LIFEICON_SIZE_Y			 (100)	 // 

#define LIFEGAUGE_OUTERPOS_X_01				 (100)	 // 初期X座標 
#define LIFEGAUGE_OUTERPOS_Y_01				 (0)	 // 初期Y座標
#define LIFEGAUGE_INNERPOS_X_01				 (110)	 // 初期X座標 
#define LIFEGAUGE_INNERPOS_Y_01				 (10)	 // 初期Y座標

#define LIFEGAUGE_OUTERPOS_X_02				 (650)	 // 初期X座標 
#define LIFEGAUGE_OUTERPOS_Y_02				 (0)	 // 初期Y座標
#define LIFEGAUGE_INNERPOS_X_02				 (660)	 // 初期X座標 
#define LIFEGAUGE_INNERPOS_Y_02				 (10)	 // 初期Y座標

#define LIFEICON_POS_X_01				 (0)	 // P1アイコン初期X座標 
#define LIFEICON_POS_Y_01				 (0)	 // P1アイコン初期Y座標
#define LIFEICON_POS_X_02				 (1170)	 // P2アイコン初期X座標 
#define LIFEICON_POS_Y_02				 (0)	 // P2アイコン初期Y座標


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLifeGauge(int type);
void UninitLifeGauge(void);
void UpdateLifeGauge(void);
void DrawLifeGauge(void);


#endif
