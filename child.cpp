//=============================================================================
//
// 子供モデル処理 [child.cpp]
// Author : GP11A341_22_田中太陽
//
//=============================================================================
#include "child.h"
#include "input.h"
#include "shadow.h"
#include "player.h"
#include "checkhit.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CHILD_MODEL01		"data/MODEL/child01.x"				// 読み込むモデル名
#define	CHILD_MODEL02		"data/MODEL/child02.x"				// 読み込むモデル名

#define RATE_MOVE_CHILD		(0.20f)
#define CHILD_SIZE			(17.0f)					// モデルサイズ

enum {
	MODEL_TYPE001,
	MODEL_TYPE002
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MoveChild(int index, int cno);
void ChaseChild(int index, int cno);
void InitPosChild(void);
void AlignmentChild(int index, int cno);
void ChesionChild(int index, int cno);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9	D3DTexture;		// テクスチャへのポインタ
static LPD3DXMESH			D3DMesh[CHILD_SET_MAX];		// メッシュ情報へのポインタ
static LPD3DXBUFFER			D3DXBuffMat[CHILD_SET_MAX];	// マテリアル情報へのポインタ
static DWORD				NumMat[CHILD_SET_MAX];			// マテリアル情報の数


static int					g_shadownum;
static D3DXCOLOR			g_shadowcolor;			
static D3DXMATRIX			MtxWorld;				// ワールドマトリックス
static float				angle[CHILD_SET_MAX];
CHILD						childWk[CHILD_SET_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitChild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CHILD *child = &childWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < CHILD_SET_MAX; i++)
	{
		D3DMesh[i] = NULL;
		D3DXBuffMat[i] = NULL;
		NumMat[i] = 0;
		angle[i] = 0.0f;
		for (int j = 0; j < CHILD_ONESET_MAX; j++)
		{
			// 位置・回転・スケールの初期設定
			child[i].pos[j] = player[i].pos;
			child[i].rot[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			child[i].scl[j] = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
			child[i].use[j] = true;
			child[i].prevPos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			child[i].vec[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	//InitPosChild();

	D3DTexture = NULL;

	// Xファイルの読み込み
	{
		if(FAILED(D3DXLoadMeshFromX(CHILD_MODEL01,				// 読み込むモデルファイル名(Xファイル)
								D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
								pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
								NULL,					// 隣接性データを含むバッファへのポインタ
								&D3DXBuffMat[MODEL_TYPE001],	// マテリアルデータを含むバッファへのポインタ　テクスチャやカラー情報
								NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
								&NumMat[MODEL_TYPE001],		// D3DXMATERIAL構造体の数	登録されてるマテリアル情報数
								&D3DMesh[MODEL_TYPE001])))	// ID3DXMeshインターフェイスへのポインタのアドレス　頂点情報のデータ
	{
		return E_FAIL;
	}

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(CHILD_MODEL02,				// 読み込むモデルファイル名(Xファイル)
									D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
									pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
									NULL,					// 隣接性データを含むバッファへのポインタ
									&D3DXBuffMat[MODEL_TYPE002],	// マテリアルデータを含むバッファへのポインタ　テクスチャやカラー情報
									NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
									&NumMat[MODEL_TYPE002],		// D3DXMATERIAL構造体の数	登録されてるマテリアル情報数
									&D3DMesh[MODEL_TYPE002])))	// ID3DXMeshインターフェイスへのポインタのアドレス　頂点情報のデータ
		{
			return E_FAIL;
		}
	}

	//g_shadownum = CreateShadow(child[i].pos[j], 25.0f, 25.0f);
	//g_shadowcolor = D3DXCOLOR(1.5f,1.5f,1.5f,1.5f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitChild(void)
{
	if (D3DTexture != NULL)
	{// テクスチャの開放
		D3DTexture->Release();
		D3DTexture = NULL;
	}

	for (int i = 0; i < CHILD_SET_MAX; i++)
	{
		if (D3DMesh != NULL)
		{// メッシュの開放
			D3DMesh[i]->Release();
			D3DMesh[i] = NULL;
		}

		if (D3DXBuffMat != NULL)
		{// マテリアルの開放
			D3DXBuffMat[i]->Release();
			D3DXBuffMat[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateChild(void)
{
	CHILD *child = &childWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < CHILD_SET_MAX; i++)
	{
		for (int j = 0; j < CHILD_ONESET_MAX; j++)
		{		
			child[i].prevPos[j] = child[i].pos[j];

			AlignmentChild(i, j);

			ChesionChild(i, j);

			// 追跡
			//ChaseChild(i, j);
			// 移動
			MoveChild(i, j);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawChild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	CHILD *child = &childWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < CHILD_SET_MAX; i++)
	{
		for (int j = 0; j < CHILD_ONESET_MAX; j++)
		{
			if (child[i].use[j])
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&MtxWorld);

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, child[i].scl[j].x, child[i].scl[j].y, child[i].scl[j].z);
				D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxScl);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, child[i].rot[j].y, child[i].rot[j].x, child[i].rot[j].z);
				D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, child[i].pos[j].x, child[i].pos[j].y, child[i].pos[j].z);
				D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxTranslate);

				//D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &player->mtxWorld);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);

				// 現在のマテリアルを取得　（元に戻すためのバックアップ）
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				pD3DXMat = (D3DXMATERIAL*)D3DXBuffMat[i]->GetBufferPointer();

				// マテリアルの数分の表示が必要なためFOR文を使用
				for (int nCntMat = 0; nCntMat < (int)NumMat[i]; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

					// テクスチャの設定
					pDevice->SetTexture(0, D3DTexture);

					// 描画
					D3DMesh[i]->DrawSubset(nCntMat);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//========================================================================
// 子供モデルの反射回数判定処理
// 引　数：int index(組子供モデルのアドレス), int blockNo(ブロックのアドレス)
//		   D3DXVECTOR3 pos(対象子供モデルの位置)
// 戻り値：bool型　trueであれば、falseならば消滅
//========================================================================
CHILD *GetChild(int index)
{
	return &childWk[index];
}

//========================================================================
// 子供モデルのダメージ判定処理
// 引　数：int index(組子供モデルのアドレス), int blockNo(ブロックのアドレス)
//		   D3DXVECTOR3 pos(対象子供モデルの位置)
// 戻り値：bool型　trueであれば、falseならば消滅
//========================================================================
void ChildDamageManager(void)
{
	
}

//========================================================================
// 子供モデルのダメージ判定処理
// 引　数：int index(組子供モデルのアドレス), int blockNo(ブロックのアドレス)
//		   D3DXVECTOR3 pos(対象子供モデルの位置)
// 戻り値：bool型　trueであれば、falseならば消滅
//========================================================================
void InitPosChild(void)
{
	CHILD *child = &childWk[0];

	child[0].pos[0] = D3DXVECTOR3(0.0f, 0.0f, -20.0f);
	child[0].pos[1] = D3DXVECTOR3(20.0f, 0.0f, -40.0f);
	child[0].pos[2] = D3DXVECTOR3(-20.0f, 0.0f, -40.0f);
	child[0].pos[3] = D3DXVECTOR3(40.0f, 0.0f, -60.0f);
	child[0].pos[4] = D3DXVECTOR3(-40.0f, 0.0f, -60.0f);
	//child[0].pos[5] = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);

}

//========================================================================
// 子供モデルの移動処理
// 引　数：int index(組子供モデルのアドレス), int blockNo(ブロックのアドレス)
// 戻り値：bool型　trueであれば、falseならば消滅
//========================================================================
void MoveChild(int index, int cno)
{
	CHILD *child = &childWk[index];

	// 移動量に慣性をかける
	child->move[cno].x += (0.0f - child->move[cno].x) * RATE_MOVE_CHILD;
	child->move[cno].y += (0.0f - child->move[cno].y) * RATE_MOVE_CHILD;
	child->move[cno].z += (0.0f - child->move[cno].z) * RATE_MOVE_CHILD;

	// 位置移動
	child->pos[cno].x += child->move[cno].x;
	child->pos[cno].y += child->move[cno].y;
	child->pos[cno].z += child->move[cno].z;

}

//============================================================================
// 子供モデルの追従判定処理
// 引　数：int index(組子供モデルのアドレス), int cno(子供モデルのアドレス)
// 戻り値：なし
//============================================================================
void ChaseChild(int index, int cno)
{
	CHILD *child = &childWk[index];
	PLAYER *player = GetPlayer(index);
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	 // プレイヤーと子供の先頭アドレス番号の追従
	if (cno == 0)
	{
		// 追従ベクトルの計算
		vec = player->pos - child->pos[cno];
		D3DXVec3Normalize(&vec, &vec);
		// プレイヤーが範囲外に移動した場合追従
		if (!CheckHitBC(player->pos, child->pos[cno], CHILD_SIZE, CHILD_SIZE))
		{
			child->move[cno].x = vec.x * 5.0f;
			child->move[cno].z = vec.z * 5.0f;
		}
	}
	// 子供同士の追従
	else
	{
		// 追従ベクトルの計算
		vec = child->pos[cno - 1] - child->pos[cno];
		D3DXVec3Normalize(&vec, &vec);
		// 前アドレスの子供が範囲外に移動した場合追従
		if (!CheckHitBC(child->pos[cno], child->pos[cno - 1], CHILD_SIZE, CHILD_SIZE))
		{
			child->move[cno].x = vec.x * 5.0f;
			child->move[cno].z = vec.z * 5.0f;
		}
	}
}
// 整列
void AlignmentChild(int index, int cno)
{
	CHILD *child = &childWk[index];
	PLAYER *player = GetPlayer(0);

	child->vec[cno] = player[index].pos - player[index].prevPos;
	D3DXVec3Normalize(&child->vec[cno], &child->vec[cno]);
	child->move[cno].x = child->vec[cno].x;
	child->move[cno].z = child->vec[cno].z;
}

// 結合
void ChesionChild(int index, int cno)
{
	CHILD *child = &childWk[index];
	PLAYER *player = GetPlayer(index);
	D3DXVECTOR3 val = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3	ave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < CHILD_ONESET_MAX; i++)
	{
		val = val + child->pos[i];
	}

	val = val + player->pos;
	ave = val / CHILD_ONESET_MAX;

	child->vec[cno] = ave - child->pos[cno];
	D3DXVec3Normalize(&child->vec[cno], &child->vec[cno]);
	child->move[cno].x = child->vec[cno].x * 5.0f;
	child->move[cno].z = child->vec[cno].z * 5.0f;
}