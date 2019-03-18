//=============================================================================
//
// リザルト処理 [result.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WINNER_SIZE_X	(300)	// テクスチャサイズ
#define TEXTURE_WINNER_SIZE_Y	(150)	// 同上

#define TEXTURE_ICON_SIZE_X		(200)	// テクスチャサイズ
#define TEXTURE_ICON_SIZE_Y		(200)	// 同上

#define TEXTURE_WINNER_POS_X	(690)	// テクスチャの表示位置
#define TEXTURE_WINNER_POS_Y	(450)	// 同上
#define TEXTURE_ICON_POS_X		(970)	// 同上 
#define TEXTURE_ICON_POS_Y		(425)	// 同上 

#define	NUM_RESULT						(2)		// ポリゴン数
#define RESULT_MAX						(2)		// リザルトの描画数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(int type);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResult(int pno0, int pno1);


#endif
