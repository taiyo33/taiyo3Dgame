//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "player.h"
#include "gameCall.h"
#include "lifeGauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WINNER01		"data/TEXTURE/winner_logo01.png"
#define TEXTURE_WINNER02		"data/TEXTURE/winner_logo02.png"
#define TEXTURE_MAX			(5)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

enum {
	WINNER,
	ICON
};

// テクスチャー番号
enum {
	TEX_WINNER01,
	TEX_WINNER02,
	TEX_PLAYERICON01,
	TEX_PLAYERICON02,
	TEX_NPCICON
};

LPDIRECT3DTEXTURE9		D3DTextureResult[TEXTURE_MAX];				// テクスチャへのポリゴン
VERTEX_2D				vertexWk_RESULT[RESULT_MAX][NUM_VERTEX];	// 頂点情報格納ワーク
int						TextureNumResult;
int						TextureNumIcon;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_WINNER01,						// ファイルの名前
			&D3DTextureResult[TEX_WINNER01]);			// 読み込むメモリのポインタ

				// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_WINNER02,						// ファイルの名前
			&D3DTextureResult[TEX_WINNER02]);			// 読み込むメモリのポインタ

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_ICON01,					// ファイルの名前
			&D3DTextureResult[TEX_PLAYERICON01]);		// 読み込むメモリのポインタ
		
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_ICON02,					// ファイルの名前
			&D3DTextureResult[TEX_PLAYERICON02]);		// 読み込むメモリのポインタ
			
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_ICON03,					// ファイルの名前
			&D3DTextureResult[TEX_NPCICON]);		// 読み込むメモリのポインタ
	}


	// 頂点情報の作成
	MakeVertexResult();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureResult[i] != NULL)
		{	// テクスチャの開放
			D3DTextureResult[i]->Release();
			D3DTextureResult[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	PLAYER *player = GetPlayer(0);
	StopSound(player[P1].chargeSE);
	StopSound(player[P2].chargeSE);

	// 遷移
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		StopSound(GetGameBGM03());
		SetStage(TITLE);
		InitGame();
	}
	else if (IsButtonTriggered(P1, BUTTON_A))
	{
		StopSound(GetGameBGM03());
		SetStage(TITLE);
		InitGame();
	}
	else if (IsButtonTriggered(P2, BUTTON_B))
	{
		StopSound(GetGameBGM03());
		SetStage(TITLE);
		InitGame();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// テクスチャの設定
	pDevice->SetTexture( 0, D3DTextureResult[TextureNumResult] );

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RESULT, vertexWk_RESULT[WINNER], sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, D3DTextureResult[TextureNumIcon + 2]);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RESULT, vertexWk_RESULT[ICON], sizeof(VERTEX_2D));

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 勝者テクスチャー
	{
		// 頂点座標の設定
		vertexWk_RESULT[WINNER][0].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X, TEXTURE_WINNER_POS_Y, 0.0f);
		vertexWk_RESULT[WINNER][1].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X + TEXTURE_WINNER_SIZE_X, TEXTURE_WINNER_POS_Y, 0.0f);
		vertexWk_RESULT[WINNER][2].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X, TEXTURE_WINNER_POS_Y + TEXTURE_WINNER_SIZE_Y, 0.0f);
		vertexWk_RESULT[WINNER][3].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X + TEXTURE_WINNER_SIZE_X, TEXTURE_WINNER_POS_Y + TEXTURE_WINNER_SIZE_Y, 0.0f);

		// rhwの設定
		vertexWk_RESULT[WINNER][0].rhw =
			vertexWk_RESULT[WINNER][1].rhw =
			vertexWk_RESULT[WINNER][2].rhw =
			vertexWk_RESULT[WINNER][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk_RESULT[WINNER][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[WINNER][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[WINNER][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[WINNER][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk_RESULT[WINNER][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk_RESULT[WINNER][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk_RESULT[WINNER][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk_RESULT[WINNER][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// プレイヤーアイコン
	{
		// 頂点座標の設定
		vertexWk_RESULT[ICON][0].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X, TEXTURE_ICON_POS_Y, 0.0f);
		vertexWk_RESULT[ICON][1].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X + TEXTURE_ICON_SIZE_X, TEXTURE_ICON_POS_Y, 0.0f);
		vertexWk_RESULT[ICON][2].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X, TEXTURE_ICON_POS_Y + TEXTURE_ICON_SIZE_Y, 0.0f);
		vertexWk_RESULT[ICON][3].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X + TEXTURE_ICON_SIZE_X, TEXTURE_ICON_POS_Y + TEXTURE_ICON_SIZE_Y, 0.0f);

		// rhwの設定
		vertexWk_RESULT[ICON][0].rhw =
			vertexWk_RESULT[ICON][1].rhw =
			vertexWk_RESULT[ICON][2].rhw =
			vertexWk_RESULT[ICON][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk_RESULT[ICON][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[ICON][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[ICON][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[ICON][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk_RESULT[ICON][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk_RESULT[ICON][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk_RESULT[ICON][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk_RESULT[ICON][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//===============================================================================
// リザルトへの遷移処理
// 引　数：int pno0(勝ったプレイヤー番号),int pno1(プレイヤーかCPUか)
// 戻り値：な　し
//===============================================================================
void SetResult(int pno0, int pno1)
{
	// 各リザルトの表示テクスチャ番号
	TextureNumResult = pno0;
	TextureNumIcon = pno1;

	// リザルトへ遷移
	SetStage(RESULT);
}