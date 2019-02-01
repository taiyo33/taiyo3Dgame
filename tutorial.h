//=============================================================================
//
// チュートリアル処理 [Tutorial.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_


// マクロ定義



#define TEXTURE_MAX				 (2)	 // 画像枚数
#define TEXTURE_TUTORIAL_SIZE_X01	 (800)	 // テクスチャサイズ
#define TEXTURE_TUTORIAL_SIZE_Y01	 (600)	 // 

#define TUTORIAL_POS_X01	 (250)	 // 初期X座標 
#define TUTORIAL_POS_Y01	 (100)	 // 初期Y座標

#define TEXTURE_TUTORIAL_SIZE_X02	 (200)	 // テクスチャサイズ
#define TEXTURE_TUTORIAL_SIZE_Y02	 (150)	 // 

#define TUTORIAL_POS_X02	 (570)	 // 初期X座標 
#define TUTORIAL_POS_Y02	 (640)	 // 初期Y座標

#define	NUM_TUTORIAL				 (2)	 // ポリゴン数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorial(int type);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);


#endif
