//=============================================================================
//
// リザルト処理 [result.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_


// マクロ定義
#define TEXTURE_CLEAR 		("data/TEXTURE/gameclear.png")	// サンプル用画像
#define TEXTURE_GAMEOVER 	("data/TEXTURE/gameover.png")	// サンプル用画像


#define TEXTURE_SAMPLE00_SIZE_X_RESULT	(1280) // テクスチャサイズ
#define TEXTURE_SAMPLE00_SIZE_Y_RESULT	(800) // 同上

#define TEXTURE_PATTERN_DIVIDE_X_RESULT	(8)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_RESULT	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define TIME_ANIMATION_RESULT			(100)	// アニメーションの切り替わるカウント
#define	NUM_RESULT						(2)		// ポリゴン数
#define RESULT_MAX						(2)		// リザルトの種類

enum {
	CLEAR,
	GAMEOVER
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(int type);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetTextureResult(int num);


#endif
