//=============================================================================
//
// チュートリアル処理 [Tutorial.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_


// マクロ定義
#define TEXTURE_MAX					 (3)	 // 画像枚数
#define TEXTURE_TUTORIAL_SIZE_X01	 (800)	 // テクスチャサイズ
#define TEXTURE_TUTORIAL_SIZE_Y01	 (600)	 // 

#define TUTORIAL_POS_X01	 (250)	 // 初期X座標 
#define TUTORIAL_POS_Y01	 (100)	 // 初期Y座標

#define TEXTURE_TUTORIAL_SIZE_X02	 (200)	 // テクスチャサイズ
#define TEXTURE_TUTORIAL_SIZE_Y02	 (100)	 // 

#define TUTORIAL_POS_X02	 (500)	 // 初期X座標 
#define TUTORIAL_POS_Y02	 (710)	 // 初期Y座標

#define TEXTURE_TUTORIAL_SIZE_X03	 (75)	 // テクスチャサイズ
#define TEXTURE_TUTORIAL_SIZE_Y03	 (75)	 // 

#define TUTORIAL_POS_X03	 (720)	 // 初期X座標 
#define TUTORIAL_POS_Y03	 (720)	 // 初期Y座標

#define	NUM_TUTORIAL				 (2)	 // ポリゴン数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorial(int type);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

#endif
