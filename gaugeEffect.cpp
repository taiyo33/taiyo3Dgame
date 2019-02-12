//=============================================================================
//
// ゲージエフェクト処理 [gaugeEffect.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "gaugeEffect.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "debugproc.h"
#include "player.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_GAUGEEFFECT01		"data/TEXTURE/gague_effect01.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_GAUGEEFFECT02		"data/TEXTURE/gague_effect02.png"	// 読み込むテクスチャファイル名

#define GAUGEEFFECT_MAX			(2)									// 最大数
#define TEXTURE_PATTERN_X		(5)									// テクスチャー分割数X方向
#define TEXTURE_PATTERN_Y		(1)									// テクスチャー分割数Y方向
#define ANIM_TIME				(8)									// アニメーションの間隔
#define ANIM_PATTERN_NUM		(TEXTURE_PATTERN_X * TEXTURE_PATTERN_Y)	// 
#define TEXTURE_MAX				(2)									// テクスチャーの最大数	

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGaugeEffect(void);
void SetTextureGaugeEffect(int index, int cntPattern);
void TextureAnimGaugeEffect(int index);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
enum {
	GAUGE_EFFECT01,
	GAUGE_EFFECT02
};

LPDIRECT3DTEXTURE9			D3DTextureGaugeEffect[TEXTURE_MAX];			// テクスチャへのポインタ
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];	// 頂点情報格納ワーク
static int			PatternAnim[GAUGEEFFECT_MAX];
static int			CntAnim[GAUGEEFFECT_MAX];
static bool			UseAnim[GAUGEEFFECT_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGaugeEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_GAUGEEFFECT01,					// ファイルの名前
			&D3DTextureGaugeEffect[GAUGE_EFFECT01]);			// 読み込むメモリー
		
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_GAUGEEFFECT02,					// ファイルの名前
			&D3DTextureGaugeEffect[GAUGE_EFFECT02]);			// 読み込むメモリー

	}

	// 頂点情報の作成
	MakeVertexGaugeEffect();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGaugeEffect(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureGaugeEffect != NULL)
		{// テクスチャの開放
			D3DTextureGaugeEffect[i]->Release();
			D3DTextureGaugeEffect[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGaugeEffect(void)
{
	for (int i = 0; i < GAUGEEFFECT_MAX; i++)
	{
		CntAnim[i]++;		// アニメーションカウントの更新 

		// アニメーションWaitチェック
		if ((CntAnim[i] % ANIM_TIME) == 0)
		{
			// パターンの切り替え
			PatternAnim[i] = (PatternAnim[i] + 1) % ANIM_PATTERN_NUM;
		}

		SetTextureGaugeEffect(i, PatternAnim[i]);	// テクスチャー座標の計算
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGaugeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < GAUGEEFFECT_MAX; i++)
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureGaugeEffect[GAUGE_EFFECT01 + i]);
		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[GAUGE_EFFECT01 + i], sizeof(VERTEX_2D));
	}

	// αテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGaugeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	vertexWk[GAUGE_EFFECT01][0].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01, GAUGEEFFECT_POS_Y_01, 0.0f);
	vertexWk[GAUGE_EFFECT01][1].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_01, 0.0f);
	vertexWk[GAUGE_EFFECT01][2].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01, GAUGEEFFECT_POS_Y_01 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
	vertexWk[GAUGE_EFFECT01][3].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_01 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	vertexWk[GAUGE_EFFECT01][0].rhw =
		vertexWk[GAUGE_EFFECT01][1].rhw =
		vertexWk[GAUGE_EFFECT01][2].rhw =
		vertexWk[GAUGE_EFFECT01][3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[GAUGE_EFFECT01][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT01][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT01][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT01][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWk[GAUGE_EFFECT01][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[GAUGE_EFFECT01][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[GAUGE_EFFECT01][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[GAUGE_EFFECT01][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	vertexWk[GAUGE_EFFECT02][0].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02, GAUGEEFFECT_POS_Y_02, 0.0f);
	vertexWk[GAUGE_EFFECT02][1].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_02, 0.0f);
	vertexWk[GAUGE_EFFECT02][2].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02, GAUGEEFFECT_POS_Y_02 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
	vertexWk[GAUGE_EFFECT02][3].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_02 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	vertexWk[GAUGE_EFFECT02][0].rhw =
		vertexWk[GAUGE_EFFECT02][1].rhw =
		vertexWk[GAUGE_EFFECT02][2].rhw =
		vertexWk[GAUGE_EFFECT02][3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[GAUGE_EFFECT02][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT02][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT02][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT02][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWk[GAUGE_EFFECT02][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[GAUGE_EFFECT02][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[GAUGE_EFFECT02][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[GAUGE_EFFECT02][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}


//=================================================================================
// テクスチャーのアニメーション処理
// 引　数：int index(爆発エフェクトのアドレス番号)
// 戻り値：な　し
//==================================================================================
void TextureAnimGaugeEffect(int index)
{

	CntAnim[index]++;		// アニメーションカウントの更新 

	// アニメーションWaitチェック
	if ((CntAnim[index] % ANIM_TIME) == 0)
	{
		// パターンの切り替え
		PatternAnim[index] = (PatternAnim[index] + 1) % ANIM_PATTERN_NUM;
	}
	//// アニメーションの停止
	//if (PatternAnim[index] == ANIM_STOP)
	//{
	//	UseAnim[index] = false;
	//	PatternAnim[index] = 0;
	//}

	SetTextureGaugeEffect(index,PatternAnim[index]);	// テクスチャー座標の計算
}

//=============================================================================
// テクスチャ座標の設定
// 引数：アニメーションのパターンカウント
//=============================================================================
void SetTextureGaugeEffect(int index, int cntPattern)
{
	// アニメーションするテクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_X;
	int y = cntPattern / TEXTURE_PATTERN_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_Y;

	vertexWk[index][0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[index][1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[index][2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[index][3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}
