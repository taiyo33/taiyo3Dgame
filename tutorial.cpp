//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "field.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_TUTORIAL1	("data/TEXTURE/tutorial02.png")		// 歩行用画像
#define TEXTURE_TUTORIAL2	("data/TEXTURE/space.png")		// 歩行用画像
#define TEXTURE_TUTORIAL3	("data/TEXTURE/tutorial3.png")		// 歩行用画像

enum {
	TUTORIAL001,
	TUTORIAL002,
	TUTORIAL003
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTutorial(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureTutorial[TEXTURE_MAX];	// テクスチャへのポインタ
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];				// 頂点情報格納ワーク

static int					TextureNum;							// 

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTutorial(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_TUTORIAL1,						// ファイルの名前
			&D3DTextureTutorial[TUTORIAL001]);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_TUTORIAL2,						// ファイルの名前
			&D3DTextureTutorial[TUTORIAL002]);		// 読み込むメモリー
		
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_TUTORIAL3,						// ファイルの名前
			&D3DTextureTutorial[TUTORIAL003]);		// 読み込むメモリー

	}

	TextureNum = TUTORIAL001;

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
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetStage(START);
	}
	else if (IsButtonTriggered(P1, BUTTON_A))
	{
		SetStage(START);
	}
	else if (IsButtonPressed(P2, BUTTON_B))
	{
		SetStage(START);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture( 0, D3DTextureTutorial[TUTORIAL001] );

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TUTORIAL, vertexWk[0], sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, D3DTextureTutorial[TUTORIAL002]);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TUTORIAL, vertexWk[1], sizeof(VERTEX_2D));


}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	vertexWk[0][0].vtx = D3DXVECTOR3(TUTORIAL_POS_X01, TUTORIAL_POS_Y01, 0.0f);
	vertexWk[0][1].vtx = D3DXVECTOR3(TUTORIAL_POS_X01 + TEXTURE_TUTORIAL_SIZE_X01, TUTORIAL_POS_Y01, 0.0f);
	vertexWk[0][2].vtx = D3DXVECTOR3(TUTORIAL_POS_X01, TUTORIAL_POS_Y01 + TEXTURE_TUTORIAL_SIZE_Y01, 0.0f);
	vertexWk[0][3].vtx = D3DXVECTOR3(TUTORIAL_POS_X01 + TEXTURE_TUTORIAL_SIZE_X01, TUTORIAL_POS_Y01 + TEXTURE_TUTORIAL_SIZE_Y01, 0.0f);

	// テクスチャのパースペクティブコレクト用
	vertexWk[0][0].rhw =
	vertexWk[0][1].rhw =
	vertexWk[0][2].rhw =
	vertexWk[0][3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[0][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[0][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[0][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWk[0][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[0][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[0][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[0][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	vertexWk[1][0].vtx = D3DXVECTOR3(TUTORIAL_POS_X02, TUTORIAL_POS_Y02, 0.0f);
	vertexWk[1][1].vtx = D3DXVECTOR3(TUTORIAL_POS_X02 + TEXTURE_TUTORIAL_SIZE_X02, TUTORIAL_POS_Y02, 0.0f);
	vertexWk[1][2].vtx = D3DXVECTOR3(TUTORIAL_POS_X02, TUTORIAL_POS_Y02 + TEXTURE_TUTORIAL_SIZE_Y02, 0.0f);
	vertexWk[1][3].vtx = D3DXVECTOR3(TUTORIAL_POS_X02 + TEXTURE_TUTORIAL_SIZE_X02, TUTORIAL_POS_Y02 + TEXTURE_TUTORIAL_SIZE_Y02, 0.0f);

	// テクスチャのパースペクティブコレクト用
	vertexWk[1][0].rhw =
		vertexWk[1][1].rhw =
		vertexWk[1][2].rhw =
		vertexWk[1][3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[1][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWk[1][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[1][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[1][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTutorial1(void)
{
	// アニメーションしないテクスチャ座標の設定
	int x = NULL;
	int y = NULL;
	float sizeX = 1.0f;
	float sizeY = 1.0f;

	vertexWk[0][0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[0][1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[0][2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[0][3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}
