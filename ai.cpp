//=============================================================================
//
// AI処理 [ai.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "ai.h"
#include "bullet.h"
#include "player.h"
#include "block.h"
#include "checkhit.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// NPCの移動マクロ
#define MOVE_NPCLIFE_CHASE_FUZZY_X1			(0.0f)
#define MOVE_NPCLIFE_CHASE_FUZZY_X2			(80.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X2		(80.0f)

#define MOVE_DISTANCE_CHASE_FUZZY_X1		(150.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X2		(250.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X3		(400.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X4		(600.0f)
#define MOVE_DISTANCE_ESCAPE_FUZZY_X1		(150.0f)
#define MOVE_DISTANCE_ESCAPE_FUZZY_X2		(500.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X1			(100.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X2			(200.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X3			(250.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X4			(300.0f)

#define MOVE_PLAYERLIFE_CHASE_FUZZY_X1		(0.0f)
#define MOVE_PLAYERLIFE_CHASE_FUZZY_X2		(100.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1		(0.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2		(100.0f)

#define ATTACK_DISTANCE_ATC_FUZZY_X1		(150.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X2		(250.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X3		(400.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X4		(600.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
enum {
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4
};

AI			aiWk;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAi(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	AI *ai = &aiWk;

	ai->patternA = 0.0f;				// 比較の合計値
	ai->patternB = 0.0f;				//
	ai->patternC = 0.0f;				// 
	ai->decision = 0.0f;				// 比較結果

	for (int i = 0; i < MOVE_CMP_MAX; i++)
	{
		ai->atc[i] = 0.0f;		// 攻撃
		ai->chase[i] = 0.0f;	// 追跡
		ai->escape[i] = 0.0f;	// 逃走
		ai->wait[i] = 0.0f;		// 待機
	}

	return S_OK;
}


//===========================================================================
// NPCの移動処理
// 引　数：な　し
// 戻り値：な　し
//==========================================================================
void NonePlayerMove(void)
{
	AI *ai = &aiWk;
	PLAYER *player = GetPlayer(0);

	// 相手との距離による判定
	D3DXVECTOR3 pvpVec = player[P2].pos - player[P1].pos;
	float totalVec = D3DXVec3Length(&pvpVec);
	ai->chase[PATTERN1] = FuzzyTrapezoid(totalVec, MOVE_DISTANCE_CHASE_FUZZY_X1, MOVE_DISTANCE_CHASE_FUZZY_X2,
												 MOVE_DISTANCE_CHASE_FUZZY_X3, MOVE_DISTANCE_CHASE_FUZZY_X4);
	ai->escape[PATTERN1] = FuzzyRightDown(totalVec, MOVE_DISTANCE_ESCAPE_FUZZY_X1, MOVE_DISTANCE_ESCAPE_FUZZY_X2);
	ai->wait[PATTERN1] = FuzzyTrapezoid(totalVec, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2, 
												  MOVE_DISTANCE_WAIT_FUZZY_X3, MOVE_DISTANCE_WAIT_FUZZY_X4);
	
	// 自ライフによる判定
	if (player[P2].life > 0.0f)
	{
		ai->chase[PATTERN2] = FuzzyRightDown(player[P2].life, MOVE_NPCLIFE_CHASE_FUZZY_X1, MOVE_NPCLIFE_CHASE_FUZZY_X2);
		ai->escape[PATTERN2] = FuzzyRightUp(player[P2].life, MOVE_NPCLIFE_ESCAPE_FUZZY_X1, MOVE_NPCLIFE_ESCAPE_FUZZY_X2);
		
		ai->patternA = ai->chase[PATTERN1] * ai->chase[PATTERN2];
		ai->patternB = ai->escape[PATTERN1] * ai->escape[PATTERN2];
		ai->patternC = ai->wait[PATTERN1];
	}
	// 相手ライフが最高値の時は考慮しない
	else if (player[P2].life == 0.0f)
	{
		ai->patternA = ai->chase[PATTERN1];
		ai->patternB = ai->escape[PATTERN1];
		ai->patternC = ai->wait[PATTERN1];
	}

	// 相手ライフによる判定
	if (player[P1].life < PLAYER_LIFE_MAX)
	{
		ai->chase[PATTERN3] = FuzzyRightDown(player[P1].life, MOVE_PLAYERLIFE_CHASE_FUZZY_X1, MOVE_PLAYERLIFE_CHASE_FUZZY_X2);
		ai->escape[PATTERN3] = FuzzyRightUp(player[P1].life, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2);

		ai->patternA = ai->chase[PATTERN1] * ai->chase[PATTERN2] * ai->chase[PATTERN3];
		ai->patternB = ai->escape[PATTERN1] * ai->escape[PATTERN2] * ai->escape[PATTERN3];
		ai->patternC = ai->wait[PATTERN1];
	}

	// 追跡か逃走か比較
	ai->decision = Or(ai->patternA, ai->patternB);
	ai->decision = Or(ai->patternC, ai->decision);

	// 結果反映
	// 待機
	if (ai->decision == ai->patternC)
	{
		return;
	}
	// 追跡
	else if (ai->decision == ai->patternA)
	{
		D3DXVECTOR3 vec = player[P1].pos - player[P2].pos;
		D3DXVec3Normalize(&vec, &vec);
		player[P2].speed = 5.0f * ai->patternA;
		player[P2].move.x = vec.x * player[P2].speed;
		player[P2].move.z = vec.z * player[P2].speed;
	}
	// 逃走
	else if (ai->decision == ai->patternB)
	{
		D3DXVECTOR3 vec = player[P2].pos - player[P1].pos;
		D3DXVec3Normalize(&vec, &vec);
		player[P2].speed = 5.0f * ai->patternB;
		player[P2].move.x = vec.x * player[P2].speed;
		player[P2].move.z = vec.z * player[P2].speed;
	}
}

//===========================================================================
// NPCの攻撃処理
// 引　数：な　し
// 戻り値：な　し
//==========================================================================
void NonePlayerAttack(void)
{
	AI *ai = &aiWk;
	PLAYER *player = GetPlayer(0);
	BULLET *bullet = GetBullet(P2);
	BLOCK *block = GetBlock(0);
	
	// 相手との距離による判定
	D3DXVECTOR3 pvpVec = player[P2].pos - player[P1].pos;
	float totalVec = D3DXVec3Length(&pvpVec);
	ai->atc[PATTERN1] = FuzzyTrapezoid(totalVec, ATTACK_DISTANCE_ATC_FUZZY_X1, ATTACK_DISTANCE_ATC_FUZZY_X2,
												 ATTACK_DISTANCE_ATC_FUZZY_X3, ATTACK_DISTANCE_ATC_FUZZY_X4);
	ai->wait[PATTERN1] = FuzzyTrapezoid(totalVec, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2,
												  MOVE_DISTANCE_WAIT_FUZZY_X3, MOVE_DISTANCE_WAIT_FUZZY_X4);
	ai->patternA = ai->atc[PATTERN1];
	ai->patternB = ai->wait[PATTERN1];

	// 相手ライフによる判定
	if (player[P1].life < PLAYER_LIFE_MAX)
	{
		ai->atc[PATTERN3] = FuzzyRightUp(player[P1].life, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2);

		ai->patternA = ai->atc[PATTERN1] * ai->atc[PATTERN2];
		ai->patternB = ai->wait[PATTERN1];
	}

	// 追跡か逃走か比較
	ai->decision = Or(ai->patternA, ai->patternB);

	if (ai->decision == ai->patternA)
	{
		D3DXVECTOR3 rayPos = player[P2].frontVec + player[P2].pos;	// 前方ベクトルの終点
		D3DXVECTOR3 reflectVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 反射後のベクトル

		// 前方ベクトルの反射処理
		for (int i = 0; i < BLOCK_MAX; i++, block++)
		{
			if (!HitCheckBlock(rayPos, player[P2].pos))
			{
				reflectVec = ReflectVector(rayPos, player[P2].pos, GetNormal());
				break;
			}
		}

		// 最大値になった場合
		if (bullet->speedIncrease > BULLET_CHARGE_MAX)
		{
			bullet->speedIncrease = BULLET_CHARGE_MAX;
		}
		// 10フレーム
		else if (player->cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
		{
			bullet->speedIncrease += 0.2f;
		}

		// 攻撃開始判定
		if (CheckHitRay(player[P1].pos, player[P2].pos, player[P2].frontVec, 15.0f))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
		}
		// 反射を考慮した攻撃
		else if (CheckHitRay(player[P1].pos, player[P2].pos, reflectVec, 15.0f))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
		}
	}
}

//*****************************************************************************
// ファジーメンバーシップ右上がり型
// 引　数：float val(参考にする数値), float x0(範囲の最小値), float x1(範囲の最大値)
// 戻り値：float型　0.0～1.0の数値
//*****************************************************************************
float FuzzyRightUp(float val, float x0, float x1)
{
	if (val <= x0)
	{
		return 0.0f;
	}
	else if (val >= x1)
	{
		return 1.0f;
	}
	else
	{
		// 分母を計算
		float denom = x1 - x0;
		return (val / denom) - (x0 / denom);
	}
}

//*****************************************************************************
// ファジーメンバーシップ右下がり型
// 引　数：float val(参考にする数値), float x0(範囲の最小値), float x1(範囲の最大値)
// 戻り値：float型　0.0～1.0の数値
//*****************************************************************************
float FuzzyRightDown(float val, float x0, float x1)
{
	if (val <= 0)
	{
		return 1;
	}
	else if (val >= x1)
	{
		return 0;
	}
	else
	{
		float denom = x1 - x0;
		return (x1 / denom) - (val / denom);
	}
}

//*****************************************************************************
// ファジーメンバーシップ三角型
// 引　数：float val(参考にする数値), float x0(範囲の最小値), float x1(返り値の最大値)
//		   float x2()
// 戻り値：float型　0.0～1.0の数値
//*****************************************************************************
float FuzzyTriangle(float val, float x0, float x1, float x2)
{
	if (val <= x0)
	{
		return 0;
	}
	else if (val == x1)
	{
		return 1;
	}
	else if ((val > x0) && (val < x1))
	{
		float denom = x1 - x0;
		return (val / denom) - (x0 / denom);
	}
	else
	{
		float denom = x2 - x1;
		return (x2 / denom) - (val / denom);
	}
}

//*****************************************************************************
// ファジーメンバーシップ台形型
// 引　数：float val(参考にする数値), float x0(範囲の最小値), float x1(範囲の最大値)
//		   float x2(), float x3()
// 戻り値：float型　0.0～1.0の数値
//*****************************************************************************
float FuzzyTrapezoid(float val, float x0, float x1, float x2, float x3)
{
	if (val <= x0)
	{
		return 0;
	}
	else if ((val >= x1) && (val <= x2))
	{
		return 1;
	}
	else if ((val > x0) && (val < x1))
	{
		float denom = x1 - x0;
		return (val / denom) - (x0 / denom);
	}
	else
	{
		float denom = x3 - x2;
		return (x3 / denom) - (val / denom);
	}
}

//*****************************************************************************
// ファジーAND演算子
// 引　数：float a(比較値1), float b(比較値2)
// 戻り値：float型　小さい方の値を返す
//*****************************************************************************
float And(float a, float b)
{
	return min(a, b);
}

//*****************************************************************************
// ファジーOR演算子
// 引　数：float a(比較値1), float b(比較値2)
// 戻り値：float型　大きい方の値を返す
//*****************************************************************************

float Or(float a, float b)
{
	return max(a, b);
}

//*****************************************************************************
// ファジーNot演算子
// 引　数：float a(比較値1), float b(比較値2)
// 戻り値：float型　反転した値を返す
//*****************************************************************************
float Not(float a, float b)
{
	return 1.0f - a;
}
