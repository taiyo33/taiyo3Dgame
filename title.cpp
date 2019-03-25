//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "player.h"
#include "ai.h"
#include "lifeGauge.h"
#include "ball.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	POS_X						(550)			// タイトルの表示位置
#define	POS_Y						(380)			// タイトルの表示位置
#define	SIZE_X						(750)			// タイトルの幅
#define	SIZE_Y						(310)			// タイトルの高さ
#define	LOGO_POS_X					(840)			// タイトルロゴの表示位置
#define	LOGO_POS_Y					(700)			// タイトルロゴの表示位置
#define	LOGO_SIZE_X					(200)			// タイトルロゴの幅
#define	LOGO_SIZE_Y					(150)			// タイトルロゴの高さ
#define LOGO_PUSHKEY_POS_X01		(710)			// 遷移キーの表示位置
#define LOGO_PUSHKEY_POS_Y01		(625)			//
#define LOGO_PUSHKEY_SIZE_X01		(300)			// 大きさ
#define LOGO_PUSHKEY_SIZE_Y01		(100)			//
#define LOGO_PUSHKEY_POS_X02		(1055)			// 遷移キーの表示位置
#define LOGO_PUSHKEY_POS_Y02		(645)			//
#define LOGO_PUSHKEY_SIZE_X02		(70)			// 大きさ
#define LOGO_PUSHKEY_SIZE_Y02		(70)			//
#define LOGO_MAX					(5)				// ロゴの最大数
#define	SELECT_POS_X				(770)			// タイトルロゴの表示位置
#define	SELECT_POS_Y				(745)			// タイトルロゴの表示位置
#define	SELECT_SIZE_X				(60)			// タイトルロゴの幅
#define	SELECT_SIZE_Y				(60)			// タイトルロゴの高さ
#define SELECT_MOVE					(75)			// セレクトの移動値
#define PUSHKEY_MAX					(2)				// 遷移キーの最大表示数
#define SELECT_NPC					(0)				// vs NPCモード
#define SELECT_PVP					(1)				// vs P2モード
#define SELECT_EXIT					(2)				// 終了
#define LOGOSPACE_ON				(1)				// 遷移ボタンのロゴ点滅状態
#define LOGOSPACE_OFF				(0)				// 
#define ON_COUNT					(70)			// 点灯の時間
#define OFF_COUNT					(30)			// 消灯の時間
#define MOVE_MENU_CNT_FREAM			(60)			// 遷移までの時間
#define BUTTON_OFF_CNT				(30)			// 操作不能時間

#define	TEXTURE_TITLE				("data/TEXTURE/title_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_PVP					("data/TEXTURE/pvp_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_NPC					("data/TEXTURE/pvc_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_EXIT				("data/TEXTURE/exit_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_SELECT				("data/TEXTURE/yajirusi.png")	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(void);
void SetVertexSelect(int selectNum);
void SetLogoTextureDiffuse(int ver);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9				D3DTextureTitle = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9				D3DTextureLogo[LOGO_MAX];	// テクスチャへのポインタ
LPDIRECT3DTEXTURE9				D3DTextureSelect;			// テクスチャへのポインタ
LPDIRECTSOUNDBUFFER8			TitleBGM = NULL;			// タイトルのBGM
LPDIRECTSOUNDBUFFER8			DecisionSE = NULL;			// タイトルのメニュー決定SE
LPDIRECTSOUNDBUFFER8			SelectSE = NULL;			// 選択ロゴのSE
VERTEX_2D						VertexWkTitle[NUM_VERTEX];				// 頂点情報格納ワーク
VERTEX_2D						VertexWkLogo[LOGO_MAX][NUM_VERTEX];		// 頂点情報格納ワーク
VERTEX_2D						VertexWkSelect[NUM_VERTEX];				// 頂点情報格納ワーク
static float					MoveY;						// ロゴの位置調整
static int						SelectNum;					// セレクトのされているロゴ番号
static int						CntFrame;					// 点滅処理のフレームカウント
static int						ButtonCntFrame;				// 操作可能までのフレームカウント
bool							SelectFlag;					// メニュー決定フラグ

//=============================================================================
// 初期化処理
// 引　数：int type(再初期化時の2数判定変数)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 変数初期化
	MoveY = 0.0;
	CntFrame = 0;
	SelectFlag = false;
	SelectNum = 0;
	ButtonCntFrame = 0;

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_TITLE,					// ファイルの名前
			&D3DTextureTitle);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_ENTHER,					// ファイルの名前
			&D3DTextureLogo[TEX_NUM001]);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_BUTTON,					// ファイルの名前
			&D3DTextureLogo[TEX_NUM002]);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_NPC,					// ファイルの名前
			&D3DTextureLogo[TEX_NUM003]);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_PVP,					// ファイルの名前
			&D3DTextureLogo[TEX_NUM004]);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_EXIT,					// ファイルの名前
			&D3DTextureLogo[TEX_NUM005]);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_SELECT,					// ファイルの名前
			&D3DTextureSelect);				// 読み込むメモリー
	}
	
	// 頂点情報の作成
	MakeVertexTitle();
	
	// BGMのロードと再生
	TitleBGM = LoadSound(BGM_00);
	PlaySound(TitleBGM, E_DS8_FLAG_LOOP);
	
	// SEのロード
	DecisionSE = LoadSound(SE_SPACE);
	SelectSE = LoadSound(SE_SELECT);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if(D3DTextureTitle != NULL)
	{// テクスチャの開放
		D3DTextureTitle->Release();
		D3DTextureTitle = NULL;
	}

	for (int i = 0; i < LOGO_MAX; i++)
	{
		if (D3DTextureLogo[i] != NULL)
		{// テクスチャの開放
			D3DTextureLogo[i]->Release();
			D3DTextureLogo[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	PLAYER *player = GetPlayer(0);

	// 遷移
	if (SelectFlag)
	{
		if (CntFrame % MOVE_MENU_CNT_FREAM == 0)
		{
			SetInitPosPlayer();
			SetInitPosBall();
			SetStage(TUTORIAL);
		}
	}

	// 各フレームの更新
	CntFrame++;		 
	ButtonCntFrame++;

	if (CntFrame % OFF_COUNT == 0)
	{
		SetLogoTextureDiffuse(LOGOSPACE_OFF);
	}
	if (CntFrame % ON_COUNT == 0)
	{
		SetLogoTextureDiffuse(LOGOSPACE_ON);
		CntFrame = 0;
	}

	// 操作可能になるまで
	if (ButtonCntFrame > BUTTON_OFF_CNT)
	{
		// 下方向へセレクトを移動
		if (GetKeyboardTrigger(DIK_DOWN))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 1) % 3;
			SetVertexSelect(SelectNum);
		}
		else if (IsButtonTriggered(P1, BUTTON_DOWN))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 1) % 3;
			SetVertexSelect(SelectNum);
		}
		else if (IsButtonTriggered(P2, BUTTON_DOWN))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 1) % 3;
			SetVertexSelect(SelectNum);
		}

		// 上方向のセレクトを移動
		if (GetKeyboardTrigger(DIK_UP))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 2) % 3;
			SetVertexSelect(SelectNum);
		}
		else if (IsButtonTriggered(P1, BUTTON_UP))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 2) % 3;
			SetVertexSelect(SelectNum);
		}
		else if (IsButtonTriggered(P2, BUTTON_UP))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 2) % 3;
			SetVertexSelect(SelectNum);
		}

		// NPC戦へ遷移
		if (SelectNum == SELECT_NPC)
		{
			if ((GetKeyboardTrigger(DIK_RETURN)))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				player[P2].npc = true;
				//player[P3].npc = true;
				//player[P4].npc = true;
				SetIconTextureType(NPCICON);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
			else if (IsButtonPressed(P1, BUTTON_A))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				player[P2].npc = true;
				//player[P3].npc = true;
				//player[P4].npc = true;
				SetIconTextureType(NPCICON);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
			else if (IsButtonPressed(P2, BUTTON_B))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				player[P2].npc = true;
				//player[P3].npc = true;
				//player[P4].npc = true;
				SetIconTextureType(NPCICON);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
		}
		// PVPへ遷移
		else if (SelectNum == SELECT_PVP)
		{
			if ((GetKeyboardTrigger(DIK_RETURN)))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				SetIconTextureType(PLAYERICON02);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
			else if (IsButtonPressed(P1, BUTTON_A))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				SetIconTextureType(PLAYERICON02);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
			else if (IsButtonPressed(P2, BUTTON_B))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				SetIconTextureType(PLAYERICON02);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
		}
		// 強制終了
		else if (SelectNum == SELECT_EXIT)
		{
			if ((GetKeyboardTrigger(DIK_RETURN)))
			{
				exit(NULL);
			}
			else if (IsButtonPressed(P1, BUTTON_A))
			{
				exit(NULL);
			}
			else if (IsButtonPressed(P2, BUTTON_B))
			{
				exit(NULL);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// タイトルの背景描画
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureTitle);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkTitle, sizeof(VERTEX_2D));
	}

	// セレクトの描画
	{
		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureSelect);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkSelect, sizeof(VERTEX_2D));
	}

	// セレクトロゴの描画
	for (int i = 0; i < LOGO_MAX; i++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureLogo[i]);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkLogo[i], sizeof(VERTEX_2D));
	}

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	float set = 0.0;
	// タイトル背景
	{
		// 頂点座標の設定
		VertexWkTitle[0].vtx = D3DXVECTOR3(POS_X, POS_Y, 0.0f);
		VertexWkTitle[1].vtx = D3DXVECTOR3(POS_X + SIZE_X, POS_Y, 0.0f);
		VertexWkTitle[2].vtx = D3DXVECTOR3(POS_X, POS_Y + SIZE_Y, 0.0f);
		VertexWkTitle[3].vtx = D3DXVECTOR3(POS_X + SIZE_X, POS_Y + SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		VertexWkTitle[0].rhw =
		VertexWkTitle[1].rhw =
		VertexWkTitle[2].rhw =
		VertexWkTitle[3].rhw = 1.0f;

		// 反射光の設定
		VertexWkTitle[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTitle[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTitle[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTitle[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWkTitle[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkTitle[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkTitle[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkTitle[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// プッシュキーの表示
	{
		// 頂点座標の設定
		VertexWkLogo[TEX_NUM001][0].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X01 , LOGO_PUSHKEY_POS_Y01, 0.0f);
		VertexWkLogo[TEX_NUM001][1].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X01 + LOGO_PUSHKEY_SIZE_X01 , LOGO_PUSHKEY_POS_Y01, 0.0f);
		VertexWkLogo[TEX_NUM001][2].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X01 , LOGO_PUSHKEY_POS_Y01 + LOGO_PUSHKEY_SIZE_Y01, 0.0f);
		VertexWkLogo[TEX_NUM001][3].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X01 + LOGO_PUSHKEY_SIZE_X01 , LOGO_PUSHKEY_POS_Y01 + LOGO_PUSHKEY_SIZE_Y01, 0.0f);

		// テクスチャのパースペクティブコレクト用
		VertexWkLogo[TEX_NUM001][0].rhw =
		VertexWkLogo[TEX_NUM001][1].rhw =
		VertexWkLogo[TEX_NUM001][2].rhw =
		VertexWkLogo[TEX_NUM001][3].rhw = 1.0f;

		// 反射光の設定
		VertexWkLogo[TEX_NUM001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[TEX_NUM001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[TEX_NUM001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[TEX_NUM001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWkLogo[TEX_NUM001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkLogo[TEX_NUM001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkLogo[TEX_NUM001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkLogo[TEX_NUM001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点座標の設定
	VertexWkLogo[TEX_NUM002][0].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X02 , LOGO_PUSHKEY_POS_Y02, 0.0f);
	VertexWkLogo[TEX_NUM002][1].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X02 + LOGO_PUSHKEY_SIZE_X02 , LOGO_PUSHKEY_POS_Y02, 0.0f);
	VertexWkLogo[TEX_NUM002][2].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X02 , LOGO_PUSHKEY_POS_Y02 + LOGO_PUSHKEY_SIZE_Y02, 0.0f);
	VertexWkLogo[TEX_NUM002][3].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X02 + LOGO_PUSHKEY_SIZE_X02 , LOGO_PUSHKEY_POS_Y02 + LOGO_PUSHKEY_SIZE_Y02, 0.0f);

	// テクスチャのパースペクティブコレクト用
	VertexWkLogo[TEX_NUM002][0].rhw =
	VertexWkLogo[TEX_NUM002][1].rhw =
	VertexWkLogo[TEX_NUM002][2].rhw =
	VertexWkLogo[TEX_NUM002][3].rhw = 1.0f;

	// 反射光の設定
	VertexWkLogo[TEX_NUM002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkLogo[TEX_NUM002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkLogo[TEX_NUM002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkLogo[TEX_NUM002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	VertexWkLogo[TEX_NUM002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkLogo[TEX_NUM002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkLogo[TEX_NUM002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkLogo[TEX_NUM002][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// セレクトロゴ
	for (int j = PUSHKEY_MAX; j < LOGO_MAX; j++, set += SELECT_MOVE)
	{
		// 頂点座標の設定
		VertexWkLogo[j][0].vtx = D3DXVECTOR3(LOGO_POS_X, LOGO_POS_Y + set, 0.0f);
		VertexWkLogo[j][1].vtx = D3DXVECTOR3(LOGO_POS_X + LOGO_SIZE_X, LOGO_POS_Y + set, 0.0f);
		VertexWkLogo[j][2].vtx = D3DXVECTOR3(LOGO_POS_X, LOGO_POS_Y + LOGO_SIZE_Y + set, 0.0f);
		VertexWkLogo[j][3].vtx = D3DXVECTOR3(LOGO_POS_X + LOGO_SIZE_X, LOGO_POS_Y + LOGO_SIZE_Y + set, 0.0f);

		// テクスチャのパースペクティブコレクト用
		VertexWkLogo[j][0].rhw =
		VertexWkLogo[j][1].rhw =
		VertexWkLogo[j][2].rhw =
		VertexWkLogo[j][3].rhw = 1.0f;

		// 反射光の設定
		VertexWkLogo[j][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[j][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[j][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[j][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWkLogo[j][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkLogo[j][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkLogo[j][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkLogo[j][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// セレクト
	{
		// 頂点座標の設定
		VertexWkSelect[0].vtx = D3DXVECTOR3(SELECT_POS_X, SELECT_POS_Y, 0.0f);
		VertexWkSelect[1].vtx = D3DXVECTOR3(SELECT_POS_X + SELECT_SIZE_X, SELECT_POS_Y, 0.0f);
		VertexWkSelect[2].vtx = D3DXVECTOR3(SELECT_POS_X, SELECT_POS_Y + SELECT_SIZE_Y, 0.0f);
		VertexWkSelect[3].vtx = D3DXVECTOR3(SELECT_POS_X + SELECT_SIZE_X, SELECT_POS_Y + SELECT_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		VertexWkSelect[0].rhw =
		VertexWkSelect[1].rhw =
		VertexWkSelect[2].rhw =
		VertexWkSelect[3].rhw = 1.0f;

		// 反射光の設定
		VertexWkSelect[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkSelect[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkSelect[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkSelect[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWkSelect[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkSelect[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkSelect[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkSelect[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//============================================================================
// セレクトの設置
// 引　数：int selectNum（セレクトロゴの種類）
// 戻り値：な　し
//============================================================================
void SetVertexSelect(int selectNum)
{
	// セレクトテクスチャの移動値
	MoveY = (SELECT_MOVE * (float) selectNum);

	// 頂点座標の設定
	VertexWkSelect[0].vtx = D3DXVECTOR3(SELECT_POS_X, SELECT_POS_Y + MoveY, 0.0f);
	VertexWkSelect[1].vtx = D3DXVECTOR3(SELECT_POS_X + SELECT_SIZE_X, SELECT_POS_Y + MoveY, 0.0f);
	VertexWkSelect[2].vtx = D3DXVECTOR3(SELECT_POS_X, SELECT_POS_Y + SELECT_SIZE_Y + MoveY, 0.0f);
	VertexWkSelect[3].vtx = D3DXVECTOR3(SELECT_POS_X + SELECT_SIZE_X, SELECT_POS_Y + SELECT_SIZE_Y + MoveY, 0.0f);
}

//============================================================================
// 遷移ボタンの点滅処理
// 引　数：int ver（透過の値）
// 戻り値：な　し
//============================================================================
void SetLogoTextureDiffuse(int ver) 
{
	// 反射光の設定
	VertexWkLogo[TEX_NUM001][0].diffuse = 
	VertexWkLogo[TEX_NUM001][1].diffuse = 
	VertexWkLogo[TEX_NUM001][2].diffuse = 
	VertexWkLogo[TEX_NUM001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * ver);

	// 反射光の設定
	VertexWkLogo[TEX_NUM002][0].diffuse =
	VertexWkLogo[TEX_NUM002][1].diffuse =
	VertexWkLogo[TEX_NUM002][2].diffuse =
	VertexWkLogo[TEX_NUM002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * ver);

}

//============================================================================
// タイトルBGMの取得
// 引　数：な　し
// 戻り値：LPDIRECTSOUNDBUFFER8型
//============================================================================
LPDIRECTSOUNDBUFFER8 *GetTitleSound(void)
{
	return &TitleBGM;
}

