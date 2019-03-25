//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "field.h"
#include "player.h"
#include "title.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_TUTORIAL1	("data/TEXTURE/tutorial.png")		// 読み込む画像
#define TEXTURE_SIZE_X01	(1090)  // テクスチャサイズ
#define TEXTURE_SIZE_Y01	(800)	// 
#define POS_X01				(400)	// 初期X座標 
#define POS_Y01				(150)	// 初期Y座標

#define TEXTURE_SIZE_X02	(200)	// テクスチャサイズ
#define TEXTURE_SIZE_Y02	(100)	// 
#define POS_X02				(790)	// 初期X座標 
#define POS_Y02				(960)	// 初期Y座標

#define TEXTURE_SIZE_X03	(75)	// テクスチャサイズ
#define TEXTURE_SIZE_Y03	(75)	// 
#define POS_X03				(1010)	// 初期X座標 
#define POS_Y03				(970)	// 初期Y座標

#define TEXTURE_MAX			(3)		// テクスチャの最大数 

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTutorial(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureTutorial[TEXTURE_MAX];	// テクスチャへのポインタ
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];	// 頂点情報格納ワーク

static int					TextureNum;							// テクスチャ番号

//=============================================================================
// 初期化処理
// 引　数：int type(再初期化時の2数判定変数)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitTutorial(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_TUTORIAL1,						// ファイルの名前
			&D3DTextureTutorial[TEX_NUM001]);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_ENTHER,							// ファイルの名前
			&D3DTextureTutorial[TEX_NUM002]);		// 読み込むメモリー
		
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BUTTON,						// ファイルの名前
			&D3DTextureTutorial[TEX_NUM003]);		// 読み込むメモリー
	}

	// テクスチャ番号の初期化
	TextureNum = TEX_NUM001;

	// 頂点情報の作成
	MakeVertexTutorial();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
{
	if (D3DTextureTutorial[TextureNum] != NULL)
	{	// テクスチャの開放
		D3DTextureTutorial[TextureNum]->Release();
		D3DTextureTutorial[TextureNum] = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTutorial(void)
{
	// ステージ遷移
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		StopSound(*GetTitleSound());
		SetStage(STARTCALL);
	}
	else if (IsButtonTriggered(P1, BUTTON_A))
	{
		StopSound(*GetTitleSound());
		SetStage(STARTCALL);
	}
	else if (IsButtonPressed(P2, BUTTON_B))
	{
		StopSound(*GetTitleSound());
		SetStage(STARTCALL);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureTutorial[TEX_NUM001]);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk[TEX_NUM001], sizeof(VERTEX_2D));
	}
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureTutorial[TEX_NUM002]);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk[TEX_NUM002], sizeof(VERTEX_2D));
	}
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureTutorial[TEX_NUM003]);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk[TEX_NUM003], sizeof(VERTEX_2D));
	}

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	{
		// 頂点座標の設定
		vertexWk[TEX_NUM001][0].vtx = D3DXVECTOR3(POS_X01, POS_Y01, 0.0f);
		vertexWk[TEX_NUM001][1].vtx = D3DXVECTOR3(POS_X01 + TEXTURE_SIZE_X01, POS_Y01, 0.0f);
		vertexWk[TEX_NUM001][2].vtx = D3DXVECTOR3(POS_X01, POS_Y01 + TEXTURE_SIZE_Y01, 0.0f);
		vertexWk[TEX_NUM001][3].vtx = D3DXVECTOR3(POS_X01 + TEXTURE_SIZE_X01, POS_Y01 + TEXTURE_SIZE_Y01, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vertexWk[TEX_NUM001][TEX_NUM001].rhw =
		vertexWk[TEX_NUM001][1].rhw =
		vertexWk[TEX_NUM001][2].rhw =
		vertexWk[TEX_NUM001][3].rhw = 1.0f;

		// 反射光の設定
		vertexWk[TEX_NUM001][TEX_NUM001].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vertexWk[TEX_NUM001][TEX_NUM001].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TEX_NUM001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TEX_NUM001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TEX_NUM001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	{
		// 頂点座標の設定
		vertexWk[TEX_NUM002][0].vtx = D3DXVECTOR3(POS_X02, POS_Y02, 0.0f);
		vertexWk[TEX_NUM002][1].vtx = D3DXVECTOR3(POS_X02 + TEXTURE_SIZE_X02, POS_Y02, 0.0f);
		vertexWk[TEX_NUM002][2].vtx = D3DXVECTOR3(POS_X02, POS_Y02 + TEXTURE_SIZE_Y02, 0.0f);
		vertexWk[TEX_NUM002][3].vtx = D3DXVECTOR3(POS_X02 + TEXTURE_SIZE_X02, POS_Y02 + TEXTURE_SIZE_Y02, 0.0f);

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
	{
		// 頂点座標の設定
		vertexWk[TEX_NUM003][0].vtx = D3DXVECTOR3(POS_X03, POS_Y03, 0.0f);
		vertexWk[TEX_NUM003][1].vtx = D3DXVECTOR3(POS_X03 + TEXTURE_SIZE_X03, POS_Y03, 0.0f);
		vertexWk[TEX_NUM003][2].vtx = D3DXVECTOR3(POS_X03, POS_Y03 + TEXTURE_SIZE_Y03, 0.0f);
		vertexWk[TEX_NUM003][3].vtx = D3DXVECTOR3(POS_X03 + TEXTURE_SIZE_X03, POS_Y03 + TEXTURE_SIZE_Y03, 0.0f);

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

	return S_OK;
}
