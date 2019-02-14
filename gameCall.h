//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _GAMECALL_H_
#define _GAMECALL_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************


#define	GAMECALL_POS_X		(260)			// タイトルの表示位置
#define	GAMECALL_POS_Y		(230)			// タイトルの表示位置
#define	GAMECALL_SIZE_X		(300)			// タイトルの幅
#define	GAMECALL_SIZE_Y		(100)			// タイトルの高さ

#define NUM_GAMECALL		(2)

enum {

	READY,
	GO,
	FINISH,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGameCall(int type);
void UninitGameCall(void);
void UpdateGameCall(void);
void DrawGameCall(void);
void SetGameCallTextureNum(int num);

#endif
