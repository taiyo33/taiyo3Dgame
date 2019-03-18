//=============================================================================
//
// ボール消滅エフェクト処理 [explosion.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "explosion.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EXPLOSION		"data/TEXTURE/explosion000.png"	// 読み込むテクスチャファイル名
#define	EXPLOSION_SIZE			(50.0f)							// ビルボードの幅
#define MAX_EXPLOSION			(60)							// 最大数
#define DEL_TIME				(60)							// 消滅時間
#define TEXTURE_PATTERN_X		(8)								// テクスチャー分割数X方向
#define TEXTURE_PATTERN_Y		(2)								// テクスチャー分割数Y方向
#define ANIM_TIME				(8)								// アニメーションの間隔
#define ANIM_STOP				(7)								// アニメーションの停止
#define ANIM_PATTERN_NUM		(TEXTURE_PATTERN_X * TEXTURE_PATTERN_Y)	// 

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);
void SetVertexExplosion(int index, float Size);
void SetDiffuseExplosion(int index, float val);
void SetTextureExplosion(int index, int cntPattern);
void TextureAnim(int index);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureExplosion = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffExplosion = NULL;	// 頂点バッファインターフェースへのポインタ

EXPLOSION				ExplosionWk[MAX_EXPLOSION]; 
//=============================================================================
// 初期化処理
// 引　数：int type(再初期化時の2数判定変数)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitExplosion(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EXPLOSION *explosion = &ExplosionWk[0];

	// 頂点情報の作成
	MakeVertexExplosion(pDevice);

	// 再初期化時は読み込まない
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_EXPLOSION,					// ファイルの名前
			&D3DTextureExplosion);			// 読み込むメモリー
	}

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		explosion[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		explosion[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 大きさの初期化
		explosion[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転量の初期化
		explosion[i].cntAnim = 0;							// アニメーションカウントの初期化
		explosion[i].patternAnim = 0;						// アニメーションパターンの初期化
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitExplosion(void)
{
	if (D3DTextureExplosion != NULL)
	{// テクスチャの開放
		D3DTextureExplosion->Release();
		D3DTextureExplosion = NULL;
	}

	if (D3DVtxBuffExplosion != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffExplosion->Release();
		D3DVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateExplosion(void)
{
	EXPLOSION *explosion = &ExplosionWk[0];

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (explosion[i].use)
		{
			TextureAnim(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxRot, mtxTranslate;
	EXPLOSION *explosion = &ExplosionWk[0];

	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 254);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		// ラインティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (explosion[i].use)
		{
			// ビューマトリックスを取得
			mtxView = GetMtxView();

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&explosion[i].mtxWorld);

			// ポリゴンを正面に向ける
			explosion[i].mtxWorld._11 = mtxView._11;
			explosion[i].mtxWorld._12 = mtxView._21;
			explosion[i].mtxWorld._13 = mtxView._31;
			explosion[i].mtxWorld._21 = mtxView._12;
			explosion[i].mtxWorld._22 = mtxView._22;
			explosion[i].mtxWorld._23 = mtxView._32;
			explosion[i].mtxWorld._31 = mtxView._13;
			explosion[i].mtxWorld._32 = mtxView._23;
			explosion[i].mtxWorld._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, explosion[i].scl.x, explosion[i].scl.y, explosion[i].scl.z);
			D3DXMatrixMultiply(&explosion[i].mtxWorld, &explosion[i].mtxWorld, &mtxScale);
			
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, explosion[i].rot.y, explosion[i].rot.x, explosion[i].rot.z);
			D3DXMatrixMultiply(&explosion[i].mtxWorld, &explosion[i].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, explosion[i].pos.x, explosion[i].pos.y, explosion[i].pos.z);
			D3DXMatrixMultiply(&explosion[i].mtxWorld, &explosion[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &explosion[i].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, D3DVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTextureExplosion);

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
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EXPLOSION,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffExplosion,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	EXPLOSION *explosion = &ExplosionWk[0];
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EXPLOSION; i++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-EXPLOSION_SIZE / 2, EXPLOSION_SIZE / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(EXPLOSION_SIZE / 2, EXPLOSION_SIZE / 2, 0.0f);

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
		SetTextureExplosion(i, explosion[i].cntAnim);
	}

	// 頂点データをアンロックする
	D3DVtxBuffExplosion->Unlock();

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
// 引　数：int index(爆発エフェクトのアドレス番号), float size(テクスチャーのサイズ)
// 戻り値：なし
//=============================================================================
void SetVertexExplosion(int index, float Size)
{
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (index * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-Size / 2, -Size / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(-Size / 2, Size / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(Size / 2, -Size / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(Size / 2, Size / 2, 0.0f);

	// 頂点データをアンロックする
	D3DVtxBuffExplosion->Unlock();
}

//=============================================================================
// テクスチャ座標の設定
// 引　数：intindex(爆発エフェクトのアドレス番号), 
//		 　int cntPatten(アニメーションのカウントパターン)
// 戻り値：な　し
//=============================================================================
void SetTextureExplosion(int index, int cntPattern)
{
	VERTEX_3D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += (index * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(cntPattern * 0.125f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((cntPattern + 1) * 0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(cntPattern * 0.125f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((cntPattern + 1) * 0.125f, 1.0f);
	
	// 頂点データをアンロックする
	D3DVtxBuffExplosion->Unlock();
}


//============================================================================
// アルファ値の設定
// 引　数：　int index(アドレス番号), flaot val(アルファ値)
// 戻り値；　なし
//============================================================================
void SetDiffuseExplosion(int index, float val)
{
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (index * 4);

	// 反射光の設定
	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

	// 頂点データをアンロックする
	D3DVtxBuffExplosion->Unlock();
}

//==========================================================================
// ボール消滅エフェクトの設置
// 引　数：D3DXVECTOR3 pos(位置), D3DXVECTOR3 rot(回転), flaot Dest(距離)
// 戻り値：bool型　使用中ならfalse , 未使用ならtrue
//==========================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest)
{
	EXPLOSION *explosion = &ExplosionWk[0];		// アドレスを取得

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		// 未使用なら
		if (!explosion[i].use)
		{
			explosion[i].use = true;	 // 使用へ
			explosion[i].pos.x = pos.x + sinf(rot.y) * Dest;		// 位置を代入
			explosion[i].pos.z = pos.z + cosf(rot.y) * Dest;		//
			explosion[i].pos.y = pos.y;								//
			
			return;
		}
	}
}

//=================================================================================
// テクスチャーのアニメーション処理
// 引　数：int index(爆発エフェクトのアドレス番号)
// 戻り値：な　し
//==================================================================================
void TextureAnim(int index)
{
	EXPLOSION *explosion = &ExplosionWk[0];

	explosion[index].cntAnim++;		// アニメーションカウントの更新 

	// アニメーションWaitチェック
	if ((explosion[index].cntAnim % ANIM_TIME) == 0)
	{
		// パターンの切り替え
		explosion[index].patternAnim = (explosion[index].patternAnim + 1) % ANIM_PATTERN_NUM;
	}
	// アニメーションの停止
	if (explosion[index].patternAnim == ANIM_STOP)
	{
		explosion[index].use = false;
		explosion[index].patternAnim = 0;
	}

	SetVertexExplosion(index, EXPLOSION_SIZE);					// 頂点作成
	SetTextureExplosion(index, explosion[index].patternAnim);	// テクスチャー座標の計算
}

////==========================================================================
//// ボール消滅エフェクトの向き
//// 引　数：D3DXVECTOR3 nor0(ボール消滅の法線), D3DXVECTOR3 nor1(ポリゴンの法線), 
////		   D3DXVECTOR3 *rot(ボール消滅の回転量)
//// 戻り値：な　し
////==========================================================================
//void SetRotExplosion(D3DXVECTOR3 nor0, D3DXVECTOR3 nor1, D3DXVECTOR3 *rot)
//{
//	// 法線がX軸方向なら
//	if (nor1.x != 0.0f)
//	{
//		nor1.x > 0.0f ? rot->y = D3DX_PI * 0.5f : rot->y = -D3DX_PI * 0.5f;
//		rot->x = 0.0f;
//	}
//	// 法線がZ軸方向なら
//	else if (nor1.z != 0.0f)
//	{
//		nor1.z > 0.0f ? rot->y = 0.0f : rot->y = D3DX_PI;
//		rot->x = 0.0f;
//	}
//	// 法線がY軸方向なら
//	else if (nor1.y != 0.0f)
//	{
//		nor1.y > 0.0f ? rot->x = -D3DX_PI * 0.5f : rot->x = D3DX_PI * 0.5f;
//	}
//
//}

////===================================================================================
//// 爆発エフェクトの消滅処理
//// 引　数：int eno(爆発エフェクトのアドレス番号)
//// 戻り値：なし
////===================================================================================
//void DeleteExplosion(int eno)
//{
//	EXPLOSION *explosion = &ExplosionWk[0];
//
//	dif_mi[eno] -= 0.01f;	// 透過の値
//
//	explosion[eno].time--;	// 生存時間をデクリメント
//
//	// 消滅時間になったら消滅
//	if (explosion[eno].time % DEL_TIME == 0)
//	{
//		dif_mi[eno] = INIT_ALPHA;
//		explosion[eno].use = false;
//		explosion[eno].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//	}
//}