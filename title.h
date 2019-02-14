//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	TEXTURE_BUTTON		("data/TEXTURE/button_a.png")	// 読み込むテクスチャファイル名

#define	TITLE_POS_X					(260)			// タイトルの表示位置
#define	TITLE_POS_Y					(230)			// タイトルの表示位置
#define	TITLE_SIZE_X				(750)			// タイトルの幅
#define	TITLE_SIZE_Y				(310)			// タイトルの高さ

#define	TITLE_LOGO_POS_X			(550)			// タイトルロゴの表示位置
#define	TITLE_LOGO_POS_Y			(550)			// タイトルロゴの表示位置
#define	TITLE_LOGO_SIZE_X			(200)			// タイトルロゴの幅
#define	TITLE_LOGO_SIZE_Y			(150)			// タイトルロゴの高さ
#define SET_LOGO					(50)

#define TITLE_LOGO_PUSHKEY_POS_X01	(420)			// 遷移キーの表示位置
#define TITLE_LOGO_PUSHKEY_POS_Y01	(475)			//
#define TITLE_LOGO_PUSHKEY_SIZE_X01	(300)			// 大きさ
#define TITLE_LOGO_PUSHKEY_SIZE_Y01	(100)			//

#define TITLE_LOGO_PUSHKEY_POS_X02	(745)			// 遷移キーの表示位置
#define TITLE_LOGO_PUSHKEY_POS_Y02	(495)			//
#define TITLE_LOGO_PUSHKEY_SIZE_X02	(70)			// 大きさ
#define TITLE_LOGO_PUSHKEY_SIZE_Y02	(70)			//

#define PUSHKEY_MAX					(2)				// 遷移キーの最大表示数

#define	TITLE_SELECT_POS_X	(480)					// タイトルロゴの表示位置
#define	TITLE_SELECT_POS_Y	(595)					// タイトルロゴの表示位置
#define	TITLE_SELECT_SIZE_X	(60)					// タイトルロゴの幅
#define	TITLE_SELECT_SIZE_Y	(60)					// タイトルロゴの高さ
#define SELECT_MOVE			(50)					// セレクトの移動値

#define NUM_TITLE			(2)
#define LOGO_MAX			(5)
#define SELECT_START		(0)
#define SELECT_OPTION		(1)
#define SELECT_EXIT			(2)

enum {

	ENTHER,
	BUTTON,
	GAMESTART,
	OPTION,
	EXIT,
	SELECT,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(int type);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif
