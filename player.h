//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP11A_341_22_田中太陽 GP11A341_22_田中太陽
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================

#define PLAYER_MAX				(2)



typedef struct {
	
	LPD3DXALLOCATEHIERARCHY		D3DXAlloc;			// ID3DXAllocインターフェースへのポインタ
	LPD3DXLOADUSERDATA			D3DXUserData;		// Xファイルのユーザー定義
	LPD3DXFRAME					D3DXFrame;			// フレーム階層の親のポインタ
	LPD3DXANIMATIONCONTROLLER	D3DXAnim;			// アニメーション情報
	D3DXMATRIX					mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3					pos;				// 現在の位置
	D3DXVECTOR3					prevPos;			// 過去の位置
	D3DXVECTOR3					move;				// 移動量
	D3DXVECTOR3					rot;				// 回転量
	D3DXVECTOR3					rotDest;			// 現在の向き
	bool						use;				// 使用状態
	float						speed;				// 移動の速さ
	int							IdxShadow;			// 影ID
	float						SizeShadow;			// 影のサイズ
	D3DXCOLOR					colShadow;			// 影の色

}PLAYER;

enum {
	P1,
	P2
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(int index);
bool PlayerDamageManager(int index);
bool CheckFieldInPlayer(int index);

#endif
