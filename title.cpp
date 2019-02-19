//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "player.h"
#include "ai.h"
#include "lifeGauge.h"
#include "child.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOGOSPACE_ON				(1)		//
#define LOGOSPACE_OFF				(0)		//
#define ON_COUNT					(70)	//
#define OFF_COUNT					(30)	//
#define MOVE_MENU_CNT_FREAM			(60)	//
#define	TEXTURE_TITLE		("data/TEXTURE/title_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_ENTHER		("data/TEXTURE/push_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_PVP			("data/TEXTURE/pvp_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_NPC			("data/TEXTURE/pvc_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_EXIT		("data/TEXTURE/exit_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_SELECT		("data/TEXTURE/yajirusi.png")	// 読み込むテクスチャファイル名


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(void);
void SetVertexSelect(int move);
void SetLogoTextureDiffuse(int ver);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9				D3DTextureTitle = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9				D3DTextureLogo[LOGO_MAX];	// テクスチャへのポインタ
LPDIRECT3DTEXTURE9				D3DTextureSelect;			// テクスチャへのポインタ

VERTEX_2D						vertexWkTitle[NUM_VERTEX];				// 頂点情報格納ワーク
VERTEX_2D						vertexWkLogo[LOGO_MAX][NUM_VERTEX];		// 頂点情報格納ワーク
VERTEX_2D						vertexWkSelect[NUM_VERTEX];				// 頂点情報格納ワーク
static float					yMove;				// ロゴの位置調整
static int						SelectNum;			// セレクトのされているロゴ番号
static int						CntFrame;			// 点滅処理のフレームカウント
LPDIRECTSOUNDBUFFER8			TitleBGM = NULL;	// タイトルのBGM
LPDIRECTSOUNDBUFFER8			DecisionSE = NULL;	// タイトルのメニュー決定SE
LPDIRECTSOUNDBUFFER8			SelectSE = NULL;	// 選択ロゴのSE
bool							SelectFlag;			// メニュー決定フラグ
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 変数初期化
	yMove = 0.0;
	CntFrame = 0;
	SelectFlag = false;
	SelectNum = 0;

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_TITLE,					// ファイルの名前
			&D3DTextureTitle);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_ENTHER,					// ファイルの名前
			&D3DTextureLogo[ENTHER]);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_BUTTON,					// ファイルの名前
			&D3DTextureLogo[BUTTON]);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_NPC,					// ファイルの名前
			&D3DTextureLogo[NCP]);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_PVP,					// ファイルの名前
			&D3DTextureLogo[PVP]);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_EXIT,					// ファイルの名前
			&D3DTextureLogo[EXIT]);			// 読み込むメモリー

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

	if (SelectFlag)
	{
		if (CntFrame % MOVE_MENU_CNT_FREAM == 0)
		{
			SetInitPosPlayer();
			SetInitPosChild();
			SetStage(TUTORIAL);
		}
	}

	CntFrame++;

	if (CntFrame % OFF_COUNT == 0)
	{
		SetLogoTextureDiffuse(LOGOSPACE_OFF);
	}
	if (CntFrame % ON_COUNT == 0)
	{
		SetLogoTextureDiffuse(LOGOSPACE_ON);
		CntFrame = 0;
	}

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
			SetIconTextureType(NPCICON);
			SelectFlag = true;
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{
			PlaySound(DecisionSE, E_DS8_FLAG_NONE);
			player[P2].npc = true;
			SetIconTextureType(NPCICON);
			SelectFlag = true;
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			PlaySound(DecisionSE, E_DS8_FLAG_NONE);
			player[P2].npc = true;
			SetIconTextureType(NPCICON);
			SelectFlag = true;
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
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{
			PlaySound(DecisionSE, E_DS8_FLAG_NONE);
			SetIconTextureType(PLAYERICON02);
			SelectFlag = true;
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			PlaySound(DecisionSE, E_DS8_FLAG_NONE);
			SetIconTextureType(PLAYERICON02);
			SelectFlag = true;
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
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, vertexWkTitle, sizeof(VERTEX_2D));
	}

	// セレクトの描画
	{
		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureSelect);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, vertexWkSelect, sizeof(VERTEX_2D));
	}

	// セレクトロゴの描画
	for (int i = 0; i < LOGO_MAX; i++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureLogo[i]);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, vertexWkLogo[i], sizeof(VERTEX_2D));
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
		vertexWkTitle[0].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
		vertexWkTitle[1].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_SIZE_X, TITLE_POS_Y, 0.0f);
		vertexWkTitle[2].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_SIZE_Y, 0.0f);
		vertexWkTitle[3].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_SIZE_X, TITLE_POS_Y + TITLE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWkTitle[0].rhw =
		vertexWkTitle[1].rhw =
		vertexWkTitle[2].rhw =
		vertexWkTitle[3].rhw = 1.0f;

		// 反射光の設定
		vertexWkTitle[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkTitle[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkTitle[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkTitle[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWkTitle[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkTitle[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkTitle[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkTitle[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// プッシュキーの表示
	{
		// 頂点座標の設定
		vertexWkLogo[ENTHER][0].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X01 , TITLE_LOGO_PUSHKEY_POS_Y01, 0.0f);
		vertexWkLogo[ENTHER][1].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X01 + TITLE_LOGO_PUSHKEY_SIZE_X01 , TITLE_LOGO_PUSHKEY_POS_Y01, 0.0f);
		vertexWkLogo[ENTHER][2].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X01 , TITLE_LOGO_PUSHKEY_POS_Y01 + TITLE_LOGO_PUSHKEY_SIZE_Y01, 0.0f);
		vertexWkLogo[ENTHER][3].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X01 + TITLE_LOGO_PUSHKEY_SIZE_X01 , TITLE_LOGO_PUSHKEY_POS_Y01 + TITLE_LOGO_PUSHKEY_SIZE_Y01, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWkLogo[ENTHER][0].rhw =
			vertexWkLogo[ENTHER][1].rhw =
			vertexWkLogo[ENTHER][2].rhw =
			vertexWkLogo[ENTHER][3].rhw = 1.0f;

		// 反射光の設定
		vertexWkLogo[ENTHER][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[ENTHER][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[ENTHER][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[ENTHER][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWkLogo[ENTHER][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkLogo[ENTHER][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkLogo[ENTHER][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkLogo[ENTHER][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点座標の設定
	vertexWkLogo[BUTTON][0].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X02 , TITLE_LOGO_PUSHKEY_POS_Y02, 0.0f);
	vertexWkLogo[BUTTON][1].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X02 + TITLE_LOGO_PUSHKEY_SIZE_X02 , TITLE_LOGO_PUSHKEY_POS_Y02, 0.0f);
	vertexWkLogo[BUTTON][2].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X02 , TITLE_LOGO_PUSHKEY_POS_Y02 + TITLE_LOGO_PUSHKEY_SIZE_Y02, 0.0f);
	vertexWkLogo[BUTTON][3].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X02 + TITLE_LOGO_PUSHKEY_SIZE_X02 , TITLE_LOGO_PUSHKEY_POS_Y02 + TITLE_LOGO_PUSHKEY_SIZE_Y02, 0.0f);

	// テクスチャのパースペクティブコレクト用
	vertexWkLogo[BUTTON][0].rhw =
		vertexWkLogo[BUTTON][1].rhw =
		vertexWkLogo[BUTTON][2].rhw =
		vertexWkLogo[BUTTON][3].rhw = 1.0f;

	// 反射光の設定
	vertexWkLogo[BUTTON][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkLogo[BUTTON][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkLogo[BUTTON][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkLogo[BUTTON][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWkLogo[BUTTON][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkLogo[BUTTON][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkLogo[BUTTON][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkLogo[BUTTON][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// セレクトロゴ
	for (int j = PUSHKEY_MAX; j < LOGO_MAX; j++, set += SET_LOGO)
	{
		// 頂点座標の設定
		vertexWkLogo[j][0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + set, 0.0f);
		vertexWkLogo[j][1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_SIZE_X, TITLE_LOGO_POS_Y + set, 0.0f);
		vertexWkLogo[j][2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + TITLE_LOGO_SIZE_Y + set, 0.0f);
		vertexWkLogo[j][3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_SIZE_X, TITLE_LOGO_POS_Y + TITLE_LOGO_SIZE_Y + set, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWkLogo[j][0].rhw =
		vertexWkLogo[j][1].rhw =
		vertexWkLogo[j][2].rhw =
		vertexWkLogo[j][3].rhw = 1.0f;

		// 反射光の設定
		vertexWkLogo[j][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[j][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[j][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[j][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWkLogo[j][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkLogo[j][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkLogo[j][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkLogo[j][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// セレクト
	{
		// 頂点座標の設定
		vertexWkSelect[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y, 0.0f);
		vertexWkSelect[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y, 0.0f);
		vertexWkSelect[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y, 0.0f);
		vertexWkSelect[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWkSelect[0].rhw =
		vertexWkSelect[1].rhw =
		vertexWkSelect[2].rhw =
		vertexWkSelect[3].rhw = 1.0f;

		// 反射光の設定
		vertexWkSelect[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkSelect[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkSelect[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkSelect[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWkSelect[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkSelect[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkSelect[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkSelect[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//============================================================================
// セレクトの設置
// 引数：move（セレクトロゴの種類）
//============================================================================
void SetVertexSelect(int move)
{
	// セレクトテクスチャの移動値
	yMove = (SELECT_MOVE * (float) move);

	// 頂点座標の設定
	vertexWkSelect[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y + yMove, 0.0f);
	vertexWkSelect[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y + yMove, 0.0f);
	vertexWkSelect[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y + yMove, 0.0f);
	vertexWkSelect[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y + yMove, 0.0f);
}

void SetLogoTextureDiffuse(int ver) 
{
	// 反射光の設定
	vertexWkLogo[ENTHER][0].diffuse = 
	vertexWkLogo[ENTHER][1].diffuse = 
	vertexWkLogo[ENTHER][2].diffuse = 
	vertexWkLogo[ENTHER][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * ver);

	// 反射光の設定
	vertexWkLogo[BUTTON][0].diffuse =
		vertexWkLogo[BUTTON][1].diffuse =
		vertexWkLogo[BUTTON][2].diffuse =
		vertexWkLogo[BUTTON][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * ver);

}

LPDIRECTSOUNDBUFFER8 *GetTitleSound(void)
{
	return &TitleBGM;
}

