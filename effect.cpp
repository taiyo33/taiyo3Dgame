//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "effect.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "debugproc.h"
#include "child.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EFFECT		"data/TEXTURE/bullet001.png"	// 読み込むテクスチャファイル名
#define	EFFECT_SIZE_X		(100.0f)							// ビルボードの幅
#define	EFFECT_SIZE_Y		(100.0f)							// ビルボードの高さ
#define EFFECT_MAX			(25)
#define MAX_EFFECT			(256)
#define DEL_TIME			(60)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(int Index, float fSizeX, float fSizeY);
void SetDiffuseEffect(int Index, float val);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureEffect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DTVtxBuffEffect = NULL;	// 頂点バッファインターフェースへのポインタ

static int				cnt_frame;

EFFECT					effectWk[EFFECT_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *effect = &effectWk[0];

	// 頂点情報の作成
	MakeVertexEffect(pDevice);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_EFFECT,					// ファイルの名前
			&D3DTextureEffect);			// 読み込むメモリー
	}

	for (int i = 0; i < EFFECT_MAX ; i++)
	{
		for (int j = 0; j < DELETEFFECT_ONESET_MAX; j++)
		{
			effect[i].pos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			effect[i].time[j] = DEL_TIME;
			effect[i].dif_mi[j] = INIT_ALPHA;
		}
	}
	cnt_frame = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	if(D3DTextureEffect != NULL)
	{// テクスチャの開放
		D3DTextureEffect->Release();
		D3DTextureEffect = NULL;
	}

	if(D3DTVtxBuffEffect != NULL)
	{// 頂点バッファの開放
		D3DTVtxBuffEffect->Release();
		D3DTVtxBuffEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	EFFECT *effect = &effectWk[0];

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		for (int j = 0; j < DELETEFFECT_ONESET_MAX; j++)
		{
			if (effect[i].use[j])
			{
				effect[i].dif_mi[j] -= 0.01f;		// 透過の値
				effect[i].time[j]--;	// 生存時間をデクリメント

				// 消滅時間になったら消滅
				if (effect[i].time[j] % DEL_TIME == 0)
				{
					effect[i].dif_mi[j] = INIT_ALPHA;
					effect[i].use[j] = false;
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	EFFECT *effect = &effectWk[0];
	
	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		for (int j = 0; j < DELETEFFECT_ONESET_MAX; j++)
		{
			SetDiffuseEffect(i, effect[i].dif_mi[j]);

			// ラインティングを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 通常ブレンド
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			if (effect[i].use[j])
			{
				// ビューマトリックスを取得
				mtxView = GetMtxView();

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&effect[i].mtxWorld);

				// ポリゴンを正面に向ける
				effect[i].mtxWorld._11 = mtxView._11;
				effect[i].mtxWorld._12 = mtxView._21;
				effect[i].mtxWorld._13 = mtxView._31;
				effect[i].mtxWorld._21 = mtxView._12;
				effect[i].mtxWorld._22 = mtxView._22;
				effect[i].mtxWorld._23 = mtxView._32;
				effect[i].mtxWorld._31 = mtxView._13;
				effect[i].mtxWorld._32 = mtxView._23;
				effect[i].mtxWorld._33 = mtxView._33;

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, effect[i].pos[j].x,
					effect[i].pos[j].y,
					effect[i].pos[j].z);
				D3DXMatrixMultiply(&effect[i].mtxWorld,
					&effect[i].mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &effect[i].mtxWorld);

				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, D3DTVtxBuffEffect, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureEffect);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
			}
		}
	}

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);

	// Z比較あり
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&D3DTVtxBuffEffect,	// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DTVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < MAX_EFFECT; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);

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
		D3DTVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEffect(int Index, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DTVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DTVtxBuffEffect->Unlock();
	}
}

//============================================================================
//
//============================================================================
void SetDiffuseEffect(int Index, float val)
{	
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DTVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);
		
		// 反射光の設定
		pVtx[0].diffuse = 
		pVtx[1].diffuse = 
		pVtx[2].diffuse = 
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

		// 頂点データをアンロックする
		D3DTVtxBuffEffect->Unlock();
	}
}

//==========================================================================
//
//==========================================================================
bool SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest,int index)
{
	EFFECT *effect = &effectWk[index];

	for (int i = 0; i < DELETEFFECT_ONESET_MAX ; i++)
	{
		if (!effect->use[i])
		{
			effect->use[i] = true;	// 使用中
			effect->pos[i] = pos;
			SetVertexEffect(i, 5.0f, 5.0f);

			return true;
		}
	}
	return false;
}
