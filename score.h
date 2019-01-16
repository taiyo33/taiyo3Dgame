//=============================================================================
//
// スコア処理 [score.h]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

// マクロ定義
#define TEXTURE_GAME_SCORE		("data/TEXTURE/number16x32.png")	// サンプル用画像
#define TEXTURE_SCORE00_SIZE_X	(30)	// テクスチャサイズ
#define TEXTURE_SCORE00_SIZE_Y	(50)	// 同上
#define SCORE_POS_X				(600)		// ポリゴンの初期位置X
#define SCORE_POS_Y				(0)		// 同上
#define SCORE_MAX				(999999)		// スコアの最大値
#define SCORE_DIGIT				(6)			// 桁数
#define NUM_SCORE				(2)


#define BULLET_HIT			(10)
#define ENEMY001_POINT		(20)
#define ENEMY002_POINT		(30)
#define BOSS_POINT			(100)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(int type);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void AddScore( int add );

#endif
