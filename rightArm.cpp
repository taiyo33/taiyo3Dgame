//=============================================================================
//
// モデル処理 [rightArm.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "rightArm.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_AIRPLANE		"data/MODEL/model_arm.x"	// 読み込むモデル名
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
static LPDIRECT3DTEXTURE9	 D3DTexture;			// テクスチャ読み込み場所
static LPD3DXMESH			 D3DXMesh;			// ID3DXMeshインターフェイスへのポインタ
static LPD3DXBUFFER		 D3DXBuffMat;		// メッシュのマテリアル情報を格納
static DWORD				 NumMat;				// 属性情報の総数

RIGHTARM			rightArm[PLAYER_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRightArm(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DTexture = NULL;
	D3DXMesh = NULL;
	D3DXBuffMat = NULL;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		rightArm[i].pos = D3DXVECTOR3(12.0f, 10.0f, 0.0f);
		rightArm[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRightArm(void)
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
void UpdateRightArm(void)
{

	// 影の位置設定
	//SetPositionShadow(nIdxShadow, D3DXVECTOR3(rightArm[i].pos.x, 0.1f, rightArm[i].pos.z));

	//SetVertexShadow(nIdxShadow, fSizeShadow, fSizeShadow);
	//SetColorShadow(nIdxShadow, colShadow);

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRightArm(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&rightArm[i].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rightArm[i].rot.y, rightArm[i].rot.x, rightArm[i].rot.z);
			D3DXMatrixMultiply(&rightArm[i].mtxWorld, &rightArm[i].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, rightArm[i].pos.x, rightArm[i].pos.y, rightArm[i].pos.z);
			D3DXMatrixMultiply(&rightArm[i].mtxWorld, &rightArm[i].mtxWorld, &mtxTranslate);


			D3DXMatrixMultiply(&rightArm[i].mtxWorld, &rightArm[i].mtxWorld, &player->mtxWorld);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &rightArm[i].mtxWorld);


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

