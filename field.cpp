//=============================================================================
//
// 地面処理 [field.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "field.h"
#include "ai.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/field003.jpg"	// 読み込むテクスチャファイル名
#define FIELD_MAX		(9)							// フィールドの枚数
#define FIELD_SIZE_X	(125.0f)					// 地面の初期位置
#define FIELD_SIZE_Y	(0.0f)						//
#define FIELD_SIZE_Z	(100.0f)					//			

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);
void InitPosField(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureField = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffField = NULL;	// 頂点バッファへのポインタ


D3DXMATRIX				MtxWorldField;			// ワールドマトリックス
D3DXVECTOR3				PosField[FIELD_MAX];	// 現在の位置
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	HRESULT hr;

	InitPosField();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FIELD,			// ファイルの名前
								&D3DTextureField);	// 読み込むメモリー
	}

	// 頂点情報の作成
	hr = MakeVertexField(pDevice);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	if(D3DTextureField != NULL)
	{// テクスチャの開放
		D3DTextureField->Release();
		D3DTextureField = NULL;
	}

	if(D3DVtxBuffField != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffField->Release();
		D3DVtxBuffField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate;

	for (int i = 0; i < FIELD_MAX; i++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&MtxWorldField);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, PosField[i].x, PosField[i].y, PosField[i].z);
		D3DXMatrixMultiply(&MtxWorldField, &MtxWorldField, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &MtxWorldField);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, D3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureField);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * FIELD_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffField,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{	
		//頂点バッファの中身を埋める
		VERTEX_3D			   *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < FIELD_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-FIELD_SIZE_X, FIELD_SIZE_Y, FIELD_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(FIELD_SIZE_X, FIELD_SIZE_Y, FIELD_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-FIELD_SIZE_X, FIELD_SIZE_Y, -FIELD_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(FIELD_SIZE_X, FIELD_SIZE_Y, -FIELD_SIZE_Z);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// 頂点データをアンロックする
		D3DVtxBuffField->Unlock();
	}

	return S_OK;
}

//=============================================================================
// フィールドの初期位置
// 引　数：な　し
// 戻り値：な　し
//=============================================================================
void InitPosField(void)
{
	PosField[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PosField[1] = D3DXVECTOR3(FIELD_SIZE_X + FIELD_SIZE_X, 0.0f, FIELD_SIZE_Z + FIELD_SIZE_Z);
	PosField[2] = D3DXVECTOR3(-FIELD_SIZE_X - FIELD_SIZE_X, 0.0f, FIELD_SIZE_Z + FIELD_SIZE_Z);
	PosField[3] = D3DXVECTOR3(FIELD_SIZE_X + FIELD_SIZE_X, 0.0f, -FIELD_SIZE_Z - FIELD_SIZE_Z);
	PosField[4] = D3DXVECTOR3(-FIELD_SIZE_X - FIELD_SIZE_X, 0.0f, -FIELD_SIZE_Z - FIELD_SIZE_Z);
	PosField[5] = D3DXVECTOR3(FIELD_SIZE_X + FIELD_SIZE_X, 0.0f, 0.0f);
	PosField[6] = D3DXVECTOR3(-FIELD_SIZE_X - FIELD_SIZE_X, 0.0f, 0.0f);
	PosField[7] = D3DXVECTOR3(0.0f, 0.0f, FIELD_SIZE_Z + FIELD_SIZE_Z);
	PosField[8] = D3DXVECTOR3(0.0f, 0.0f, -FIELD_SIZE_Z - FIELD_SIZE_Z);
}

//=============================================================================
// フィールドの初期位置
// 引　数：な　し
// 戻り値：な　し
//=============================================================================
void SetCollarField(int fno, float colR, float colB, float colG)
{
	//頂点バッファの中身を埋める
	VERTEX_3D			   *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (fno * 4);
	
	// 反射光の設定
	pVtx[0].diffuse = D3DXCOLOR(colR, colB, colG, 1.0f);
	pVtx[1].diffuse = D3DXCOLOR(colR, colB, colG, 1.0f);
	pVtx[2].diffuse = D3DXCOLOR(colR, colB, colG, 1.0f);
	pVtx[3].diffuse = D3DXCOLOR(colR, colB, colG, 1.0f);

	// 頂点データをアンロックする
	D3DVtxBuffField->Unlock();

}