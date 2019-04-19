//=============================================================================
//
// AI処理 [ai.h]
// Author : GP11A_341_22_田中太陽
//
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

//=============================================================================
// マクロ定義
//=============================================================================
#define MOVE_PATTERN_MAX			(4)
#define CMP_PATTERN_MAX				(4)
#define DECISION_MEMORY_MAX			(5)
#define ROUTEDATA_MAX				(9) 
#define ROUTEDATA_ARRAY_MAX			(3)

//=============================================================================
// 構造体宣言
//=============================================================================
typedef struct {

	int		patrolNum;							// 巡回のパターン番号
	float	cmpPattern[CMP_PATTERN_MAX];		// 比較の合計値
	float	decision;							// 比較結果
	float	chase[MOVE_PATTERN_MAX];			// 追跡
	float	escape[MOVE_PATTERN_MAX];			// 逃走
	float	routine[MOVE_PATTERN_MAX];			// 巡回
	int		deciMemory[DECISION_MEMORY_MAX];	//
	int		cntMemory;
	
	// 巡回データ関係変数
	float	lenVec[ROUTEDATA_MAX];				// ベクトルの大きさ
	int		lapingTime[ROUTEDATA_MAX];			// 経路通過の経過時間
	float	routeRot[ROUTEDATA_MAX][ROUTEDATA_ARRAY_MAX];	// 
	int		routeIndex;							// ルートのアドレス番号
	int		routineCntFrame;					// ルート通過後の経過時間
	int		atcCntFrame;						// 攻撃後の経過時間
	bool	routineStart;

}AI;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitAi(void);
void NonePlayerMove(void);
void NonePlayerAttack(void);
void NonePlayerPatrol(int index);
AI *GetAi(int pno);

#endif
