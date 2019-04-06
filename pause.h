//=============================================================================
//
// ポーズ画面処理 [pause.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PAUSE_POS_X					(370)	// ポーズの表示位置
#define	PAUSE_POS_Y					(300)	// ポーズの表示位置
#define	PAUSE_SIZE_X				(1120)	// ポーズの幅
#define	PAUSE_SIZE_Y				(520)	// ポーズの高さ

#define	PAUSE_LOGO_POS_X			(840)	// ポーズロゴの表示位置
#define	PAUSE_LOGO_POS_Y			(400)	// ポーズロゴの表示位置
#define	PAUSE_LOGO_SIZE_X			(200)	// ポーズロゴの幅
#define	PAUSE_LOGO_SIZE_Y			(150)	// ポーズロゴの高さ
#define SET_LOGO					(75)

#define PUSHKEY_MAX					(2)		// 遷移キーの最大表示数

#define	PAUSE_SELECT_POS_X			(770)	// ポーズロゴの表示位置
#define	PAUSE_SELECT_POS_Y			(445)	// ポーズロゴの表示位置
#define	PAUSE_SELECT_SIZE_X			(60)	// ポーズロゴの幅
#define	PAUSE_SELECT_SIZE_Y			(60)	// ポーズロゴの高さ
#define SELECT_MOVE					(75)	// セレクトの移動値

#define SELECT_RESTART				(0)		// 再開
#define SELECT_REOPNE				(1)		// やり直し
#define SELECT_RETITLE				(2)		// タイトルへ

enum PAUSE_TEX{

	PAUSE,
	RESTART,
	REOPNE,
	RETITLE,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPause(int type);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
//bool GetPause(void);
//bool SetPause(bool on);

#endif
