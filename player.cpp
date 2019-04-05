//=============================================================================
//
// プレイヤー処理 [Player.cpp]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bullet.h"
#include "field.h"
#include "block.h"
#include "checkhit.h"
#include "ai.h"
#include "result.h"
#include "chargeEffect.h"
#include "pause.h"
#include "hitEffect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_BODY			"data/MODEL/model_body.x"	// 読み込むモデル名
#define	RATE_MOVE_PLAYER	(0.20f)						// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.02f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.20f)						// 回転慣性係数
#define VALUE_MOVE_PLAYER	(0.50f)						// 移動速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MovePlayer(int index);
void InputPlayer1(void);
void InputKeyPlayer2(void);
void InputGamePadPlayer(int index);
void WallShearPlayer(int index);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9			D3DTexture = NULL;		// テクスチャ読み込み場所
static LPD3DXMESH					D3DXMesh = NULL;		// ID3DXMeshインターフェイスへのポインタ
static LPD3DXBUFFER					D3DXBuffMat = NULL;		// メッシュのマテリアル情報を格納
static DWORD						NumMat = NULL;			// 属性情報の総数
static int							cntFrame[PLAYER_MAX];	// バレットチャージのフレームカウント

PLAYER								Player[PLAYER_MAX];		// プレイヤー構造体
//=============================================================================
// 初期化処理
// 引　数：int type(再初期化２数判定値)
// 戻り値：HRESULT型
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA *camera = GetCamera();

	Player[P1].pos = D3DXVECTOR3(PLAYER01_INITPOS_X, PLAYER01_INITPOS_Y, PLAYER01_INITPOS_Z);	// 位置の初期化
	Player[P2].pos = D3DXVECTOR3(PLAYER02_INITPOS_X, PLAYER02_INITPOS_Y, PLAYER02_INITPOS_Z);	//
	Player[P1].rot = D3DXVECTOR3(0.0f, 90.0f, 0.0f);	// 回転の初期化
	Player[P2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転の初期化

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		Player[i].use = true;								// 使用状態を初期化
		Player[i].npc = false;								// 使用状態を初期化
		Player[i].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転の目的位置を初期化
		Player[i].life = PLAYER_LIFE_MAX;					// プレイヤーの体力を初期化
		Player[i].frontVec = D3DXVECTOR3(sinf(Player[i].rot.y) * 100.0f, 0.0f, cosf(Player[i].rot.y) * 100.0f);
		Player[i].speed = VALUE_MOVE_PLAYER;				// 移動速度の初期化
		Player[i].cntFrame= 0;
		Player[i].hitSE = LoadSound(SE_HIT);
		Player[i].chargeSE = LoadSound(SE_CHARGE);
	}

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(PLAYER_BODY,
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
	CAMERA *camera = GetCamera();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// 現在位置を保存
		Player[i].prevPos = Player[i].pos;
		Player[i].cntFrame++;
		// 前方ベクトルの更新
		Player[i].frontVec = D3DXVECTOR3(sinf(Player[i].rot.y) * 75.0f, 0.0f, cosf(Player[i].rot.y) * 75.0f);
		
		// タイトル内でのプレイヤー制御
		if (GetStage() == TITLE)
		{
			NonePlayerPatrol(i);
		}
		// ゲーム開始時
		else if (GetStage() == START)
		{
			// AIの行動
			if (Player[i].npc)
			{
				NonePlayerMove();
				NonePlayerAttack();
			}
			// 操作
			else
			{
				InputPlayer1();
				InputGamePadPlayer(i);
				InputKeyPlayer2();
			}
		}

		// 壁ずり処理
		WallShearPlayer(i);
		// プレイヤーの操作
		MovePlayer(i);

		PrintDebugProc("プレイヤーの回転[(%f)]\n", Player[i].rot.y);
		PrintDebugProc("プレイヤーの位置: [X:(%f),z:(%f)]\n", Player[i].pos.x, Player[i].pos.z);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, *mtxView;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&Player[i].mtxWorld);

		mtxView = GetMtxView();

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Player[i].rot.y, Player[i].rot.x, Player[i].rot.z);
		D3DXMatrixMultiply(&Player[i].mtxWorld, &Player[i].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, Player[i].pos.x, Player[i].pos.y, Player[i].pos.z);
		D3DXMatrixMultiply(&Player[i].mtxWorld, &Player[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &Player[i].mtxWorld);

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

//=============================================================================
// プレイヤーのアドレス取得
// 引　数：int index(プレイヤーのアドレス番号)
// 戻り値：なし
//=============================================================================
PLAYER *GetPlayer(int index)
{
	return &Player[index];
}

//=============================================================================
// プレイヤー1のキーボード操作処理
// 引　数：なし
// 戻り値：なし
//=============================================================================
void InputPlayer1(void)
{
	if (Player[P1].use)
	{
		CAMERA *camera = GetCamera();	// カメラのアドレスを取得
		BULLET *bullet = GetBullet(P1);	// バレットのアドレスを取得

		if (GetKeyboardPress(DIK_RIGHT))
		{
			if (GetKeyboardPress(DIK_UP))
			{// 右前移動
				Player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// 右後移動
				Player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// 右移動
				Player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_LEFT))
		{
			if (GetKeyboardPress(DIK_UP))
			{// 左前移動
				Player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// 左後移動
				Player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// 左移動
				Player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{
			if(GetKeyboardPress(DIK_RIGHT))
			{// 右移動
				Player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y + D3DX_PI * 0.50f;
			}
			else
			{
				// 前移動
				Player[P1].move.x -= sinf(D3DX_PI + camera->rot.y) * Player[P1].speed;
				Player[P1].move.z -= cosf(D3DX_PI + camera->rot.y) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{
			if (GetKeyboardPress(DIK_LEFT))
			{// 左移動
				Player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y - D3DX_PI * 0.50f;
			}
			else
			{
				// 後移動
				Player[P1].move.x -= sinf(camera->rot.y) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y) * Player[P1].speed;

				Player[P1].rot.y = D3DX_PI + camera->rot.y;
			}
		}

		// バレットのチャージ
		if (GetKeyboardPress(DIK_SPACE))
		{
			 SetChargeEffect(Player[P1].pos, P1);
			PlaySound(Player[P1].chargeSE, E_DS8_FLAG_NONE);
			// 最大値になった場合
			if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
			{
                bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10フレーム
			else if (Player[P1].cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.2f;
			}
		}
		// バレットの発射
        else if(GetKeyboardRelease(DIK_SPACE))
		{
			StopSound(Player[P1].chargeSE);
          	SetBullet(Player[P1].pos, Player[P1].rot, bullet->speedIncrease, 0, P1);
			cntFrame[P1] = 0;
		}

		if (GetKeyboardTrigger(DIK_M))
		{
			SetPause(true);
		}

#ifdef _DEBUG

		// 位置を初期化
		if (GetKeyboardTrigger(DIK_O))
		{
			Player[P1].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		}
#endif
	}
}

//=============================================================================
// プレイヤー2のキーボード操作処理
// 引　数：なし
// 戻り値：なし
//=============================================================================
void InputKeyPlayer2(void)
{
	// カメラの向き取得
	CAMERA *camera = GetCamera();
	BULLET *bullet = GetBullet(P2);

	if (Player[P2].use)
	{
		if (GetKeyboardPress(DIK_D))
		{
			if (GetKeyboardPress(DIK_W))
			{// 右前移動
				Player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_S))
			{// 右後移動
				Player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// 右移動
				Player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_A))
		{
			if (GetKeyboardPress(DIK_W))
			{// 左前移動
				Player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_S))
			{// 左後移動
				Player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// 左移動
				Player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_W))
		{
			// 前移動
			Player[P2].move.x -= sinf(D3DX_PI + camera->rot.y) * Player[P2].speed;
			Player[P2].move.z -= cosf(D3DX_PI + camera->rot.y) * Player[P2].speed;

			Player[P2].rot.y = camera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S))
		{
			// 後移動
			Player[P2].move.x -= sinf(camera->rot.y) * Player[P2].speed;
			Player[P2].move.z -= cosf(camera->rot.y) * Player[P2].speed;

			Player[P2].rot.y = D3DX_PI + camera->rot.y;
		}

		if (GetKeyboardPress(DIK_Z))
		{
			// バレットが使用中はチャージ不可
			if (bullet->use[P2]) return;

			// 最大値になった場合
			if (bullet->speedIncrease > BULLET_CHARGE_MAX)
			{
				bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10フレーム
			else if (Player[P2].cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
			{
				PlaySound(Player[P2].chargeSE, E_DS8_FLAG_NONE);
				bullet->speedIncrease += 0.5f;
			}
		}
		else if (GetKeyboardRelease(DIK_Z))
		{			
			StopSound(Player[P2].chargeSE);
			SetBullet(Player[P2].pos, Player[P2].rot, bullet->speedIncrease, 0, P2);
			cntFrame[P2] = 0;
		}
		
		if (GetKeyboardTrigger(DIK_M))
		{
			SetPause(true);
		}

	}
}

//=============================================================================
// プレイヤーのゲームパッド操作処理
// 引　数：なし
// 戻り値：なし
//=============================================================================
void InputGamePadPlayer(int index)
{
	if (Player[index].use)
	{
		CAMERA *camera = GetCamera();	// カメラのアドレスを取得
		BULLET *bullet = GetBullet(index);	// バレットのアドレスを取得
		if (IsButtonPressed(index,BUTTON_RIGHT))
		{
			if (IsButtonPressed(index,BUTTON_UP))
			{// 右前移動
				Player[index].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (IsButtonPressed(index,BUTTON_DOWN))
			{// 右後移動
				Player[index].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// 右移動
				Player[index].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (IsButtonPressed(index,BUTTON_LEFT))
		{
			if (IsButtonPressed(index,BUTTON_UP))
			{// 左前移動
				Player[index].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (IsButtonPressed(index,BUTTON_DOWN))
			{// 左後移動
				Player[index].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// 左移動
				Player[index].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (IsButtonPressed(index,BUTTON_UP))
		{
			// 前移動
			Player[index].move.x -= sinf(D3DX_PI + camera->rot.y) * Player[index].speed;
			Player[index].move.z -= cosf(D3DX_PI + camera->rot.y) * Player[index].speed;

			Player[index].rot.y = camera->rot.y;
		}
		else if (IsButtonPressed(index,BUTTON_DOWN))
		{
			// 後移動
			Player[index].move.x -= sinf(camera->rot.y) * Player[index].speed;
			Player[index].move.z -= cosf(camera->rot.y) * Player[index].speed;

			Player[index].rot.y = D3DX_PI + camera->rot.y;
		}

		// バレットのチャージ
		if (IsButtonPressed(index, BUTTON_B))
		{
			 SetChargeEffect(Player[index].pos, index);
			PlaySound(Player[index].chargeSE, E_DS8_FLAG_NONE);

			// 最大値になった場合
			if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
			{
				bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10フレーム
			else if (Player[index].cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.2f;
			}
		}
		// バレットの発射
		else if (IsButtonRelease(index, BUTTON_B))
		{
			StopSound(Player[index].chargeSE);
			SetBullet(Player[index].pos, Player[index].rot, bullet->speedIncrease, 0, index);
			cntFrame[index] = 0;
		}

		if (IsButtonTriggered(index, BUTTON_R))
		{
			SetPause(true);
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
	if (Player[index].rot.y > D3DX_PI)
	{
		Player[index].rot.y -= D3DX_PI * 2.0f;
	}
	if (Player[index].rot.y < -D3DX_PI)
	{
		Player[index].rot.y += D3DX_PI * 2.0f;
	}

	// 移動量に慣性をかける
	Player[index].move.x += (0.0f - Player[index].move.x) * RATE_MOVE_PLAYER;
	Player[index].move.y += (0.0f - Player[index].move.y) * RATE_MOVE_PLAYER;
	Player[index].move.z += (0.0f - Player[index].move.z) * RATE_MOVE_PLAYER;

	// 位置移動
	Player[index].pos.x += Player[index].move.x;
	Player[index].pos.y += Player[index].move.y;
	Player[index].pos.z += Player[index].move.z;

}
 
//==========================================================================
// 壁ずり判定処理
// 引　数：int index(プレイヤーのアドレス番号)
// 戻り値：な　し
//==========================================================================
void WallShearPlayer(int index)
{
	if (!HitCheckBlock(Player[index].prevPos + Player[index].move, Player[index].prevPos, BLOCK_VTX_MAX))
	{
		Player[index].move = WallShear(Player[index].pos + Player[index].move, GetNormal(), index);
		CheckNorPlayer(GetNormal(), index);
	}
}

//=============================================================================
// プレイヤーのダメージ処理
// 引　数：int pno00(ダメージを受けたプレイヤーアドレス番号), 
//		   int pno01(ダメージを与えたプレイヤーアドレス番号), 
//		   int bno(バレット単体のアドレス番号)
// 戻り値：なし
//=============================================================================
void PlayerDamageManager(int pno00, int pno01, int bno)
{
	BULLET *bullet = GetBullet(0);

	if (CheckHitBC(bullet[pno01].pos[bno], Player[pno00].pos,
		bullet[pno01].size[bno].x, 10.0f))
	{
		// プレイヤーに対する処理
		PlaySound(Player[pno00].hitSE, E_DS8_FLAG_NONE);
		SetHitEffect(bullet[pno01].pos[bno], D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
		Player[pno00].life -= BULLET_DAMAGE;
		
		// バレットに対する処理
		bullet[pno01].use[bno] = false;
		bullet[pno01].reflect[bno] = false;
		bullet[pno01].cntReflect[bno] = INIT_REFLECT_CNT;
		bullet[pno01].speed[bno] = INIT_BULLET_SPEED;
	}

	// ダメージを受けたプレイヤーのライフがなくなったとき
	if (Player[pno00].life < 0)
	{
		SetStage(FINISHCALL);
	}
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
		Player[index].move.x = 0.0f;
		Player[index].pos.x = Player[index].prevPos.x;
		return;
	}
	// 法線がZ軸方向なら
	if (nor0.z != 0.0f)
	{
		Player[index].move.z = 0.0f;
		Player[index].pos.z = Player[index].prevPos.z;
		return;
	}
}

//=============================================================================
// プレイヤーの位置初期化処理
// 引　数：なし
// 戻り値：なし
//=============================================================================
void SetInitPosPlayer(void)
{
	PLAYER *Player = GetPlayer(0);

	Player[P1].pos = D3DXVECTOR3(PLAYER01_INITPOS_X, PLAYER01_INITPOS_Y, PLAYER01_INITPOS_Z);	// 位置の初期化
	Player[P2].pos = D3DXVECTOR3(PLAYER02_INITPOS_X, PLAYER02_INITPOS_Y, PLAYER02_INITPOS_Z);	//
}
