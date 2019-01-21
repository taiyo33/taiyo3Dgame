//=============================================================================
//
// チュートリアル処理 [Tutorial.h]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_


// マクロ定義
#define TEXTURE_TUTORIAL1	("data/TEXTURE/tutorial1.png")		// 歩行用画像
#define TEXTURE_TUTORIAL2	("data/TEXTURE/tutorial2.png")		// 歩行用画像
#define TEXTURE_TUTORIAL3	("data/TEXTURE/tutorial3.png")		// 歩行用画像



#define TEXTURE_MAX				 (10)	 // 画像枚数
#define TEXTURE_TUTORIAL_SIZE_X	 (800)	 // テクスチャサイズ
#define TEXTURE_TUTORIAL_SIZE_Y	 (600)	 // 

#define TUTORIAL_POS_X	 (0)	 // 初期X座標 
#define TUTORIAL_POS_Y	 (0)	 // 初期Y座標

#define	NUM_TUTORIAL				 (2)	 // ポリゴン数
#define TUTORIAL_MAX			 	 (3)	 // プレイヤー人数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorial(int type);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);


#endif
