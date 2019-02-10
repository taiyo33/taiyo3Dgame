//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "camera.h"
#include "player.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VIEW_ANGLE			(D3DXToRadian(90.0f))	// 視野角
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z			(10.0f)				// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(1000.0f)			// ビュー平面のFarZ値
#define	VALUE_MOVE_CAMERA	(5.0f)				// カメラの移動量
#define INIT_POS_X			(0.0f)
#define INIT_POS_Y			(450.0f)
#define INIT_POS_Z			(-250.0f)
#define ROT_SPEED_X			(D3DX_PI * 0.01f)	// X軸回転速度の初期値
#define ROT_SPEED_Y			(D3DX_PI * 0.005f)	// Y軸回転速度の初期値

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

CAMERA				camera;		// カメラ構造体
//=============================================================================
// カメラの初期化
//=============================================================================
HRESULT InitCamera(void)
{
	camera.pos = D3DXVECTOR3(INIT_POS_X, INIT_POS_Y, INIT_POS_Z);		// 位置を初期化
	camera.at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点を初期化
	camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向を初期化
	camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転を初期化
	camera.rotSpeedX = ROT_SPEED_X;
	camera.rotSpeedY = ROT_SPEED_Y;

	float vx, vz;
	vx = camera.pos.x - camera.at.x;
	vz = camera.pos.z - camera.at.z;
	camera.fLengthInterval = sqrtf(vx * vx + vz * vz);

	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate,mtxQua;

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&camera.mtxView);

	D3DXMatrixLookAtLH(&camera.mtxView,
								&camera.pos,	// 位置
								&camera.at,		// 注視点
								&camera.up);	// 上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&camera.mtxProjec);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&camera.mtxProjec,
								VIEW_ANGLE,		// 視野角
								VIEW_ASPECT,	// アスペクト比
								VIEW_NEAR_Z,	// ビュー平面のNearZ値
								VIEW_FAR_Z);	// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camera.mtxProjec);
}

//=============================================================================
// カメラの向きの取得
// 引　数：なし
// 戻り値：CAMERA型構造体
//=============================================================================
CAMERA *GetCamera(void)
{
	return &camera;
}

//=============================================================================
// ビューマトリックスの取得
// 引　数：なし
// 戻り値：D3DXMATRIX型構造体
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return camera.mtxView;
}
