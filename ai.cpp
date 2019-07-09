//=============================================================================
//
// AI処理 [ai.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "main.h"
#include "ai.h"
#include "fuzzy.h"
#include "bullet.h"
#include "player.h"
#include "block.h"
#include "checkhit.h"
#include "field.h"
#include "chargeEffect.h"
#include "ball.h"
#include "field.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

// 距離についてのマクロ
#define MOVE_DISTANCE_CHASE_FUZZY_X1		(50.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X2		(200.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X3		(250.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X4		(700.0f)
#define MOVE_DISTANCE_ESCAPE_FUZZY_X1		(100.0f)
#define MOVE_DISTANCE_ESCAPE_FUZZY_X2		(700.0f)
#define MOVE_DISTANCE_PATROL_FUZZY_X1		(100.0f)
#define MOVE_DISTANCE_PATROL_FUZZY_X2		(200.0f)
#define MOVE_DISTANCE_PATROL_FUZZY_X3		(350.0f)
#define MOVE_DISTANCE_PATROL_FUZZY_X4		(700.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X1			(0.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X2			(100.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X3			(150.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X4			(500.0f)

// NPCのライフについてのマクロ
#define MOVE_NPCLIFE_CHASE_FUZZY_X1			(20.0f)
#define MOVE_NPCLIFE_CHASE_FUZZY_X2			(80.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X2		(80.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X1		(10.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X2		(30.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X3		(70.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X4		(90.0f)

// プレイヤーのライフについてのマクロ
#define MOVE_PLAYERLIFE_CHASE_FUZZY_X1		(10.0f)
#define MOVE_PLAYERLIFE_CHASE_FUZZY_X2		(70.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2		(50.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X1	(10.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X2	(30.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X3	(80.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X4	(100.0f)

// NPCのボール所持数についてのマクロ
#define MOVE_NPCBALL_ROUTINE_FUZZY_X1		(30.0f)
#define MOVE_NPCBALL_ROUTINE_FUZZY_X2		(50.0f)
#define MOVE_NPCBALL_ROUTINE_FUZZY_X3		(70.0f)
#define MOVE_NPCBALL_ROUTINE_FUZZY_X4		(100.0f)
#define MOVE_NPCBALL_CHASE_FUZZY_X1			(30.0f)
#define MOVE_NPCBALL_CHASE_FUZZY_X2			(75.0f)
#define MOVE_NPCBALL_ESCAPE_FUZZY_X1		(50.0f)
#define MOVE_NPCBALL_ESCAPE_FUZZY_X2		(80.0f)

// プレイヤーのボール所持数についてのマクロ
#define MOVE_PLAYERBALL_ROUTINE_FUZZY_X1	(0.0f)
#define MOVE_PLAYERBALL_ROUTINE_FUZZY_X2	(10.0f)
#define MOVE_PLAYERBALL_ROUTINE_FUZZY_X3	(70.0f)
#define MOVE_PLAYERBALL_ROUTINE_FUZZY_X4	(100.0f)
#define MOVE_PLAYERBALL_CHASE_FUZZY_X1		(20.0f)
#define MOVE_PLAYERBALL_CHASE_FUZZY_X2		(75.0f)
#define MOVE_PLAYERBALL_ESCAPE_FUZZY_X1		(0.0f)
#define MOVE_PLAYERBALL_ESCAPE_FUZZY_X2		(100.0f)

// ルート巡回マクロ
#define STOP_ROUTINE_CNT					(2)
#define ROUTE_DISTANCE_FUZZY_X1				(0.0f)
#define ROUTE_DISTANCE_FUZZY_X2				(500.0f)
#define ROUTE_TIME_FUZZY_X1					(0.0f)
#define ROUTE_TIME_FUZZY_X2					(30.0f)
#define ROUTE_CNT_TIME						(70)

#define RELOAD_ATTCK_FRAME					(60.0f)		// 攻撃の可能までの間隔

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void NonePlayerDest(D3DXVECTOR3 vecDest, int index);
void NonePlayerFuzzy(void);
void DistanceRoutePlayer01(D3DXVECTOR3 vec, int cnt, int cntMax, int raiseCnt);
void DistanceRoutePlayer02(D3DXVECTOR3 vec, int cnt, int cntMax, int raiseCnt);
void SwitchRoutePlayer(int pattern, D3DXVECTOR3 vec);
int SwitchPatrolPattern(int pattern);
void DistanceNearVector(int index, D3DXVECTOR3 pos, int cnt, int cntMax);
void MoveVectorNonePlayer(D3DXVECTOR3 vec, float fuzzy);
void RouteLapTimeCnt(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************

// ファジーの要素数
enum FUZZY_PATTERN{
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4
};

// 比較合計の種類
enum MOVE_PATTERN{
	CHASE,
	ESCAPE,
	ROUTINE,
};

// 巡回のルート番号
enum ROUTENUM{
	ROUTE00,
	ROUTE01,
	ROUTE02,
	ROUTE03,
	ROUTE04,
	ROUTE05,
	ROUTE06,
	ROUTE07,
	ROUTE08,
};

// 巡回参照値の種類
enum ROUTE_PATTERN{
	ROUTE_DECISION,
	ROUTE_LENGTH,
	ROUTE_TIME
};

// 巡回のパターン
D3DXVECTOR3		RouteData[ROUTEDATA_MAX]{

	D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// 中央
	D3DXVECTOR3(-400.0f + 100.0f, 10.0f, 300.0f - 50.0f),	// 左上
	D3DXVECTOR3(400.0f - 100.0f, 10.0f, 300.0f - 50.0f),	// 右上
	D3DXVECTOR3(-400.0f + 100.0f, 10.0f, -300.0f + 50.0f),	// 左下
	D3DXVECTOR3(400.0f - 100.0f, 10.0f, -300.0f + 50.0f),	// 右下
	D3DXVECTOR3(-400.0f + 100.0f, 10.0f, 0.0f),				// 左中央
	D3DXVECTOR3(400.0f - 100.0f, 10.0f, 0.0f),				// 右中央
	D3DXVECTOR3(0.0f, 10.0f, 300.0f - 80.0f),				// 上中央
	D3DXVECTOR3(0.0f, 10.0f, -300.0f + 80.0f),				// 下中央
};

AI			AiWk[PLAYER_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAi(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	AI *ai = &AiWk[0];

	ai->cmpPattern[CHASE] = 0.0f;	// 比較の合計値を初期化
	ai->cmpPattern[ESCAPE] = 0.0f;	//
	ai->cmpPattern[ROUTINE] = 0.0f;	// 
	ai->decision = 0.0f;			// 比較結果を初期化
	ai[P1].patrolNum = ROUTE01;		// 巡回パターンの番号を初期化
	ai[P2].patrolNum = ROUTE04;		//

	ai->routineCntFrame = 0;		// 思考間隔
	ai->atcCntFrame = 0;			// 攻撃の間隔
	ai->routineStart = true;		// 巡回しているか

	for (int i = 0; i < CMP_PATTERN_MAX; i++)
	{
		ai->chase[i] = 0.0f;	// 追跡の値を初期化
		ai->escape[i] = 0.0f;	// 逃走の値を初期化
		ai->routine[i] = 0.0f;	// 待機の値を初期化
	}

	for (int k = 0; k < ROUTEDATA_MAX; k++)
	{
		ai->routeRot[k][ROUTE_DECISION] = 0.0f;	// 結果
		ai->routeRot[k][ROUTE_LENGTH] = 0.0f;	// ベクトルのファジー値
		ai->routeRot[k][ROUTE_TIME] = 0.0f;		// 経過時間のファジー値
		ai->lenVec[k] = 0.0f;					// ベクトルの大きさ
		ai->lapingTime[k] = 0;					// 経過時間の初期化
	}

	ai->routeIndex = ROUTE08;	// ルートアドレス番号の初期化

	return S_OK;
}

//===========================================================================
// NPCの移動処理
// 引　数：な　し
// 戻り値：な　し
//==========================================================================
void NonePlayerMove(void)
{
	AI *ai = &AiWk[0];
	PLAYER *player = GetPlayer(0);

	// 思考時間と経路経過時間を加算
	ai->routineCntFrame++;
	RouteLapTimeCnt();

	// 60フレームで再考
	if (ai->routineCntFrame % ROUTE_CNT_TIME == 0)
	{
		// 移動処理の結果算出
		NonePlayerFuzzy();
		
		// 追跡
		if (ai->decision == ai->cmpPattern[CHASE])
		{
			ai->routineStart = false;
			// ルートデータとプレイヤーの距離を計算
			DistanceNearVector(P1, player[P1].pos, 0, ROUTEDATA_MAX);
			DistanceNearVector(P2, player[P2].pos, 0, ROUTEDATA_MAX);
			// P2に隣接してるルートデータでP1に近いルートデータを計算
			SwitchRoutePlayer(ai[P2].routeIndex, RouteData[ai[P1].routeIndex]);
			ai->routineCntFrame = 0;

#ifdef _DEBUG
			
			SetCollarField(ai->routeIndex, 1.0f, 0.0f, 0.0f);	// フィールド色を赤へ

#endif
		}
		// 逃走
		else if (ai->decision == ai->cmpPattern[ESCAPE])
		{
			// ルートデータとプレイヤーの距離を計算
			DistanceNearVector(P1, player[P1].pos, 0, ROUTEDATA_MAX);
			DistanceNearVector(P2, player[P2].pos, 0, ROUTEDATA_MAX);
			// P2に隣接してるルートデータでP1に遠いルートデータを計算
			SwitchRoutePlayer(ai[P2].routeIndex, RouteData[ai[P1].routeIndex]);
			ai->routineCntFrame = 0;
#ifdef _DEBUG

			SetCollarField(ai->routeIndex, 0.0f, 0.0f, 1.0f);	// フィールド色を青へ

#endif

		}
		// 巡回
		else if (ai->decision == ai->cmpPattern[ROUTINE])
		{
			ai->routineStart = true;	// 巡回の開始
			SwitchRoutePlayer(ai->routeIndex, player[P2].pos);	// ルートの切替
			ai->routineCntFrame = 0;	// ルート再考時間
#ifdef _DEBUG

			SetCollarField(ai->routeIndex, 0.0f, 1.0f, 0.0f);	// フィールド色を緑へ

#endif

		}
	}

	// 移動処理
	MoveVectorNonePlayer(RouteData[ai->routeIndex], 1.0f);

	PrintDebugProc("ルート[%d]", ai->routeIndex);

}

//==============================================================================
// 各ルートの経過時間
// 引　数：な　し
// 戻り値：な　し
//==============================================================================
void RouteLapTimeCnt(void)
{
	AI *ai = &AiWk[0];

	if (ai->routineCntFrame % 60 == 0)
	{
		for (int i = 0; i < ROUTEDATA_MAX; i++)
		{
			ai->lapingTime[i]++;
		}
	}
}

//===========================================================================
// ファジー値による行動結果の算出処理
// 引　数：な　し
// 戻り値：な　し
//==========================================================================
void NonePlayerFuzzy(void)
{
	AI *ai = &AiWk[0];
	PLAYER *player = GetPlayer(0);
	BALL *ball = GetBall(0);

	// 自ボールよる判定
	ai->chase[PATTERN1] = FuzzyRightDown((float)(float)ball[P2].cnt, MOVE_NPCBALL_CHASE_FUZZY_X1, MOVE_NPCBALL_CHASE_FUZZY_X2);
	ai->escape[PATTERN1] = FuzzyRightUp((float)ball[P2].cnt, MOVE_NPCBALL_ESCAPE_FUZZY_X1, MOVE_NPCBALL_ESCAPE_FUZZY_X2);
	ai->routine[PATTERN1] = FuzzyTrapezoid((float)ball[P2].cnt, MOVE_NPCBALL_ROUTINE_FUZZY_X1, MOVE_NPCBALL_ROUTINE_FUZZY_X2,
																 MOVE_NPCBALL_ROUTINE_FUZZY_X3, MOVE_NPCBALL_ROUTINE_FUZZY_X4);
	
	// 相手ボールよる判定
	ai->chase[PATTERN2] = FuzzyRightDown((float)ball[P1].cnt, MOVE_PLAYERBALL_CHASE_FUZZY_X1, MOVE_PLAYERBALL_CHASE_FUZZY_X2);
	ai->escape[PATTERN2] = FuzzyRightUp((float)ball[P1].cnt, MOVE_PLAYERBALL_ESCAPE_FUZZY_X1, MOVE_PLAYERBALL_ESCAPE_FUZZY_X2);
	ai->routine[PATTERN2] = FuzzyTrapezoid((float)ball[P1].cnt, MOVE_PLAYERBALL_ROUTINE_FUZZY_X1, MOVE_PLAYERBALL_ROUTINE_FUZZY_X2,
																MOVE_PLAYERBALL_ROUTINE_FUZZY_X3, MOVE_PLAYERBALL_ROUTINE_FUZZY_X4);
	
	// 自ライフによる判定
	if (player[P2].life < PLAYER_LIFE_MAX)
	{
		ai->chase[PATTERN3] = FuzzyRightDown(player[P2].life, MOVE_NPCLIFE_CHASE_FUZZY_X1, MOVE_NPCLIFE_CHASE_FUZZY_X2);
		ai->escape[PATTERN3] = FuzzyRightUp(player[P2].life, MOVE_NPCLIFE_ESCAPE_FUZZY_X1, MOVE_NPCLIFE_ESCAPE_FUZZY_X2);
		ai->routine[PATTERN3] = FuzzyTrapezoid(player[P2].life, MOVE_NPCLIFE_ROUTINE_FUZZY_X1, MOVE_NPCLIFE_ROUTINE_FUZZY_X2,
			MOVE_NPCLIFE_ROUTINE_FUZZY_X3, MOVE_NPCLIFE_ROUTINE_FUZZY_X4);
		// 結果反映
		ai->cmpPattern[CHASE] = ai->chase[PATTERN1] * ai->chase[PATTERN2] * ai->chase[PATTERN3];
		ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1] * ai->escape[PATTERN2] * ai->escape[PATTERN3];
		ai->cmpPattern[ROUTINE] = ai->routine[PATTERN1] * ai->routine[PATTERN2] * ai->routine[PATTERN3];

	}
	// 相手ライフが最高値の時は考慮しない
	else if (player[P2].life == PLAYER_LIFE_MAX)
	{
		// 結果反映
		ai->cmpPattern[CHASE] = ai->chase[PATTERN1] * ai->chase[PATTERN2];
		ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1] * ai->escape[PATTERN2];
		ai->cmpPattern[ROUTINE] = ai->routine[PATTERN1] * ai->routine[PATTERN2];
	}

	// 相手ライフによる判定
	if (player[P1].life < PLAYER_LIFE_MAX)
	{
		ai->chase[PATTERN4] = FuzzyRightDown(player[P1].life, MOVE_PLAYERLIFE_CHASE_FUZZY_X1, MOVE_PLAYERLIFE_CHASE_FUZZY_X2);
		ai->escape[PATTERN4] = FuzzyRightUp(player[P1].life, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2);
		ai->routine[PATTERN4] = FuzzyTrapezoid(player[P1].life, MOVE_PLAYERLIFE_ROUTINE_FUZZY_X1, MOVE_PLAYERLIFE_ROUTINE_FUZZY_X2,
			MOVE_PLAYERLIFE_ROUTINE_FUZZY_X3, MOVE_PLAYERLIFE_ROUTINE_FUZZY_X4);

		// 結果反映
		ai->cmpPattern[CHASE] *= ai->chase[PATTERN4];
		ai->cmpPattern[ESCAPE] *= ai->escape[PATTERN4];
		ai->cmpPattern[ROUTINE] *= ai->routine[PATTERN4];
	}

	// 各行動のファジー値を比較
	ai->decision = Or(ai->cmpPattern[CHASE], ai->cmpPattern[ESCAPE]);
	ai->decision = Or(ai->cmpPattern[ROUTINE], ai->decision);
}

//===========================================================================
// プレイヤーと最も近い経路データの計算処理
// 引　数：int index(プレイヤーのアドレス番号), D3DXVECTOR3 pos(プレイヤーの位置)
//		   int cnt(ループカウンター), int cntMax(カウンターの最大値)
// 戻り値：な　し
//==========================================================================
void DistanceNearVector(int index, D3DXVECTOR3 pos, int cnt, int cntMax)
{
	AI *ai = &AiWk[index];
	D3DXVECTOR3 vec01 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ルートまでのベクトル
	float nearVec = 1000.0f;							// 最も近いルート

	for (cnt; cnt < cntMax; cnt++)
	{
		// ベクトルの大きさを計算
		vec01 = pos - RouteData[cnt];
		ai->lenVec[cnt] = D3DXVec3Length(&vec01);
		
		// ベクトルの大きさを比較
		nearVec = min(ai->lenVec[cnt], nearVec);

		// 比較結果を保存
		if (nearVec == ai->lenVec[cnt])
		{
			ai->routeIndex = cnt;
		}
	}
}

//==========================================================================
// NPCの巡回行動遷移処理
// 引　数：int pattern(巡回パターン番号), D3DXVECTOR3 vec(進行方向ベクトル)
// 戻り値：な　し
//==========================================================================
void SwitchRoutePlayer(int pattern, D3DXVECTOR3 vec)
{
	AI *ai = &AiWk[0];
	switch (pattern)
	{
		case ROUTE00:
			// 5,6,7,8のルートデータを比較
			DistanceRoutePlayer01(vec, ROUTE05, ROUTEDATA_MAX, NULL);
			break;

		case ROUTE01:
			// 5,7のルートデータを比較
			DistanceRoutePlayer01(vec, ROUTE05, ROUTE08, 1);
			break;

		case ROUTE02:
			// 6,7のルートデータを比較
			DistanceRoutePlayer01(vec, ROUTE06, ROUTE08, NULL);
			break;

		case ROUTE03:
			// 5,8のルートデータを比較
			DistanceRoutePlayer01(vec, ROUTE05, ROUTEDATA_MAX, 2);
			break;

		case ROUTE04:
			// 6,8のルートデータを比較
			DistanceRoutePlayer01(vec, ROUTE06, ROUTEDATA_MAX, 1);
			break;

		case ROUTE05:
			// 0,1,3のルートデータを比較
			DistanceRoutePlayer02(vec, ROUTE01, ROUTE04, ROUTE03);
			break;

		case ROUTE06:
			// 0,2,4のルートデータを比較
			DistanceRoutePlayer02(vec, ROUTE02, ROUTE05, ROUTE04);
			break;

		case ROUTE07:
			// 0,1,2のルートデータを比較
			DistanceRoutePlayer02(vec, ROUTE01, ROUTE03, ROUTE02);
			break;

		case ROUTE08:
			// 0,3,4のルートデータを比較
			DistanceRoutePlayer02(vec, ROUTE03, ROUTE05, ROUTE04);
			break;

		default:
			assert("AIのルート切替でエラー");
			break;
	}
}

//===========================================================================
// 隣接してる経路データへの移動選択処理(データROUTE00～ROUTE04)
// 引　数：D3DXVECTOR3 vec(相手のルートデータ), int cnt(ループカウンター), 
//		   int cntMax(カウンターの最大値), int raiseCnt(ループカウントの調整値)
// 戻り値：D3DXVECTOR3型
//==========================================================================
void DistanceRoutePlayer01(D3DXVECTOR3 vec, int cnt, int cntMax, int raiseCnt)
{
	AI *ai = &AiWk[0];
	PLAYER *player = GetPlayer(P2);
	D3DXVECTOR3 vec01 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ルートまでのベクトル
	float vestRoute01 = 0.0;							// 最も好ましいルート
	float vestRoute02 = 1000.0;							// 最も好ましいルート

	for (cnt; cnt < cntMax; cnt++)
	{
		// ベクトルの大きさを計算
		vec01 = vec - RouteData[cnt];
		ai->lenVec[cnt] = D3DXVec3Length(&vec01);

		// 巡回の場合
		if (ai->decision == ai->cmpPattern[ROUTINE])
		{
			// 距離と通過経過時間
			ai->routeRot[cnt][ROUTE_LENGTH] = FuzzyRightDown(ai->lenVec[cnt], ROUTE_DISTANCE_FUZZY_X1, ROUTE_DISTANCE_FUZZY_X2);
			ai->routeRot[cnt][ROUTE_TIME] = FuzzyRightUp((float)ai->lapingTime[cnt], ROUTE_TIME_FUZZY_X1, ROUTE_TIME_FUZZY_X2);
			// 結果
			ai->routeRot[cnt][ROUTE_DECISION] = ai->routeRot[cnt][ROUTE_LENGTH] * ai->routeRot[cnt][ROUTE_TIME];

			// 好ましい隣接したルートを算出
			vestRoute01 = max(ai->routeRot[cnt][ROUTE_DECISION], vestRoute01);
			
			// ルートを決定
			if (vestRoute01 == ai->routeRot[cnt][ROUTE_DECISION])
			{
				ai->routeIndex = cnt;
			}

			// ループカウントの調整
			cnt = cnt + raiseCnt;
		}
		// 追跡の場合
		else if (ai->decision == ai->cmpPattern[CHASE])
		{	
			// 好ましい隣接したルートを算出
			vestRoute02 = min(ai->lenVec[cnt], vestRoute02);

			// ルートを決定
			if (vestRoute02 == ai->lenVec[cnt])
			{
				ai->routeIndex = cnt;
			}

			// ループカウントの調整
			cnt = cnt + raiseCnt;
		}
		// 逃走の場合
		else if (ai->decision == ai->cmpPattern[ESCAPE])
		{
			// 好ましい隣接したルートを算出
			vestRoute01 = max(ai->lenVec[cnt], vestRoute01);

			// ルートを決定
			if (vestRoute01 == ai->lenVec[cnt])
			{
				ai->routeIndex = cnt;
			}

			// ループカウントの調整
			cnt = cnt + raiseCnt;
		}

	}

	// 決定したルートの通過経過時間を初期化
	ai->lapingTime[ai->routeIndex] = 0;
}

//===========================================================================
// 隣接してる経路データへの移動選択処理(データROUTE05～ROUTE08)
// 引　数：D3DXVECTOR3 vec(相手のルートデータ), int cnt(ループカウンター), 
//		   int cntMax(カウンターの最大値), int raiseCnt(ループカウントの調整値)
// 戻り値：D3DXVECTOR3型
//==========================================================================
void DistanceRoutePlayer02(D3DXVECTOR3 vec , int cnt, int cntMax, int raiseCnt)
{
	AI *ai = &AiWk[0];
	PLAYER *player = GetPlayer(P2);
	D3DXVECTOR3 vec01 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ルートまでのベクトル
	float vestRoute01 = 0.0f;							// 最も好ましいルート
	float vestRoute02 = 1000.0f;						// 最も好ましいルート
	int arrey = 0;										// 配列添え字

	for (int i = 0; i < cntMax; i++)
	{
		// ベクトルの大きさを計算
		vec01 = vec - RouteData[arrey];
		ai->lenVec[arrey] = D3DXVec3Length(&vec01);

		if (ai->decision == ai->cmpPattern[ROUTINE])
		{
			// メンバーシップの作成
			ai->routeRot[arrey][ROUTE_LENGTH] = FuzzyRightDown(ai->lenVec[arrey], ROUTE_DISTANCE_FUZZY_X1, ROUTE_DISTANCE_FUZZY_X2);
			ai->routeRot[arrey][ROUTE_TIME] = FuzzyRightUp((float)ai->lapingTime[arrey], ROUTE_TIME_FUZZY_X1, ROUTE_TIME_FUZZY_X2);
			ai->routeRot[arrey][ROUTE_DECISION] = ai->routeRot[arrey][ROUTE_LENGTH] * ai->routeRot[arrey][ROUTE_TIME];

			vestRoute01 = max(ai->routeRot[arrey][ROUTE_DECISION], vestRoute01);

			if (vestRoute01 == ai->routeRot[arrey][ROUTE_DECISION])
			{
				ai->routeIndex = arrey;
			}

			// 配列の調整
			i == 0 ? arrey = cnt : arrey = raiseCnt;
		}
		// 結果が追跡だった場合
		else if (ai->decision == ai->cmpPattern[CHASE])
		{
			vestRoute02 = min(ai->lenVec[arrey], vestRoute02);

			if (vestRoute02 == ai->lenVec[arrey])
			{
				ai->routeIndex = arrey;
			}

			// 配列の調整
			i == 0 ? arrey = cnt : arrey = raiseCnt;
		}
		// 結果が逃走だった場合
		else if (ai->decision == ai->cmpPattern[ESCAPE])
		{
			vestRoute01 = max(ai->lenVec[arrey], vestRoute01);

			if (vestRoute01 == ai->lenVec[arrey])
			{
				ai->routeIndex = arrey;
			}

			// 配列の調整
			i == 0 ? arrey = cnt : arrey = raiseCnt;
		}

	}

	// ファジー値の最も高いルートへ
	ai->lapingTime[ai->routeIndex] = 0;	// 経過時間を初期化
}

//==============================================================================
// 進行方向へ回転
// 引　数：D3DXVECTOR3 vecDest(進む方向の終点)
// 戻り値：な　し
//==============================================================================
void NonePlayerDest(D3DXVECTOR3 vecDest,int index)
{
	PLAYER *player = GetPlayer(index);
	CAMERA *camera = GetCamera();
	D3DXVECTOR3 vec = vecDest - player->pos;					// 進行方向ベクトル
	D3DXVec3Normalize(&vec, &vec);
	D3DXVec3Normalize(&player->frontVec, &player->frontVec);	// 現在のモデルの正面

	// 角度を計算
	float vecAngle = atan2f(vec.z, vec.x);
	float frontAngle = atan2f(player->frontVec.z, player->frontVec.x);
	
	if (frontAngle != vecAngle)
	{
		// 角度の差分を算出し回転
		float difAngle = (frontAngle - vecAngle);
		player->rot.y += difAngle;
	}
}

//==============================================================================
// 進行方向への移動処理
// 引　数：D3DXVECTOR3 vec(進む方向), float fuzzy(ファジー値)
// 戻り値：な　し
//==============================================================================
void MoveVectorNonePlayer(D3DXVECTOR3 vec, float fuzzy)
{
	PLAYER *player = GetPlayer(P2);
	NonePlayerDest(vec, P2);
	D3DXVECTOR3 vec01 = vec - player->pos;
	D3DXVec3Normalize(&vec01, &vec01);

	player->speed = fuzzy * 5.0f;
	player->move.x = vec01.x * player->speed;
	player->move.z = vec01.z * player->speed;
}

//===========================================================================
// NPCの攻撃処理
// 引　数：な　し
// 戻り値：な　し
//==========================================================================
void NonePlayerAttack(void)
{
	AI *ai = &AiWk[0];
	PLAYER *player = GetPlayer(0);
	BULLET *bullet = GetBullet(P2);
	BLOCK *block = GetBlock(0);
	BALL *ball = GetBall(0);

	ai->atcCntFrame++;

	// バレット使用中チャージ不可
	if (!bullet->use[0])
	{
		PlaySound(player[P2].chargeSE, E_DS8_FLAG_LOOP);
		// 最大値になった場合
		if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
		{
			SetChargeEffect(player[P2].pos, P2);
			bullet->speedIncrease = BULLET_CHARGE_MAX;
		}
		// 10フレーム
		else if (player->cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
		{
			SetChargeEffect(player[P2].pos, P2);
			bullet->speedIncrease += 0.2f;
		}
	}

	D3DXVECTOR3 rayPos = player[P2].frontVec + player[P2].pos;	// 前方ベクトルの終点
	D3DXVECTOR3 reflectVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 反射後のベクトル

	// 前方ベクトルの反射処理
	for (int i = 0; i < BLOCK_MAX; i++, block++)
	{
		if (!HitCheckBlock(rayPos, player[P2].pos, BLOCK_VTX_MAX))
		{
			reflectVec = ReflectVector(rayPos, player[P2].pos, GetNormal());
			break;
		}
	}

	// 60フレーム処理を開ける
	if (ai->atcCntFrame > RELOAD_ATTCK_FRAME)
	{
		// 攻撃開始判定
		if (CheckHitRay(player[P1].pos, player[P2].pos, player[P2].frontVec, 15.0f))
		{
			StopSound(player[P2].chargeSE);
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			ai->atcCntFrame = 0;
		}
		// 反射を考慮した攻撃
		else if (CheckHitRay(player[P1].pos, player[P2].pos, reflectVec, 10.0f))
		{
			StopSound(player[P2].chargeSE);
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			ai->atcCntFrame = 0;
		}		
		
		for (int i = 0; i < BALL_ONESET_MAX; i++)
		{
			if (!ball->use) continue;
			// 攻撃開始判定
			if (CheckHitRay(ball->pos[i], player[P2].pos, player[P2].frontVec, 5.0f))
			{
				StopSound(player[P2].chargeSE);
				SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
				ai->atcCntFrame = 0;
			}
		}

	}
}

//==============================================================================
// タイトル時のNPCのフィールド巡回
// 引　数：な　し
// 戻り値：な　し
//==============================================================================
void NonePlayerPatrol(int index)
{
	PLAYER *player = GetPlayer(index);
	BLOCK *block = GetBlock(0);
	AI *ai = &AiWk[index];
	D3DXVECTOR3 rayPos = player->frontVec + player->pos;	// 前方ベクトルの終点
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 前方ベクトルのとブロックの判定
	if (CheckHitBC(player->pos, RouteData[ai->patrolNum], 15.0f, 15.0f))
	{
		ai->patrolNum = SwitchPatrolPattern(ai->patrolNum);
	}

	// 向きを進行方向へ
	NonePlayerDest(RouteData[ai->patrolNum], index);

	// 目的地まで移動
	vec = RouteData[ai->patrolNum] - player->pos;
	D3DXVec3Normalize(&vec, &vec);
	player->move.x = vec.x * 5.0f;
	player->move.z = vec.z * 5.0f;
}

//==========================================================================
// タイトル時のNPCの巡回行動遷移処理
// 引　数：int pattern(巡回パターン番号)
// 戻り値：な　し
//==========================================================================
int SwitchPatrolPattern(int pattern)
{
	int out = 0;

	switch (pattern)
	{
		case ROUTE01 : out = ROUTE02;
			break;

		case ROUTE02 : out = ROUTE04;
			break;

		case ROUTE04 : out = ROUTE03;
			break;

		case ROUTE03 : out = ROUTE01;
			break;

	}

	return out;
}

//==========================================================================
// AIのアドレス取得処理
// 引　数：int pno(プレイヤーのアドレス番号)
// 戻り値：AI型構造体
//==========================================================================
AI *GetAi(int pno)
{
	return &AiWk[pno];
}