//=============================================================================
//
// AI処理 [ai.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "ai.h"
#include "bullet.h"
#include "player.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_CMP_MAX			(4)

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

//===========================================================================
// NPCの移動処理
// 引　数：D3DXVECTOR3 pos(次の移動位置)、D3DXVECTOR3 normal(ポリゴンの法線)
//		   int index(プレイヤーのアドレス番号)
// 戻り値：
//==========================================================================
void NonePlayerMove(void)
{
	BULLET *bullet = GetBullet(P2);
	PLAYER *player = GetPlayer(P2);
	float box, out;
	float atc[MOVE_CMP_MAX], chase[MOVE_CMP_MAX], escape[MOVE_CMP_MAX];
	float pattern[MOVE_CMP_MAX];

	// ライフによる比較
	atc[PATTERN1] = FuzzyRightDown(player->life, 40, 80);
	chase[PATTERN1] = FuzzyTrapezoid(player->life, 0, 20, 60, 80);
	escape[PATTERN1] = FuzzyRightUp(player->life, 50, 80);

	// 距離による比較
	float distance = 0.0f;
	float a = player[P1].pos.x - player[P2].pos.x;
	float b = player[P1].pos.z - player[P2].pos.z;

	distance = a + b;

	atc[PATTERN2] = FuzzyRightDown(distance, 100, 500);
	chase[PATTERN2] = FuzzyTrapezoid(distance, 0, 200, 400, 500);
	escape[PATTERN2] = FuzzyRightUp(distance, 0, 100);

	pattern[PATTERN1] = atc[PATTERN1] + atc[PATTERN2];
	pattern[PATTERN2] = chase[PATTERN1] + chase[PATTERN2];
	pattern[PATTERN3] = escape[PATTERN1] + escape[PATTERN2];
	
	box = And(pattern[PATTERN1], pattern[PATTERN2]);
	out = And(box, pattern[PATTERN3]);

	if (out == pattern[PATTERN1])
	{
		bullet->sclIncrease = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
		SetBullet(player->pos, player->rot, bullet->sclIncrease, 0, P2);
		bullet->sclIncrease = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (out == pattern[PATTERN2])
	{
		D3DXVECTOR3 vec = player[P1].pos - player[P2].pos;
		D3DXVec3Normalize(&vec, &vec);
		player->move.x += vec.x * player->speed;
		player->move.z += vec.z * player->speed;
	}

	if (out == pattern[PATTERN3])
	{

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
// 引　数：float val(参考にする数値), float x0(範囲の最小値), float x1(範囲の最大値)
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
//  ファジーAND演算子
//
//
//*****************************************************************************
float And(float a, float b)
{
	return min(a, b);
}

//*****************************************************************************
// ファジーOR演算子
//
//
//*****************************************************************************

float Or(float a, float b)
{
	return max(a, b);
}

//*****************************************************************************
// ファジーNot演算子
//
//
//*****************************************************************************
float Not(float a, float b)
{
	return 1.0f - a;
}
