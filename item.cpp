//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "item.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "hitEffect.h"
#include "checkhit.h"
#include "explosion.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	ITEM_TEXTURE001			"data/TEXTURE/hp_up.png"	// 読み込むモデル名
#define	ITEM_TEXTURE002			"data/TEXTURE/charge_up.png"	// 読み込むモデル名
#define	ITEM_TEXTURE003			"data/TEXTURE/speed_up.png"	// 読み込むモデル名
#define TEXTURE_MAX				(5)
#define ITEM_VERTEX				(4)			// 
#define ITEM_SIZE				(15.0f)		// アイテムの1辺のサイズ
#define INIT_LIFE				(5)			// 寿命の初期値	

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);
void SetVertexItem(int Index, float fSizeX, float fSizeY);

enum {
	TEX_TYPE001,
	TEX_TYPE002,
	TEX_TYPE003
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffItem;						// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		D3DTextureItem[TEXTURE_MAX];		// テクスチャへのポインタ

ITEM					itemWk[ITEM_MAX];		// アイテム構造体
//=============================================================================
// 初期化処理
// 引　数：int type(再初期化２数判定値)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitItem(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ITEM *item = &itemWk[0];

	// 頂点情報の作成
	MakeVertexItem(pDevice);
	
	for (int i = 0; i < ITEM_MAX; i++)
	{
		item[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		item[i].use = false;		// 使用へ初期化
		item[i].textureNum = TEX_TYPE002;
	}
	
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
							ITEM_TEXTURE001,	// ファイルの名前
						&D3DTextureItem[TEX_TYPE001]);	// 読み込むメモリー
		
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			ITEM_TEXTURE002,	// ファイルの名前
			&D3DTextureItem[TEX_TYPE002]);	// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
			ITEM_TEXTURE003,	// ファイルの名前
			&D3DTextureItem[TEX_TYPE003]);	// 読み込むメモリー

	}	
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureItem[i] != NULL)
		{// テクスチャの開放
			D3DTextureItem[i]->Release();
			D3DTextureItem[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxView;
	ITEM *item = &itemWk[0];

	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for (int i = 0; i < ITEM_MAX; i++)
	{
		// ラインティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (item[i].use)
		{
			// ビューマトリックスを取得
			mtxView = GetMtxView();

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&item[i].mtxWorld);
		
			// ポリゴンを正面に向ける
			item[i].mtxWorld._11 = mtxView._11;
			item[i].mtxWorld._12 = mtxView._21;
			item[i].mtxWorld._13 = mtxView._31;
			item[i].mtxWorld._21 = mtxView._12;
			item[i].mtxWorld._22 = mtxView._22;
			item[i].mtxWorld._23 = mtxView._32;
			item[i].mtxWorld._31 = mtxView._13;
			item[i].mtxWorld._32 = mtxView._23;
			item[i].mtxWorld._33 = mtxView._33;

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, item[i].pos.x, item[i].pos.y, item[i].pos.z);
			D3DXMatrixMultiply(&item[i].mtxWorld, &item[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &item[i].mtxWorld);
			
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, D3DVtxBuffItem, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTextureItem[item[i].textureNum]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
		}
	}

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z比較あり
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);


}

//=============================================================================
// アイテムのアドレス取得
//=============================================================================
ITEM *GetItem(int bno)
{
	return &itemWk[bno];
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * ITEM_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffItem,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < ITEM_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-ITEM_SIZE, -ITEM_SIZE, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-ITEM_SIZE, ITEM_SIZE, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(ITEM_SIZE, -ITEM_SIZE, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(ITEM_SIZE, ITEM_SIZE, 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		}

		// 頂点データをアンロックする
		D3DVtxBuffItem->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexItem(int Index, float fSizeX, float fSizeY)
{
	ITEM *item = &itemWk[0];

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffItem->Unlock();
	}
}


//=============================================================================
// アイテムの透明ポリゴンとの当たり判定
// pos0:始点（移動前）
// pos1:終点（移動後）
//=============================================================================
void SetItem(D3DXVECTOR3 pos)
{
	ITEM *item = &itemWk[0];

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (!item[i].use)
		{
			if (rand() % 10 == 0)
			{
				item[i].use = true;
				item[i].pos = pos;
				item[i].textureNum = TEX_TYPE002;
				SetVertexItem(i, ITEM_SIZE, ITEM_SIZE);


				return;
			}
		}
	}
}


////=============================================================================
//// アイテムの透明ポリゴンとの当たり判定
//// pos0:始点（移動前）
//// pos1:終点（移動後）
////=============================================================================
//bool HitCheckItem(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	D3DXVECTOR3		pos[NUM_VERTEX]; // 頂点座標の保存
//	bool			ans;
//	VERTEX_3D	    *pVtx;
//
//	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//	D3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int i = 0; i < ITEM_MAX; i++, pVtx += 4)
//	{
//		if (item[i].use)
//		{
//			// 頂点座標の設定
//			pos[0] = pVtx[0].vtx;
//			pos[1] = pVtx[1].vtx;
//			pos[2] = pVtx[2].vtx;
//			pos[3] = pVtx[3].vtx;
//
//			// 左下側ポリゴンと線分の当たり判定
//			ans = CheckHitPolygon(pos[0], pos[2], pos[3], pos0, pos1);
//			if (!ans)
//			{
//				break;
//			}
//			// 右上側ポリゴンと線分の当たり判定
//			ans = CheckHitPolygon(pos[0], pos[3], pos[1], pos0, pos1);
//			if (!ans)
//			{
//				break;
//			}
//		}
//	}
//
//	// 頂点データをアンロックする
//	D3DVtxBuffItem->Unlock();
//
//	return ans;
//}
