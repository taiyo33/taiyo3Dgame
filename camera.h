//=============================================================================
//
// カメラ処理 [camera.h]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

typedef struct {

	D3DXVECTOR3		pos;				// カメラの視点
	D3DXVECTOR3		at;					// カメラの注視点
	D3DXVECTOR3		up;					// カメラの上方向
	D3DXVECTOR3		nor;				// カメラの向き
	D3DXVECTOR3		rot;				// カメラの回転
	D3DXVECTOR3		fw;					// カメラの前方
	D3DXMATRIX		mtxView;			// ビューマトリックス
	D3DXMATRIX		mtxProjec;			// プロジェクションマトリックス
	float			rotSpeedX;			// カメラのX軸の回転速度
	float			rotSpeedY;			// カメラのY軸の回転速度
	float			fLengthInterval;	// カメラの視点と注視点の距離
	bool			mouse;

}CAMERA;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

void SetCamera(void);

CAMERA *GetCamera(void);
D3DXMATRIX GetMtxView(void);

#endif
