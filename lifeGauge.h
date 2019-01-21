//=============================================================================
//
// チュートリアル処理 [LifeGauge.h]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#ifndef _LIFEGAUGE_H_
#define _LIFEGAUGE_H_

// マクロ定義
#define TEXTURE_LIFEGAUGE_SIZE_X	 (500)	 // テクスチャサイズ
#define TEXTURE_LIFEGAUGE_SIZE_Y	 (100)	 // 
#define TEXTURE_LIFEICON_SIZE_X		 (100)	 // テクスチャサイズ
#define TEXTURE_LIFEICON_SIZE_Y		 (100)	 // 
#define LIFEGAUGE_POS_X_01			 (100)	 // 初期X座標 
#define LIFEGAUGE_POS_Y_01			 (0)	 // 初期Y座標
#define LIFEGAUGE_POS_X_02			 (650)	 // 初期X座標 
#define LIFEGAUGE_POS_Y_02			 (0)	 // 初期Y座標
#define LIFEICON_POS_X_01			 (0)	 // P1アイコン初期X座標 
#define LIFEICON_POS_Y_01			 (0)	 // P1アイコン初期Y座標
#define LIFEICON_POS_X_02			 (1170)	 // P2アイコン初期X座標 
#define LIFEICON_POS_Y_02			 (0)	 // P2アイコン初期Y座標


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLifeGauge(int type);
void UninitLifeGauge(void);
void UpdateLifeGauge(void);
void DrawLifeGauge(void);
void SetTextureLifeGauge(int index, float x);


#endif
