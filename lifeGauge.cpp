//=============================================================================
//
// ライフゲージ処理 [lifeGauge.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "lifeGauge.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_LIFEGAUGE1	("data/TEXTURE/gauge00.png")	// 歩行用画像
#define TEXTURE_LIFEGAUGE2	("data/TEXTURE/gauge_01.png")	// 歩行用画像
#define TEXTURE_LIFEGAUGE3	("data/TEXTURE/gauge_02.png")	// 歩行用画像
#define TEXTURE_LIFEGAUGE4	("data/TEXTURE/gauge_03.png")	// 歩行用画像
#define TEXTURE_LIFEICON01	("data/TEXTURE/player01.png")	// 歩行用画像
#define TEXTURE_LIFEICON02	("data/TEXTURE/player02.png")	// 歩行用画像
#define LIFEGAUGE_RED_P1	(20.0f)
#define LIFEGAUGE_YELLOW_P1	(50.0f)
#define LIFEGAUGE_RED_P2	(80.0f)
#define LIFEGAUGE_YELLOW_P2	(50.0f)
#define LIFEGAUGE_VTX_MAX	(12)
#define TEXTURE_MAX			(6)
#define LIFEGAUGE_MAX		(2)

enum {
	LIFEGAUGE001,
	LIFEGAUGE002,
	LIFEGAUGE003,
	LIFEGAUGE004,
	PLAYERICON01,
	PLAYERICON02
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexLifeGauge(void);
void SetVertexLifeGauge01(float val);
void SetTextureLifeGauge01(float val);
void SetLifeGaugeTextureType01(int index, float life);
void SetVertexLifeGauge02(float val);
void SetTextureLifeGauge02(float val);
void SetLifeGaugeTextureType02(int index, float life);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureLifeGauge[TEXTURE_MAX];		// テクスチャへのポインタ
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];	// 頂点情報格納ワーク

int							TextureNumLifeGauge[LIFEGAUGE_MAX];			// 
bool						LifeGaugeUse[LIFEGAUGE_MAX];		// 
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLifeGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_LIFEGAUGE1,						// ファイルの名前
			&D3DTextureLifeGauge[LIFEGAUGE001]);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_LIFEGAUGE2,						// ファイルの名前
			&D3DTextureLifeGauge[LIFEGAUGE002]);		// 読み込むメモリー
		
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_LIFEGAUGE3,						// ファイルの名前
			&D3DTextureLifeGauge[LIFEGAUGE003]);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_LIFEGAUGE4,						// ファイルの名前
			&D3DTextureLifeGauge[LIFEGAUGE004]);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_LIFEICON01,						// ファイルの名前
			&D3DTextureLifeGauge[PLAYERICON01]);		// 読み込むメモリー
			
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_LIFEICON02,						// ファイルの名前
			&D3DTextureLifeGauge[PLAYERICON02]);		// 読み込むメモリー

	}

	for (int i = 0; i < LIFEGAUGE_MAX; i++)
	{
		TextureNumLifeGauge[i] = LIFEGAUGE002;
		LifeGaugeUse[i] = true;
	}

	// 頂点情報の作成
	MakeVertexLifeGauge();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLifeGauge(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureLifeGauge[i] != NULL)
		{	// テクスチャの開放
			D3DTextureLifeGauge[i]->Release();
			D3DTextureLifeGauge[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLifeGauge(void)
{
	float val;
	PLAYER *player = GetPlayer(0);
	
	for (int i = 0; i < LIFEGAUGE_MAX; i++, player++)
	{
		val = player->life / PLAYER_LIFE_MAX;
		if (i == 0)
		{
			SetLifeGaugeTextureType01(i, player->life);
			SetTextureLifeGauge01(val);
			SetVertexLifeGauge01(val);
		}
		if (i == 1)
		{
			SetLifeGaugeTextureType02(i, player->life);
			SetTextureLifeGauge02(val);
			SetVertexLifeGauge02(val);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLifeGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < LIFEGAUGE_MAX; i++)
	{
		if (LifeGaugeUse[i])
		{
			// 外枠
			{	// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureLifeGauge[LIFEGAUGE001]);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[LIFEGAUGE001 + i], sizeof(VERTEX_2D));
			}

			// 中身
			{	// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureLifeGauge[TextureNumLifeGauge[i]]);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[LIFEGAUGE003 + i], sizeof(VERTEX_2D));
			}
			
			// アイコン
			{	// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureLifeGauge[PLAYERICON01 + i]);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[PLAYERICON01 + i], sizeof(VERTEX_2D));
			}
		}
	}

	// αテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexLifeGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 外枠
	{
		// 頂点座標の設定
		vertexWk[LIFEGAUGE001][0].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_01, LIFEGAUGE_OUTERPOS_Y_01, 0.0f);
		vertexWk[LIFEGAUGE001][1].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_01 + TEXTURE_LIFEGAUGE_OUTER_SIZE_X, LIFEGAUGE_OUTERPOS_Y_01, 0.0f);
		vertexWk[LIFEGAUGE001][2].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_01, LIFEGAUGE_OUTERPOS_Y_01 + TEXTURE_LIFEGAUGE_OUTER_SIZE_Y, 0.0f);
		vertexWk[LIFEGAUGE001][3].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_01 + TEXTURE_LIFEGAUGE_OUTER_SIZE_X, LIFEGAUGE_OUTERPOS_Y_01 + TEXTURE_LIFEGAUGE_OUTER_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[LIFEGAUGE001][0].rhw =
			vertexWk[LIFEGAUGE001][1].rhw =
			vertexWk[LIFEGAUGE001][2].rhw =
			vertexWk[LIFEGAUGE001][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[LIFEGAUGE001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[LIFEGAUGE001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[LIFEGAUGE001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[LIFEGAUGE001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[LIFEGAUGE001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 外枠
	{
		// 頂点座標の設定
		vertexWk[LIFEGAUGE002][0].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_02,LIFEGAUGE_OUTERPOS_Y_02, 0.0f);
		vertexWk[LIFEGAUGE002][1].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_02 + TEXTURE_LIFEGAUGE_OUTER_SIZE_X,LIFEGAUGE_OUTERPOS_Y_02, 0.0f);
		vertexWk[LIFEGAUGE002][2].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_02,LIFEGAUGE_OUTERPOS_Y_02 + TEXTURE_LIFEGAUGE_OUTER_SIZE_Y, 0.0f);
		vertexWk[LIFEGAUGE002][3].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_02 + TEXTURE_LIFEGAUGE_OUTER_SIZE_X,LIFEGAUGE_OUTERPOS_Y_02 + TEXTURE_LIFEGAUGE_OUTER_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[LIFEGAUGE002][0].rhw =
			vertexWk[LIFEGAUGE002][1].rhw =
			vertexWk[LIFEGAUGE002][2].rhw =
			vertexWk[LIFEGAUGE002][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[LIFEGAUGE002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[LIFEGAUGE002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[LIFEGAUGE002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[LIFEGAUGE002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[LIFEGAUGE002][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	
	// 中身
	{
		// 頂点座標の設定
		vertexWk[LIFEGAUGE003][0].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01, LIFEGAUGE_INNERPOS_Y_01, 0.0f);
		vertexWk[LIFEGAUGE003][1].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_X, LIFEGAUGE_INNERPOS_Y_01, 0.0f);
		vertexWk[LIFEGAUGE003][2].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01, LIFEGAUGE_INNERPOS_Y_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
		vertexWk[LIFEGAUGE003][3].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_X, LIFEGAUGE_INNERPOS_Y_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[LIFEGAUGE003][0].rhw =
			vertexWk[LIFEGAUGE003][1].rhw =
			vertexWk[LIFEGAUGE003][2].rhw =
			vertexWk[LIFEGAUGE003][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[LIFEGAUGE003][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE003][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE003][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE003][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[LIFEGAUGE003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[LIFEGAUGE003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[LIFEGAUGE003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[LIFEGAUGE003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 中身
	{
		// 頂点座標の設定
		vertexWk[LIFEGAUGE004][0].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02,LIFEGAUGE_INNERPOS_Y_02, 0.0f);
		vertexWk[LIFEGAUGE004][1].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_X,LIFEGAUGE_INNERPOS_Y_02, 0.0f);
		vertexWk[LIFEGAUGE004][2].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02,LIFEGAUGE_INNERPOS_Y_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
		vertexWk[LIFEGAUGE004][3].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_X,LIFEGAUGE_INNERPOS_Y_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[LIFEGAUGE004][0].rhw =
			vertexWk[LIFEGAUGE004][1].rhw =
			vertexWk[LIFEGAUGE004][2].rhw =
			vertexWk[LIFEGAUGE004][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[LIFEGAUGE004][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE004][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE004][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[LIFEGAUGE004][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[LIFEGAUGE004][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[LIFEGAUGE004][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[LIFEGAUGE004][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[LIFEGAUGE004][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// アイコン
	{
		// 頂点座標の設定
		vertexWk[PLAYERICON01][0].vtx = D3DXVECTOR3(LIFEICON_POS_X_01, LIFEICON_POS_Y_01, 0.0f);
		vertexWk[PLAYERICON01][1].vtx = D3DXVECTOR3(LIFEICON_POS_X_01 + TEXTURE_LIFEICON_SIZE_X, LIFEICON_POS_Y_01, 0.0f);
		vertexWk[PLAYERICON01][2].vtx = D3DXVECTOR3(LIFEICON_POS_X_01, LIFEICON_POS_Y_01 + TEXTURE_LIFEICON_SIZE_Y, 0.0f);
		vertexWk[PLAYERICON01][3].vtx = D3DXVECTOR3(LIFEICON_POS_X_01 + TEXTURE_LIFEICON_SIZE_X, LIFEICON_POS_Y_01 + TEXTURE_LIFEICON_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[PLAYERICON01][0].rhw =
			vertexWk[PLAYERICON01][1].rhw =
			vertexWk[PLAYERICON01][2].rhw =
			vertexWk[PLAYERICON01][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[PLAYERICON01][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[PLAYERICON01][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[PLAYERICON01][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[PLAYERICON01][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[PLAYERICON01][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[PLAYERICON01][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[PLAYERICON01][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[PLAYERICON01][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// アイコン
	{
		// 頂点座標の設定
		vertexWk[PLAYERICON02][0].vtx = D3DXVECTOR3(LIFEICON_POS_X_02, LIFEICON_POS_Y_02, 0.0f);
		vertexWk[PLAYERICON02][1].vtx = D3DXVECTOR3(LIFEICON_POS_X_02 + TEXTURE_LIFEICON_SIZE_X, LIFEICON_POS_Y_02, 0.0f);
		vertexWk[PLAYERICON02][2].vtx = D3DXVECTOR3(LIFEICON_POS_X_02, LIFEICON_POS_Y_02 + TEXTURE_LIFEICON_SIZE_Y, 0.0f);
		vertexWk[PLAYERICON02][3].vtx = D3DXVECTOR3(LIFEICON_POS_X_02 + TEXTURE_LIFEICON_SIZE_X, LIFEICON_POS_Y_02 + TEXTURE_LIFEICON_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[PLAYERICON02][0].rhw =
			vertexWk[PLAYERICON02][1].rhw =
			vertexWk[PLAYERICON02][2].rhw =
			vertexWk[PLAYERICON02][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[PLAYERICON02][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[PLAYERICON02][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[PLAYERICON02][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[PLAYERICON02][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[PLAYERICON02][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[PLAYERICON02][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[PLAYERICON02][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[PLAYERICON02][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
// 引　数：float val(テクスチャのX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetTextureLifeGauge01(float val)
{
	vertexWk[LIFEGAUGE003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[LIFEGAUGE003][1].tex = D3DXVECTOR2(1.0f * (float)(val), 0.0f);
	vertexWk[LIFEGAUGE003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[LIFEGAUGE003][3].tex = D3DXVECTOR2(1.0f * (float)(val), 1.0f);
}

//=============================================================================
// 頂点座標の設定
// 引　数：faloat val(頂点のX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetVertexLifeGauge01(float val)
{
	// 頂点座標の設定
	vertexWk[LIFEGAUGE003][0].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01, LIFEGAUGE_INNERPOS_Y_01, 0.0f);
	vertexWk[LIFEGAUGE003][1].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01 + (TEXTURE_LIFEGAUGE_INNER_SIZE_X * val), LIFEGAUGE_INNERPOS_Y_01, 0.0f);
	vertexWk[LIFEGAUGE003][2].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01, LIFEGAUGE_INNERPOS_Y_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
	vertexWk[LIFEGAUGE003][3].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01 + (TEXTURE_LIFEGAUGE_INNER_SIZE_X * val), LIFEGAUGE_INNERPOS_Y_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
}

//=============================================================================
// テクスチャ座標の設定
// 引　数：float val(テクスチャのX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetTextureLifeGauge02(float val)
{
	vertexWk[LIFEGAUGE004][0].tex = D3DXVECTOR2((float)(val), 0.0f);
	vertexWk[LIFEGAUGE004][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[LIFEGAUGE004][2].tex = D3DXVECTOR2((float)(val), 1.0f);
	vertexWk[LIFEGAUGE004][3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点座標の設定
// 引　数：faloat val(頂点のX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetVertexLifeGauge02(float val)
{
	// 頂点座標の設定
	vertexWk[LIFEGAUGE004][0].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + (TEXTURE_LIFEGAUGE_INNER_SIZE_X * val), LIFEGAUGE_INNERPOS_Y_02, 0.0f);
	vertexWk[LIFEGAUGE004][1].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_X, LIFEGAUGE_INNERPOS_Y_02, 0.0f);
	vertexWk[LIFEGAUGE004][2].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + (TEXTURE_LIFEGAUGE_INNER_SIZE_X * val), LIFEGAUGE_INNERPOS_Y_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
	vertexWk[LIFEGAUGE004][3].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_X, LIFEGAUGE_INNERPOS_Y_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
}

//=============================================================================
// テクスチャタイプの設定
// 引　数：int LIFEGAUGE003(テクスチャタイプのアドレス番号), faloat life(プレイヤーの体力)
// 戻り値：な　し
//=============================================================================
void SetLifeGaugeTextureType01(int index, float life)
{
	// 赤ゲージへ変化
	if (life < LIFEGAUGE_RED_P1)
	{
		TextureNumLifeGauge[index] = LIFEGAUGE004;
	}
	// 黄色ゲージへ変化
	else if (life < LIFEGAUGE_YELLOW_P1)
	{
		TextureNumLifeGauge[index] = LIFEGAUGE003;
	}
	// 緑ゲージ
	else
	{
		TextureNumLifeGauge[index] = LIFEGAUGE002;
	}
}

//=============================================================================
// テクスチャタイプの設定
// 引　数：int LIFEGAUGE003(テクスチャタイプのアドレス番号), faloat life(プレイヤーの体力)
// 戻り値：な　し
//=============================================================================
void SetLifeGaugeTextureType02(int index, float life)
{
	// 赤ゲージへ変化
	if (life > LIFEGAUGE_RED_P2)
	{
		TextureNumLifeGauge[index] = LIFEGAUGE004;
	}
	// 黄色ゲージへ変化
	else if (life > LIFEGAUGE_YELLOW_P2)
	{
		TextureNumLifeGauge[index] = LIFEGAUGE003;
	}
	// 緑ゲージ
	else
	{
		TextureNumLifeGauge[index] = LIFEGAUGE002;
	}
}
