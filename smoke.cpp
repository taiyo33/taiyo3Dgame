//=============================================================================
//
// エフェクト処理 [smoke.cpp]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#include "smoke.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SMOKE		"data/TEXTURE/shadow.jpg"	// 読み込むテクスチャファイル名
#define	SMOKE_SIZE_X		(5.0f)						// ビルボードの幅
#define	SMOKE_SIZE_Y		(5.0f)						// ビルボードの高さ
#define SMOKE_MAX			(35)						// 煙エフェクトの最大数
#define DEL_TIME			(60)						// エフェクトの寿命

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSmoke(LPDIRECT3DDEVICE9 pDevice);
void SetVertexSmoke(int Index, float fSizeX, float fSizeY);
void SetDiffuseSmoke(int Index, float val);
void MoveSmoke(int mno);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureSmoke = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffSmoke = NULL;	// 頂点バッファインターフェースへのポインタ

float					s_curveAngle[SMOKE_MAX];	// Sinカーブの角度
static int				cnt_frame;					// フレーム数
static float			dif_mi[SMOKE_MAX];			// 透過値
SMOKE					smokeWk[SMOKE_MAX];			// SMOKE構造体用変数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSmoke(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SMOKE *smoke = &smokeWk[0];

	// 頂点情報の作成
	MakeVertexSmoke(pDevice);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_SMOKE,					// ファイルの名前
			&D3DTextureSmoke);			// 読み込むメモリー
	}

	for (int i = 0; i < SMOKE_MAX; i++)
	{
		smoke[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		smoke[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		smoke[i].time = DEL_TIME;
		smoke[i].Alpha = 0.0f;
		dif_mi[i] = INIT_ALPHA;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSmoke(void)
{
	if (D3DTextureSmoke != NULL)
	{// テクスチャの開放
		D3DTextureSmoke->Release();
		D3DTextureSmoke = NULL;
	}

	if (D3DVtxBuffSmoke != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffSmoke->Release();
		D3DVtxBuffSmoke = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSmoke(void)
{
	SMOKE *smoke = &smokeWk[0];

	for (int i = 0; i < SMOKE_MAX; i++)
	{
		if (smoke[i].use == true)
		{
			MoveSmoke(i);			// 煙エフェクトの動き

				// エフェクトの拡大
			if (smoke[i].time % 5 == 0)
			{
				smoke[i].scl += D3DXVECTOR3(0.1f, 0.1f, 0.1f);
			}

			dif_mi[i] -= 0.01f;		// 透過の値
			smoke[i].time--;		// 生存時間をデクリメント

			// 消滅時間になったら消滅
			if (smoke[i].time % DEL_TIME == 0)
			{
				dif_mi[i] = INIT_ALPHA;
				smoke[i].use = false;
				smoke[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				s_curveAngle[i] = 0.0f;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSmoke(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	SMOKE *smoke = &smokeWk[0];

	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < SMOKE_MAX; i++)
	{
		SetDiffuseSmoke(i, dif_mi[i]);

		// ラインティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// 通常ブレンド
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		if (smoke[i].use)
		{
			// ビューマトリックスを取得
			mtxView = GetMtxView();

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&smoke[i].mtxWorld);

			// ポリゴンを正面に向ける
			smoke[i].mtxWorld._11 = mtxView._11;
			smoke[i].mtxWorld._12 = mtxView._21;
			smoke[i].mtxWorld._13 = mtxView._31;
			smoke[i].mtxWorld._21 = mtxView._12;
			smoke[i].mtxWorld._22 = mtxView._22;
			smoke[i].mtxWorld._23 = mtxView._32;
			smoke[i].mtxWorld._31 = mtxView._13;
			smoke[i].mtxWorld._32 = mtxView._23;
			smoke[i].mtxWorld._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, smoke[i].scl.x,
				smoke[i].scl.y,
				smoke[i].scl.z);
			D3DXMatrixMultiply(&smoke[i].mtxWorld,
				&smoke[i].mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, smoke[i].pos.x,
				smoke[i].pos.y,
				smoke[i].pos.z);
			D3DXMatrixMultiply(&smoke[i].mtxWorld,
				&smoke[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &smoke[i].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, D3DVtxBuffSmoke, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTextureSmoke);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
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
HRESULT MakeVertexSmoke(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SMOKE_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffSmoke,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffSmoke->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < SMOKE_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			SetVertexSmoke(i, SMOKE_SIZE_X, SMOKE_SIZE_Y);

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
		D3DVtxBuffSmoke->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexSmoke(int Index, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffSmoke->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffSmoke->Unlock();
	}
}

//============================================================================
// 煙エフェクトの透過処理
//============================================================================
void SetDiffuseSmoke(int Index, float val)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffSmoke->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// 反射光の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

		// 頂点データをアンロックする
		D3DVtxBuffSmoke->Unlock();
	}
}

//==========================================================================
// 煙エフェクトの設置
// 引　数：D3DXVECTOR3 pos(位置), D3DXVECTOR3 rot(回転), float Dest(距離)
// 戻り値：bool型　使用中なら false, 未使用ならtrue　
//==========================================================================
bool SetSmoke(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest)
{
	SMOKE *smoke = &smokeWk[0];

	for (int i = 0; i < SMOKE_MAX; i++)
	{
		if (!smoke[i].use)
		{
			smoke[i].use = true;	// 使用中
			smoke[i].pos.x = pos.x + sinf(rot.y) * Dest;
			smoke[i].pos.z = pos.z + cosf(rot.y) * Dest;
			smoke[i].pos.y = pos.y;

			SetVertexSmoke(i, 5.0f, 5.0f);

			return true;
		}
	}

	return false;
}

//==========================================================================
// 煙エフェクトのゆらめきの制御
// 引　数： int mno(煙エフェクトのアドレス番号)
// 戻り値： なし
//==========================================================================
void MoveSmoke(int mno)
{
	SMOKE *smoke = &smokeWk[mno];	// アドレスの取得

	// ゆらぎの値をランダムで指定
	s_curveAngle[mno] = (float)(rand() % 101);

	// ゆらぎの反映
	smoke[mno].pos.x += sinf(s_curveAngle[mno]);
	smoke[mno].pos.y += 0.2f;
	smoke[mno].pos.z += cosf(s_curveAngle[mno]);

}

