//===============================================================================
//
// バレット処理 [bullet.cpp]
// Author : GP11A_341_22_田中太陽 
//
//===============================================================================
#include "bullet.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include "field.h"
#include "checkhit.h"
#include "block.h"
#include "bulletEffect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLET001		"data/TEXTURE/bullet001.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_BULLET002		"data/TEXTURE/bullet002.png"	// 読み込むテクスチャファイル名

#define	BULLET_SIZE_X		(20.0f)							// バレットの幅
#define	BULLET_SIZE_Y		(20.0f)							// バレットの高さ
#define	BULLET_SIZE_Z		(20.0f)							// バレットの奥行
#define	BULLET_SPEED		(8.0f)							// 移動速度
#define BULLET_RADY_FRAME	(10)							// 発射間隔
#define TEXTURE_MAX			(2)								// テクスチャーの最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBullet(int Index, float fSizeX, float fSizeY);
bool CheckBlockInBullet(int index, int bno);
void MoveBullet(int index, int bno);
//bool CheckReflectBullet(int index, int bno);
//void SetDiffuseBullet(int Index, float val);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

LPDIRECT3DTEXTURE9		D3DTextureBullet[TEXTURE_MAX];	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBullet = NULL;	// 頂点バッファインターフェースへのポインタ
static float			dif_mi[BULLET_MAX];
static int				cntFrame[BULLET_SET_MAX];
BULLET					bulletWk[BULLET_SET_MAX];

//===============================================================================
// 初期化処理
// 引　数：int type(再初期化の2数判定値)
// 戻り値：HRESULT型
//===============================================================================
HRESULT InitBullet(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = &bulletWk[0];

	// 頂点情報の作成
	MakeVertexBullet(pDevice);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BULLET001,			// ファイルの名前
			&D3DTextureBullet[TEX_NUM001]);	// 読み込むメモリー
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_BULLET002,			// ファイルの名前
			&D3DTextureBullet[TEX_NUM002]);	// 読み込むメモリー
	}


	for (int i = 0; i < BULLET_SET_MAX; i++)
	{
		cntFrame[i] = BULLET_RADY_FRAME;						// フレームを初期化
		bullet[i].sclIncrease = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// スケールの増加量
		bullet[i].speedIncrease = 0.0f;	// スケールの増加量
		// SEのロード
		bullet[i].BulletSE = LoadSound(SE_BULLET);
		bullet[i].ReflectSE = LoadSound(SE_REFLECT);

		for (int j = 0; j < BULLET_ONESET_MAX; j++)
		{	
			bullet[i].use[j] = false;									// 使用状態を初期化
			bullet[i].reflect[j] = false;
			bullet[i].pos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置を初期化
			bullet[i].rot[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転を初期化
			bullet[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// 拡大率を初期化
			bullet[i].size[j] = D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, BULLET_SIZE_Z); // 大きさを初期化
			bullet[i].cntReflect[j] = INIT_REFLECT_CNT;					// 反発の初期化
			bullet[i].speed[j] = INIT_BULLET_SPEED;
		}
	}

	return S_OK;
}

//===============================================================================
// 終了処理
// 引　数：なし
// 戻り値：なし
//===============================================================================
void UninitBullet(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBullet[i] != NULL)
		{// テクスチャの開放
			D3DTextureBullet[i]->Release();
			D3DTextureBullet[i] = NULL;
		}
	}
	if (D3DVtxBuffBullet != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffBullet->Release();
		D3DVtxBuffBullet = NULL;
	}
}

//===============================================================================
// 更新処理
//===============================================================================
void UpdateBullet(void)
{
	BULLET *bullet = &bulletWk[0];
	int i, j;

	for (i = 0; i < BULLET_SET_MAX; i++)
	{
		cntFrame[i]++;	// フレームカウントの更新

		for (j = 0; j < BULLET_ONESET_MAX; j++)
		{
			// 使用中ならば
			if (bullet[i].use[j])
			{
				// 現在位置を保存
				bullet[i].prevPos[j] = bullet[i].pos[j];

				SetBulletEffect(bullet[i].pos[j], i);
				
				// 移動処理
				MoveBullet(i, j);

				// 移動量
				bullet[i].pos[j].x += bullet[i].move[j].x;
				bullet[i].pos[j].y -= bullet[i].move[j].y;
				bullet[i].pos[j].z += bullet[i].move[j].z;

				// 消滅処理
				if (!CheckBlockInBullet(i, j))
				{
					bullet[i].use[j] = false;
					bullet[i].reflect[j] = false;
					bullet[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					bullet[i].speed[j] = INIT_BULLET_SPEED;
				}
			}
		}
	}
 }

//===============================================================================
// 描画処理
//===============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX *mtxView, mtxScale, mtxTranslate;
	BULLET *bullet = &bulletWk[0];

	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < BULLET_SET_MAX; i++)
	{
		for (int j = 0; j < BULLET_ONESET_MAX; j++)
		{
			// ラインティングを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 通常ブレンド
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			if (bullet[i].use[j])
			{
				// ビューマトリックスを取得
				mtxView = GetMtxView();

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&bullet[i].mtxWorld);

				// ポリゴンを正面に向ける
				bullet[i].mtxWorld._11 = mtxView->_11;
				bullet[i].mtxWorld._12 = mtxView->_21;
				bullet[i].mtxWorld._13 = mtxView->_31;
				bullet[i].mtxWorld._21 = mtxView->_12;
				bullet[i].mtxWorld._22 = mtxView->_22;
				bullet[i].mtxWorld._23 = mtxView->_32;
				bullet[i].mtxWorld._31 = mtxView->_13;
				bullet[i].mtxWorld._32 = mtxView->_23;
				bullet[i].mtxWorld._33 = mtxView->_33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, bullet[i].scl[j].x, bullet[i].scl[j].y, bullet[i].scl[j].z);
				D3DXMatrixMultiply(&bullet[i].mtxWorld,&bullet[i].mtxWorld, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, bullet[i].pos[j].x, bullet[i].pos[j].y, bullet[i].pos[j].z);
				D3DXMatrixMultiply(&bullet[i].mtxWorld, &bullet[i].mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &bullet[i].mtxWorld);

				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, D3DVtxBuffBullet, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureBullet[i]);

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

//===============================================================================
// 頂点情報の作成
//===============================================================================
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BULLET_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffBullet,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < BULLET_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			SetVertexBullet(i, BULLET_SIZE_X, BULLET_SIZE_Y);

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
		D3DVtxBuffBullet->Unlock();
	}

	return S_OK;
}

//=================================================================================
// 頂点座標の設定
// 引　数：int Index(bレットのアドレス番号),float fSizeX(横幅),float fSizeY(縦幅)
// 戻り値：なし
//=================================================================================
void SetVertexBullet(int Index, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffBullet->Unlock();
	}
}


//=========================================================================
// バレットのアドレスを取得
// 引　数：バレットのアドレス番号
// 戻り値：BULLET構造体型
//=========================================================================
BULLET *GetBullet(int bno)
{
	return &bulletWk[bno];
}

//=========================================================================
// バレットの生成
// 引　数：D3DXVECTOR3 pos(位置)、D3DXVECTOR3 rot(角度)、float Dest(距離)
// 戻り値：bool型　未使用の場合 true、使用中の場合 false
//=========================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float speed, float Dest, int index)
{
	BULLET *bullet = &bulletWk[index];

	for (int i = 0; i < BULLET_ONESET_MAX; i++)
	{
		if (!bullet->use[i])
		{
			PlaySound(bullet[i].BulletSE, E_DS8_FLAG_NONE);
			bullet->use[i] = true;									// 使用中へ
			bullet->pos[i].x = pos.x + cosf(rot.y) * Dest;			// プレイヤーの位置へ　
			bullet->pos[i].z = pos.z + sinf(rot.y) * Dest;			//　
			bullet->pos[i].y = pos.y;								//
			bullet->rot[i] = rot;									// 回転量を代入
 			bullet->speed[i] = bullet->speed[i] + speed;
			//bullet->scl[i] = scl;									// スケールを代入
			//bullet->size[i] = BULLET_SIZE_X * scl;					//
			SetVertexBullet(i, bullet->size[i].x, bullet->size[i].y);	// 頂点を作成
			cntFrame[index] = 0;									// フレームカウントを初期化
			bullet->sclIncrease = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// スケールの増加値を初期化
			bullet->speedIncrease = 0.0f;							// 速度Aの増加値を初期化

			return;
		}
	}
	return;
}

//========================================================================
// バレットの移動処理
// 引　数：int index(バレットのアドレス)
// 戻り値：な　し
//========================================================================
void MoveBullet(int index, int bno)
{
	BULLET *bullet = &bulletWk[index];

	if (!bullet->reflect[bno])
	{
		bullet->move[bno].x = sinf(bullet->rot[bno].y) * bullet->speed[bno];
		bullet->move[bno].y = tanf(bullet->rot[bno].x) * bullet->speed[bno];
		bullet->move[bno].z = cosf(bullet->rot[bno].y) * bullet->speed[bno];

		// バレットとブロックの当たり判定
		if (!HitCheckBlock(bullet->pos[bno] + bullet->move[bno], bullet->prevPos[bno], BLOCK_VTX_MAX))
		{
			PlaySound(bullet->ReflectSE, E_DS8_FLAG_NONE);
			bullet->refVec[bno] = ReflectVector(bullet->pos[bno] + bullet->move[bno], bullet->prevPos[bno],GetNormal());
			bullet->move[bno] = bullet->refVec[bno] * bullet->speed[bno];
			bullet->reflect[bno] = true;
			bullet->cntReflect[bno]--;
		}
	}
	else if (bullet->reflect[bno])
	{
		bullet->move[bno] = bullet->refVec[bno] * bullet->speed[bno];
	}
}

//========================================================================
// バレットの画面内外判定処理
// 引　数：int index(組バレットのアドレス), int bno(バレット単体のアドレス)
// 戻り値：bool型　trueであれば画面内にある、falseならば画面外にある
//========================================================================
bool CheckBlockInBullet(int index, int bno)
{
	BULLET *bullet = &bulletWk[index];
	
	if (bullet->pos[bno].x > SCREEN_WIDTH  / 2)	return false;
	if (bullet->pos[bno].x < -SCREEN_WIDTH / 2)	return false;
	if (bullet->pos[bno].z > SCREEN_HEIGHT / 2)	return false;
	if (bullet->pos[bno].z < -SCREEN_HEIGHT / 2)	return false;

	return true;
}

//========================================================================
// バレットの反射回数判定処理
// 引　数：int index(組バレットのアドレス), int blockNo(ブロックのアドレス)
//		   D3DXVECTOR3 pos(対象バレットの位置)
// 戻り値：bool型　trueであれば、falseならば消滅
//========================================================================
void CheckBlockHitBullet(int blockNo, int index, D3DXVECTOR3 pos)
{
	BULLET *bullet = GetBullet(index);	//バレットのアドレスを取得
	for (int i = 0; i < BULLET_ONESET_MAX; i++)
	{
		if (!bullet->use[i]) continue;
		if (CheckHitBB(bullet->pos[i], pos,
			D3DXVECTOR3(15.0f, 15.0f, 15.0f), D3DXVECTOR3(25.0f, 25.0f, 25.0f)))
		{
			bullet->cntReflect[i]--;
			if (bullet->cntReflect[i] < 0)
			{
				bullet->use[i] = false;
				bullet->reflect[i] = false;
				bullet->cntReflect[i] = 2;
				bullet->speed[i] = INIT_BULLET_SPEED;
			}
		}
	}
}
