//=============================================================================
//
// モデル処理 [head.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "head.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_HEAD		"data/MODEL/model_head.x"	// 読み込むモデル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9	 D3DTexture;		// テクスチャ読み込み場所
static LPD3DXMESH			 D3DXMesh;			// ID3DXMeshインターフェイスへのポインタ
static LPD3DXBUFFER			 D3DXBuffMat;		// メッシュのマテリアル情報を格納
static DWORD				 NumMat;			// 属性情報の総数

HEAD						 Head[PLAYER_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHead(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DTexture = NULL;
	D3DXMesh = NULL;
	D3DXBuffMat = NULL;
	
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		Head[i].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		Head[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Head[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(PLAYER_HEAD,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&D3DXBuffMat,
		NULL,
		&NumMat,
		&D3DXMesh)))
	{
		return E_FAIL;
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,		// ファイルの名前
		&D3DTexture);	// 読み込むメモリー
#endif

	//// 影の生成
	//g_nIdxShadow = CreateShadow(Head.pos, 25.0f, 25.0f);

	//g_fSizeShadow = 25.0f;
	//g_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHead(void)
{
	if (D3DTexture != NULL)
	{// テクスチャの開放
		D3DTexture->Release();
		D3DTexture = NULL;
	}

	if (D3DXMesh != NULL)
	{// メッシュの開放
		D3DXMesh->Release();
		D3DXMesh = NULL;
	}

	if (D3DXBuffMat != NULL)
	{// マテリアルの開放
		D3DXBuffMat->Release();
		D3DXBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHead(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHead(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&Head[i].mtxWorld);

			// 拡大の反映
			D3DXMatrixScaling(&mtxScl, Head[i].scl.x, Head[i].scl.y, Head[i].scl.z);
			D3DXMatrixMultiply(&Head[i].mtxWorld, &Head[i].mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, Head[i].rot.y, Head[i].rot.x, Head[i].rot.z);
			D3DXMatrixMultiply(&Head[i].mtxWorld, &Head[i].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, Head[i].pos.x, Head[i].pos.y, Head[i].pos.z);
			D3DXMatrixMultiply(&Head[i].mtxWorld, &Head[i].mtxWorld, &mtxTranslate);


			D3DXMatrixMultiply(&Head[i].mtxWorld, &Head[i].mtxWorld, &player->mtxWorld);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &Head[i].mtxWorld);


			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)D3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)NumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTexture);

				// 描画
				D3DXMesh->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}