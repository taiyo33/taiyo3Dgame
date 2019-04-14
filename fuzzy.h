//=============================================================================
//
// ファジーメンバーシップ処理 [fuzzy.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _FUZZY_H_
#define _FUZZY_H_

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float FuzzyRightUp(float val, float x0, float x1);
float FuzzyRightDown(float val, float x0, float x1);
float FuzzyTriangle(float val, float x0, float x1, float x2);
float FuzzyTrapezoid(float val, float x0, float x1, float x2, float x3);
float And(float a, float b);
float Or(float a, float b);
float Not(float a, float b);

#endif
