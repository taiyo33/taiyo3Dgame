//=============================================================================
//
// ボールゲージ処理 [ballGauge.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "ballGauge.h"
#include "player.h"
#include "ball.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BALLGAUGE1	("data/TEXTURE/ball_gague.png")		// 読み込む画像
#define TEXTURE_BALLGAUGE2	("data/TEXTURE/ball_gugae01.png")	// 読み込む画像
#define TEXTURE_BALLGAUGE3	("data/TEXTURE/ball_gugae02.png")	// 読み込む画像
#define TEXTURE_MAX			(3)									// テクスチャーの最大数
#define BALLGAUGE_MAX		(2)									// 内ゲージの最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBallGauge(void);
void SetVertexBallGauge01(float val);
void SetTextureBallGauge01(float val);
void SetVertexBallGauge02(float val);
void SetTextureBallGauge02(float val);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureBallGauge[TEXTURE_MAX];	// テクスチャへのポインタ
static VERTEX_2D			VertexWk[TEXTURE_MAX][NUM_VERTEX];	// 頂点情報格納ワーク

bool						BallGaugeUse[BALLGAUGE_MAX];		// 使用状態 
//===============================================================================
// 初期化処理
// 引　数：int type(再初期化の2数判定値)
// 戻り値：HRESULT型
//===============================================================================
HRESULT InitBallGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BALLGAUGE1,						// ファイルの名前
			&D3DTextureBallGauge[TEX_NUM001]);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BALLGAUGE2,						// ファイルの名前
			&D3DTextureBallGauge[TEX_NUM002]);		// 読み込むメモリー
		
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BALLGAUGE3,						// ファイルの名前
			&D3DTextureBallGauge[TEX_NUM003]);		// 読み込むメモリー

	}

	// 変数の初期化
	for (int i = 0; i < BALLGAUGE_MAX; i++)
	{
		BallGaugeUse[i] = true;
	}

	// 頂点情報の作成
	MakeVertexBallGauge();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBallGauge(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBallGauge[i] != NULL)
		{	// テクスチャの開放
			D3DTextureBallGauge[i]->Release();
			D3DTextureBallGauge[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBallGauge(void)
{
	float val;
	BALL *ball = GetBall(0);

	for (int i = 0; i < BALLGAUGE_MAX; i++, ball++)
	{
		
		// P１のゲージ処理
		if (i == 0)
		{
			// ボールの所持数をゲージへ変換
			val = (float)ball->cnt / BALL_ONESET_MAX; 
			SetTextureBallGauge01(val);
			SetVertexBallGauge01(val);
		}
		// P2のゲージ処理
		else if (i == 1)
		{
			// ボールの所持数をゲージへ変換
			float cnt = BALL_ONESET_MAX - ball->cnt;
			val = cnt / BALL_ONESET_MAX;
			SetTextureBallGauge02(val);
			SetVertexBallGauge02(val);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBallGauge(void)
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
		pDevice->SetTexture(0, D3DTextureBallGauge[TEX_NUM001]);
		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWk[TEX_NUM001], sizeof(VERTEX_2D));
	}

	for (int i = 0; i < BALLGAUGE_MAX; i++)
	{
		{
			// 中身
			{	// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureBallGauge[TEX_NUM002 + i]);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWk[TEX_NUM002 + i], sizeof(VERTEX_2D));
			}
			
		}
	}

	// αテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBallGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 外枠
	{
		// 頂点座標の設定
		VertexWk[TEX_NUM001][0].vtx = D3DXVECTOR3(BALLGAUGE_OUTERPOS_X_01, BALLGAUGE_OUTERPOS_Y_01, 0.0f);
		VertexWk[TEX_NUM001][1].vtx = D3DXVECTOR3(BALLGAUGE_OUTERPOS_X_01 + TEXTURE_BALLGAUGE_OUTER_SIZE_X, BALLGAUGE_OUTERPOS_Y_01, 0.0f);
		VertexWk[TEX_NUM001][2].vtx = D3DXVECTOR3(BALLGAUGE_OUTERPOS_X_01, BALLGAUGE_OUTERPOS_Y_01 + TEXTURE_BALLGAUGE_OUTER_SIZE_Y, 0.0f);
		VertexWk[TEX_NUM001][3].vtx = D3DXVECTOR3(BALLGAUGE_OUTERPOS_X_01 + TEXTURE_BALLGAUGE_OUTER_SIZE_X, BALLGAUGE_OUTERPOS_Y_01 + TEXTURE_BALLGAUGE_OUTER_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		VertexWk[TEX_NUM001][0].rhw =
		VertexWk[TEX_NUM001][1].rhw =
		VertexWk[TEX_NUM001][2].rhw =
		VertexWk[TEX_NUM001][3].rhw = 1.0f;

		// 反射光の設定
		VertexWk[TEX_NUM001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWk[TEX_NUM001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[TEX_NUM001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[TEX_NUM001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[TEX_NUM001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 中身
	{
		// 頂点座標の設定
		VertexWk[TEX_NUM002][0].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01, BALLGAUGE_INNERPOS_Y_01, 0.0f);
		VertexWk[TEX_NUM002][1].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01 + TEXTURE_BALLGAUGE_INNER_SIZE_X, BALLGAUGE_INNERPOS_Y_01, 0.0f);
		VertexWk[TEX_NUM002][2].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01, BALLGAUGE_INNERPOS_Y_01 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
		VertexWk[TEX_NUM002][3].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01 + TEXTURE_BALLGAUGE_INNER_SIZE_X, BALLGAUGE_INNERPOS_Y_01 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		VertexWk[TEX_NUM002][0].rhw =
		VertexWk[TEX_NUM002][1].rhw =
		VertexWk[TEX_NUM002][2].rhw =
		VertexWk[TEX_NUM002][3].rhw = 1.0f;

		// 反射光の設定
		VertexWk[TEX_NUM002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWk[TEX_NUM002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[TEX_NUM002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[TEX_NUM002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[TEX_NUM002][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 中身
	{
		// 頂点座標の設定
		VertexWk[TEX_NUM003][0].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02,BALLGAUGE_INNERPOS_Y_02, 0.0f);
		VertexWk[TEX_NUM003][1].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + TEXTURE_BALLGAUGE_INNER_SIZE_X,BALLGAUGE_INNERPOS_Y_02, 0.0f);
		VertexWk[TEX_NUM003][2].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02,BALLGAUGE_INNERPOS_Y_02 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
		VertexWk[TEX_NUM003][3].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + TEXTURE_BALLGAUGE_INNER_SIZE_X,BALLGAUGE_INNERPOS_Y_02 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		VertexWk[TEX_NUM003][0].rhw =
		VertexWk[TEX_NUM003][1].rhw =
		VertexWk[TEX_NUM003][2].rhw =
		VertexWk[TEX_NUM003][3].rhw = 1.0f;

		// 反射光の設定
		VertexWk[TEX_NUM003][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM003][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM003][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM003][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWk[TEX_NUM003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[TEX_NUM003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[TEX_NUM003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[TEX_NUM003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}


	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
// 引　数：float val(テクスチャのX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetTextureBallGauge01(float val)
{
	VertexWk[TEX_NUM002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWk[TEX_NUM002][1].tex = D3DXVECTOR2(1.0f * (float)(val), 0.0f);
	VertexWk[TEX_NUM002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWk[TEX_NUM002][3].tex = D3DXVECTOR2(1.0f * (float)(val), 1.0f);
}

//=============================================================================
// 頂点座標の設定
// 引　数：faloat val(頂点のX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetVertexBallGauge01(float val)
{
	// 頂点座標の設定
	VertexWk[TEX_NUM002][0].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01, BALLGAUGE_INNERPOS_Y_01, 0.0f);
	VertexWk[TEX_NUM002][1].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01 + (TEXTURE_BALLGAUGE_INNER_SIZE_X * val), BALLGAUGE_INNERPOS_Y_01, 0.0f);
	VertexWk[TEX_NUM002][2].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01, BALLGAUGE_INNERPOS_Y_01 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
	VertexWk[TEX_NUM002][3].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01 + (TEXTURE_BALLGAUGE_INNER_SIZE_X * val), BALLGAUGE_INNERPOS_Y_01 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
}

//=============================================================================
// テクスチャ座標の設定
// 引　数：float val(テクスチャのX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetTextureBallGauge02(float val)
{
	VertexWk[TEX_NUM003][0].tex = D3DXVECTOR2((float)(val), 0.0f);
	VertexWk[TEX_NUM003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWk[TEX_NUM003][2].tex = D3DXVECTOR2((float)(val), 1.0f);
	VertexWk[TEX_NUM003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点座標の設定
// 引　数：faloat val(頂点のX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetVertexBallGauge02(float val)
{
	// 頂点座標の設定
	VertexWk[TEX_NUM003][0].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + (TEXTURE_BALLGAUGE_INNER_SIZE_X * val), BALLGAUGE_INNERPOS_Y_02, 0.0f);
	VertexWk[TEX_NUM003][1].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + TEXTURE_BALLGAUGE_INNER_SIZE_X, BALLGAUGE_INNERPOS_Y_02, 0.0f);
	VertexWk[TEX_NUM003][2].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + (TEXTURE_BALLGAUGE_INNER_SIZE_X * val), BALLGAUGE_INNERPOS_Y_02 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
	VertexWk[TEX_NUM003][3].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + TEXTURE_BALLGAUGE_INNER_SIZE_X, BALLGAUGE_INNERPOS_Y_02 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
}

