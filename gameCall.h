//=============================================================================
//
// ゲームの開始終了処理 [gameCall.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _GAMECALL_H_
#define _GAMECALL_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	GAMECALL_POS_X		(690)			// 表示X位置
#define	GAMECALL_POS_Y		(350)			// 表示Y位置
#define	GAMECALL_SIZE_X		(500)			// 幅
#define	GAMECALL_SIZE_Y		(350)			// 高さ
#define NUM_GAMECALL		(2)				// ポリゴン数

enum GAMECALL{

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
LPDIRECTSOUNDBUFFER8 GetGameBGM03(void);

#endif
