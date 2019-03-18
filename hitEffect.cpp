//=============================================================================
//
// エフェクト処理 [hitEffect.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "hitEffect.h"
#include "ball.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_HITEFFECT01		"data/TEXTURE/bullet001.png"	// 読み込むテクスチャファイル名
#define	HITEFFECT_SIZE_X		(25.0f)							// ビルボードの幅
#define	HITEFFECT_SIZE_Y		(50.0f)							// ビルボードの高さ
#define INIT_SPEED				(2.0f)							// 
#define TEXTURE_MAX				(2)								// テクスチャーの最大数					

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexHitEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexHitEffect(int Index, float fSizeX, float fSizeY);
void SetDiffuseHitEffect(int Index, float val);
void MoveHitEffect(int index, int hno);
void TyouseiHitEffect(int index, int hno);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureHitEffect[TEXTURE_MAX];	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DTVtxBuffHitEffect = NULL;		// 頂点バッファインターフェースへのポインタ

HITEFFECT				hitEffectWk[HITEFFECT_SET_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHitEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HITEFFECT *hitEffect = &hitEffectWk[0];

	// 頂点情報の作成
	MakeVertexHitEffect(pDevice);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_HITEFFECT01,					// ファイルの名前
			&D3DTextureHitEffect[TEX_NUM001]);			// 読み込むメモリー

	}

	for (int i = 0; i < HITEFFECT_SET_MAX ; i++)
	{
		hitEffect[i].use = false;

		for (int j = 0; j < HITEFFECT_ONESET_MAX; j++)
		{
			hitEffect[i].pos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			hitEffect[i].move[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			hitEffect[i].speed[j] = INIT_SPEED;
			hitEffect[i].dif_mi[j] = INIT_ALPHA;
			hitEffect[i].angle[j] = 0.0f;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHitEffect(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureHitEffect[i] != NULL)
		{// テクスチャの開放
			D3DTextureHitEffect[i]->Release();
			D3DTextureHitEffect[i] = NULL;
		}
	}
	if(D3DTVtxBuffHitEffect != NULL)
	{// 頂点バッファの開放
		D3DTVtxBuffHitEffect->Release();
		D3DTVtxBuffHitEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHitEffect(void)
{
	HITEFFECT *hitEffect = &hitEffectWk[0];

	for (int i = 0; i < HITEFFECT_SET_MAX; i++)
	{
		if (hitEffect[i].use)
		{
			for (int j = 0; j < HITEFFECT_ONESET_MAX; j++)
			{
				hitEffect[i].move[j].x = cosf(hitEffect[i].angle[j]) * hitEffect[i].speed[j];
				hitEffect[i].move[j].y = sinf(hitEffect[i].angle[j]) * hitEffect[i].speed[j];

				MoveHitEffect(i, j);
				
				TyouseiHitEffect(i, j);

				// 消滅時間になったら消滅
				if (hitEffect[i].speed[j] < 0.0f)
				{
					hitEffect[i].use = false;
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	HITEFFECT *hitEffect = &hitEffectWk[0];
	
	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < HITEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < HITEFFECT_ONESET_MAX; j++)
		{
			// 透過処理
			SetDiffuseHitEffect(i, hitEffect[i].dif_mi[j]);

			if (hitEffect[i].use)
			{
				// ビューマトリックスを取得
				mtxView = GetMtxView();

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&hitEffect[i].mtxWorld);

				// ポリゴンを正面に向ける
				hitEffect[i].mtxWorld._11 = mtxView._11;
				hitEffect[i].mtxWorld._12 = mtxView._21;
				hitEffect[i].mtxWorld._13 = mtxView._31;
				hitEffect[i].mtxWorld._21 = mtxView._12;
				hitEffect[i].mtxWorld._22 = mtxView._22;
				hitEffect[i].mtxWorld._23 = mtxView._32;
				hitEffect[i].mtxWorld._31 = mtxView._13;
				hitEffect[i].mtxWorld._32 = mtxView._23;
				hitEffect[i].mtxWorld._33 = mtxView._33;

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, hitEffect[i].pos[j].x,
					hitEffect[i].pos[j].y,
					hitEffect[i].pos[j].z);
				D3DXMatrixMultiply(&hitEffect[i].mtxWorld,
					&hitEffect[i].mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &hitEffect[i].mtxWorld);

				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, D3DTVtxBuffHitEffect, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureHitEffect[TEX_NUM001]);

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
HRESULT MakeVertexHitEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * HITEFFECT_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&D3DTVtxBuffHitEffect,	// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DTVtxBuffHitEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < HITEFFECT_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HITEFFECT_SIZE_X / 2, -HITEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-HITEFFECT_SIZE_X / 2, HITEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(HITEFFECT_SIZE_X / 2, -HITEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HITEFFECT_SIZE_X / 2, HITEFFECT_SIZE_Y / 2, 0.0f);

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
		D3DTVtxBuffHitEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexHitEffect(int Index, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DTVtxBuffHitEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DTVtxBuffHitEffect->Unlock();
	}
}

//============================================================================
//
//============================================================================
void SetDiffuseHitEffect(int Index, float val)
{	
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DTVtxBuffHitEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);
		
		// 反射光の設定
		pVtx[0].diffuse = 
		pVtx[1].diffuse = 
		pVtx[2].diffuse = 
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

		// 頂点データをアンロックする
		D3DTVtxBuffHitEffect->Unlock();
	}
}

//==========================================================================
// 被弾エフェクトの設置処理 
// 引　数：D3DXVECTOR3 pos(発生位置)、D3DXVECTOR3 rot(回転量)、
//		   float Dest(距離)、int index(組アドレス番号)
// 戻り値；な　し
//==========================================================================
void SetHitEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest)
{
	HITEFFECT *hitEffect = &hitEffectWk[0];

	for (int i = 0; i < HITEFFECT_SET_MAX; i++)
	{
		if (!hitEffect[i].use)
		{
			for (int j = 0; j < HITEFFECT_ONESET_MAX; j++)
			{
				hitEffect[i].use = true;	// 使用中
				hitEffect[i].pos[j] = pos;
				hitEffect[i].dif_mi[j] = INIT_ALPHA;
				hitEffect[i].speed[j] = INIT_SPEED;
				SetVertexHitEffect(i, HITEFFECT_SIZE_X, HITEFFECT_SIZE_Y);
				hitEffect[i].angle[j] = atan2f(pos.y, pos.x);			// 角度計算
				hitEffect[i].angle[j] = hitEffect[i].angle[j] + (2.0f * D3DX_PI) * ((float)j / HITEFFECT_ONESET_MAX);	// 12分割 30度間隔
			}
		}

		return;
	}
}

//==========================================================================
// 被弾エフェクトの移動処理 
// 引　数：int index(被弾エフェクトのアドレス番号)、int index(組のアドレス番号)
// 戻り値；な　し
//==========================================================================
void MoveHitEffect(int index, int hno)
{
	HITEFFECT *hitEffect = &hitEffectWk[index];
	// 位置移動
	hitEffect->pos[hno].x += hitEffect->move[hno].x;
	hitEffect->pos[hno].y += hitEffect->move[hno].y;
	hitEffect->pos[hno].z += hitEffect->move[hno].z;
}

//==========================================================================
// 被弾エフェクトの移動処理 
// 引　数：int index(被弾エフェクトのアドレス番号)、int index(組のアドレス番号)
// 戻り値；な　し
//==========================================================================
void TyouseiHitEffect(int index, int hno)
{
	HITEFFECT *hitEffect = &hitEffectWk[index];
	hitEffect->dif_mi[hno] -= 0.01f;	// 透過の値
	hitEffect->speed[hno] -= 0.1f;		// 移動速度の減速
}