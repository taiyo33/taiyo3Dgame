//=============================================================================
//
// プレイヤー処理 [child.cpp]
// Author : GP11A341_22_田中太陽
//
//=============================================================================
#include "child.h"
#include "input.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CHILD_MODEL01		"data/MODEL/child01.x"				// 読み込むモデル名
#define	CHILD_MODEL02		"data/MODEL/child02.x"				// 読み込むモデル名

#define	TEXTURE_FILENAME ("data/TEXTURE/field003.jpg")		// 読み込むモデル名
#define	VALUE_MOVE		(5.0f)								// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)					// 回転量

enum {
	MODEL_TYPE001,
	MODEL_TYPE002
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9	D3DTexture;		// テクスチャへのポインタ
static LPD3DXMESH			D3DMesh[CHILD_SET_MAX];		// メッシュ情報へのポインタ
static LPD3DXBUFFER			D3DXBuffMat[CHILD_SET_MAX];	// マテリアル情報へのポインタ
static DWORD				NumMat[CHILD_SET_MAX];			// マテリアル情報の数


static int					g_shadownum;
static D3DXCOLOR			g_shadowcolor;			
static D3DXMATRIX			g_mtxWorld;				// ワールドマトリックス

CHILD						childWk[CHILD_SET_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitChild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CHILD *child = &childWk[0];

	for (int i = 0; i < CHILD_SET_MAX; i++)
	{
		D3DMesh[i] = NULL;
		D3DXBuffMat[i] = NULL;
		NumMat[i] = 0;

		for (int j = 0; j < CHILD_ONESET_MAX; j++)
		{
			// 位置・回転・スケールの初期設定
			child[i].pos[j] = D3DXVECTOR3(-300.0f + (10.0f * j), 10.0f, 50.0f * i);
			child[i].rot[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			child[i].scl[j] = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
			child[i].use[j] = true;
			child[i].life[j] = 100;
		}
	}

	D3DTexture = NULL;

	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromX(CHILD_MODEL01,				// 読み込むモデルファイル名(Xファイル)
								D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
								pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
								NULL,					// 隣接性データを含むバッファへのポインタ
								&D3DXBuffMat[MODEL_TYPE001],	// マテリアルデータを含むバッファへのポインタ　テクスチャやカラー情報
								NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
								&NumMat[MODEL_TYPE001],		// D3DXMATERIAL構造体の数	登録されてるマテリアル情報数
								&D3DMesh[MODEL_TYPE001])))	// ID3DXMeshインターフェイスへのポインタのアドレス　頂点情報のデータ
	{
		return E_FAIL;
	}

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(CHILD_MODEL02,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMat[MODEL_TYPE002],	// マテリアルデータを含むバッファへのポインタ　テクスチャやカラー情報
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMat[MODEL_TYPE002],		// D3DXMATERIAL構造体の数	登録されてるマテリアル情報数
		&D3DMesh[MODEL_TYPE002])))	// ID3DXMeshインターフェイスへのポインタのアドレス　頂点情報のデータ
	{
		return E_FAIL;
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FILENAME,		// ファイルの名前
								&D3DTexture);	// 読み込むメモリー
#endif

	//g_shadownum = CreateShadow(child[i].pos[j], 25.0f, 25.0f);
	//g_shadowcolor = D3DXCOLOR(1.5f,1.5f,1.5f,1.5f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitChild(void)
{
	if (D3DTexture != NULL)
	{// テクスチャの開放
		D3DTexture->Release();
		D3DTexture = NULL;
	}

	for (int i = 0; i < CHILD_SET_MAX; i++)
	{
		if (D3DMesh != NULL)
		{// メッシュの開放
			D3DMesh[i]->Release();
			D3DMesh[i] = NULL;
		}

		if (D3DXBuffMat != NULL)
		{// マテリアルの開放
			D3DXBuffMat[i]->Release();
			D3DXBuffMat[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateChild(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawChild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	CHILD *child = &childWk[0];

	for (int i = 0; i < CHILD_SET_MAX; i++)
	{
		for (int j = 0; j < CHILD_ONESET_MAX; j++)
		{
			if (child[i].use[j] == true)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_mtxWorld);

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, child[i].scl[j].x, child[i].scl[j].y, child[i].scl[j].z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, child[i].rot[j].y, child[i].rot[j].x, child[i].rot[j].z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, child[i].pos[j].x, child[i].pos[j].y, child[i].pos[j].z);
				D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

				// 現在のマテリアルを取得　（元に戻すためのバックアップ）
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				pD3DXMat = (D3DXMATERIAL*)D3DXBuffMat[i]->GetBufferPointer();

				// マテリアルの数分の表示が必要なためFOR文を使用
				for (int nCntMat = 0; nCntMat < (int)NumMat[i]; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

					// テクスチャの設定
					pDevice->SetTexture(0, D3DTexture);

					// 描画
					D3DMesh[i]->DrawSubset(nCntMat);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

CHILD *GetChild(int index)
{
	return &childWk[index];
}

void ChildDamageManager(void)
{
	
}