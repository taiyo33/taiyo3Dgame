//=============================================================================
//
// チュートリアル処理 [Tutorial.cpp]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "field.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

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
static VERTEX_2D			vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク

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
	if (GetKeyboardTrigger(DIK_M) && TextureNum < TUTORIAL_MAX)
	{
		TextureNum++;	// 
	}
	else if(TextureNum == TUTORIAL_MAX)
	{
		//SetMakeField();
		SetStage(START);
	}

	if (IsButtonTriggered(0, BUTTON_A) && TextureNum < TUTORIAL_MAX)
	{
		TextureNum++;	// 
	}
	else if (TextureNum == TUTORIAL_MAX)
	{
		//SetMakeField();
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
	pDevice->SetTexture( 0, D3DTextureTutorial[TextureNum] );

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TUTORIAL, vertexWk, sizeof(VERTEX_2D));

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	vertexWk[0].vtx = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(TUTORIAL_POS_X + TEXTURE_TUTORIAL_SIZE_X, TUTORIAL_POS_Y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y + TEXTURE_TUTORIAL_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(TUTORIAL_POS_X + TEXTURE_TUTORIAL_SIZE_X, TUTORIAL_POS_Y + TEXTURE_TUTORIAL_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	vertexWk[0].rhw =
	vertexWk[1].rhw =
	vertexWk[2].rhw =
	vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

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

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}
