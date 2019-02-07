//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "debugproc.h"
#include "bullet.h"
#include "field.h"
#include "block.h"
#include "checkhit.h"
#include "ai.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_AIRPLANE		"data/MODEL/model_body.x"	// 読み込むモデル名
#define HANDGUM_TEXTURE		"data/TEXTURE/handgun.jpg"	// 読み込むテクスチャー
#define	RATE_MOVE_PLAYER		(0.20f)					// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.02f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.20f)						// 回転慣性係数
#define VALUE_MOVE_PLAYER	(0.50f)						// 移動速度
#define PLAYER_POS_Y_LIMIT	(-200.0f)					// Y軸の上限値
#define AI_UPDATE_TIME		(10)						// ＡＩの情報更新時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MovePlayer(int index);
void InputPlayer1(void);
void InputGamePadPlayer1(void);
void InputPlayer2(void);
D3DXVECTOR3 WallShear(D3DXVECTOR3 pos, D3DXVECTOR3 normal, int index);
void WallShearPlayer(int index);
void NonePlayerMove(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9			D3DTexture;				// テクスチャ読み込み場所
static LPD3DXMESH					D3DXMesh;				// ID3DXMeshインターフェイスへのポインタ
static LPD3DXBUFFER					D3DXBuffMat;			// メッシュのマテリアル情報を格納
static DWORD						NumMat;					// 属性情報の総数
static int							cntFrame[PLAYER_MAX];	// フレームカウント
PLAYER								player[PLAYER_MAX];		// プレイヤー構造体
//=============================================================================
// 初期化処理
// 引　数：int type(再初期化２数判定値)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DTexture = NULL;	// テクスチャーの初期化
	D3DXMesh = NULL;		// インタフェースの初期化
	D3DXBuffMat = NULL;	// マテリアルの初期化
	player[P1].pos = D3DXVECTOR3(-200.0f, 10.0f, 0.0f);	// 位置の初期化
	player[P2].pos = D3DXVECTOR3(200.0f, 10.0f, 0.0f);	//
	player[P1].use = true;								// 使用状態を初期化
	player[P2].use = true;								//
	player[P1].life = PLAYER_LIFE_MAX;					// プレイヤーの体力を初期化
	player[P2].life = 0;								// 

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		player[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転の初期化
		player[i].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転の目的位置を初期化
		player[i].speed = VALUE_MOVE_PLAYER;				// 移動速度の初期化
		player[i].special = 100.0f;
		player[i].angle = 0.0f;
		cntFrame[i] = 0;
	}

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(PLAYER_AIRPLANE,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&D3DXBuffMat,
		NULL,
		&NumMat,
		&D3DXMesh)))
	{
		return E_FAIL;
	}

	// 法線正規化の設定
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//// 再初期化の場合読み込まない
	//if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
							HANDGUM_TEXTURE,	// ファイルの名前
						&D3DTexture);	// 読み込むメモリー
	}	

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if(D3DTexture != NULL)
	{// テクスチャの開放
		D3DTexture->Release();
		D3DTexture = NULL;
	}

	if(D3DXMesh != NULL)
	{// メッシュの開放
		D3DXMesh->Release();
		D3DXMesh = NULL;
	}

	if(D3DXBuffMat != NULL)
	{// マテリアルの開放
		D3DXBuffMat->Release();
		D3DXBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// 現在位置を保存
		player[i].prevPos = player[i].pos;
		cntFrame[i]++;
		
		// 操作の処理
		InputPlayer1();
		InputPlayer2();
		InputGamePadPlayer1();

		// 壁ずり処理
		WallShearPlayer(i);
		
		NonePlayerMove();

		// プレイヤーの操作
		MovePlayer(i);

		PlayerDamageManager(i);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxView;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (player[i].use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&player[i].mtxWorld);

			mtxView = GetMtxView();

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, player[i].rot.y, player[i].rot.x, player[i].rot.z);
			D3DXMatrixMultiply(&player[i].mtxWorld, &player[i].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, player[i].pos.x, player[i].pos.y, player[i].pos.z);
			D3DXMatrixMultiply(&player[i].mtxWorld, &player[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &player[i].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)D3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)NumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, D3DTexture);

				// 描画
				D3DXMesh->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// プレイヤーのアドレス取得
// 引　数：int index(プレイヤーのアドレス番号)
// 戻り値：なし
//=============================================================================
PLAYER *GetPlayer(int index)
{
	return &player[index];
}

//=============================================================================
// プレイヤー1の操作処理
// 引　数：なし
// 戻り値：なし
//=============================================================================
void InputPlayer1(void)
{
	if (player[P1].use)
	{
		float fDiffRotY;

		CAMERA *camera = GetCamera();	// カメラのアドレスを取得
		BULLET *bullet = GetBullet(P1);	// バレットのアドレスを取得

		if (GetKeyboardPress(DIK_RIGHT))
		{
			if (GetKeyboardPress(DIK_UP))
			{// 右前移動
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// 右後移動
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// 右移動
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_LEFT))
		{
			if (GetKeyboardPress(DIK_UP))
			{// 左前移動
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// 左後移動
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// 左移動
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{
			// 前移動
			player[P1].move.x -= sinf(D3DX_PI + camera->rot.y) * player[P1].speed;
			player[P1].move.z -= cosf(D3DX_PI + camera->rot.y) * player[P1].speed;

			player[P1].rotDest.y = camera->rot.y;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{
			// 後移動
			player[P1].move.x -= sinf(camera->rot.y) * player[P1].speed;
			player[P1].move.z -= cosf(camera->rot.y) * player[P1].speed;

			player[P1].rotDest.y = D3DX_PI + camera->rot.y;
		}

		// 目的の角度までの差分
		fDiffRotY = player[P1].rotDest.y - player[P1].rot.y;
		if (fDiffRotY > D3DX_PI)
		{
			fDiffRotY -= D3DX_PI * 2.0f;
		}
		if (fDiffRotY < -D3DX_PI)
		{
			fDiffRotY += D3DX_PI * 2.0f;
		}

		// 目的の角度まで慣性をかける
		player[P1].rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
		if (player[P1].rot.y > D3DX_PI)
		{
			player[P1].rot.y -= D3DX_PI * 2.0f;
		}
		if (player[P1].rot.y < -D3DX_PI)
		{
			player[P1].rot.y += D3DX_PI * 2.0f;
		}

		// バレットのチャージ
		if (GetKeyboardPress(DIK_SPACE))
		{
			// 最大値になった場合
			if (bullet->speedIncrease > BULLET_CHARGE_MAX)
			{
                bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10フレーム
			else if (cntFrame[P1] % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.5f;
			}
		}
		// バレットの発射
        else if(GetKeyboardRelease(DIK_SPACE))
		{
          	SetBullet(player[P1].pos, player[P1].rot, bullet->speedIncrease, 0, P1);
			cntFrame[P1] = 0;
		}

		if (GetKeyboardTrigger(DIK_O))
		{
			player[P1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
}

//=============================================================================
// プレイヤー1の操作処理
// 引　数：なし
// 戻り値：なし
//=============================================================================
void InputGamePadPlayer1(void)
{
	if (player[P1].use)
	{
		float fDiffRotY;


		CAMERA *camera = GetCamera();	// カメラのアドレスを取得
		BULLET *bullet = GetBullet(P1);	// バレットのアドレスを取得
		if (IsButtonPressed(0,BUTTON_RIGHT))
		{
			if (IsButtonPressed(0,BUTTON_UP))
			{// 右前移動
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (IsButtonPressed(0,BUTTON_DOWN))
			{// 右後移動
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// 右移動
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (IsButtonPressed(0,BUTTON_LEFT))
		{
			if (IsButtonPressed(0,BUTTON_UP))
			{// 左前移動
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (IsButtonPressed(0,BUTTON_DOWN))
			{// 左後移動
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// 左移動
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (IsButtonPressed(0,BUTTON_UP))
		{
			// 前移動
			player[P1].move.x -= sinf(D3DX_PI + camera->rot.y) * player[P1].speed;
			player[P1].move.z -= cosf(D3DX_PI + camera->rot.y) * player[P1].speed;

			player[P1].rotDest.y = camera->rot.y;
		}
		else if (IsButtonPressed(0,BUTTON_DOWN))
		{
			// 後移動
			player[P1].move.x -= sinf(camera->rot.y) * player[P1].speed;
			player[P1].move.z -= cosf(camera->rot.y) * player[P1].speed;

			player[P1].rotDest.y = D3DX_PI + camera->rot.y;
		}

		// 目的の角度までの差分
		fDiffRotY = player[P1].rotDest.y - player[P1].rot.y;
		if (fDiffRotY > D3DX_PI)
		{
			fDiffRotY -= D3DX_PI * 2.0f;
		}
		if (fDiffRotY < -D3DX_PI)
		{
			fDiffRotY += D3DX_PI * 2.0f;
		}

		// 目的の角度まで慣性をかける
		player[P1].rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
		if (player[P1].rot.y > D3DX_PI)
		{
			player[P1].rot.y -= D3DX_PI * 2.0f;
		}
		if (player[P1].rot.y < -D3DX_PI)
		{
			player[P1].rot.y += D3DX_PI * 2.0f;
		}

		// バレットのチャージ
		if (IsButtonPressed(0, BUTTON_B))
		{
			// 最大値になった場合
			if (bullet->speedIncrease > BULLET_CHARGE_MAX)
			{
				bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10フレーム
			else if (cntFrame[P1] % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.5f;
			}
		}
		// バレットの発射
		else if (IsButtonRelease(0, BUTTON_B))
		{
			SetBullet(player[P1].pos, player[P1].rot, bullet->speedIncrease, 0, P1);
			cntFrame[P1] = 0;
		}
	}
}

//=============================================================================
// プレイヤー2の操作処理
// 引　数：なし
// 戻り値：なし
//=============================================================================
void InputPlayer2(void)
{
	float fDiffRotY;

	// カメラの向き取得
	CAMERA *camera = GetCamera();
	BULLET *bullet = GetBullet(P2);

	if (player[P2].use)
	{
		if (GetKeyboardPress(DIK_D))
		{
			if (GetKeyboardPress(DIK_W))
			{// 右前移動
				player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_S))
			{// 右後移動
				player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// 右移動
				player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_A))
		{
			if (GetKeyboardPress(DIK_W))
			{// 左前移動
				player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_S))
			{// 左後移動
				player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// 左移動
				player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_W))
		{
			// 前移動
			player[P2].move.x -= sinf(D3DX_PI + camera->rot.y) * player[P2].speed;
			player[P2].move.z -= cosf(D3DX_PI + camera->rot.y) * player[P2].speed;

			player[P2].rotDest.y = camera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S))
		{
			// 後移動
			player[P2].move.x -= sinf(camera->rot.y) * player[P2].speed;
			player[P2].move.z -= cosf(camera->rot.y) * player[P2].speed;

			player[P2].rotDest.y = D3DX_PI + camera->rot.y;
		}

		// 目的の角度までの差分
		fDiffRotY = player[P2].rotDest.y - player[P2].rot.y;
		if (fDiffRotY > D3DX_PI)
		{
			fDiffRotY -= D3DX_PI * 2.0f;
		}
		if (fDiffRotY < -D3DX_PI)
		{
			fDiffRotY += D3DX_PI * 2.0f;
		}

		// 目的の角度まで慣性をかける
		player[P2].rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
		if (player[P2].rot.y > D3DX_PI)
		{
			player[P2].rot.y -= D3DX_PI * 2.0f;
		}
		if (player[P2].rot.y < -D3DX_PI)
		{
			player[P2].rot.y += D3DX_PI * 2.0f;
		}

		if (GetKeyboardPress(DIK_P))
		{
			// 最大値になった場合
			if (bullet->speedIncrease > BULLET_CHARGE_MAX)
			{
				bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10フレーム
			else if (cntFrame[P2] % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.5f;
			}
		}
		else if (GetKeyboardRelease(DIK_P))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			cntFrame[P2] = 0;
		}
		
	}
}

//=============================================================================
// プレイヤーの移動処理
// 引　数：int index(プレイヤーのアドレス番号)
// 戻り値：なし
//=============================================================================
void MovePlayer(int index)
{
	// 移動量に慣性をかける
	player[index].move.x += (0.0f - player[index].move.x) * RATE_MOVE_PLAYER;
	player[index].move.y += (0.0f - player[index].move.y) * RATE_MOVE_PLAYER;
	player[index].move.z += (0.0f - player[index].move.z) * RATE_MOVE_PLAYER;

	// 位置移動
	player[index].pos.x += player[index].move.x;
	player[index].pos.y += player[index].move.y;
	player[index].pos.z += player[index].move.z;

}
 
//==========================================================================
// 壁ずりのすり抜け予防処理(ブロック使用中にマップ外へ出ないように)
// 引　数：D3DXVECTOR3 nor0(ポリゴンの法線), int index(プレイヤーのアドレス番号)
// 戻り値：な　し
//==========================================================================
void WallShearPlayer(int index)
{
	if (!HitCheckBlock(player[index].prevPos + player[index].move, player[index].prevPos))
	{
		player[index].move = WallShear(player[index].pos + player[index].move, GetNormal(), index);
		CheckNorPlayer(GetNormal(), index);
	}
}

//=============================================================================
// プレイヤーのダメージ処理
// 引　数：なし
// 戻り値：なし
//=============================================================================
bool PlayerDamageManager(int index)
{
	if (player[index].pos.y < PLAYER_POS_Y_LIMIT)
	{
		SetStage(RESULT);
		return false;
	}

	return true;
}

//=============================================================================
// プレイヤーのフィールド内外判定処理
// 引　数：int index(プレイヤーのアドレス番号)
// 戻り値：bool型　trueだとフィールド内、falseだとフィールド外を返す
//=============================================================================
bool CheckFieldInPlayer(int index)
{
	if (player[index].pos.x > FIELD_SIZE_X) return false;
	if (player[index].pos.x < -FIELD_SIZE_X) return false;
	if (player[index].pos.z > FIELD_SIZE_Z) return false;
	if (player[index].pos.z < -FIELD_SIZE_Z) return false;

	return true;
}

//==========================================================================
// 壁ずりのすり抜け予防処理(ブロック使用中にマップ外へ出ないように)
// 引　数：D3DXVECTOR3 nor0(ポリゴンの法線), int index(プレイヤーのアドレス番号)
// 戻り値：な　し
//==========================================================================
void CheckNorPlayer(D3DXVECTOR3 nor0, int index)
{
	// 法線がX軸方向なら
	if (nor0.x != 0.0f)
	{
		player[index].move.x = 0.0f;
		player[index].pos.x = player[index].prevPos.x;
		return;
	}
	// 法線がZ軸方向なら
	if (nor0.z != 0.0f)
	{
		player[index].move.z = 0.0f;
		player[index].pos.z = player[index].prevPos.z;
		return;
	}
}

//===========================================================================
// NPCの移動処理
// 引　数：D3DXVECTOR3 pos(次の移動位置)、D3DXVECTOR3 normal(ポリゴンの法線)
//		   int index(プレイヤーのアドレス番号)
// 戻り値：
//==========================================================================
void NonePlayerMove(void)
{
	BULLET *bullet = GetBullet(P2);
	float box, out;
	float atc, chase, escape;

	atc = FuzzyRightDown(player[P2].life, 40, 80);
	chase = FuzzyTrapezoid(player[P2].life, 0, 20, 40, 80);
	box = Or(atc, chase);
	escape = FuzzyRightUp(player[P2].life, 50, 80);
	out = Or(box, escape);

	if (0.5f < atc)
	{
		// 最大値になった場合
		if (bullet->speedIncrease > BULLET_CHARGE_MAX)
		{
			bullet->speedIncrease = BULLET_CHARGE_MAX;
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			cntFrame[P2] = 0;

		}
		// 10フレーム
		else if (cntFrame[P1] % BULLET_CHARGE_FRAME_CNT == 0)
		{
			bullet->speedIncrease += 0.5f;
		}
	}
	if (0.8f < chase)
	{
		D3DXVECTOR3 vec = player[P1].pos - player[P2].pos;
		D3DXVec3Normalize(&vec, &vec);
		player[P2].move.x += vec.x * player[P2].speed;
		player[P2].move.z += vec.z * player[P2].speed;
	}
	if (out == escape)
	{
	}
}