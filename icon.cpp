//=============================================================================
//
// アイコン処理 [icon.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "icon.h"
#include "player.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_ICON01			"data/TEXTURE/player_icon01.png"	// 読み込むテクスチャー
#define TEXTURE_ICON02			"data/TEXTURE/player_icon02.png"	// 読み込むテクスチャー
#define ICON_TEXTURE_SIZE		(75)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexIcon(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9			D3DModelTexture;		// テクスチャ読み込み場所
static LPDIRECT3DTEXTURE9			D3DTexture[ICON_MAX];	// テクスチャ読み込み場所
static LPDIRECT3DVERTEXBUFFER9		D3DVtxBuff;				// 頂点バッファインターフェースへのポインタ

ICON								iconWk[ICON_MAX];
//=============================================================================
// 初期化処理
// 引　数：int type(再初期化２数判定値)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitIcon(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ICON *icon = &iconWk[0];

	// 頂点情報の作成
	MakeVertexIcon(pDevice);

	for (int i = 0; i < ICON_MAX; i++)
	{
		icon[i].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
		icon[i].use = true;
	}


	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_ICON01,		// ファイルの名前
			&D3DTexture[P1]);	// 読み込むメモリー

			// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_ICON02,		// ファイルの名前
			&D3DTexture[P2]);	// 読み込むメモリー
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitIcon(void)
{
	for (int i = 0; i < ICON_MAX; i++)
	{
		if (D3DTexture[i] != NULL)
		{// テクスチャの開放
			D3DTexture[i]->Release();
			D3DTexture[i] = NULL;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateIcon(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawIcon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxView;
	ICON *icon = &iconWk[0];
	PLAYER *player = GetPlayer(0);

	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < ICON_MAX; i++, player++)
	{
		if (icon[i].use)
		{
			mtxView = GetMtxView();

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&icon[i].mtxWorld);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, icon[i].pos.x, icon[i].pos.y, icon[i].pos.z);
			D3DXMatrixMultiply(&icon[i].mtxWorld, &icon[i].mtxWorld, &mtxTranslate);

			D3DXMatrixMultiply(&icon[i].mtxWorld, &icon[i].mtxWorld, &player->mtxWorld);

			// ポリゴンを正面に向ける
			icon[i].mtxWorld._11 = mtxView._11;
			icon[i].mtxWorld._12 = mtxView._21;
			icon[i].mtxWorld._13 = mtxView._31;
			icon[i].mtxWorld._21 = mtxView._12;
			icon[i].mtxWorld._22 = mtxView._22;
			icon[i].mtxWorld._23 = mtxView._32;
			icon[i].mtxWorld._31 = mtxView._13;
			icon[i].mtxWorld._32 = mtxView._23;
			icon[i].mtxWorld._33 = mtxView._33;

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &icon[i].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, D3DVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTexture[i]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * ICON_MAX), NUM_POLYGON);
		}
	}

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z比較あり
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//===============================================================================
// 頂点情報の作成
//===============================================================================
HRESULT MakeVertexIcon(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * ICON_MAX ,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuff,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < ICON_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-ICON_TEXTURE_SIZE / 2, ICON_TEXTURE_SIZE / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(ICON_TEXTURE_SIZE / 2, ICON_TEXTURE_SIZE / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-ICON_TEXTURE_SIZE / 2, -ICON_TEXTURE_SIZE / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(ICON_TEXTURE_SIZE / 2, -ICON_TEXTURE_SIZE / 2, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}
		// 頂点データをアンロックする
		D3DVtxBuff->Unlock();
	}

	return S_OK;
}
