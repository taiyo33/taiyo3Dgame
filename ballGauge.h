//=============================================================================
//
// ボールゲージ処理 [ballGauge.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _BALLGAUGE_H_
#define _BALLGAUGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BALLGAUGE_OUTER_SIZE_X		(700)	 // テクスチャサイズ
#define TEXTURE_BALLGAUGE_OUTER_SIZE_Y		(350)	 // 
#define TEXTURE_BALLGAUGE_INNER_SIZE_X		(680)	 // テクスチャサイズ
#define TEXTURE_BALLGAUGE_INNER_SIZE_Y		(330)	 // 

#define BALLGAUGE_OUTERPOS_X_01				(580)	 // 初期X座標 
#define BALLGAUGE_OUTERPOS_Y_01				(-100)	 // 初期Y座標

#define BALLGAUGE_INNERPOS_X_01				(590)	 // 初期X座標 
#define BALLGAUGE_INNERPOS_Y_01				(-90)	 // 初期Y座標

#define BALLGAUGE_INNERPOS_X_02				(590)	 // 初期X座標 
#define BALLGAUGE_INNERPOS_Y_02				(-90)	 // 初期Y座標


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBallGauge(int type);
void UninitBallGauge(void);
void UpdateBallGauge(void);
void DrawBallGauge(void);


#endif
