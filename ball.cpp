//=============================================================================
//
// ボールモデル処理 [ball.cpp]
// Author : GP11A341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "ball.h"
#include "bullet.h"
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

#define RATE_MOVE_BALL		(0.20f)				// 移動速度
#define BALL_SIZE			(17.0f)				// モデルサイズ
#define BALL_FALL_SPEED	(5.0f)					// 落下速度

enum BALLMODEL{
	MODEL_TYPE001,
	MODEL_TYPE002
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MoveBall(int index, int cno);
void ChaseBall(int index, int cno);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9	D3DTexture = NULL;			// テクスチャへのポインタ
static LPD3DXMESH			D3DMesh[BALL_SET_MAX];		// メッシュ情報へのポインタ
static LPD3DXBUFFER			D3DXBuffMat[BALL_SET_MAX];	// マテリアル情報へのポインタ
static DWORD				NumMat[BALL_SET_MAX];		// マテリアル情報の数

static D3DXMATRIX			MtxWorld;					// ワールドマトリックス
BALL						BallWk[BALL_SET_MAX];		// ボール構造体
//===============================================================================
// 初期化処理
// 引　数：int type(再初期化の2数判定値)
// 戻り値：HRESULT型
//===============================================================================
HRESULT InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BALL *ball = &BallWk[0];
	PLAYER *player = GetPlayer(0);

	// 各変数の初期化
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
			ball[i].prevPos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ball[i].vec[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		for (int k = 0; k < BALL_ONESET_MAX / 2; k++)
		{
			ball[i].use[k] = true;
		}
	}

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
	BALL *ball = &BallWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		for (int j = 0; j < BALL_ONESET_MAX; j++)
		{
			if (ball[i].use[j])
			{
				// 過去の位置を保存
				ball[i].prevPos[j] = ball[i].pos[j];
				
				// フィールドへ落下
				if (ball[i].pos[j].y > 10.0f)
				{
					ball[i].pos[j].y -= BALL_FALL_SPEED;
				}

				// フィールド上での動き
				if (ball[i].pos[j].y > 0.0f)
				{
					// 追跡
					ChaseBall(i, j);
					// 移動
					MoveBall(i, j);
				}
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
	BALL *ball = &BallWk[0];

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
// ボールのアドレス取得
// 引　数：int index(組ボールのアドレス)
// 戻り値：BALL型
//========================================================================
BALL *GetBall(int index)
{
	return &BallWk[index];
}

//========================================================================
// ボール数のによるリザルト変更判定処理
// 引　数：な　し
// 戻り値：な　し
//========================================================================
void ComprareBall(void)
{
	BALL *ball = &BallWk[0]; 
	PLAYER *player = GetPlayer(0);
	int max = 0;
	int index = 0;

	// 所持数の比較
	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		max = max(ball[i].cnt, max);
		if (ball[i].cnt == max)
		{
			index = i;
		}
	}

	// ゲームモードによるアイコン変更
	// NPCモード
	if (player[index].npc)
	{
		max == ball[index].cnt ? SetResult(index, index) : SetResult(index, NPC);
	}
	// P2モード
	else
	{
		max == ball[index].cnt ? SetResult(index, index) : SetResult(index, index);
	}
}

//========================================================================
// ボールの移動処理
// 引　数：int index(組ボールのアドレス), int cno(ボール単体のアドレス)
// 戻り値：な　し
//========================================================================
void MoveBall(int index, int cno)
{
	BALL *ball = &BallWk[index];

	// 移動量に慣性をかける
	ball->move[cno].x += (0.0f - ball->move[cno].x) * RATE_MOVE_BALL;
	ball->move[cno].y += (0.0f - ball->move[cno].y) * RATE_MOVE_BALL;
	ball->move[cno].z += (0.0f - ball->move[cno].z) * RATE_MOVE_BALL;

	// 位置移動
	ball->pos[cno].x += ball->move[cno].x;
	ball->pos[cno].y += ball->move[cno].y;
	ball->pos[cno].z += ball->move[cno].z;

}

//============================================================================
// ボールの追従判定処理
// 引　数：int index(組ボールのアドレス), int cno(ボールのアドレス)
// 戻り値：なし
//============================================================================
void ChaseBall(int index, int cno)
{
	BALL *ball = &BallWk[index];
	PLAYER *player = GetPlayer(index);
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 追従ベクトルの計算
	vec = player->pos - ball->pos[cno];
	D3DXVec3Normalize(&vec, &vec);

	// プレイヤーが範囲外に移動した場合追従
	if (!CheckHitBC(player->pos, ball->pos[cno], BALL_SIZE, BALL_SIZE))
	{
		ball->move[cno].x = vec.x * 5.0f;
		ball->move[cno].z = vec.z * 5.0f;
	}

	// ボール同士の離散処理
	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (!ball->use[i]) continue;
		if (CheckHitBC(ball->pos[cno], ball->pos[i], BALL_SIZE, BALL_SIZE))
		{
			// 重なり合うボールと逆方向ベクトルへ
			ball->vec[cno] = ball->pos[cno] - ball->pos[i];
			D3DXVec3Normalize(&ball->vec[cno], &ball->vec[cno]);
			ball->move[cno].x = (ball->move[cno].x + ball->vec[cno].x);
			ball->move[cno].z = (ball->move[cno].z + ball->vec[cno].z);
		}
	}
}

//=========================================================================
// ボールの発生処理
// 引　数：int index(組ボールのアドレス番号)
// 戻り値：な　し
//=========================================================================
void SetBall(int index)
{
	BALL *ball = &BallWk[index];

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
// ボールのダメージ判定処理
// 引　数：int index00(ダメージを受けたボールのアドレス)、
//		   int index01(ダメージを与えた相手のアドレス)、int bno(組バレットのアドレス)
// 戻り値：bool型
//=========================================================================
void DamageBall(int index00, int index01, int bno)
{
	BALL *ball = GetBall(0);
	BULLET *bullet = GetBullet(0);

	for (int j = 0; j < BALL_ONESET_MAX; j++)
	{
		if (!ball[index00].use[j]) continue;

		if (CheckHitBC(bullet[index01].pos[bno], ball[index00].pos[j], 15.0f, 15.0f))
		{
			PlaySound(ball[index00].damageSE, E_DS8_FLAG_NONE);				// 消滅音の再生
			ball[index00].use[j] = false;	// ボールを消滅
			ball[index00].cnt--;			// ボールの所持数を減らす
			ball[index01].cnt++;			// ボールの所持数を増やす
			SetBall(index01);				// ボールの設置
			SetExplosion(ball[index00].pos[j], ball[index00].rot[j], 0);	// 消滅エフェクト
		}
	}
}

//=========================================================================
// ボールの位置初期化処理
// 引　数：int index(ボールのアドレス番号)
// 戻り値：bool型　未使用の場合 true、使用中の場合 false
//=========================================================================
void SetInitPosBall(void)
{
	BALL *ball = &BallWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		for (int j = 0; j < BALL_ONESET_MAX / 2; j++)
		{
			ball[i].pos[j].x = player[i].pos.x + rand() % 50;
			ball[i].pos[j].y = player[i].pos.y;
			ball[i].pos[j].z = player[i].pos.z + rand() % 50;
		}
	}
}
