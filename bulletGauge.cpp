//=============================================================================
//
// チュートリアル処理 [BulletGauge.cpp]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "bulletGauge.h"
#include "input.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BULLETGAUGE1	("data/TEXTURE/bullet_gauge.png")		// ゲージの外枠
#define TEXTURE_BULLETGAUGE2	("data/TEXTURE/bullet_gauge_01.png")	// ゲージ緑色
#define TEXTURE_BULLETGAUGE3	("data/TEXTURE/bullet_gauge_02.png")	// ゲージ黄色
#define TEXTURE_BULLETGAUGE4	("data/TEXTURE/bullet_gauge_03.png")	// ゲージ赤色
#define BULLETGAUGE_MAX			(2)										// ゲージの数
#define TEXTURE_MAX				(4)
#define BULLETGAUGE_RED			(70.0f)
#define BULLETGAUGE_YELLOW		(30.0f)


enum {
	BULLETGAUGE001,
	BULLETGAUGE002,
	BULLETGAUGE003,
	BULLETGAUGE004
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBulletGauge(void);
void SetTextureBulletGauge(int index, float val);
void SetVertexBulletGauge(int index, float val);
void SettBulletGaugeTextureType(int index, float val);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureBulletGauge[TEXTURE_MAX];		// テクスチャへのポインタ
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];	// 頂点情報格納ワーク

int							TexNumBulletGauge[BULLETGAUGE_MAX];		// 
bool						BulletGaugeUse[BULLETGAUGE_MAX];		// 
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBulletGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BULLETGAUGE1,						// ファイルの名前
			&D3DTextureBulletGauge[BULLETGAUGE001]);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BULLETGAUGE2,						// ファイルの名前
			&D3DTextureBulletGauge[BULLETGAUGE002]);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BULLETGAUGE3,						// ファイルの名前
			&D3DTextureBulletGauge[BULLETGAUGE003]);		// 読み込むメモリー
		
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BULLETGAUGE4,						// ファイルの名前
			&D3DTextureBulletGauge[BULLETGAUGE004]);		// 読み込むメモリー

	}

	for (int i = 0; i < BULLETGAUGE_MAX; i++)
	{
		TexNumBulletGauge[i] = BULLETGAUGE004;
		BulletGaugeUse[i] = true;		// 外枠の使用状態を初期化
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
          		float val = bullet[i].sclIncrease.x / 2.0f;
		SettBulletGaugeTextureType(i, bullet[i].sclIncrease.x);
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
				pDevice->SetTexture(0, D3DTextureBulletGauge[BULLETGAUGE001]);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[BULLETGAUGE001 + i], sizeof(VERTEX_2D));
			}

			// 中身
			{	// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureBulletGauge[TexNumBulletGauge[i]]);

				// ポリゴンの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[BULLETGAUGE003 + i], sizeof(VERTEX_2D));
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
		vertexWk[BULLETGAUGE001][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[BULLETGAUGE001][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[BULLETGAUGE001][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[BULLETGAUGE001][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[BULLETGAUGE001][0].rhw =
			vertexWk[BULLETGAUGE001][1].rhw =
			vertexWk[BULLETGAUGE001][2].rhw =
			vertexWk[BULLETGAUGE001][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[BULLETGAUGE001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[BULLETGAUGE001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[BULLETGAUGE001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[BULLETGAUGE001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[BULLETGAUGE001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 外枠
	{
		// 頂点座標の設定
		vertexWk[BULLETGAUGE002][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[BULLETGAUGE002][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[BULLETGAUGE002][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[BULLETGAUGE002][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[BULLETGAUGE002][0].rhw =
			vertexWk[BULLETGAUGE002][1].rhw =
			vertexWk[BULLETGAUGE002][2].rhw =
			vertexWk[BULLETGAUGE002][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[BULLETGAUGE002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[BULLETGAUGE002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[BULLETGAUGE002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[BULLETGAUGE002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[BULLETGAUGE002][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 中身
	{
		// 頂点座標の設定
		vertexWk[BULLETGAUGE003][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[BULLETGAUGE003][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[BULLETGAUGE003][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[BULLETGAUGE003][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[BULLETGAUGE003][0].rhw =
			vertexWk[BULLETGAUGE003][1].rhw =
			vertexWk[BULLETGAUGE003][2].rhw =
			vertexWk[BULLETGAUGE003][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[BULLETGAUGE003][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE003][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE003][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE003][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[BULLETGAUGE003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[BULLETGAUGE003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[BULLETGAUGE003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[BULLETGAUGE003][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// 中身
	{
		// 頂点座標の設定
		vertexWk[BULLETGAUGE004][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[BULLETGAUGE004][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[BULLETGAUGE004][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[BULLETGAUGE004][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[BULLETGAUGE004][0].rhw =
			vertexWk[BULLETGAUGE004][1].rhw =
			vertexWk[BULLETGAUGE004][2].rhw =
			vertexWk[BULLETGAUGE004][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[BULLETGAUGE004][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE004][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE004][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BULLETGAUGE004][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[BULLETGAUGE004][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[BULLETGAUGE004][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[BULLETGAUGE004][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[BULLETGAUGE004][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureBulletGauge(int index, float val)
{
	vertexWk[BULLETGAUGE003 + index][0].tex = D3DXVECTOR2(0.0f, 1.0f - (1.0f * val));
	vertexWk[BULLETGAUGE003 + index][1].tex = D3DXVECTOR2(1.0f, 1.0f - (1.0f * val));
	vertexWk[BULLETGAUGE003 + index][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[BULLETGAUGE003 + index][3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetVertexBulletGauge(int index, float val)
{
	if (index == 0)
	{
		vertexWk[BULLETGAUGE003][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, (BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[BULLETGAUGE003][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, (BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[BULLETGAUGE003][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[BULLETGAUGE003][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
	}
	else if (index == 1)
	{
		vertexWk[BULLETGAUGE004][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, (BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[BULLETGAUGE004][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, (BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[BULLETGAUGE004][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[BULLETGAUGE004][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
	}
}

//=============================================================================
// テクスチャタイプの設定
// 引　数：int index(テクスチャタイプのアドレス番号), faloat val(バレットのチャージ量)
// 戻り値：な　し
//=============================================================================
void SettBulletGaugeTextureType(int index, float val)
{
	// 赤ゲージへ変化
	if (val > BULLETGAUGE_RED)
	{
		TexNumBulletGauge[index] = BULLETGAUGE004;

	}
	// 黄色ゲージへ変化
	else if (val > BULLETGAUGE_YELLOW)
	{
		TexNumBulletGauge[index] = BULLETGAUGE003;
	}
	// 緑ゲージ
	else 
	{
		TexNumBulletGauge[index] = BULLETGAUGE002;
	}
}