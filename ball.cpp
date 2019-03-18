//=============================================================================
//
// 子供モデル処理 [ball.cpp]
// Author : GP11A341_22_田中太陽
//
//=============================================================================
#include "ball.h"
#include "player.h"
#include "checkhit.h"
#include "block.h"
#include "explosion.h"
#include "result.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	BALL_MODEL01		"data/MODEL/ball01.x"				// 読み込むモデル名
#define	BALL_MODEL02		"data/MODEL/ball02.x"				// 読み込むモデル名

#define RATE_MOVE_BALL		(0.20f)
#define BALL_SIZE			(17.0f)					// モデルサイズ
#define CENTER_PULL_BALL	(25.0f)


enum BALLMODEL{
	MODEL_TYPE001,
	MODEL_TYPE002
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MoveBall(int index, int cno);
void ChaseBall(int index, int cno);
void InitPosBall(void);
void WallShearBall(int index, int cno);
void CheckNorBall(D3DXVECTOR3 nor0, int index, int cno);

void AlignmentBall(int index, int cno);
void ChesionBall(int index, int cno);
void SeparationBall(int index, int cno);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9	D3DTexture;		// テクスチャへのポインタ
static LPD3DXMESH			D3DMesh[BALL_SET_MAX];		// メッシュ情報へのポインタ
static LPD3DXBUFFER			D3DXBuffMat[BALL_SET_MAX];	// マテリアル情報へのポインタ
static DWORD				NumMat[BALL_SET_MAX];			// マテリアル情報の数


static D3DXMATRIX			MtxWorld;				// ワールドマトリックス
BALL						ballWk[BALL_SET_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BALL *ball = &ballWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		D3DMesh[i] = NULL;
		D3DXBuffMat[i] = NULL;
		NumMat[i] = 0;
		ball[i].cnt = 50;
		ball[i].damageSE = LoadSound(SE_BALLDAMAGE);

		for (int j = 0; j < BALL_ONESET_MAX; j++)
		{
			ball[i].pos[j].x = player[i].pos.x + rand() % 50;
			ball[i].pos[j].y = player[i].pos.y;
			ball[i].pos[j].z = player[i].pos.z + rand() % 50;
			ball[i].rot[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ball[i].scl[j] = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
			ball[i].use[j] = true;
			ball[i].prevPos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ball[i].vec[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}


	D3DTexture = NULL;

	// Xファイルの読み込み
	{
		if(FAILED(D3DXLoadMeshFromX(BALL_MODEL01,				// 読み込むモデルファイル名(Xファイル)
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
		if (FAILED(D3DXLoadMeshFromX(BALL_MODEL02,				// 読み込むモデルファイル名(Xファイル)
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

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBall(void)
{
	if (D3DTexture != NULL)
	{// テクスチャの開放
		D3DTexture->Release();
		D3DTexture = NULL;
	}

	for (int i = 0; i < BALL_SET_MAX; i++)
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
void UpdateBall(void)
{
	BALL *ball = &ballWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		for (int j = 0; j < BALL_ONESET_MAX; j++)
		{
			if (ball[i].use[j])
			{
				ball[i].prevPos[j] = ball[i].pos[j];

				if (ball[i].pos[j].y > 10.0f)
				{
					ball[i].pos[j].y -= PLAYER_FALL_SPEED;
				}

				if (ball[i].pos[j].y > 0.0f)
				{
					// 追跡
					ChaseBall(i, j);
					// 移動
					MoveBall(i, j);
				}

				//// 壁ずり
				//WallShearBall(i, j);

				//AlignmentBall(i, j);
				//ChesionBall(i, j);
				//SeparationBall(i, j);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	BALL *ball = &ballWk[0];

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		for (int j = 0; j < BALL_ONESET_MAX; j++)
		{
			if (ball[i].use[j])
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&MtxWorld);

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, ball[i].scl[j].x, ball[i].scl[j].y, ball[i].scl[j].z);
				D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxScl);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, ball[i].rot[j].y, ball[i].rot[j].x, ball[i].rot[j].z);
				D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, ball[i].pos[j].x, ball[i].pos[j].y, ball[i].pos[j].z);
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
BALL *GetBall(int index)
{
	return &ballWk[index];
}

//========================================================================
// 子供モデル数のによるリザルト変更判定処理
// 引　数：な　し
// 戻り値：な　し
//========================================================================
void ComprareBall(void)
{
	BALL *ball = &ballWk[0]; 
	PLAYER *player = GetPlayer(0);
	int max = 0;
	int index = 0;

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		max = max(ball[i].cnt, max);
		if (ball[i].cnt == max)
		{
			index = i;
		}
	}

	if (player[index].npc)
	{
		max == ball[index].cnt ? SetResult(index, index) : SetResult(index, NPC);
	}
	else
	{
		max == ball[index].cnt ? SetResult(index, index) : SetResult(index, index);
	}
}

//========================================================================
// 子供モデルのダメージ判定処理
// 引　数：int index(組子供モデルのアドレス), int blockNo(ブロックのアドレス)
//		   D3DXVECTOR3 pos(対象子供モデルの位置)
// 戻り値：bool型　trueであれば、falseならば消滅
//========================================================================
void InitPosBall(void)
{
	BALL *ball = &ballWk[0];

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		ball[0].pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ball[1].pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//========================================================================
// 子供モデルの移動処理
// 引　数：int index(組子供モデルのアドレス), int blockNo(ブロックのアドレス)
// 戻り値：bool型　trueであれば、falseならば消滅
//========================================================================
void MoveBall(int index, int cno)
{
	BALL *ball = &ballWk[index];

	//ball->move[cno].x = ball->move[cno].x + (ball->v1[cno].x * 0.1f) + (ball->v2[cno].x * 1.0f) + (ball->v3[cno].x * 0.8f);
	//ball->move[cno].z = ball->move[cno].z + (ball->v1[cno].z * 0.1f) + (ball->v2[cno].z * 1.0f) + (ball->v3[cno].z * 0.8f);

	// 移動量に慣性をかける
	ball->move[cno].x += (0.0f - ball->move[cno].x) * RATE_MOVE_BALL;
	ball->move[cno].y += (0.0f - ball->move[cno].y) * RATE_MOVE_BALL;
	ball->move[cno].z += (0.0f - ball->move[cno].z) * RATE_MOVE_BALL;

	// 位置移動
	ball->pos[cno].x += ball->move[cno].x;
	ball->pos[cno].y += ball->move[cno].y;
	ball->pos[cno].z += ball->move[cno].z;

	//ball->v1[cno] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//ball->v2[cno] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//ball->v3[cno] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//============================================================================
// 子供モデルの追従判定処理
// 引　数：int index(組子供モデルのアドレス), int cno(子供モデルのアドレス)
// 戻り値：なし
//============================================================================
void ChaseBall(int index, int cno)
{
	BALL *ball = &ballWk[index];
	PLAYER *player = GetPlayer(index);
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	 // プレイヤーと子供の先頭アドレス番号の追従
	//if (cno % 4 == 0)
	{
		// 追従ベクトルの計算
		vec = player->pos - ball->pos[cno];
		D3DXVec3Normalize(&vec, &vec);
		// プレイヤーが範囲外に移動した場合追従
		if (!CheckHitBC(player->pos, ball->pos[cno], BALL_SIZE, BALL_SIZE))
		{
			ball->move[cno].x = vec.x * 5.0f;
			ball->move[cno].z = vec.z * 5.0f;
		}
	}
	// 子供同士の追従
	//else
	//{
	//	// 追従ベクトルの計算
	//	vec = ball->pos[cno - 1] - ball->pos[cno];
	//	D3DXVec3Normalize(&vec, &vec);
	//	// 前アドレスの子供が範囲外に移動した場合追従
	//	if (!CheckHitBC(ball->pos[cno], ball->pos[cno - 1], BALL_SIZE, BALL_SIZE))
	//	{
	//		ball->move[cno].x = vec.x * 5.0f;
	//		ball->move[cno].z = vec.z * 5.0f;
	//	}
	//}

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (!ball->use[i]) continue;
		if (CheckHitBC(ball->pos[cno], ball->pos[i], BALL_SIZE, BALL_SIZE))
		{
			ball->vec[cno] = ball->pos[cno] - ball->pos[i];
			D3DXVec3Normalize(&ball->vec[cno], &ball->vec[cno]);
			ball->move[cno].x = (ball->move[cno].x + ball->vec[cno].x);
			ball->move[cno].z = (ball->move[cno].z + ball->vec[cno].z);
		}
	}
}

//==========================================================================
// 壁ずりのすり抜け予防処理(ブロック使用中にマップ外へ出ないように)
// 引　数：D3DXVECTOR3 nor0(ポリゴンの法線), int index(プレイヤーのアドレス番号)
// 戻り値：な　し
//==========================================================================
void WallShearBall(int index, int cno)
{
	BALL *ball = &ballWk[index];
	PLAYER *player = GetPlayer(index);
	if (!HitCheckBlock(ball->prevPos[cno] + ball->move[cno], ball->prevPos[cno], BLOCK_VTX_MAX))
	{
		ball->move[cno] = WallShear(ball->pos[cno] + ball->move[cno], GetNormal(), index);
		CheckNorBall(GetNormal(), index, cno);
	}
}

//==========================================================================
// 壁ずりのすり抜け予防処理(ブロック使用中にマップ外へ出ないように)
// 引　数：D3DXVECTOR3 nor0(ポリゴンの法線), int index(プレイヤーのアドレス番号)
// 戻り値：な　し
//==========================================================================
void CheckNorBall(D3DXVECTOR3 nor0, int index, int cno)
{
	BALL *ball = &ballWk[index];

	// 法線がX軸方向なら
	if (nor0.x != 0.0f)
	{
		ball->move[cno].x = 0.0f;
		ball->pos[cno].x = ball->prevPos[cno].x;
		return;
	}
	// 法線がZ軸方向なら
	if (nor0.z != 0.0f)
	{
		ball->move[cno].z = 0.0f;
		ball->pos[cno].z = ball->prevPos[cno].z;
		return;
	}
}

//=========================================================================
// 子供モデルの増加
// 引　数：D3DXVECTOR3 pos(位置)、int index(子供モデルのアドレス番号)
// 戻り値：bool型　未使用の場合 true、使用中の場合 false
//=========================================================================
void SetBall(int index)
{
	BALL *ball = &ballWk[index];

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (!ball->use[i])
		{
			// 初期設定
			ball->use[i] = true;
			ball->pos[i] = D3DXVECTOR3(0.0f, 200.0f, 0.0f);
			ball->rot[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ball->scl[i] = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
			ball->prevPos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ball->vec[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			
			SetExplosion(ball->pos[i], ball->rot[i], 0);
			return;
		}
	}

	return;
}

//=========================================================================
// 子供モデルの増加
// 引　数：D3DXVECTOR3 pos(位置)、int index(子供モデルのアドレス番号)
// 戻り値：bool型　未使用の場合 true、使用中の場合 false
//=========================================================================
void SetInitPosBall(void)
{
	BALL *ball = &ballWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		for (int j = 0; j < BALL_ONESET_MAX / 2; j++)
		{
			// 位置・回転・スケールの初期設定
			ball[i].pos[j].x = player[i].pos.x + rand() % 50;
			ball[i].pos[j].y = player[i].pos.y;
			ball[i].pos[j].z = player[i].pos.z + rand() % 50;
		}
	}
}


// 整列
void AlignmentBall(int index, int cno)
{
	BALL *ball = &ballWk[index];
	PLAYER *player = GetPlayer(index);

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (cno == i) continue;
		ball->v1[cno].x += ball->move[i].x;
		ball->v1[cno].z += ball->move[i].z;
	}
	ball->v1[cno].x += player->move.x;
	ball->v1[cno].z += player->move.z;

	ball->v1[cno].x /= (BALL_ONESET_MAX);
	ball->v1[cno].z /= (BALL_ONESET_MAX);


	ball->v1[cno].x = (ball->v1[cno].x - ball->move[cno].x) / 2;
	ball->v1[cno].z = (ball->v1[cno].z - ball->move[cno].z) / 2;

}

// 結合
void ChesionBall(int index, int cno)
{
	BALL *ball = &ballWk[index];
	PLAYER *player = GetPlayer(index);

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (cno == i) continue;
		ball->v2[cno].x += ball->pos[i].x;
		ball->v2[cno].z += ball->pos[i].z;
	}

	ball->v2[cno].x += player->pos.x;
	ball->v2[cno].z += player->pos.z;

	ball->v2[cno].x /= (BALL_ONESET_MAX);
	ball->v2[cno].z /= (BALL_ONESET_MAX);

	ball->v2[cno].x = (ball->v2[cno].x - ball->pos[cno].x) / CENTER_PULL_BALL;
	ball->v2[cno].z = (ball->v2[cno].z - ball->pos[cno].z) / CENTER_PULL_BALL;
}


// 分離
void SeparationBall(int index, int cno)
{
	BALL *ball = &ballWk[index];
	PLAYER *player = GetPlayer(index);

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (cno == i) continue;
		if (CheckHitBC(ball->pos[cno], ball->pos[i], 15.0f, 15.0f))
		{
			ball->v3[cno].x -= ball->pos[cno].x - ball->pos[i].x;
			ball->v3[cno].z -= ball->pos[cno].z - ball->pos[i].z;
		}
	}
}
