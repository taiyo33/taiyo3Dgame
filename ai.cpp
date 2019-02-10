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
#include "field.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_PATTERN_MAX					(4) 

// NPCの移動マクロ
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

#define MOVE_NPCLIFE_CHASE_FUZZY_X1			(0.0f)
#define MOVE_NPCLIFE_CHASE_FUZZY_X2			(80.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X2		(80.0f)

#define MOVE_PLAYERLIFE_CHASE_FUZZY_X1		(0.0f)
#define MOVE_PLAYERLIFE_CHASE_FUZZY_X2		(100.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1		(0.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2		(100.0f)

// NPCの攻撃マクロ
#define ATTACK_DISTANCE_ATC_FUZZY_X1		(150.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X2		(250.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X3		(400.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X4		(600.0f)
#define ATTACK_PLAYERLIFE_WAIT_FUZZY_X1		(0.0f)
#define ATTACK_PLAYERLIFE_WAIT_FUZZY_X2		(100.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int CheckNorNonePlayer(D3DXVECTOR3 nor0);
void NonePlayerDest(D3DXVECTOR3 vecDest);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
enum {
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4
};

D3DXVECTOR3			MovePattern[MOVE_PATTERN_MAX]{

	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 0.0f, -FIELD_SIZE_Z + 100.0f),
	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 0.0f, FIELD_SIZE_Z - 100.0f), 
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 0.0f, FIELD_SIZE_Z - 100.0f),
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 0.0f, FIELD_SIZE_Z + 100.0f),

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

	NonePlayerDest(player[P1].pos);

	// 相手との距離による判定
	D3DXVECTOR3 pvpVec = player[P2].pos - player[P1].pos;
	float vecLength = D3DXVec3Length(&pvpVec);
	ai->chase[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_CHASE_FUZZY_X1, MOVE_DISTANCE_CHASE_FUZZY_X2,
													MOVE_DISTANCE_CHASE_FUZZY_X3, MOVE_DISTANCE_CHASE_FUZZY_X4);
	ai->escape[PATTERN1] = FuzzyRightDown(vecLength, MOVE_DISTANCE_ESCAPE_FUZZY_X1, MOVE_DISTANCE_ESCAPE_FUZZY_X2);
	ai->wait[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2, 
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

	// 比較
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
		D3DXVec3Normalize(&player[P2].frontVec, &player[P2].frontVec);

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

//==============================================================================
// NPCがプレイヤーの方向を向く
// 引　数：な　し
// 戻り値：な　し
//==============================================================================
void NonePlayerDest(D3DXVECTOR3 vecDest)
{
	PLAYER *player = GetPlayer(0);

	D3DXVECTOR3 vec = vecDest - player[P2].pos;
	D3DXVec3Normalize(&vec, &vec);
	D3DXVec3Normalize(&player[P2].frontVec, &player[P2].frontVec);

	float vecAngle = atan2f(vec.z, vec.x);
	float frontAngle = atan2f(player[P2].frontVec.z, player[P2].frontVec.x);

	if (frontAngle != vecAngle)
	{
		player[P2].rotDest.y += frontAngle - vecAngle;
	}

}
//==============================================================================
// NPCのフィールド巡回
// 引　数：な　し
// 戻り値：な　し
//==============================================================================
void NonePlayerPatrol(void)
{
	PLAYER *player = GetPlayer(P2);
	BLOCK *block = GetBlock(0);
	D3DXVECTOR3 rayPos = player->frontVec + player->pos;	// 前方ベクトルの終点
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int pattern = PATTERN1;


	// 前方ベクトルのとブロックの判定
	if (!HitCheckBlock(rayPos, player->pos))
	{
		pattern = CheckNorNonePlayer(GetNormal());
	}

	NonePlayerDest(MovePattern[pattern]);

	vec = MovePattern[pattern] - player->pos;
	
	D3DXVec3Normalize(&vec, &vec);

	player->move.x = vec.x * 5.0f;
	player->move.z = vec.z * 5.0f;
}

//==========================================================================
// NPCの巡回行動切替処理
// 引　数：D3DXVECTOR3 nor0(ポリゴンの法線)
// 戻り値：な　し
//==========================================================================
int CheckNorNonePlayer(D3DXVECTOR3 nor0)
{
	int out = 0;

	// 法線がX軸方向なら
	if (nor0.x != 0.0f)
	{
		nor0.x > 1.0f ? out = PATTERN2 : out = PATTERN4;
		return out;
	}
	// 法線がZ軸方向なら
	if (nor0.z != 0.0f)
	{
		nor0.z > 1.0f ? out = PATTERN1 : out = PATTERN3;
		return out;
	}

	return out;
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
	
	// バレット使用中チャージ不可
	if (!bullet->use[P2])
	{
		// 最大値になった場合
		if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
		{
			bullet->speedIncrease = BULLET_CHARGE_MAX;
		}
		// 10フレーム
		else if (player->cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
		{
			bullet->speedIncrease += 0.2f;
		}
	}

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

	//// 相手との距離による判定
	//D3DXVECTOR3 pvpVec = player[P2].pos - player[P1].pos;
	//float vecLength = D3DXVec3Length(&pvpVec);
	//ai->atc[PATTERN1] = FuzzyTrapezoid(vecLength, ATTACK_DISTANCE_ATC_FUZZY_X1, ATTACK_DISTANCE_ATC_FUZZY_X2,
	//											 ATTACK_DISTANCE_ATC_FUZZY_X3, ATTACK_DISTANCE_ATC_FUZZY_X4);
	//ai->wait[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2,
	//											  MOVE_DISTANCE_WAIT_FUZZY_X3, MOVE_DISTANCE_WAIT_FUZZY_X4);

	////チャージ率による判定
	//ai->atc[PATTERN2] = FuzzyRightUp(bullet->speedIncrease, ATTACK_DISTANCE_ATC_FUZZY_X1, ATTACK_DISTANCE_ATC_FUZZY_X2);
	//ai->wait[PATTERN2] = FuzzyTrapezoid(bullet->speedIncrease, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2);

	//ai->patternA = ai->atc[PATTERN1] * ai->atc[PATTERN2];
	//ai->patternB = ai->wait[PATTERN1] * ai->wait[PATTERN2];

	//
	//// 相手ライフによる判定
	//if (player[P1].life < PLAYER_LIFE_MAX)
	//{
	//	ai->atc[PATTERN3] = FuzzyRightUp(player[P1].life, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2);

	//	ai->patternA = ai->atc[PATTERN1] * ai->atc[PATTERN2];
	//	ai->patternB = ai->wait[PATTERN1];
	//}

	//// 攻撃か待機か比較
	//ai->decision = Or(ai->patternA, ai->patternB);
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
