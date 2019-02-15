//=============================================================================
//
// ゲームの開始修了処理 [gameCall.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _GAMECALL_H_
#define _GAMECALL_H_



//*****************************************************************************
// マクロ定義
//*****************************************************************************


#define	GAMECALL_POS_X		(400)			// タイトルの表示位置
#define	GAMECALL_POS_Y		(250)			// タイトルの表示位置
#define	GAMECALL_SIZE_X		(500)			// タイトルの幅
#define	GAMECALL_SIZE_Y		(300)			// タイトルの高さ

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
LPDIRECTSOUNDBUFFER8 GetGameBGM01(void);

#endif
