//=============================================================================
//
// 地面処理 [field.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "field.h"
#include "checkhit.h"
#include "explosion.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/field003.jpg"	// 読み込むテクスチャファイル名
#define FIELD_MAX				(1)					// 最大数
//#define FIELD_BOTTOMUP		(50.0f)				// 底上げ調整
//#define GAMEFIELD_SIZE_X		(500.0f)
//#define GAMEFIELD_SIZE_Y		(300.0f)
//#define GAMEFIELD_SIZE_Z		(500.0f)					

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);
//void SetMakeField(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureField = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffField = NULL;	// 頂点バッファへのポインタ


FIELD					fieldWk[FIELD_MAX];			// フィールド構造体
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	HRESULT hr;

	FIELD *field = &fieldWk[0];

	// 位置、向きの初期設定
	for (int i = 0; i < FIELD_MAX; i++)
	{
		field[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}


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
	D3DXMATRIX mtxRot, mtxTranslate;
	FIELD *field = &fieldWk[0];

	for (int i = 0; i < FIELD_MAX; i++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&field[i].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, field[i].rot.y, field[i].rot.x, field[i].rot.z);
		D3DXMatrixMultiply(&field[i].mtxWorld, &field[i].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, field[i].pos.x, field[i].pos.y, field[i].pos.z);
		D3DXMatrixMultiply(&field[i].mtxWorld, &field[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &field[i].mtxWorld);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, D3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureField);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
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
		&D3DVtxBuffField,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{	//頂点バッファの中身を埋める
		VERTEX_3D			   *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

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

		// 頂点データをアンロックする
		D3DVtxBuffField->Unlock();

		/* 頂点情報の設定 */
		//InitMakeVertex();	
	}
	return S_OK;
}

//=============================================================================
// との当たり判定
// pos0:始点（移動前）
// pos1:終点（移動後）
//=============================================================================
FIELD *GetField(int fno)
{
	return &fieldWk[fno];
}

//=============================================================================
// 壁との当たり判定
// pos0:始点（移動前）
// pos1:終点（移動後）
//=============================================================================
bool HitCheckField(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	FIELD *field = &fieldWk[0];

	D3DXVECTOR3		pos[NUM_VERTEX]; // 頂点座標の保存
	bool			ans;
	VERTEX_3D	    *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < FIELD_MAX; i++, pVtx += 4)
	{
		// 頂点座標の設定
		pos[0] = pVtx[0].vtx;
		pos[1] = pVtx[1].vtx;
		pos[2] = pVtx[2].vtx;
		pos[3] = pVtx[3].vtx;

		// 左下側ポリゴンと線分の当たり判定
		ans = CheckHitPolygon(pos[0], pos[2], pos[3], pos0, pos1);
		if (!ans)
		{
			break;
		}
		// 右上側ポリゴンと線分の当たり判定
		ans = CheckHitPolygon(pos[0], pos[3], pos[1], pos0, pos1);
		if (!ans)
		{
			break;
		}
	}

	// 頂点データをアンロックする
	D3DVtxBuffField->Unlock();

	return ans;
}

////============================================================================
//// ゲームフィールドの作成
////============================================================================
//void SetMakeField(void)
//{
//	FIELD *field = &fieldWk[0];
//
//	// BOXの頂点情報の設定
//	{
//		//頂点バッファの中身を埋める
//		VERTEX_3D			   *pVtx;
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);
//
//		// 上面
//		{
//			// 頂点座標の設定
//			pVtx[0].vtx = D3DXVECTOR3(GAMEFIELD_SIZE_X, GAMEFIELD_SIZE_Y + 50.0f, GAMEFIELD_SIZE_Z);
//			pVtx[1].vtx = D3DXVECTOR3(-GAMEFIELD_SIZE_X, GAMEFIELD_SIZE_Y + 50.0f, GAMEFIELD_SIZE_Z);
//			pVtx[2].vtx = D3DXVECTOR3(GAMEFIELD_SIZE_X, GAMEFIELD_SIZE_Y + 50.0f, -GAMEFIELD_SIZE_Z);
//			pVtx[3].vtx = D3DXVECTOR3(-GAMEFIELD_SIZE_X, GAMEFIELD_SIZE_Y + 50.0f, -GAMEFIELD_SIZE_Z);
//
//			// 法線ベクトルの設定
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//			// 反射光の設定
//			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 底面
//		{
//			// 頂点座標の設定
//			pVtx[4].vtx = D3DXVECTOR3(-GAMEFIELD_SIZE_X, -GAMEFIELD_SIZE_Y + 50.0f, GAMEFIELD_SIZE_Z);
//			pVtx[5].vtx = D3DXVECTOR3(GAMEFIELD_SIZE_X, -GAMEFIELD_SIZE_Y + 50.0f, GAMEFIELD_SIZE_Z);
//			pVtx[6].vtx = D3DXVECTOR3(-GAMEFIELD_SIZE_X, -GAMEFIELD_SIZE_Y + 50.0f, -GAMEFIELD_SIZE_Z);
//			pVtx[7].vtx = D3DXVECTOR3(GAMEFIELD_SIZE_X, -GAMEFIELD_SIZE_Y + 50.0f, -GAMEFIELD_SIZE_Z);
//
//			// 法線ベクトルの設定
//			pVtx[4].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
//			pVtx[5].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
//			pVtx[6].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
//			pVtx[7].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
//
//			// 反射光の設定
//			pVtx[4].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[5].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[6].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[7].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 左面
//		{
//			// 頂点座標の設定
//			pVtx[8] = pVtx[3];
//			pVtx[9] = pVtx[1];
//			pVtx[10] = pVtx[6];
//			pVtx[11] = pVtx[4];
//
//			// 法線ベクトルの設定
//			pVtx[8].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[9].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[10].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[11].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//
//			// 反射光の設定
//			pVtx[8].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[9].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[10].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[11].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 右面
//		{
//			// 頂点座標の設定
//			pVtx[12] = pVtx[0];
//			pVtx[13] = pVtx[2];
//			pVtx[14] = pVtx[5];
//			pVtx[15] = pVtx[7];
//
//			// 法線ベクトルの設定
//			pVtx[12].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[13].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[14].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[15].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//
//			// 反射光の設定
//			pVtx[12].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[13].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[14].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[15].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 正面
//		{
//			// 頂点座標の設定
//			pVtx[16] = pVtx[2];
//			pVtx[17] = pVtx[3];
//			pVtx[18] = pVtx[7];
//			pVtx[19] = pVtx[6];
//
//			// 法線ベクトルの設定
//			pVtx[16].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[17].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[18].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[19].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//
//			// 反射光の設定
//			pVtx[16].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[17].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[18].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[19].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 背面
//		{
//			// 頂点座標の設定
//			pVtx[20] = pVtx[5];
//			pVtx[21] = pVtx[4];
//			pVtx[22] = pVtx[0];
//			pVtx[23] = pVtx[1];
//
//			// 法線ベクトルの設定
//			pVtx[20].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//			pVtx[21].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//			pVtx[22].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//			pVtx[23].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//
//			// 反射光の設定
//			pVtx[20].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[21].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[22].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[23].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[20].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[21].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[22].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[23].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//
//
//		// 頂点データをアンロックする
//		D3DVtxBuffField->Unlock();
//	}
//
//}
////============================================================================
//// ボックスの頂点　作成
////============================================================================
//void InitMakeVertex(void)
//{
//	FIELD *field = &fieldWk[0];
//
//	// BOXの頂点情報の設定
//	{
//		//頂点バッファの中身を埋める
//		VERTEX_3D			   *pVtx;
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);
//
//		// 上面
//		{
//			// 頂点座標の設定
//			pVtx[0].vtx = D3DXVECTOR3(FIELD_SIZE_X, FIELD_SIZE_Y + 50.0f, FIELD_SIZE_Z);
//			pVtx[1].vtx = D3DXVECTOR3(-FIELD_SIZE_X, FIELD_SIZE_Y + 50.0f, FIELD_SIZE_Z);
//			pVtx[2].vtx = D3DXVECTOR3(FIELD_SIZE_X, FIELD_SIZE_Y + 50.0f, -FIELD_SIZE_Z);
//			pVtx[3].vtx = D3DXVECTOR3(-FIELD_SIZE_X, FIELD_SIZE_Y + 50.0f, -FIELD_SIZE_Z);
//
//			// 法線ベクトルの設定
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//			// 反射光の設定
//			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 底面
//		{
//			// 頂点座標の設定
//			pVtx[4].vtx = D3DXVECTOR3(-FIELD_SIZE_X, -FIELD_SIZE_Y + 50.0f, FIELD_SIZE_Z);
//			pVtx[5].vtx = D3DXVECTOR3(FIELD_SIZE_X, -FIELD_SIZE_Y + 50.0f, FIELD_SIZE_Z);
//			pVtx[6].vtx = D3DXVECTOR3(-FIELD_SIZE_X, -FIELD_SIZE_Y + 50.0f, -FIELD_SIZE_Z);
//			pVtx[7].vtx = D3DXVECTOR3(FIELD_SIZE_X, -FIELD_SIZE_Y + 50.0f, -FIELD_SIZE_Z);
//
//			// 法線ベクトルの設定
//			pVtx[4].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
//			pVtx[5].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
//			pVtx[6].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
//			pVtx[7].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
//
//			// 反射光の設定
//			pVtx[4].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[5].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[6].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[7].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 左面
//		{
//			// 頂点座標の設定
//			pVtx[8] = pVtx[3];
//			pVtx[9] = pVtx[1];
//			pVtx[10] = pVtx[6];
//			pVtx[11] = pVtx[4];
//
//			// 法線ベクトルの設定
//			pVtx[8].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[9].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[10].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[11].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//
//			// 反射光の設定
//			pVtx[8].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[9].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[10].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[11].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 右面
//		{
//			// 頂点座標の設定
//			pVtx[12] = pVtx[0];
//			pVtx[13] = pVtx[2];
//			pVtx[14] = pVtx[5];
//			pVtx[15] = pVtx[7];
//
//			// 法線ベクトルの設定
//			pVtx[12].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[13].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[14].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//			pVtx[15].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
//
//			// 反射光の設定
//			pVtx[12].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[13].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[14].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[15].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 正面
//		{
//			// 頂点座標の設定
//			pVtx[16] = pVtx[2];
//			pVtx[17] = pVtx[3];
//			pVtx[18] = pVtx[7];
//			pVtx[19] = pVtx[6];
//
//			// 法線ベクトルの設定
//			pVtx[16].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[17].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[18].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[19].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//
//			// 反射光の設定
//			pVtx[16].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[17].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[18].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[19].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 背面
//		{
//			// 頂点座標の設定
//			pVtx[20] = pVtx[5];
//			pVtx[21] = pVtx[4];
//			pVtx[22] = pVtx[0];
//			pVtx[23] = pVtx[1];
//
//			// 法線ベクトルの設定
//			pVtx[20].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//			pVtx[21].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//			pVtx[22].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//			pVtx[23].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//
//			// 反射光の設定
//			pVtx[20].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[21].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[22].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[23].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[20].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[21].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[22].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[23].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 頂点データをアンロックする
//		D3DVtxBuffField->Unlock();
//	}
//
//}
