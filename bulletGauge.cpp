//=============================================================================
//
// バレットゲージ処理 [BulletGauge.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "bulletGauge.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BULLETGAUGE1	("data/TEXTURE/bullet_gauge.png")		// ゲージの外枠
#define TEXTURE_BULLETGAUGE2	("data/TEXTURE/bullet_gauge_01.png")	// ゲージ緑色
#define BULLETGAUGE_MAX			(2)										// ゲージの数
#define TEXTURE_MAX				(2)										// テクスチャの最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBulletGauge(void);
void SetTextureBulletGauge(int index, float val);
void SetVertexBulletGauge(int index, float val);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureBulletGauge[TEXTURE_MAX];		// テクスチャへのポインタ
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];		// 頂点情報格納ワーク

int							TexNumBulletGauge[BULLETGAUGE_MAX];		// テクスチャーの種類
bool						BulletGaugeUse[BULLETGAUGE_MAX];		// 使用状態
//===============================================================================
// 初期化処理
// 引　数：int type(再初期化の2数判定値)
// 戻り値：HRESULT型
//===============================================================================
HRESULT InitBulletGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BULLETGAUGE1,						// ファイルの名前
			&D3DTextureBulletGauge[TEX_NUM001]);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BULLETGAUGE2,						// ファイルの名前
			&D3DTextureBulletGauge[TEX_NUM002]);		// 読み込むメモリー

	}

	// 各変数の初期化
	for (int i = 0; i < BULLETGAUGE_MAX; i++)
	{
		TexNumBulletGauge[i] = TEX_NUM004;
		BulletGaugeUse[i] = true;
	}

	// 頂点情報の作成
	MakeVertexBulletGauge();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBulletGauge(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBulletGauge[i] != NULL)
		{	// テクスチャの開放
			D3DTextureBulletGauge[i]->Release();
			D3DTextureBulletGauge[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBulletGauge(void)
{
	BULLET *bullet = GetBullet(0);
	for (int i = 0; i < BULLETGAUGE_MAX; i++)
	{		
		// チャージ率をゲージへ変換
       	float val = bullet[i].speedIncrease / BULLET_CHARGE_MAX;
		SetTextureBulletGauge(i, val);
		SetVertexBulletGauge(i, val);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBulletGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < BULLETGAUGE_MAX; i++)
	{
		if (BulletGaugeUse[i])
		{
			// 外枠
			{	// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureBulletGauge[TEX_NUM001]);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[TEX_NUM001 + i], sizeof(VERTEX_2D));
			}

			// 中身
			{	// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureBulletGauge[TexNumBulletGauge[i]]);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[TEX_NUM003 + i], sizeof(VERTEX_2D));
			}
		}
	}

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBulletGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 外枠
	{
		// 頂点座標の設定
		vertexWk[TEX_NUM001][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[TEX_NUM001][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[TEX_NUM001][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM001][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[TEX_NUM001][0].rhw =
			vertexWk[TEX_NUM001][1].rhw =
			vertexWk[TEX_NUM001][2].rhw =
			vertexWk[TEX_NUM001][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[TEX_NUM001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[TEX_NUM001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TEX_NUM001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TEX_NUM001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TEX_NUM001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 外枠
	{
		// 頂点座標の設定
		vertexWk[TEX_NUM002][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[TEX_NUM002][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[TEX_NUM002][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM002][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[TEX_NUM002][0].rhw =
			vertexWk[TEX_NUM002][1].rhw =
			vertexWk[TEX_NUM002][2].rhw =
			vertexWk[TEX_NUM002][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[TEX_NUM002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[TEX_NUM002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TEX_NUM002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TEX_NUM002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TEX_NUM002][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 中身
	{
		// 頂点座標の設定
		vertexWk[TEX_NUM003][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[TEX_NUM003][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[TEX_NUM003][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM003][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[TEX_NUM003][0].rhw =
			vertexWk[TEX_NUM003][1].rhw =
			vertexWk[TEX_NUM003][2].rhw =
			vertexWk[TEX_NUM003][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[TEX_NUM003][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM003][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM003][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM003][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[TEX_NUM003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TEX_NUM003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TEX_NUM003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TEX_NUM003][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// 中身
	{
		// 頂点座標の設定
		vertexWk[TEX_NUM004][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[TEX_NUM004][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[TEX_NUM004][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM004][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[TEX_NUM004][0].rhw =
			vertexWk[TEX_NUM004][1].rhw =
			vertexWk[TEX_NUM004][2].rhw =
			vertexWk[TEX_NUM004][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[TEX_NUM004][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM004][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM004][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM004][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[TEX_NUM004][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TEX_NUM004][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TEX_NUM004][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TEX_NUM004][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
// 引　数：int index(ゲージのアドレス), float val(ゲージの変動率)
// 戻り値：な　し
//=============================================================================
void SetTextureBulletGauge(int index, float val)
{
	vertexWk[TEX_NUM003 + index][0].tex = D3DXVECTOR2(0.0f, 1.0f - (1.0f * val));
	vertexWk[TEX_NUM003 + index][1].tex = D3DXVECTOR2(1.0f, 1.0f - (1.0f * val));
	vertexWk[TEX_NUM003 + index][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[TEX_NUM003 + index][3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点情報の設定
// 引　数：int index(ゲージのアドレス), float val(ゲージの変動率)
// 戻り値：な　し
//=============================================================================
void SetVertexBulletGauge(int index, float val)
{
	if (index == 0)
	{
		vertexWk[TEX_NUM003][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, (BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[TEX_NUM003][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, (BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[TEX_NUM003][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM003][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
	}
	else if (index == 1)
	{
		vertexWk[TEX_NUM004][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, (BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[TEX_NUM004][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, (BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[TEX_NUM004][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM004][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
	}
}
