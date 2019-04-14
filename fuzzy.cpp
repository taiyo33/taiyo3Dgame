//=============================================================================
//
// ファジーメンバーシップ処理 [fuzzy.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "fuzzy.h"

//*****************************************************************************
// ファジーメンバーシップ右上がり型
// 引　数：float val(参考にする数値), float x0(範囲の最小値), float x1(範囲の最大値)
// 戻り値：float型　0.0〜1.0の数値
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
// 戻り値：float型　0.0〜1.0の数値
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
// 戻り値：float型　0.0〜1.0の数値
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
// 戻り値：float型　0.0〜1.0の数値
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
