//=============================================================================
//
// バレットゲージ処理 [bulletGauge.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _BULLETGAUGE_H_
#define _BULLETGAUGE_H_


// マクロ定義
#define TEXTURE_BULLETGAUGE_SIZE_X	 (100)	 // テクスチャサイズ
#define TEXTURE_BULLETGAUGE_SIZE_Y	 (75)	 // 

#define BULLETGAUGE_POS_X_01			 (0)	 // 初期X座標 
#define BULLETGAUGE_POS_Y_01			 (170)	 // 初期Y座標
#define BULLETGAUGE_POS_X_02			 (1170)	 // 初期X座標 
#define BULLETGAUGE_POS_Y_02			 (170)	 // 初期Y座標


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBulletGauge(int type);
void UninitBulletGauge(void);
void UpdateBulletGauge(void);
void DrawBulletGauge(void);


#endif
