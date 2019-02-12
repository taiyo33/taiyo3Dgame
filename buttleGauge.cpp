//=============================================================================
//
// バトルゲージ処理 [buttleGauge.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "buttleGauge.h"
#include "input.h"
#include "player.h"
#include "child.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BUTTLEGAUGE1	("data/TEXTURE/buttle_gague.png")	// 歩行用画像
#define TEXTURE_BUTTLEGAUGE2	("data/TEXTURE/buttle_gugae01.png")	// 歩行用画像
#define TEXTURE_BUTTLEGAUGE3	("data/TEXTURE/buttle_gugae02.png")	// 歩行用画像
#define BUTTLEGAUGE_VTX_MAX	(6)
#define TEXTURE_MAX			(3)
#define BUTTLEGAUGE_MAX		(2)

enum {
	BUTTLEGAUGE001,
	BUTTLEGAUGE002,
	BUTTLEGAUGE003,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexButtleGauge(void);
void SetVertexButtleGauge01(float val);
void SetTextureButtleGauge01(float val);
void SetVertexButtleGauge02(float val);
void SetTextureButtleGauge02(float val);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureButtleGauge[TEXTURE_MAX];		// テクスチャへのポインタ
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];	// 頂点情報格納ワーク

bool						ButtleGaugeUse[BUTTLEGAUGE_MAX];		// 
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitButtleGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BUTTLEGAUGE1,						// ファイルの名前
			&D3DTextureButtleGauge[BUTTLEGAUGE001]);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BUTTLEGAUGE2,						// ファイルの名前
			&D3DTextureButtleGauge[BUTTLEGAUGE002]);		// 読み込むメモリー
		
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BUTTLEGAUGE3,						// ファイルの名前
			&D3DTextureButtleGauge[BUTTLEGAUGE003]);		// 読み込むメモリー

	}

	for (int i = 0; i < BUTTLEGAUGE_MAX; i++)
	{
		ButtleGaugeUse[i] = true;
	}

	// 頂点情報の作成
	MakeVertexButtleGauge();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitButtleGauge(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureButtleGauge[i] != NULL)
		{	// テクスチャの開放
			D3DTextureButtleGauge[i]->Release();
			D3DTextureButtleGauge[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateButtleGauge(void)
{
	float val;
	CHILD *child = GetChild(0);

	for (int i = 0; i < BUTTLEGAUGE_MAX; i++, child++)
	{
		val = (float)child->cnt / CHILD_ONESET_MAX; 
		
		if (i == 0)
		{
			SetTextureButtleGauge01(val);
			SetVertexButtleGauge01(val);
		}
		else if (i == 1)
		{
			float cnt = CHILD_ONESET_MAX - child->cnt;
			val = cnt / CHILD_ONESET_MAX;

			SetTextureButtleGauge02(val);
			SetVertexButtleGauge02(val);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawButtleGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// 外枠
	{	// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureButtleGauge[BUTTLEGAUGE001]);
		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[BUTTLEGAUGE001], sizeof(VERTEX_2D));
	}

	for (int i = 0; i < BUTTLEGAUGE_MAX; i++)
	{
		{
			// 中身
			{	// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureButtleGauge[BUTTLEGAUGE002 + i]);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[BUTTLEGAUGE002 + i], sizeof(VERTEX_2D));
			}
			
		}
	}

	// αテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexButtleGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 外枠
	{
		// 頂点座標の設定
		vertexWk[BUTTLEGAUGE001][0].vtx = D3DXVECTOR3(BUTTLEGAUGE_OUTERPOS_X_01, BUTTLEGAUGE_OUTERPOS_Y_01, 0.0f);
		vertexWk[BUTTLEGAUGE001][1].vtx = D3DXVECTOR3(BUTTLEGAUGE_OUTERPOS_X_01 + TEXTURE_BUTTLEGAUGE_OUTER_SIZE_X, BUTTLEGAUGE_OUTERPOS_Y_01, 0.0f);
		vertexWk[BUTTLEGAUGE001][2].vtx = D3DXVECTOR3(BUTTLEGAUGE_OUTERPOS_X_01, BUTTLEGAUGE_OUTERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_OUTER_SIZE_Y, 0.0f);
		vertexWk[BUTTLEGAUGE001][3].vtx = D3DXVECTOR3(BUTTLEGAUGE_OUTERPOS_X_01 + TEXTURE_BUTTLEGAUGE_OUTER_SIZE_X, BUTTLEGAUGE_OUTERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_OUTER_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[BUTTLEGAUGE001][0].rhw =
			vertexWk[BUTTLEGAUGE001][1].rhw =
			vertexWk[BUTTLEGAUGE001][2].rhw =
			vertexWk[BUTTLEGAUGE001][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[BUTTLEGAUGE001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[BUTTLEGAUGE001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[BUTTLEGAUGE001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[BUTTLEGAUGE001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[BUTTLEGAUGE001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	
	// 中身
	{
		// 頂点座標の設定
		vertexWk[BUTTLEGAUGE002][0].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01, BUTTLEGAUGE_INNERPOS_Y_01, 0.0f);
		vertexWk[BUTTLEGAUGE002][1].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X, BUTTLEGAUGE_INNERPOS_Y_01, 0.0f);
		vertexWk[BUTTLEGAUGE002][2].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01, BUTTLEGAUGE_INNERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
		vertexWk[BUTTLEGAUGE002][3].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X, BUTTLEGAUGE_INNERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[BUTTLEGAUGE002][0].rhw =
			vertexWk[BUTTLEGAUGE002][1].rhw =
			vertexWk[BUTTLEGAUGE002][2].rhw =
			vertexWk[BUTTLEGAUGE002][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[BUTTLEGAUGE002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[BUTTLEGAUGE002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[BUTTLEGAUGE002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[BUTTLEGAUGE002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[BUTTLEGAUGE002][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 中身
	{
		// 頂点座標の設定
		vertexWk[BUTTLEGAUGE003][0].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02,BUTTLEGAUGE_INNERPOS_Y_02, 0.0f);
		vertexWk[BUTTLEGAUGE003][1].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X,BUTTLEGAUGE_INNERPOS_Y_02, 0.0f);
		vertexWk[BUTTLEGAUGE003][2].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02,BUTTLEGAUGE_INNERPOS_Y_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
		vertexWk[BUTTLEGAUGE003][3].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X,BUTTLEGAUGE_INNERPOS_Y_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[BUTTLEGAUGE003][0].rhw =
			vertexWk[BUTTLEGAUGE003][1].rhw =
			vertexWk[BUTTLEGAUGE003][2].rhw =
			vertexWk[BUTTLEGAUGE003][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[BUTTLEGAUGE003][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE003][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE003][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE003][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[BUTTLEGAUGE003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[BUTTLEGAUGE003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[BUTTLEGAUGE003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[BUTTLEGAUGE003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}


	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
// 引　数：float val(テクスチャのX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetTextureButtleGauge01(float val)
{
	vertexWk[BUTTLEGAUGE002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[BUTTLEGAUGE002][1].tex = D3DXVECTOR2(1.0f * (float)(val), 0.0f);
	vertexWk[BUTTLEGAUGE002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[BUTTLEGAUGE002][3].tex = D3DXVECTOR2(1.0f * (float)(val), 1.0f);
}

//=============================================================================
// 頂点座標の設定
// 引　数：faloat val(頂点のX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetVertexButtleGauge01(float val)
{
	// 頂点座標の設定
	vertexWk[BUTTLEGAUGE002][0].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01, BUTTLEGAUGE_INNERPOS_Y_01, 0.0f);
	vertexWk[BUTTLEGAUGE002][1].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01 + (TEXTURE_BUTTLEGAUGE_INNER_SIZE_X * val), BUTTLEGAUGE_INNERPOS_Y_01, 0.0f);
	vertexWk[BUTTLEGAUGE002][2].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01, BUTTLEGAUGE_INNERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
	vertexWk[BUTTLEGAUGE002][3].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01 + (TEXTURE_BUTTLEGAUGE_INNER_SIZE_X * val), BUTTLEGAUGE_INNERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
}

//=============================================================================
// テクスチャ座標の設定
// 引　数：float val(テクスチャのX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetTextureButtleGauge02(float val)
{
	vertexWk[BUTTLEGAUGE003][0].tex = D3DXVECTOR2((float)(val), 0.0f);
	vertexWk[BUTTLEGAUGE003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[BUTTLEGAUGE003][2].tex = D3DXVECTOR2((float)(val), 1.0f);
	vertexWk[BUTTLEGAUGE003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点座標の設定
// 引　数：faloat val(頂点のX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetVertexButtleGauge02(float val)
{
	// 頂点座標の設定
	vertexWk[BUTTLEGAUGE003][0].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + (TEXTURE_BUTTLEGAUGE_INNER_SIZE_X * val), BUTTLEGAUGE_INNERPOS_Y_02, 0.0f);
	vertexWk[BUTTLEGAUGE003][1].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X, BUTTLEGAUGE_INNERPOS_Y_02, 0.0f);
	vertexWk[BUTTLEGAUGE003][2].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + (TEXTURE_BUTTLEGAUGE_INNER_SIZE_X * val), BUTTLEGAUGE_INNERPOS_Y_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
	vertexWk[BUTTLEGAUGE003][3].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X, BUTTLEGAUGE_INNERPOS_Y_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
}

