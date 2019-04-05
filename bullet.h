//=============================================================================
//
// バレット処理 [bullet.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_MAX				(128)				// 使用可能のバレット数
#define BULLET_ONESET_MAX		(1)					// バレット1組の最大数
#define BULLET_SET_MAX			(2)					// バレットの組数
#define BULLET_CHARGE_FRAME_CNT	(10)				// チャージ速度の調整フレーム数
#define BULLET_CHARGE_MAX		(10.0f)				// 速度チャージの最大値
#define INIT_REFLECT_CNT		(2)					// 反射回数
#define INIT_BULLET_SPEED		(5.0f)				// バレット速度の初期値
#define BULLET_DAMAGE			(10.0f)				// バレットのダメージ値

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	D3DXMATRIX		mtxWorld;						// ワールドマトリックス
	LPD3DXMESH		D3DXMesh;						// ID3DXMeshインターフェイスへのポインタ
	LPD3DXBUFFER	D3DXBuffMat;					// メッシュのマテリアル情報を格納
	DWORD			NumMat;							// 属性情報の総数
	D3DXVECTOR3		pos[BULLET_ONESET_MAX];			// 位置
	D3DXVECTOR3		prevPos[BULLET_ONESET_MAX];		// 前回の位置
	D3DXVECTOR3		move[BULLET_ONESET_MAX];		// 移動量
	D3DXVECTOR3		refVec[BULLET_ONESET_MAX];		// 反射ベクトル
	D3DXVECTOR3		rot[BULLET_ONESET_MAX];			// 回転量
	D3DXVECTOR3		scl[BULLET_ONESET_MAX];			// スケール
	D3DXVECTOR3		size[BULLET_ONESET_MAX];		// 大きさ
	D3DXVECTOR3		sclIncrease;					// スケールの増加値
	LPDIRECTSOUNDBUFFER8	BulletSE;				// バレット発射時のSE
	LPDIRECTSOUNDBUFFER8	ReflectSE;				// 反射時のSE
	int				time[BULLET_ONESET_MAX];		// 寿命
	int				cntReflect[BULLET_ONESET_MAX];	// 反射回数
	bool			use[BULLET_ONESET_MAX];			// 使用状態
	bool			reflect[BULLET_ONESET_MAX];		// 反射の前か後か
	float			speed[BULLET_ONESET_MAX];		// 速さ
	float			speedIncrease;					// 速さの増加

} BULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(int type);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
BULLET *GetBullet(int bno);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float speed, float Dest, int index);

#endif
