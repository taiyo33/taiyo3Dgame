//=============================================================================
//
// バレットエフェクト処理 [bulletEffect.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "bulletEffect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_01				"data/TEXTURE/bullet001.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_02				"data/TEXTURE/bullet002.png"	// 読み込むテクスチャファイル名
#define	BULLETEFFECT_SIZE_X		(5.0f)							// ビルボードの幅
#define	BULLETEFFECT_SIZE_Y		(5.0f)							// ビルボードの高さ
#define MAX_BULLETEFFECT		(BULLETEFFECT_SET_MAX * BULLETEFFECT_ONESET_MAX)	// バレットエフェクトの最大数
#define DEL_TIME				(10)							// エフェクトの寿命
#define TEXTURE_MAX				(2)								// テクスチャ枚数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBulletEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBulletEffect(int index, float sizeX, float sizeY);
void SetDiffuseBulletEffect(int index, float val);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBulletEffect[TEXTURE_MAX];	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBulletEffect = NULL;			// 頂点バッファインターフェースへのポインタ

static int				CntFrame;								// フレーム数
BULLETEFFECT			BulletEffectWk[BULLETEFFECT_SET_MAX];	// BULLETEFFECT構造体用変数

//=============================================================================
// 初期化処理
// 引　数：int type(再初期化の２数判定変数)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitBulletEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLETEFFECT *bulletEffect = &BulletEffectWk[0];

	// 頂点情報の作成
	MakeVertexBulletEffect(pDevice);

	// 再初期化時は読み込まない
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_01,					// ファイルの名前
			&D3DTextureBulletEffect[TEX_NUM001]);			// 読み込むメモリー

				// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_02,					// ファイルの名前
			&D3DTextureBulletEffect[TEX_NUM002]);			// 読み込むメモリー
	}

	for (int i = 0; i < BULLETEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < BULLETEFFECT_ONESET_MAX; j++)
		{
			bulletEffect[i].pos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			bulletEffect[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			bulletEffect[i].time[j] = DEL_TIME;
			bulletEffect[i].dif[j] = INIT_ALPHA;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBulletEffect(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBulletEffect[i] != NULL)
		{// テクスチャの開放
			D3DTextureBulletEffect[i]->Release();
			D3DTextureBulletEffect[i] = NULL;
		}
	}

	if (D3DVtxBuffBulletEffect != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffBulletEffect->Release();
		D3DVtxBuffBulletEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBulletEffect(void)
{
	BULLETEFFECT *bulletEffect = &BulletEffectWk[0];

	for (int i = 0; i < BULLETEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < BULLETEFFECT_ONESET_MAX; j++)
		{
			if (bulletEffect[i].use[j])
			{
				bulletEffect[i].scl[j] -= D3DXVECTOR3(0.1f, 0.1f, 0.1f);

				bulletEffect[i].dif[j] -= 0.01f;		// 透過の値
				bulletEffect[i].time[j]--;		// 生存時間をデクリメント

				// 消滅時間になったら消滅
				if (bulletEffect[i].time[j] % DEL_TIME == 0)
				{
					bulletEffect[i].dif[j] = INIT_ALPHA;
					bulletEffect[i].use[j] = false;
					bulletEffect[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBulletEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	BULLETEFFECT *bulletEffect = &BulletEffectWk[0];

	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < BULLETEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < BULLETEFFECT_ONESET_MAX; j++)
		{
			SetDiffuseBulletEffect(i, bulletEffect[i].dif[j]);

			// ラインティングを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 通常ブレンド
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			if (bulletEffect[i].use[j])
			{
				// ビューマトリックスを取得
				mtxView = GetMtxView();

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&bulletEffect[i].mtxWorld);

				// ポリゴンを正面に向ける
				bulletEffect[i].mtxWorld._11 = mtxView._11;
				bulletEffect[i].mtxWorld._12 = mtxView._21;
				bulletEffect[i].mtxWorld._13 = mtxView._31;
				bulletEffect[i].mtxWorld._21 = mtxView._12;
				bulletEffect[i].mtxWorld._22 = mtxView._22;
				bulletEffect[i].mtxWorld._23 = mtxView._32;
				bulletEffect[i].mtxWorld._31 = mtxView._13;
				bulletEffect[i].mtxWorld._32 = mtxView._23;
				bulletEffect[i].mtxWorld._33 = mtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, bulletEffect[i].scl[j].x,
					bulletEffect[i].scl[j].y,
					bulletEffect[i].scl[j].z);
				D3DXMatrixMultiply(&bulletEffect[i].mtxWorld,
					&bulletEffect[i].mtxWorld, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, bulletEffect[i].pos[j].x,
					bulletEffect[i].pos[j].y,
					bulletEffect[i].pos[j].z);
				D3DXMatrixMultiply(&bulletEffect[i].mtxWorld,
					&bulletEffect[i].mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &bulletEffect[i].mtxWorld);

				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, D3DVtxBuffBulletEffect, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureBulletEffect[i]);

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
HRESULT MakeVertexBulletEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BULLETEFFECT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffBulletEffect,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBulletEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < MAX_BULLETEFFECT; i++, pVtx += 4)
		{
			// 頂点座標の設定
			SetVertexBulletEffect(i, BULLETEFFECT_SIZE_X, BULLETEFFECT_SIZE_Y);

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
		D3DVtxBuffBulletEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
// 引　数：int index(アドレス番号), folat sizeX(横幅), float sizeY(高さ)
// 戻り値：な　し
//=============================================================================
void SetVertexBulletEffect(int index, float sizeX, float sizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBulletEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (index * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-sizeX / 2, -sizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-sizeX / 2, sizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(sizeX / 2, -sizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(sizeX / 2, sizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffBulletEffect->Unlock();
	}
}

//============================================================================
// バレットエフェクトの透過処理
// 引　数：int index(アドレス番号), float val(透過率の値)
// 戻り値：な し
//============================================================================
void SetDiffuseBulletEffect(int index, float val)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBulletEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (index * 4);

		// 反射光の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

		// 頂点データをアンロックする
		D3DVtxBuffBulletEffect->Unlock();
	}
}

//==========================================================================
// バレットエフェクトの設置
// 引　数：D3DXVECTOR3 pos(位置), int index(組バレットのアドレス番号)
// 戻り値：な　し
//==========================================================================
void SetBulletEffect(D3DXVECTOR3 pos, int index)
{
	BULLETEFFECT *bulletEffect = &BulletEffectWk[index];

	for (int i = 0; i < BULLETEFFECT_ONESET_MAX; i++)
	{
		if (!bulletEffect->use[i])
		{
			bulletEffect->use[i] = true;	// 使用中
			bulletEffect->pos[i] = pos;		// 位置を設定

			// 頂点を設定
			SetVertexBulletEffect(i, BULLETEFFECT_SIZE_X, BULLETEFFECT_SIZE_Y);

			return;
		}
	}
}
