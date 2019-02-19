//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_



//*****************************************************************************
// マクロ定義
//*****************************************************************************


#define	PAUSE_POS_X					(370)			// タイトルの表示位置
#define	PAUSE_POS_Y					(300)			// タイトルの表示位置
#define	PAUSE_SIZE_X				(1120)			// タイトルの幅
#define	PAUSE_SIZE_Y				(520)			// タイトルの高さ

#define	PAUSE_LOGO_POS_X			(840)			// タイトルロゴの表示位置
#define	PAUSE_LOGO_POS_Y			(400)			// タイトルロゴの表示位置
#define	PAUSE_LOGO_SIZE_X			(200)			// タイトルロゴの幅
#define	PAUSE_LOGO_SIZE_Y			(150)			// タイトルロゴの高さ
#define SET_LOGO					(75)

#define PAUSE_LOGO_PUSHKEY_POS_X01	(710)			// 遷移キーの表示位置
#define PAUSE_LOGO_PUSHKEY_POS_Y01	(625)			//
#define PAUSE_LOGO_PUSHKEY_SIZE_X01	(300)			// 大きさ
#define PAUSE_LOGO_PUSHKEY_SIZE_Y01	(100)			//

#define PAUSE_LOGO_PUSHKEY_POS_X02	(1055)			// 遷移キーの表示位置
#define PAUSE_LOGO_PUSHKEY_POS_Y02	(645)			//
#define PAUSE_LOGO_PUSHKEY_SIZE_X02	(70)			// 大きさ
#define PAUSE_LOGO_PUSHKEY_SIZE_Y02	(70)			//

#define PUSHKEY_MAX					(2)				// 遷移キーの最大表示数

#define	PAUSE_SELECT_POS_X	(770)					// タイトルロゴの表示位置
#define	PAUSE_SELECT_POS_Y	(445)					// タイトルロゴの表示位置
#define	PAUSE_SELECT_SIZE_X	(60)					// タイトルロゴの幅
#define	PAUSE_SELECT_SIZE_Y	(60)					// タイトルロゴの高さ
#define SELECT_MOVE			(75)					// セレクトの移動値

#define NUM_PAUSE			(2)
#define SELECT_RESTART		(0)
#define SELECT_REOPNE		(1)
#define SELECT_RETITLE		(2)

enum {

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
