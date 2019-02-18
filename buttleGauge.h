//=============================================================================
//
// バトルゲージ処理 [buttleGauge.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _BUTTLEGAUGE_H_
#define _BUTTLEGAUGE_H_

// マクロ定義
#define TEXTURE_BUTTLEGAUGE_OUTER_SIZE_X	 (700)	 // テクスチャサイズ
#define TEXTURE_BUTTLEGAUGE_OUTER_SIZE_Y	 (350)	 // 
#define TEXTURE_BUTTLEGAUGE_INNER_SIZE_X	 (680)	 // テクスチャサイズ
#define TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y	 (330)	 // 

#define BUTTLEGAUGE_OUTERPOS_X_01				 (580)	 // 初期X座標 
#define BUTTLEGAUGE_OUTERPOS_Y_01				 (-100)	 // 初期Y座標

#define BUTTLEGAUGE_INNERPOS_X_01				 (590)	 // 初期X座標 
#define BUTTLEGAUGE_INNERPOS_Y_01				 (-90)	 // 初期Y座標

#define BUTTLEGAUGE_INNERPOS_X_02				 (590)	 // 初期X座標 
#define BUTTLEGAUGE_INNERPOS_Y_02				 (-90)	 // 初期Y座標


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitButtleGauge(int type);
void UninitButtleGauge(void);
void UpdateButtleGauge(void);
void DrawButtleGauge(void);


#endif
