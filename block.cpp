//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "block.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "checkhit.h"
#include "explosion.h"
#include "bullet.h"
#include "item.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	BLOCK_MODEL				"data/MODEL/block001.x"	// 読み込むモデル名
#define TEXTURE_MAX				(5)
#define INIT_POS_X_RIGHTSIDE	(375.0f)	// 右側のブロックの初期X座標
#define INIT_POS_X_LEFTSIDE		(-375.0f)	// 左側のブロックの初期X座標
#define INIT_POS_Y				(0.0f)		// ブロックの初期Y座標
#define INIT_POS_Z_NEARSIDE		(-287.5f)	// 手前のブロックの初期Z座標
#define INIT_POS_Z_FEARSIDE		(287.5f)	// 奥のブロックの初期Z座標
#define BLOCK_ONEUNIT_SIDEOBJ_NUM	(9)			// 横1纏まりのブロック数
#define BLOCK_ONEUNIT_DEPTHOBJ_NUM	(11)			// 縦1纏まりのブロック数

#define BLOCK_VERTEX			(4)			// 
#define VTX_SIZE_X				(355.0f)	// 頂点のX軸サイズ
#define VTX_SIZE_Y				(25.0f)		// 　　　Y軸サイズ
#define VTX_SIZE_Z				(273.5f)	// 　　　Z軸サイズ
#define BLOCK_DAMAGE			(1)			// ブロックへのダメージ
#define BLOCK_SIZE				(25.0f)		// ブロックの1辺のサイズ
#define INIT_LIFE				(5)			// 寿命の初期値	

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetPosBlock(void);
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice);
void InitVertexBlock(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBlock[TEXTURE_MAX] = { 
									NULL,NULL,NULL,NULL,NULL }; // テクスチャ読み込み場所
LPD3DXMESH				D3DXMeshBlock;		// ID3DXMeshBlockインターフェイスへのポインタ
LPD3DXBUFFER			D3DXBuffMatBlock;	// メッシュのマテリアル情報を格納
DWORD					NumMat;				// 属性情報の総数
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBlock;	// 頂点バッファへのポインタ
D3DXMATRIX				MtxWorld;			// ワールドマトリックス
D3DXMATRIX				MtxWorldVtx;		// ワールドマトリックス
static int				TextureNum;			// テクスチャーの番号
BLOCK					block[BLOCK_MAX];	// ブロック構造体

//=============================================================================
// 初期化処理
// 引　数：int type(再初期化２数判定値)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitBlock(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// 頂点情報の作成
	hr = MakeVertexBlock(pDevice);
	
	// ブロックの初期位置を設定
	SetPosBlock();

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		block[i].use = true;		// 使用へ初期化
		block[i].life = INIT_LIFE;	// ライフの初期化
	}

	D3DXMeshBlock = NULL;			// インターフェースの初期化
	D3DXBuffMatBlock = NULL;		// マテリアルの初期化
	TextureNum = NULL;				// テクスチャー番号を初期化

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(BLOCK_MODEL,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&D3DXBuffMatBlock,
		NULL,
		&NumMat,
		&D3DXMeshBlock)))
	{
		return E_FAIL;
	}
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlock(void)
{
	if(D3DXMeshBlock != NULL)
	{// メッシュの開放
		D3DXMeshBlock->Release();
		D3DXMeshBlock = NULL;
	}

	if(D3DXBuffMatBlock != NULL)
	{// マテリアルの開放
		D3DXBuffMatBlock->Release();
		D3DXBuffMatBlock = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBlock[i] != NULL)
		{// テクスチャの開放
			D3DTextureBlock[i]->Release();
			D3DTextureBlock[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlock(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (block[i].use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&MtxWorld);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, block[i].pos.x, block[i].pos.y, block[i].pos.z);
			D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatBlock->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)NumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureBlock[TextureNum]);

				// 描画
				D3DXMeshBlock->DrawSubset(nCntMat);
			}

			//// ワールドマトリックスの初期化
			//D3DXMatrixIdentity(&MtxWorldVtx);

			//// ワールドマトリックスの設定
			//pDevice->SetTransform(D3DTS_WORLD, &MtxWorldVtx);

			//// 頂点バッファをデバイスのデータストリームにバインド
			//pDevice->SetStreamSource(0, D3DVtxBuffBlock, 0, sizeof(VERTEX_3D));

			//// 頂点フォーマットの設定
			//pDevice->SetFVF(FVF_VERTEX_3D);

			//// テクスチャの設定
			//pDevice->SetTexture(0, D3DTextureBlock[0]);

			//// ポリゴンの描画
			//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);

		}
	}
}


//=============================================================================
// ブロックのアドレス取得
//=============================================================================
BLOCK *GetBlock(int bno)
{
	return &block[bno];
}

//=============================================================================
// ブロックの設置
// 引　数：なし
// 戻り値：なし
//=============================================================================
void SetPosBlock(void)
{
	int i = 0;
	float size = 0.0f;

	// フィールド周囲のブロック配置
		for (i = 0; i < BLOCK_NUM_LEFTSIDE; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, INIT_POS_Z_FEARSIDE - size);
		}

		size = BLOCK_SIZE;

		for (i = BLOCK_NUM_LEFTSIDE; i < BLOCK_NUM_NEARSIDE; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE + size, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
		}

		size = 0.0f;

		for (i = BLOCK_NUM_NEARSIDE; i < BLOCK_NUM_RIGHTSIDE; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, INIT_POS_Z_NEARSIDE + size);
		}

		size = BLOCK_SIZE;

		for (i = BLOCK_NUM_RIGHTSIDE; i < BLOCK_NUM_FEARSIDE; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE - size, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
		}
	

	// 障害物ブロック配置
	
		size = 0.0f;
		for (i = BLOCK_NUM_FEARSIDE; i < BLOCK_NUM_LEFTSIDE_OBJ - 5; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE + 170.0f, INIT_POS_Y, INIT_POS_Z_FEARSIDE - 125.0f - size);
		}
		for (i = BLOCK_NUM_LEFTSIDE_OBJ - 5; i < BLOCK_NUM_LEFTSIDE_OBJ; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE + 170.0f, INIT_POS_Y, INIT_POS_Z_FEARSIDE - 225.0f - size);
		}

		size = 0.0f;
		for (i = BLOCK_NUM_LEFTSIDE_OBJ; i < BLOCK_NUM_NEARSIDE_OBJ - 5; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE + 195.0f + size, INIT_POS_Y, INIT_POS_Z_NEARSIDE + 125.0f);
		}
		for (i = BLOCK_NUM_NEARSIDE_OBJ - 5; i < BLOCK_NUM_NEARSIDE_OBJ; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE + 320.0f + size, INIT_POS_Y, INIT_POS_Z_NEARSIDE + 125.0f);
		}

		size = 0.0f;
		for (i = BLOCK_NUM_NEARSIDE_OBJ; i < BLOCK_NUM_RIGHTSIDE_OBJ - 5; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE - 180.0f, INIT_POS_Y, INIT_POS_Z_NEARSIDE + 125.0f + size);
		}
		for (i = BLOCK_NUM_RIGHTSIDE_OBJ - 5; i < BLOCK_NUM_RIGHTSIDE_OBJ; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE - 180.0f, INIT_POS_Y, INIT_POS_Z_NEARSIDE + 225.0f + size);
		}

		size = 0.0f;
		for (i = BLOCK_NUM_RIGHTSIDE_OBJ; i < BLOCK_NUM_FEARSIDE_OBJ - 5; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE - 205.0f - size, INIT_POS_Y, INIT_POS_Z_FEARSIDE - 125.0f);
		}
		for (i = BLOCK_NUM_FEARSIDE_OBJ - 5; i < BLOCK_NUM_FEARSIDE_OBJ; i++, size += BLOCK_SIZE)
		{
			block[i].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE - 330.0f - size, INIT_POS_Y, INIT_POS_Z_FEARSIDE - 125.0f);
		}

}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BLOCK_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffBlock,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	/* 頂点情報の設定 */
	InitVertexBlock();

	return S_OK;
}

//============================================================================
// ブロックの頂点作成
//============================================================================
void InitVertexBlock(void)
{
	int i = 0;
	float size = 0.0f;

	// BOXの頂点情報の設定
	{	//頂点バッファの中身を埋める
		VERTEX_3D			   *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

		// フィールド周囲のブロック
		for (i = 0; i < BLOCK_NUM_LEFTSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X, VTX_SIZE_Y, VTX_SIZE_Z - size);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X, VTX_SIZE_Y, VTX_SIZE_Z + BLOCK_SIZE - size);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X, 0.0f, VTX_SIZE_Z - size);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X, 0.0f, VTX_SIZE_Z + BLOCK_SIZE - size);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		size = BLOCK_SIZE;

		for (i = BLOCK_NUM_LEFTSIDE; i < BLOCK_NUM_NEARSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X - BLOCK_SIZE + size, VTX_SIZE_Y, -VTX_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X + size, VTX_SIZE_Y, -VTX_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X - BLOCK_SIZE + size, 0.0f, -VTX_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X + size, 0.0f, -VTX_SIZE_Z);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}

		size = 0.0f;

		for (i = BLOCK_NUM_NEARSIDE; i < BLOCK_NUM_RIGHTSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X, VTX_SIZE_Y, -VTX_SIZE_Z + size);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X, VTX_SIZE_Y, -VTX_SIZE_Z - BLOCK_SIZE + size);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X, 0.0f, -VTX_SIZE_Z + size);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X, 0.0f, -VTX_SIZE_Z - BLOCK_SIZE + size);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		size = BLOCK_SIZE;

		for (i = BLOCK_NUM_RIGHTSIDE; i < BLOCK_NUM_FEARSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X - size, VTX_SIZE_Y, VTX_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X + BLOCK_SIZE - size, VTX_SIZE_Y, VTX_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X - size, 0.0f, VTX_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X + BLOCK_SIZE - size, 0.0f, VTX_SIZE_Z);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 障害物ブロック配置
		size = 0.0f;
		for (i = BLOCK_NUM_FEARSIDE; i < BLOCK_LEFT_OUTSIDE_OBJ_VTX; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f, VTX_SIZE_Y, VTX_SIZE_Z - 225.0f - size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f, VTX_SIZE_Y, VTX_SIZE_Z - 225.0f + 125.0f - size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f, 0.0f, VTX_SIZE_Z - 225.0f - size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f, 0.0f, VTX_SIZE_Z - 225.0f + 125.0f - size * BLOCK_ONEUNIT_SIDEOBJ_NUM);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		size = 0.0f;
		for (i = BLOCK_LEFT_OUTSIDE_OBJ_VTX; i < BLOCK_NEAR_OUTSIDE_OBJ_VTX; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f + size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, VTX_SIZE_Y, -VTX_SIZE_Z + 100.0f);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f + 150.0f + size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, VTX_SIZE_Y, -VTX_SIZE_Z + 100.0f);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f + size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, 0.0f, -VTX_SIZE_Z + 95.0f);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f + 150.0f + size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, 0.0f, -VTX_SIZE_Z + 95.0f);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		size = 0.0f;
		for (i = BLOCK_NEAR_OUTSIDE_OBJ_VTX; i < BLOCK_RIGHT_OUTSIDE_OBJ_VTX; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X - 150.0f, VTX_SIZE_Y, -VTX_SIZE_Z + 95.0f + 125.0f + size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X - 150.0f, VTX_SIZE_Y, -VTX_SIZE_Z + 95.0f + size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X - 150.0f, 0.0f, -VTX_SIZE_Z + 95.0f + 125.0f + size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X - 150.0f, 0.0f, -VTX_SIZE_Z + 95.0f + size * BLOCK_ONEUNIT_SIDEOBJ_NUM);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		size = 0.0f;
		for (i = BLOCK_RIGHT_OUTSIDE_OBJ_VTX; i < BLOCK_FEAR_OUTSIDE_OBJ_VTX; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X - 147.0f - 150.0f - size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, VTX_SIZE_Y, VTX_SIZE_Z - 105.0f);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X - 147.0f - size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, VTX_SIZE_Y, VTX_SIZE_Z - 105.0f);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X - 147.0f - 150.0f - size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, 0.0f, VTX_SIZE_Z - 105.0f);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X - 147.0f - size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, 0.0f, VTX_SIZE_Z - 105.0f);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		size = 0.0f;
		for (i = BLOCK_FEAR_OUTSIDE_OBJ_VTX; i < BLOCK_LEFT_INSIDE_OBJ_VTX; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X + 165.0f, VTX_SIZE_Y, VTX_SIZE_Z - 225.0f - size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X + 165.0f, VTX_SIZE_Y, VTX_SIZE_Z - 225.0f + 125.0f - size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X + 165.0f, 0.0f, VTX_SIZE_Z - 225.0f - size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X + 165.0f, 0.0f, VTX_SIZE_Z - 225.0f + 125.0f - size * BLOCK_ONEUNIT_SIDEOBJ_NUM);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		size = 0.0f;
		for (i = BLOCK_LEFT_INSIDE_OBJ_VTX; i < BLOCK_NEAR_INSIDE_OBJ_VTX; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f + size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, VTX_SIZE_Y, -VTX_SIZE_Z + 120.0f);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f + 150.0f + size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, VTX_SIZE_Y, -VTX_SIZE_Z + 120.0f);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f + size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, 0.0f, -VTX_SIZE_Z + 120.0f);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X + 140.0f + 150.0f + size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, 0.0f, -VTX_SIZE_Z + 120.0f);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		size = 0.0f;
		for (i = BLOCK_NEAR_INSIDE_OBJ_VTX; i < BLOCK_RIGHT_INSIDE_OBJ_VTX; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X - 175.0f, VTX_SIZE_Y, -VTX_SIZE_Z + 95.0f + 125.0f + size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X - 175.0f, VTX_SIZE_Y, -VTX_SIZE_Z + 95.0f + size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X - 175.0f, 0.0f, -VTX_SIZE_Z + 95.0f + 125.0f + size * BLOCK_ONEUNIT_SIDEOBJ_NUM);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X - 175.0f, 0.0f, -VTX_SIZE_Z + 95.0f + size * BLOCK_ONEUNIT_SIDEOBJ_NUM);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		size = 0.0f;
		for (i = BLOCK_RIGHT_INSIDE_OBJ_VTX; i < BLOCK_FEAR_INSIDE_OBJ_VTX; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X - 147.0f - 150.0f - size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, VTX_SIZE_Y, VTX_SIZE_Z - 130.0f);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X - 147.0f - size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, VTX_SIZE_Y, VTX_SIZE_Z - 130.0f);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X - 147.0f - 150.0f - size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, 0.0f, VTX_SIZE_Z - 130.0f);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X - 147.0f - size * BLOCK_ONEUNIT_DEPTHOBJ_NUM, 0.0f, VTX_SIZE_Z - 130.0f);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

	}

	// 頂点データをアンロックする
	D3DVtxBuffBlock->Unlock();
}

//=============================================================================
// ブロックの透明ポリゴンとの当たり判定
// pos0:始点（移動前）
// pos1:終点（移動後）
//=============================================================================
bool HitCheckBlock(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, int max)
{
	D3DXVECTOR3		pos[NUM_VERTEX]; // 頂点座標の保存
	bool			ans;
	VERTEX_3D	    *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < max; i++, pVtx += 4)
	{
		if (block[i].use)
		{
			// 頂点座標の設定
			pos[0] = pVtx[0].vtx;
			pos[1] = pVtx[1].vtx;
			pos[2] = pVtx[2].vtx;
			pos[3] = pVtx[3].vtx;

			// 左下側ポリゴンと線分の当たり判定
			ans = CheckHitPolygon(pos[0], pos[2], pos[1], pos0, pos1);
			if (!ans)
			{
				break;
			}
			// 右上側ポリゴンと線分の当たり判定
			ans = CheckHitPolygon(pos[3], pos[1], pos[2], pos0, pos1);
			if (!ans)
			{
				break;
			}
		}
	}

	// 頂点データをアンロックする
	D3DVtxBuffBlock->Unlock();

	return ans;
}

//=============================================================================
// 4つ角との当たり判定
// 引　数：D3DXVECTOR3 pos0(判定対象の位置）
// 戻り値：bool型　trueだと当たっている、falseだと当たっていない
//=============================================================================
bool HitCheckCornerBlock(D3DXVECTOR3 pos0)
{
	// 外枠
	if (CheckHitBB(pos0, block[0].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(20.0f, 25.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_LEFTSIDE].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(20.0f, 25.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_NEARSIDE].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(20.0f, 25.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_RIGHTSIDE].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(20.0f, 25.0f, 20.0f))) return true;

	// 障害物
	if (CheckHitBB(pos0, block[BLOCK_NUM_RIGHTSIDE + 1].pos,
		D3DXVECTOR3(20.0f, 100.0f, 20.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_LEFTSIDE_OBJ - 1].pos,
		D3DXVECTOR3(20.0f, 100.0f, 20.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_NEARSIDE_OBJ].pos,
		D3DXVECTOR3(20.0f, 100.0f, 20.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_RIGHTSIDE_OBJ].pos,
		D3DXVECTOR3(20.0f, 100.0f, 20.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	// 各中央入口のブロック
	if (CheckHitBB(pos0, block[BLOCK_NUM_LEFTSIDE_OBJ - 5].pos,
		D3DXVECTOR3(15.0f, 100.0f, 20.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_LEFTSIDE_OBJ - 6].pos,
		D3DXVECTOR3(15.0f, 100.0f, 20.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_NEARSIDE_OBJ - 5].pos,
		D3DXVECTOR3(20.0f, 100.0f, 15.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_NEARSIDE_OBJ - 6].pos,
		D3DXVECTOR3(20.0f, 100.0f, 15.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_RIGHTSIDE_OBJ - 5].pos,
		D3DXVECTOR3(15.0f, 100.0f, 20.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_RIGHTSIDE_OBJ - 6].pos,
		D3DXVECTOR3(15.0f, 100.0f, 20.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_FEARSIDE_OBJ - 5].pos,
		D3DXVECTOR3(20.0f, 100.0f, 15.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[BLOCK_NUM_FEARSIDE_OBJ - 6].pos,
		D3DXVECTOR3(20.0f, 100.0f, 15.0f), D3DXVECTOR3(20.0f, 20.0f, 20.0f))) return true;

	return false;
}

//================================================================================
// ブロックへのダメージ処理
// 引　数：int bno(ブロックのアドレス番号)
// 戻り値：なし
//================================================================================
void BlockDamageManager(int bno)
{
	block[bno].life -= BLOCK_DAMAGE;

	if (block[bno].life == 3)
	{
		//TextureNum = TEX_TYPE02;
	}
	else if (block[bno].life == 0)
	{
		block[bno].use = false;
		SetExplosion(block[bno].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
		SetItem(block[bno].pos);
	}
}
