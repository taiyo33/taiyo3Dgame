//=============================================================================
//
// チャージエフェクト処理 [chargeEffect.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "chargeEffect.h"
#include "ball.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_01				"data/TEXTURE/bullet001.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_02				"data/TEXTURE/bullet002.png"	// 読み込むテクスチャファイル名
#define	CHARGEEFFECT_SIZE_X		(50.0f)							// ビルボードの幅
#define	CHARGEEFFECT_SIZE_Y		(50.0f)							// ビルボードの高さ
#define TEXTURE_MAX				(2)								// テクスチャーの最大数					

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexChargeEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexChargeEffect(int index, float sizeX, float sizeY);
void SetDiffuseChargeEffect(int index, float val);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureChargeEffect[TEXTURE_MAX];	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DTVtxBuffChargeEffect = NULL;			// 頂点バッファインターフェースへのポインタ


CHARGEEFFECT			ChargeEffectWk[CHARGEEFFECT_SET_MAX];
//=============================================================================
// 初期化処理
// 引　数：int type(再初期化時の2数判定変数)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitChargeEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CHARGEEFFECT *chargeEffect = &ChargeEffectWk[0];

	// 頂点情報の作成
	MakeVertexChargeEffect(pDevice);

	// 再初期化時は読み込まない
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_01,					// ファイルの名前
			&D3DTextureChargeEffect[TEX_NUM001]);			// 読み込むメモリー
				// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_02,					// ファイルの名前
			&D3DTextureChargeEffect[TEX_NUM002]);			// 読み込むメモリー
	}

	// 各変数の初期化処理
	for (int i = 0; i < CHARGEEFFECT_SET_MAX ; i++)
	{
		for (int j = 0; j < CHARGEEFFECT_ONESET_MAX; j++)
		{
			chargeEffect[i].use[j] = false;
			chargeEffect[i].pos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			chargeEffect[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			chargeEffect[i].dif[j] = INIT_ALPHA;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitChargeEffect(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureChargeEffect[i] != NULL)
		{// テクスチャの開放
			D3DTextureChargeEffect[i]->Release();
			D3DTextureChargeEffect[i] = NULL;
		}
	}

	if(D3DTVtxBuffChargeEffect != NULL)
	{// 頂点バッファの開放
		D3DTVtxBuffChargeEffect->Release();
		D3DTVtxBuffChargeEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateChargeEffect(void)
{
	CHARGEEFFECT *chargeEffect = &ChargeEffectWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < CHARGEEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < CHARGEEFFECT_ONESET_MAX; j++)
		{
			if (chargeEffect[i].use[j])
			{
				// 位置をプレイヤーへ
				chargeEffect[i].pos[j] = player[i].pos;	

				// 偶数の場合エフェクトを拡大
				if (j % 2 == 0)
				{
					// 拡大して透過
					chargeEffect[i].scl[j] += D3DXVECTOR3(0.01f, 0.01f, 0.01f);
				}

				// 消滅処理
				if (chargeEffect[i].dif[j] < 0.0f)
				{
					chargeEffect[i].use[j] = false;
				}

				// 透過値を減らす
				chargeEffect[i].dif[j] -= 0.01f;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawChargeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX *mtxView, mtxScale, mtxTranslate;
	CHARGEEFFECT *chargeEffect = &ChargeEffectWk[0];
	
	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < CHARGEEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < CHARGEEFFECT_ONESET_MAX; j++)
		{
			// 透過処理
			SetDiffuseChargeEffect(i, chargeEffect[i].dif[j]);

			if (chargeEffect[i].use[j])
			{
				// ビューマトリックスを取得
				mtxView = GetMtxView();

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&chargeEffect[i].mtxWorld);

				// ポリゴンを正面に向ける
				chargeEffect[i].mtxWorld._11 = mtxView->_11;
				chargeEffect[i].mtxWorld._12 = mtxView->_21;
				chargeEffect[i].mtxWorld._13 = mtxView->_31;
				chargeEffect[i].mtxWorld._21 = mtxView->_12;
				chargeEffect[i].mtxWorld._22 = mtxView->_22;
				chargeEffect[i].mtxWorld._23 = mtxView->_32;
				chargeEffect[i].mtxWorld._31 = mtxView->_13;
				chargeEffect[i].mtxWorld._32 = mtxView->_23;
				chargeEffect[i].mtxWorld._33 = mtxView->_33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, chargeEffect[i].scl[j].x, chargeEffect[i].scl[j].y, chargeEffect[i].scl[j].z);
				D3DXMatrixMultiply(&chargeEffect[i].mtxWorld, &chargeEffect[i].mtxWorld, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, chargeEffect[i].pos[j].x,
					chargeEffect[i].pos[j].y,
					chargeEffect[i].pos[j].z);
				D3DXMatrixMultiply(&chargeEffect[i].mtxWorld,
					&chargeEffect[i].mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &chargeEffect[i].mtxWorld);

				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, D3DTVtxBuffChargeEffect, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureChargeEffect[i]);

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
HRESULT MakeVertexChargeEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_CHARGEEFFECT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&D3DTVtxBuffChargeEffect,	// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DTVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < MAX_CHARGEEFFECT; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-CHARGEEFFECT_SIZE_X / 2, -CHARGEEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-CHARGEEFFECT_SIZE_X / 2, CHARGEEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(CHARGEEFFECT_SIZE_X / 2, -CHARGEEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(CHARGEEFFECT_SIZE_X / 2, CHARGEEFFECT_SIZE_Y / 2, 0.0f);

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
		D3DTVtxBuffChargeEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
// 引　数：int index(アドレス番号), float sizeX(横幅), float sizeY(高さ)
// 戻り値：な　し
//=============================================================================
void SetVertexChargeEffect(int index, float sizeX, float sizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DTVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (index * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-sizeX / 2, -sizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-sizeX / 2, sizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(sizeX / 2, -sizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(sizeX / 2, sizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DTVtxBuffChargeEffect->Unlock();
	}
}

//============================================================================
// 頂点カラーの透過処理
// 引　数：int index(アドレス番号), float val(透過の値)
// 戻り値：な　し
//============================================================================
void SetDiffuseChargeEffect(int index, float val)
{	
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DTVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (index * 4);
		
		// 反射光の設定
		pVtx[0].diffuse = 
		pVtx[1].diffuse = 
		pVtx[2].diffuse = 
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

		// 頂点データをアンロックする
		D3DTVtxBuffChargeEffect->Unlock();
	}
}

//==========================================================================
// 被弾チャージエフェクトの設置処理 
// 引　数：D3DXVECTOR3 pos(発生位置), int index(組アドレス番号)
// 戻り値；な　し
//==========================================================================
void SetChargeEffect(D3DXVECTOR3 pos, int index)
{
	CHARGEEFFECT *chargeEffect = &ChargeEffectWk[index];

	
	for (int i = 0; i < CHARGEEFFECT_ONESET_MAX; i++)
	{
		if (!chargeEffect->use[i])
		{
			// 各変数を初期化
			chargeEffect->use[i] = true;
			chargeEffect->pos[i] = pos;
			chargeEffect->scl[i] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			chargeEffect->dif[i] = INIT_ALPHA;
			SetVertexChargeEffect(i, CHARGEEFFECT_SIZE_X, CHARGEEFFECT_SIZE_Y);

			return;
		}
	}
}
