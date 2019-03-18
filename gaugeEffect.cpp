//=============================================================================
//
// ゲージエフェクト処理 [gaugeEffect.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "gaugeEffect.h"
#include "player.h"
#include "ball.h"

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
void SetVertexGaugeEffect01(float val);
void SetVertexGaugeEffect02(float val);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
enum GAUGEEFFECT{
	GAUGE_EFFECT01,
	GAUGE_EFFECT02,
	GAUGE_EFFECT_MAX
};

LPDIRECT3DTEXTURE9			D3DTextureGaugeEffect[TEXTURE_MAX];			// テクスチャへのポインタ
static VERTEX_2D			VertexWk[TEXTURE_MAX][NUM_VERTEX];	// 頂点情報格納ワーク
static int					PatternAnim[GAUGEEFFECT_MAX];
static int					CntAnim[GAUGEEFFECT_MAX];
static bool					UseAnim[GAUGEEFFECT_MAX];
//=============================================================================
// 初期化処理
// 引　数：int type(再初期化時の2数判定変数)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitGaugeEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 再初期化時は読み込まない
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
	float val = 0.0f;
	BALL *ball = GetBall(0);

	for (int i = 0; i < GAUGEEFFECT_MAX; i++, ball++)
	{
		CntAnim[i]++;		// アニメーションカウントの更新 

		// アニメーションWaitチェック
		if ((CntAnim[i] % ANIM_TIME) == 0)
		{
			// パターンの切り替え
			PatternAnim[i] = (PatternAnim[i] + 1) % ANIM_PATTERN_NUM;
			CntAnim[i] = 0;
		}

		if (i == 0)
		{
			val = (float)ball->cnt / BALL_ONESET_MAX;

			SetTextureGaugeEffect(i, PatternAnim[i]);	// テクスチャー座標の計算
			SetVertexGaugeEffect01(val);
		}
		else if(i == 1)
		{
			float cnt = BALL_ONESET_MAX - ball->cnt;
			val = cnt / BALL_ONESET_MAX;

			SetTextureGaugeEffect(i, PatternAnim[i]);	// テクスチャー座標の計算
			SetVertexGaugeEffect02(val);
		}
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
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, VertexWk[GAUGE_EFFECT01 + i], sizeof(VERTEX_2D));
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
	
	// P1ゲージ用エフェクト
	{
		// 頂点座標の設定
		VertexWk[GAUGE_EFFECT01][0].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01, GAUGEEFFECT_POS_Y_01, 0.0f);
		VertexWk[GAUGE_EFFECT01][1].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_01, 0.0f);
		VertexWk[GAUGE_EFFECT01][2].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01, GAUGEEFFECT_POS_Y_01 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
		VertexWk[GAUGE_EFFECT01][3].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_01 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		VertexWk[GAUGE_EFFECT01][0].rhw =
			VertexWk[GAUGE_EFFECT01][1].rhw =
			VertexWk[GAUGE_EFFECT01][2].rhw =
			VertexWk[GAUGE_EFFECT01][3].rhw = 1.0f;

		// 反射光の設定
		VertexWk[GAUGE_EFFECT01][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[GAUGE_EFFECT01][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[GAUGE_EFFECT01][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[GAUGE_EFFECT01][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		// テクスチャ座標の設定
		VertexWk[GAUGE_EFFECT01][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[GAUGE_EFFECT01][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[GAUGE_EFFECT01][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[GAUGE_EFFECT01][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// P2ゲージ用エフェクト
	{
		// 頂点座標の設定
		VertexWk[GAUGE_EFFECT02][0].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02, GAUGEEFFECT_POS_Y_02, 0.0f);
		VertexWk[GAUGE_EFFECT02][1].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_02, 0.0f);
		VertexWk[GAUGE_EFFECT02][2].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02, GAUGEEFFECT_POS_Y_02 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
		VertexWk[GAUGE_EFFECT02][3].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_02 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		VertexWk[GAUGE_EFFECT02][0].rhw =
			VertexWk[GAUGE_EFFECT02][1].rhw =
			VertexWk[GAUGE_EFFECT02][2].rhw =
			VertexWk[GAUGE_EFFECT02][3].rhw = 1.0f;

		// 反射光の設定
		VertexWk[GAUGE_EFFECT02][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[GAUGE_EFFECT02][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[GAUGE_EFFECT02][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[GAUGE_EFFECT02][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		VertexWk[GAUGE_EFFECT02][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[GAUGE_EFFECT02][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[GAUGE_EFFECT02][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[GAUGE_EFFECT02][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
// 引　数：int index(頂点情報のアドレス), int cntPattern(アニメーションのパターンカウント)
// 戻り値：な　し
//=============================================================================
void SetTextureGaugeEffect(int index, int cntPattern)
{
	// アニメーションのテクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_X;
	int y = cntPattern / TEXTURE_PATTERN_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_Y;

	VertexWk[index][0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	VertexWk[index][1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	VertexWk[index][2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	VertexWk[index][3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
// 引　数：faloat val(頂点のX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetVertexGaugeEffect01(float val)
{
	// 頂点座標の設定
	VertexWk[GAUGE_EFFECT01][0].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + (680.0f * val), GAUGEEFFECT_POS_Y_01, 0.0f);
	VertexWk[GAUGE_EFFECT01][1].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + TEXTURE_GAUGEEFFECT_SIZE_X + (680.0f * val), GAUGEEFFECT_POS_Y_01, 0.0f);
	VertexWk[GAUGE_EFFECT01][2].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + (680.0f * val), GAUGEEFFECT_POS_Y_01 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
	VertexWk[GAUGE_EFFECT01][3].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + TEXTURE_GAUGEEFFECT_SIZE_X + (680.0f * val), GAUGEEFFECT_POS_Y_01 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
}

//=============================================================================
// 頂点座標の設定
// 引　数：faloat val(頂点のX軸の変動率)
// 戻り値：な　し
//=============================================================================
void SetVertexGaugeEffect02(float val)
{
	// 頂点座標の設定
	VertexWk[GAUGE_EFFECT02][0].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 - TEXTURE_GAUGEEFFECT_SIZE_X + (680.0f * val), GAUGEEFFECT_POS_Y_02, 0.0f);
	VertexWk[GAUGE_EFFECT02][1].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 + (680.0f * val), GAUGEEFFECT_POS_Y_02, 0.0f);
	VertexWk[GAUGE_EFFECT02][2].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 - TEXTURE_GAUGEEFFECT_SIZE_X + (680.0f * val), GAUGEEFFECT_POS_Y_02 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
	VertexWk[GAUGE_EFFECT02][3].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 + (680.0f * val), GAUGEEFFECT_POS_Y_02 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
}
