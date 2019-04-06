//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "leftArm.h"
#include "rightArm.h"
#include "head.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define PLAYER_MAX				(2)			// プレイヤーの最大数
#define PLAYER_LIFE_MAX			(100.0f)	// プレイヤーの最大体力

#define PLAYER01_INITPOS_X		(-300.0f)	// P1の初期位置
#define PLAYER01_INITPOS_Y		(10.0f)		//
#define PLAYER01_INITPOS_Z		(200.0f)	//

#define PLAYER02_INITPOS_X		(300.0f)	// P2の初期位置
#define PLAYER02_INITPOS_Y		(10.0f)		//
#define PLAYER02_INITPOS_Z		(-200.0f)	//

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct {
	
	D3DXMATRIX					mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3					pos;				// 現在の位置
	D3DXVECTOR3					prevPos;			// 過去の位置
	D3DXVECTOR3					move;				// 移動量
	D3DXVECTOR3					rot;				// 回転量
	D3DXVECTOR3					rotDest;			// 現在の向き
	D3DXVECTOR3					frontVec;			// 前方ベクトル
	bool						use;				// 使用状態
	float						speed;				// 移動の速さ
	float						life;				// 体力
	int							cntFrame;
	bool						npc;				// NPCの使用状態
	LPDIRECTSOUNDBUFFER8		hitSE;				// 被弾時SE
	LPDIRECTSOUNDBUFFER8		chargeSE;			// チャージ時SE

}PLAYER;

enum PLAYER_TYPE{
	P1,
	P2,
	NPC
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(int index);
void PlayerDamageManager(int pno00, int pno01, int bno);
void CheckNorPlayer(D3DXVECTOR3 nor0, int index);
void SetInitPosPlayer(void);

#endif
