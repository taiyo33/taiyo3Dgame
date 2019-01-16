//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		pD3DTextureResult[RESULT_MAX];			// テクスチャへのポリゴン

VERTEX_2D				vertexWk_RESULT[NUM_VERTEX];		// 頂点情報格納ワーク

D3DXVECTOR3				posResult;						// ポリゴンの移動量
D3DXVECTOR3				rotResult;						// ポリゴンの回転量
static int				TextureNum;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	posResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotResult = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_CLEAR,						// ファイルの名前
			&pD3DTextureResult[CLEAR]);			// 読み込むメモリのポインタ

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAMEOVER,					// ファイルの名前
			&pD3DTextureResult[GAMEOVER]);		// 読み込むメモリのポインタ

	}

	// 頂点情報の作成
	MakeVertexResult();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if( pD3DTextureResult[TextureNum] != NULL )	//
	{	// テクスチャの開放
		 pD3DTextureResult[TextureNum]->Release();
		 pD3DTextureResult[TextureNum] = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	// アニメーション
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		SetStage(TITLE);
		InitGame();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// テクスチャの設定
	pDevice->SetTexture( 0, pD3DTextureResult[TextureNum] );

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RESULT, vertexWk_RESULT, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	vertexWk_RESULT[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexWk_RESULT[1].vtx = D3DXVECTOR3(0.0f+TEXTURE_SAMPLE00_SIZE_X_RESULT, 0.0f, 0.0f);
	vertexWk_RESULT[2].vtx = D3DXVECTOR3(0.0f, 0.0f+TEXTURE_SAMPLE00_SIZE_Y_RESULT, 0.0f);
	vertexWk_RESULT[3].vtx = D3DXVECTOR3(0.0f+TEXTURE_SAMPLE00_SIZE_X_RESULT, 0.0f+TEXTURE_SAMPLE00_SIZE_Y_RESULT, 0.0f);

	// rhwの設定
	vertexWk_RESULT[0].rhw =
	vertexWk_RESULT[1].rhw =
	vertexWk_RESULT[2].rhw =
	vertexWk_RESULT[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk_RESULT[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk_RESULT[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk_RESULT[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk_RESULT[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	vertexWk_RESULT[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk_RESULT[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk_RESULT[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk_RESULT[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}


void SetTextureResult(int num)
{
	TextureNum = num;
}
