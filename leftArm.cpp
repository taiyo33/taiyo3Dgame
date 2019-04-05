//=============================================================================
//
// 左手モデル処理 [leftArm.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "leftArm.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_LEFTARM		"data/MODEL/model_arm.x"	// 読み込むモデル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9	 D3DTexture;			// テクスチャ読み込み場所
static LPD3DXMESH			 D3DXMesh;				// ID3DXMeshインターフェイスへのポインタ
static LPD3DXBUFFER			 D3DXBuffMat;			// メッシュのマテリアル情報を格納
static DWORD				 NumMat;				// 属性情報の総数

LEFTARM						 LeftArm[PLAYER_MAX];	// 左腕モデル構造体
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLeftArm(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DTexture = NULL;
	D3DXMesh = NULL;
	D3DXBuffMat = NULL;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		LeftArm[i].pos = D3DXVECTOR3(-12.0f, 10.0f, 0.0f);
		LeftArm[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(PLAYER_LEFTARM,
		D3DXMESH_SYSTEMMEM,
		Device,
		NULL,
		&D3DXBuffMat,
		NULL,
		&NumMat,
		&D3DXMesh)))
	{
		return E_FAIL;
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLeftArm(void)
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
void UpdateLeftArm(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLeftArm(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&LeftArm[i].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, LeftArm[i].rot.y, LeftArm[i].rot.x, LeftArm[i].rot.z);
			D3DXMatrixMultiply(&LeftArm[i].mtxWorld, &LeftArm[i].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, LeftArm[i].pos.x, LeftArm[i].pos.y, LeftArm[i].pos.z);
			D3DXMatrixMultiply(&LeftArm[i].mtxWorld, &LeftArm[i].mtxWorld, &mtxTranslate);


			D3DXMatrixMultiply(&LeftArm[i].mtxWorld, &LeftArm[i].mtxWorld, &player->mtxWorld);

			// ワールドマトリックスの設定
			Device->SetTransform(D3DTS_WORLD, &LeftArm[i].mtxWorld);


			// 現在のマテリアルを取得
			Device->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			D3DXMat = (D3DXMATERIAL*)D3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)NumMat; nCntMat++)
			{
				// マテリアルの設定
				Device->SetMaterial(&D3DXMat[nCntMat].MatD3D);

				// テクスチャの設定
				Device->SetTexture(0, D3DTexture);

				// 描画
				D3DXMesh->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			Device->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 位置取得
//=============================================================================
LEFTARM *GetLeftArm(int index)
{
	return &LeftArm[index];
}
