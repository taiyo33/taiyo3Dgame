//=============================================================================
//
// プレイヤー処理 [enemy.cpp]
// Author : GP11A341_22_田中太陽
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	ENEMY_CAR		"data/MODEL/model_body.x"							// 読み込むモデル名
#define	TEXTURE_FILENAME ("data/TEXTURE/field003.jpg")							// 読み込むモデル名
#define	VALUE_MOVE		(5.0f)											// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// 回転量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9	D3DTexture;		// テクスチャへのポインタ
static LPD3DXMESH			D3DMesh;		// メッシュ情報へのポインタ
static LPD3DXBUFFER			D3DXBuffMat;	// マテリアル情報へのポインタ
static DWORD				NumMat;			// マテリアル情報の数


static int					g_shadownum;
static D3DXCOLOR			g_shadowcolor;			
static D3DXMATRIX			g_mtxWorld;				// ワールドマトリックス

ENEMY				enemy[ENEMY_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// 位置・回転・スケールの初期設定
		enemy[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		enemy[i].use = true;
		enemy[i].life = 100;
	}

	D3DTexture = NULL;
	D3DMesh = NULL;
	D3DXBuffMat = NULL;
	NumMat = 0;

	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromX(ENEMY_CAR,				// 読み込むモデルファイル名(Xファイル)
								D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
								pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
								NULL,					// 隣接性データを含むバッファへのポインタ
								&D3DXBuffMat,	// マテリアルデータを含むバッファへのポインタ　テクスチャやカラー情報
								NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
								&NumMat,		// D3DXMATERIAL構造体の数	登録されてるマテリアル情報数
								&D3DMesh)))	// ID3DXMeshインターフェイスへのポインタのアドレス　頂点情報のデータ
	{
		return E_FAIL;
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FILENAME,		// ファイルの名前
								&D3DTexture);	// 読み込むメモリー
#endif

	//g_shadownum = CreateShadow(enemy[i].pos, 25.0f, 25.0f);
	//g_shadowcolor = D3DXCOLOR(1.5f,1.5f,1.5f,1.5f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if(D3DTexture != NULL)
	{// テクスチャの開放
		D3DTexture->Release();
		D3DTexture = NULL;
	}

	if(D3DMesh != NULL)
	{// メッシュの開放
		D3DMesh->Release();
		D3DMesh = NULL;
	}

	if(D3DXBuffMat != NULL)
	{// マテリアルの開放
		D3DXBuffMat->Release();
		D3DXBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	
	//SetPositionShadow(g_shadownum, enemy[i].pos);
	//SetColorShadow(g_shadownum, g_shadowcolor);

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i].use == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, enemy[i].scl.x, enemy[i].scl.y, enemy[i].scl.z);
			D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy[i].rot.y, enemy[i].rot.x, enemy[i].rot.z);
			D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, enemy[i].pos.x, enemy[i].pos.y, enemy[i].pos.z);
			D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

			// 現在のマテリアルを取得　（元に戻すためのバックアップ）
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)D3DXBuffMat->GetBufferPointer();

			// マテリアルの数分の表示が必要なためFOR文を使用
			for (int nCntMat = 0; nCntMat < (int)NumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTexture);

				// 描画
				D3DMesh->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

ENEMY *GetEnemy(int index)
{
	return &enemy[index];
}

void EnemyDamageManager(void)
{
	
}