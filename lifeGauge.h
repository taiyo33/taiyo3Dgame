//=============================================================================
//
// チュートリアル処理 [LifeGauge.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _LIFEGAUGE_H_
#define _LIFEGAUGE_H_

// マクロ定義
#define TEXTURE_ICON01	("data/TEXTURE/player01.png")	// 歩行用画像
#define TEXTURE_ICON02	("data/TEXTURE/player02.png")	// 歩行用画像
#define TEXTURE_ICON03	("data/TEXTURE/cpu_icon.png")	// 歩行用画像

#define TEXTURE_LIFEGAUGE_OUTER_SIZE_X	 (700)	 // テクスチャサイズ
#define TEXTURE_LIFEGAUGE_OUTER_SIZE_Y	 (150)	 // 
#define TEXTURE_LIFEGAUGE_INNER_SIZE_X	 (680)	 // テクスチャサイズ
#define TEXTURE_LIFEGAUGE_INNER_SIZE_Y	 (130)	 // 
#define TEXTURE_LIFEICON_SIZE_X			 (150)	 // テクスチャサイズ
#define TEXTURE_LIFEICON_SIZE_Y			 (150)	 // 

#define LIFEGAUGE_OUTERPOS_X_01				 (150)	 // 初期X座標 
#define LIFEGAUGE_OUTERPOS_Y_01				 (90)	 // 初期Y座標
#define LIFEGAUGE_INNERPOS_X_01				 (160)	 // 初期X座標 
#define LIFEGAUGE_INNERPOS_Y_01				 (100)	 // 初期Y座標

#define LIFEGAUGE_OUTERPOS_X_02				 (1080)	 // 初期X座標 
#define LIFEGAUGE_OUTERPOS_Y_02				 (90)	 // 初期Y座標
#define LIFEGAUGE_INNERPOS_X_02				 (1090)	 // 初期X座標 
#define LIFEGAUGE_INNERPOS_Y_02				 (100)	 // 初期Y座標

#define LIFEICON_POS_X_01				 (40)	 // P1アイコン初期X座標 
#define LIFEICON_POS_Y_01				 (90)	 // P1アイコン初期Y座標
#define LIFEICON_POS_X_02				 (1740)	 // P2アイコン初期X座標 
#define LIFEICON_POS_Y_02				 (90)	 // P2アイコン初期Y座標

// テクスチャー番号と頂点情報のアドレス番号
enum {
	LIFEGAUGE001,
	LIFEGAUGE002,
	LIFEGAUGE003,
	LIFEGAUGE004,
	PLAYERICON01,
	PLAYERICON02,
	NPCICON
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLifeGauge(int type);
void UninitLifeGauge(void);
void UpdateLifeGauge(void);
void DrawLifeGauge(void);

void SetIconTextureType(int index);

#endif
