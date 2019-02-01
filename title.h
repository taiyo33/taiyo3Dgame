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


#define	TITLE_POS_X			(350)								// タイトルの表示位置
#define	TITLE_POS_Y			(250)								// タイトルの表示位置
#define	TITLE_SIZE_X		(600)							// タイトルの幅
#define	TITLE_SIZE_Y		(250)							// タイトルの高さ

#define	TITLE_LOGO_POS_X	(550)							// タイトルロゴの表示位置
#define	TITLE_LOGO_POS_Y	(500)							// タイトルロゴの表示位置
#define	TITLE_LOGO_SIZE_X	(200)							// タイトルロゴの幅
#define	TITLE_LOGO_SIZE_Y	(150)							// タイトルロゴの高さ
#define SET_LOGO			(50)

#define	TITLE_SELECT_POS_X	(5)								// タイトルロゴの表示位置
#define	TITLE_SELECT_POS_Y	(300)							// タイトルロゴの表示位置
#define	TITLE_SELECT_SIZE_X	(60)							// タイトルロゴの幅
#define	TITLE_SELECT_SIZE_Y	(60)							// タイトルロゴの高さ
#define SELECT_MOVE			(50)							// セレクトの移動値

#define NUM_TITLE			(2)
#define LOGO_MAX			(4)
#define SELECT_START		(0)
#define SELECT_OPTION		(1)
#define SELECT_EXIT			(2)

enum {

	SPACE,
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
