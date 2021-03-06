//=============================================================================
//
// ポーズ画面処理 [pause.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "player.h"
#include "ai.h"
#include "lifeGauge.h"
#include "ball.h"
#include "gameCall.h"
#include "time.h"
#include "title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PAUSE		("data/TEXTURE/menu.png")			// 読み込むテクスチャファイル名
#define TEXTURE_SELECT		("data/TEXTURE/bullet001.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_RESTART		("data/TEXTURE/restart_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_REOPNE		("data/TEXTURE/reopne_logo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_RETITLE		("data/TEXTURE/retitle_logo.png")	// 読み込むテクスチャファイル名
#define PAUSE_LOGO_MAX		(4)									// ロゴの最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPause(void);
void SetVertexPause(int move);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9				D3DTexturePause = NULL;			// テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		D3DTextureLogo[PAUSE_LOGO_MAX];	// テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		D3DTextureSelect;				// テクスチャへのポインタ

VERTEX_2D						VertexWkPause[NUM_VERTEX];					// 頂点情報格納ワーク
static VERTEX_2D				VertexWkLogo[PAUSE_LOGO_MAX][NUM_VERTEX];	// 頂点情報格納ワーク
static VERTEX_2D				VertexWkSelect[NUM_VERTEX];					// 頂点情報格納ワーク
static float					MoveY;						// ロゴの位置調整
static int						SelectNum;					// セレクトのされているロゴ番号
bool							PauseUse;					// メニュー決定フラグ
static int						CntFrame;					// 操作可能までのフレームカウント
//===============================================================================
// 初期化処理
// 引　数：int type(再初期化の2数判定値)
// 戻り値：HRESULT型
//===============================================================================
HRESULT InitPause(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 変数初期化
	MoveY = 0.0;
	SelectNum = 0;
	PauseUse = false;
	CntFrame = 0;

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_PAUSE,					// ファイルの名前
			&D3DTexturePause);				// 読み込むメモリー

				// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_SELECT,					// ファイルの名前
			&D3DTextureSelect);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_RESTART,					// ファイルの名前
			&D3DTextureLogo[RESTART]);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_REOPNE,					// ファイルの名前
			&D3DTextureLogo[REOPNE]);			// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_RETITLE,					// ファイルの名前
			&D3DTextureLogo[RETITLE]);			// 読み込むメモリー
	}
	
	// 頂点情報の作成
	MakeVertexPause();
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	if(D3DTexturePause != NULL)
	{// テクスチャの開放
		D3DTexturePause->Release();
		D3DTexturePause = NULL;
	}

	for (int i = 0; i < PAUSE_LOGO_MAX; i++)
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
void UpdatePause(void)
{
	PLAYER *player = GetPlayer(0);

	// ポーズへ遷移してからカウント
	if (GetPause())
	{
		CntFrame++;
	}

	// 下方向へセレクトを移動
	if (GetKeyboardTrigger(DIK_DOWN)) 
	{
		SelectNum = (SelectNum + 1) % 3;
		SetVertexPause(SelectNum);
	}
	else if (IsButtonTriggered(P1, BUTTON_DOWN))
	{

		SelectNum = (SelectNum + 1) % 3;
		SetVertexPause(SelectNum);
	}
	else if (IsButtonTriggered(P2, BUTTON_DOWN))
	{

		SelectNum = (SelectNum + 1) % 3;
		SetVertexPause(SelectNum);
	}

	// 上方向のセレクトを移動
	if (GetKeyboardTrigger(DIK_UP))
	{
		SelectNum = (SelectNum + 2) % 3;
		SetVertexPause(SelectNum);
	}
	else if (IsButtonTriggered(P1, BUTTON_UP))
	{
		SelectNum = (SelectNum + 2) % 3;
		SetVertexPause(SelectNum);
	}
	else if (IsButtonTriggered(P2, BUTTON_UP))
	{
		SelectNum = (SelectNum + 2) % 3;
		SetVertexPause(SelectNum);
	}

	// 30フレームで操作可能
	if (CntFrame % 30 == 0)
	{
		// ポーズ解除
		if ((GetKeyboardTrigger(DIK_M)))
		{
			SetPause(false);
		}
		else if ((GetKeyboardTrigger(DIK_V)))
		{
			SetPause(false);
		}
		else if (IsButtonPressed(P1, BUTTON_R))
		{
			SetPause(false);
		}
		else if (IsButtonPressed(P2, BUTTON_M))
		{
			SetPause(false);
		}
	}

	// チャージSEの停止
	StopSound(player[P1].chargeSE);
	StopSound(player[P2].chargeSE);

	// 再開へ
	if (SelectNum == SELECT_RESTART)
	{
		if ((GetKeyboardTrigger(DIK_RETURN)))
		{
			SetPause(false);
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{
			SetPause(false);
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			SetPause(false);
		}
	}
	// やり直しへ
	else if (SelectNum == SELECT_REOPNE)
	{
		// ゲーム開始前まで戻る
		if ((GetKeyboardTrigger(DIK_RETURN)))
		{
			// 全てのBGMの停止
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());

			// NPCモードの場合
			if (player[P2].npc)
			{
				InitGame();
				StopSound(*GetTitleSound());
				player[P2].npc = true;
				SetIconTextureType(NPCICON);
				SetStage(STARTCALL);
				SetPause(false);
				return;
			}

			// 初期状態へ
			SetStage(STARTCALL);
			SetPause(false);
			InitGame();
			StopSound(*GetTitleSound());
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{		
			// 全てのBGMの停止
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());
			
			// NPCモードの場合
			if (player[P2].npc)
			{
				InitGame();
				StopSound(*GetTitleSound());
				player[P2].npc = true;
				SetIconTextureType(NPCICON);
				SetStage(STARTCALL);
				SetPause(false);
				return;
			}

			// 初期状態へ
			InitGame();
			StopSound(*GetTitleSound());
			SetStage(STARTCALL);
			SetPause(false);
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());

			if (player[P2].npc)
			{				
				InitGame();
				StopSound(*GetTitleSound());
				player[P2].npc = true;
				SetIconTextureType(NPCICON);
				SetStage(STARTCALL);
				SetPause(false);
				return;
			}

			InitGame();
			StopSound(*GetTitleSound());
			SetStage(STARTCALL);
			SetPause(false);
		}
	}
	// タイトルへ
	else if (SelectNum == SELECT_RETITLE)
	{
		// タイトルまで戻る
		if ((GetKeyboardTrigger(DIK_RETURN)))
		{
			// BCMの停止と初期化
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());			
			SetPause(false);
			SetStage(TITLE);
			InitGame();
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{
			// BCMの停止と初期化
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());			
			SetPause(false);
			SetStage(TITLE);
			InitGame();
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			// BCMの停止と初期化
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());			
			SetPause(false);
			SetStage(TITLE);
			InitGame();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// タイトルの背景描画
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTexturePause);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkPause, sizeof(VERTEX_2D));
	}

	// セレクトの描画
	{
		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureSelect);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkSelect, sizeof(VERTEX_2D));
	}

	// セレクトロゴの描画
	for (int i = 0; i < PAUSE_LOGO_MAX; i++)
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
HRESULT MakeVertexPause(void)
{
	float set = 0.0;
	
	// ポーズ枠
	{
		// 頂点座標の設定
		VertexWkPause[0].vtx = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
		VertexWkPause[1].vtx = D3DXVECTOR3(PAUSE_POS_X + PAUSE_SIZE_X, PAUSE_POS_Y, 0.0f);
		VertexWkPause[2].vtx = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y + PAUSE_SIZE_Y, 0.0f);
		VertexWkPause[3].vtx = D3DXVECTOR3(PAUSE_POS_X + PAUSE_SIZE_X, PAUSE_POS_Y + PAUSE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		VertexWkPause[0].rhw =
		VertexWkPause[1].rhw =
		VertexWkPause[2].rhw =
		VertexWkPause[3].rhw = 1.0f;

		// 反射光の設定
		VertexWkPause[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkPause[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkPause[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkPause[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWkPause[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkPause[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkPause[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkPause[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// セレクトロゴ
	for (int j =  1; j < PAUSE_LOGO_MAX; j++, set += SET_LOGO)
	{
		// 頂点座標の設定
		VertexWkLogo[j][0].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X, PAUSE_LOGO_POS_Y + set, 0.0f);
		VertexWkLogo[j][1].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X + PAUSE_LOGO_SIZE_X, PAUSE_LOGO_POS_Y + set, 0.0f);
		VertexWkLogo[j][2].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X, PAUSE_LOGO_POS_Y + PAUSE_LOGO_SIZE_Y + set, 0.0f);
		VertexWkLogo[j][3].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X + PAUSE_LOGO_SIZE_X, PAUSE_LOGO_POS_Y + PAUSE_LOGO_SIZE_Y + set, 0.0f);

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
		VertexWkSelect[0].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y, 0.0f);
		VertexWkSelect[1].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y, 0.0f);
		VertexWkSelect[2].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y, 0.0f);
		VertexWkSelect[3].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y, 0.0f);

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
// 引　数：int move（セレクトロゴの種類）
// 戻り値：な　し
//============================================================================
void SetVertexPause(int move)
{
	// セレクトテクスチャの移動値
	MoveY = (SELECT_MOVE * (float) move);

	// 頂点座標の設定
	VertexWkSelect[0].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + MoveY, 0.0f);
	VertexWkSelect[1].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y + MoveY, 0.0f);
	VertexWkSelect[2].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y + MoveY, 0.0f);
	VertexWkSelect[3].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y + MoveY, 0.0f);
}

