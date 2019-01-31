//=============================================================================
//
// AI処理 [ai.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"


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
