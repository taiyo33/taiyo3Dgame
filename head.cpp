//=============================================================================
//
// モデル処理 [head.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "head.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_AIRPLANE		"data/MODEL/model_head.x"	// 読み込むモデル名
#define	VALUE_MOVE_PLAYER	(0.50f)						// 移動速度
#define	RATE_MOVE_PLAYER		(0.20f)						// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.05f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.20f)						// 回転慣性係数

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

HEAD				head[PLAYER_MAX];
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
		head[i].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		head[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		head[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(PLAYER_AIRPLANE,
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
	//g_nIdxShadow = CreateShadow(head.pos, 25.0f, 25.0f);

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
			D3DXMatrixIdentity(&head[i].mtxWorld);

			// 拡大の反映
			D3DXMatrixScaling(&mtxScl, head[i].scl.x, head[i].scl.y, head[i].scl.z);
			D3DXMatrixMultiply(&head[i].mtxWorld, &head[i].mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, head[i].rot.y, head[i].rot.x, head[i].rot.z);
			D3DXMatrixMultiply(&head[i].mtxWorld, &head[i].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, head[i].pos.x, head[i].pos.y, head[i].pos.z);
			D3DXMatrixMultiply(&head[i].mtxWorld, &head[i].mtxWorld, &mtxTranslate);


			D3DXMatrixMultiply(&head[i].mtxWorld, &head[i].mtxWorld, &player->mtxWorld);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &head[i].mtxWorld);


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