//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#include "block.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "effect.h"
#include "smoke.h"
#include "checkhit.h"
#include "explosion.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	BLOCK_MODEL			"data/MODEL/block001.x"	// 読み込むモデル名
#define TEXTURE_MAX				(5)
#define INIT_POS_X_RIGHTSIDE	(386.5f)	// 右側のブロックの初期X座標
#define INIT_POS_X_LEFTSIDE		(-388.5f)	// 左側のブロックの初期X座標
#define INIT_POS_Y				(0.0f)		// ブロックの初期Y座標
#define INIT_POS_Z_NEARSIDE		(-136.5f)	// 手前のブロックの初期Z座標
#define INIT_POS_Z_FEARSIDE		(138.5f)	// 奥のブロックの初期Z座標
#define BLOCK_VERTEX			(4)			// 
#define VTX_SIZE_X				(370.0f)	// 頂点のX軸サイズ
#define VTX_SIZE_Y				(25.0f)		// 　　　Y軸サイズ
#define VTX_SIZE_Z				(120.0f)	// 　　　Z軸サイズ
#define BLOCK_DAMAGE			(1)			// ブロックへのダメージ
#define INIT_LIFE				(10)		// 寿命の初期値	
#define BLOCK_SIZE				(25.0f)		// ブロックの1辺のサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetPosBlock(void);
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice);
void InitVertexBlock(void);

enum {
	TEX_TYPE01,
	TEX_TYPE02,
	TEX_TYPE03,
	TEX_TYPE04,
	TEX_TYPE05
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBlock[TEXTURE_MAX] = { 
									NULL,NULL,NULL,NULL,NULL }; // テクスチャ読み込み場所
LPD3DXMESH				D3DXMeshBlock;		// ID3DXMeshBlockインターフェイスへのポインタ
LPD3DXBUFFER			D3DXBuffMatBlock;	// メッシュのマテリアル情報を格納
DWORD					numMat;				// 属性情報の総数
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBlock;	// 頂点バッファへのポインタ
D3DXMATRIX				mtxWorld;			// ワールドマトリックス
D3DXMATRIX				mtxWorldVtx;			// ワールドマトリックス
int						textureNum;			// テクスチャーの番号
BLOCK					block[BLOCK_MAX];	// ブロック構造体
//=============================================================================
// 初期化処理
// 引　数：int type(再初期化２数判定値)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitBlock(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;
	// 頂点情報の作成
	hr = MakeVertexBlock(pDevice);
	
	// ブロックの初期位置を設定
	SetPosBlock();

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		block[i].use = true;		// 使用へ初期化
		block[i].life = INIT_LIFE;	// ライフの初期化
	}

	D3DXMeshBlock = NULL;			// インターフェースの初期化
	D3DXBuffMatBlock = NULL;		// マテリアルの初期化
	textureNum = NULL;				// テクスチャー番号を初期化

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(BLOCK_MODEL,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&D3DXBuffMatBlock,
		NULL,
		&numMat,
		&D3DXMeshBlock)))
	{
		return E_FAIL;
	}

	//if (type == 0)
	//{
	//	// テクスチャの読み込み
	//	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
	//						HANDGUM_TEXTURE,	// ファイルの名前
	//					&D3DTextureBlock[TEX_TYPE01]);	// 読み込むメモリー
	//}	
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlock(void)
{
	if(D3DXMeshBlock != NULL)
	{// メッシュの開放
		D3DXMeshBlock->Release();
		D3DXMeshBlock = NULL;
	}

	if(D3DXBuffMatBlock != NULL)
	{// マテリアルの開放
		D3DXBuffMatBlock->Release();
		D3DXBuffMatBlock = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBlock[i] != NULL)
		{// テクスチャの開放
			D3DTextureBlock[i]->Release();
			D3DTextureBlock[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlock(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (block[i].use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, block[i].pos.x, block[i].pos.y, block[i].pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatBlock->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)numMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTextureBlock[textureNum]);

				// 描画
				D3DXMeshBlock->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldVtx);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldVtx);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, D3DVtxBuffBlock, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTextureBlock[0]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
		}
	}
}


//=============================================================================
// ブロックのアドレス取得
//=============================================================================
BLOCK *GetBlock(int bno)
{
	return &block[bno];
}

//=============================================================================
// ブロックの設置
// 引　数：なし
// 戻り値：なし
//=============================================================================
void SetPosBlock(void)
{
	block[0].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[1].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, 113.5f);
	block[2].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, 88.5f);
	block[3].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, 63.5f);
	block[4].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, 38.5f);
	block[5].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, 13.5f);
	block[6].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, -11.5f);
	block[7].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, -36.5f);
	block[8].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, -61.5f);
	block[9].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, -86.5f);
	block[10].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, -111.5f);
	block[11].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[12].pos = D3DXVECTOR3(-363.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[13].pos = D3DXVECTOR3(-338.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[14].pos = D3DXVECTOR3(-313.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[15].pos = D3DXVECTOR3(-288.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[16].pos = D3DXVECTOR3(-263.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[17].pos = D3DXVECTOR3(-238.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[18].pos = D3DXVECTOR3(-213.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[19].pos = D3DXVECTOR3(-188.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[20].pos = D3DXVECTOR3(-163.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[21].pos = D3DXVECTOR3(-138.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[22].pos = D3DXVECTOR3(-113.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[23].pos = D3DXVECTOR3(-88.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[24].pos = D3DXVECTOR3(-63.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[25].pos = D3DXVECTOR3(-38.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[26].pos = D3DXVECTOR3(-13.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[27].pos = D3DXVECTOR3(11.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[28].pos = D3DXVECTOR3(36.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[29].pos = D3DXVECTOR3(61.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[30].pos = D3DXVECTOR3(86.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[31].pos = D3DXVECTOR3(111.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[32].pos = D3DXVECTOR3(136.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[33].pos = D3DXVECTOR3(161.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[34].pos = D3DXVECTOR3(186.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[35].pos = D3DXVECTOR3(211.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[36].pos = D3DXVECTOR3(236.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[37].pos = D3DXVECTOR3(261.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[38].pos = D3DXVECTOR3(286.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[39].pos = D3DXVECTOR3(311.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[40].pos = D3DXVECTOR3(336.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[41].pos = D3DXVECTOR3(361.5f, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[42].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	block[43].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, -113.5f);
	block[44].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, -88.5f);
	block[45].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, -63.5f);
	block[46].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, -38.5f);
	block[47].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, -13.5f);
	block[48].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, 11.5f);
	block[49].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, 36.5f);
	block[50].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, 61.5f);
	block[51].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, 86.5f);
	block[52].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, 111.5f);
	block[53].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[54].pos = D3DXVECTOR3(361.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[55].pos = D3DXVECTOR3(336.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[56].pos = D3DXVECTOR3(311.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[57].pos = D3DXVECTOR3(286.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[58].pos = D3DXVECTOR3(261.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[59].pos = D3DXVECTOR3(236.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[60].pos = D3DXVECTOR3(211.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[61].pos = D3DXVECTOR3(186.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[62].pos = D3DXVECTOR3(161.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[63].pos = D3DXVECTOR3(136.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[64].pos = D3DXVECTOR3(111.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[65].pos = D3DXVECTOR3(86.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[66].pos = D3DXVECTOR3(61.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[67].pos = D3DXVECTOR3(36.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[68].pos = D3DXVECTOR3(11.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[69].pos = D3DXVECTOR3(-13.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[70].pos = D3DXVECTOR3(-38.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[71].pos = D3DXVECTOR3(-63.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[72].pos = D3DXVECTOR3(-88.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[73].pos = D3DXVECTOR3(-113.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[74].pos = D3DXVECTOR3(-138.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[75].pos = D3DXVECTOR3(-163.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[76].pos = D3DXVECTOR3(-188.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[77].pos = D3DXVECTOR3(-213.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[78].pos = D3DXVECTOR3(-238.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[79].pos = D3DXVECTOR3(-263.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[80].pos = D3DXVECTOR3(-288.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[81].pos = D3DXVECTOR3(-313.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[82].pos = D3DXVECTOR3(-338.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	block[83].pos = D3DXVECTOR3(-363.5f, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
}

//================================================================================
// ブロックへのダメージ処理
// 引　数：int bno(ブロックのアドレス番号)
// 戻り値：なし
//================================================================================
void BlockDamageManager(int bno)
{
	block[bno].life -= BLOCK_DAMAGE;

	if (block[bno].life == 3)
	{
		//textureNum = TEX_TYPE02;
	}
	else if (block[bno].life == 0)
	{
		block[bno].use = false;
		SetExplosion(block[bno].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BLOCK_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffBlock,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	/* 頂点情報の設定 */
	InitVertexBlock();

	return S_OK;
}



//============================================================================
// ゲームフィールドの作成
//============================================================================
void InitVertexBlock(void)
{
	int i = 0;
	float size = 0.0f;

	// BOXの頂点情報の設定
	{	//頂点バッファの中身を埋める
		VERTEX_3D			   *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

		for (i = 0; i < BLOCK_NUM_LEFTSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X, VTX_SIZE_Y, VTX_SIZE_Z - size);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X, VTX_SIZE_Y, VTX_SIZE_Z + BLOCK_SIZE - size);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X, 0.0f, VTX_SIZE_Z - size);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X, 0.0f, VTX_SIZE_Z + BLOCK_SIZE - size);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

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

		size = 25.0f;

		for (i = BLOCK_NUM_LEFTSIDE; i < BLOCK_NUM_NEARSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X - BLOCK_SIZE + size, VTX_SIZE_Y, -VTX_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X + size, VTX_SIZE_Y, -VTX_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X - BLOCK_SIZE + size, 0.0f, -VTX_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X + size, 0.0f, -VTX_SIZE_Z);

			// 法線ベクトルの設定
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

		size = 0.0f;

		for (i = BLOCK_NUM_NEARSIDE; i < BLOCK_NUM_RIGHTSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X, VTX_SIZE_Y, -VTX_SIZE_Z + size);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X, VTX_SIZE_Y, -VTX_SIZE_Z - BLOCK_SIZE + size);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X, 0.0f, -VTX_SIZE_Z + size);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X, 0.0f, -VTX_SIZE_Z - BLOCK_SIZE + size);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

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

		size = 25.0f;

		for (i = BLOCK_NUM_RIGHTSIDE; i < BLOCK_NUM_FEARSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X - size, VTX_SIZE_Y, VTX_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X + BLOCK_SIZE - size, VTX_SIZE_Y, VTX_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X - size, 0.0f, VTX_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X + BLOCK_SIZE - size, 0.0f, VTX_SIZE_Z);

			// 法線ベクトルの設定
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
		D3DVtxBuffBlock->Unlock();
	}
}

//=============================================================================
// ブロックの透明ポリゴンとの当たり判定
// pos0:始点（移動前）
// pos1:終点（移動後）
//=============================================================================
bool HitCheckBlock(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		pos[NUM_VERTEX]; // 頂点座標の保存
	bool			ans;
	VERTEX_3D	    *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < BLOCK_NUM_FEARSIDE; i++, pVtx += 4)
	{
		if (block[i].use)
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
	}

	// 頂点データをアンロックする
	D3DVtxBuffBlock->Unlock();

	return ans;
}

//=============================================================================
// 4つ角との当たり判定
// 引　数：D3DXVECTOR3 pos0(判定対象の位置）
// 戻り値：bool型　trueだと当たっている、falseだと当たっていない
//=============================================================================
bool HitCheckCornerBlock(D3DXVECTOR3 pos0)
{
	if (CheckHitBB(pos0, block[0].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(15.0f, 25.0f, 15.0f))) return true;

	if (CheckHitBB(pos0, block[11].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(15.0f, 25.0f, 15.0f))) return true;

	if (CheckHitBB(pos0, block[42].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(15.0f, 25.0f, 15.0f))) return true;

	if (CheckHitBB(pos0, block[53].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(15.0f, 25.0f, 15.0f))) return true;

	return false;
}
