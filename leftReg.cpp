//=============================================================================
//
// 左足モデル処理 [leftReg.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "leftReg.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_AIRPLANE		"data/MODEL/model_reg.x"	// 読み込むモデル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9	D3DTexture;			// テクスチャ読み込み場所
static LPD3DXMESH			D3DXMesh;			// ID3DXMeshインターフェイスへのポインタ
static LPD3DXBUFFER		D3DXBuffMat;		// メッシュのマテリアル情報を格納
static DWORD				NumMat;				// 属性情報の総数


LEFTREG				leftReg[PLAYER_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLeftReg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DTexture = NULL;
	D3DXMesh = NULL;
	D3DXBuffMat = NULL;
	
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		leftReg[i].pos = D3DXVECTOR3(-7.0f, -20.0f, 0.0f);
		leftReg[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLeftReg(void)
{
	if (D3DTexture != NULL)
	{// テクスチャの開放
		D3DTexture ->Release();
		D3DTexture = NULL;
	}

	if (D3DXMesh != NULL)
	{// メッシュの開放
		D3DXMesh ->Release();
		D3DXMesh = NULL;
	}

	if (D3DXBuffMat != NULL)
	{// マテリアルの開放
		D3DXBuffMat ->Release();
		D3DXBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLeftReg(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLeftReg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->use)
		{

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&leftReg[i].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, leftReg[i].rot.y, leftReg[i].rot.x, leftReg[i].rot.z);
			D3DXMatrixMultiply(&leftReg[i].mtxWorld, &leftReg[i].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, leftReg[i].pos.x, leftReg[i].pos.y, leftReg[i].pos.z);
			D3DXMatrixMultiply(&leftReg[i].mtxWorld, &leftReg[i].mtxWorld, &mtxTranslate);


			D3DXMatrixMultiply(&leftReg[i].mtxWorld, &leftReg[i].mtxWorld, &player->mtxWorld);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &leftReg[i].mtxWorld);


			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			D3DXMat = (D3DXMATERIAL*)D3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)NumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&D3DXMat[nCntMat].MatD3D);

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
