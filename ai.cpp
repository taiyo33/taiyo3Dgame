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
#include "camera.h"
#include "chargeEffect.h"
#include "debugproc.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// NPCの移動マクロ
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


#define MOVE_NPCLIFE_CHASE_FUZZY_X1			(0.0f)
#define MOVE_NPCLIFE_CHASE_FUZZY_X2			(100.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X2		(80.0f)

#define MOVE_PLAYERLIFE_CHASE_FUZZY_X1		(10.0f)
#define MOVE_PLAYERLIFE_CHASE_FUZZY_X2		(70.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2		(50.0f)

// NPCの攻撃マクロ
#define ATTACK_DISTANCE_ATC_FUZZY_X1		(150.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X2		(250.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X3		(400.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X4		(600.0f)
#define ATTACK_PLAYERLIFE_WAIT_FUZZY_X1		(0.0f)
#define ATTACK_PLAYERLIFE_WAIT_FUZZY_X2		(100.0f)
#define RELOAD_ATTCK_FRAME					(60)		// 攻撃の可能までの間隔

// ルート巡回マクロ
#define STOP_ROUTINE_CNT					(2)
#define ROUTE_DISTANCE_FUZZY_X1				(0.0f)
#define ROUTE_DISTANCE_FUZZY_X2				(600.0f)
#define ROUTE_TIME_FUZZY_X1					(0.0f)
#define ROUTE_TIME_FUZZY_X2					(60.0f)
#define ROUTE_CNT_TIME						(90)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int SwitchPatrolPattern(int pattern);
void NonePlayerDest(D3DXVECTOR3 vecDest, int index);
bool StopRoutineNonePlayer(void);
void DistancePlayer(int index);
void MoveVectorNonePlayer(D3DXVECTOR3 vec, float fuzzy);


//*****************************************************************************
// グローバル変数
//*****************************************************************************

// ファジーの１要素数
enum {
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4
};
// 比較合計の種類
enum {
	CHASE,
	ESCAPE,
	PATROL,
	WAIT
};

// 巡回の種類
enum {
	PATROL01,
	PATROL02,
	PATROL03,
	PATROL04,
	PATROL05,
	PATROL06,
	PATROL07,
	PATROL08,
	PATROL09
};

enum {
	ROUTE_DECISION,
	ROUTE_LENGTH,
	ROUTE_TIME
};

// 巡回のパターン
D3DXVECTOR3			RouteData[ROUTEDATA_MAX]{

	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 0.0f, -FIELD_SIZE_Z + 100.0f),	// 左下
	D3DXVECTOR3(-FIELD_SIZE_X / 2, 0.0f, -FIELD_SIZE_Z + 50.0f),		// 左中央
	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 0.0f, FIELD_SIZE_Z - 100.0f),	// 左上
	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 0.0f, FIELD_SIZE_Z / 2),		// 上中央
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 0.0f, FIELD_SIZE_Z - 100.0f),	// 右上
	D3DXVECTOR3(FIELD_SIZE_X / 2, 0.0f, FIELD_SIZE_Z - 100.0f),			// 右中央
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 0.0f, -FIELD_SIZE_Z + 100.0f),	// 右下
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 0.0f, -FIELD_SIZE_Z / 2),		// 下中央
	D3DXVECTOR3(0.0f, 0.0f, 0.0f)										// 中央

};

int			CntFrame;
AI			aiWk[PLAYER_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAi(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	AI *ai = &aiWk[0];

	ai->cmpPattern[CHASE] = 0.0f;	// 比較の合計値を初期化
	ai->cmpPattern[ESCAPE] = 0.0f;	//
	ai->cmpPattern[PATROL] = 0.0f;	// 
	ai->decision = 0.0f;			// 比較結果を初期化
	ai->cntMemory = 0;				// 結果記憶配列の添え字を初期化
	ai[P1].patrolNum = PATROL05;	// 巡回パターンの番号を初期化
	ai[P2].patrolNum = PATROL01;	//
	CntFrame = 0;

	for (int i = 0; i < CMP_PATTERN_MAX; i++)
	{
		ai->atc[i] = 0.0f;		// 攻撃の値を初期化
		ai->chase[i] = 0.0f;	// 追跡の値を初期化
		ai->escape[i] = 0.0f;	// 逃走の値を初期化
		ai->wait[i] = 0.0f;		// 待機の値を初期化
	}

	for (int j = 0; j < DECISION_MEMORY_MAX; j++)
	{
		ai->deciMemory[j] = j;	// 結果記憶を初期化
	}

	for (int k = 0; k < ROUTEDATA_MAX; k++)
	{
		ai->routeRot[k][ROUTE_DECISION] = 0.0f;	// 結果
		ai->routeRot[k][ROUTE_LENGTH] = 0.0f;	// ベクトルのファジー値
		ai->routeRot[k][ROUTE_TIME] = 0.0f;		// 経過時間のファジー値
		ai->lenVec[k] = 0.0f;					// ベクトルの大きさ
		ai->lapingTime[k] = 0;					// 経過時間の初期化
	}

	ai->routeIndex = 0;

	return S_OK;
}

//===========================================================================
// NPCの移動処理
// 引　数：な　し
// 戻り値：な　し
//==========================================================================
void NonePlayerMove(void)
{
	AI *ai = &aiWk[0];
	PLAYER *player = GetPlayer(0);
	
	CntFrame++;
	if (CntFrame % ROUTE_CNT_TIME == 0)
	{
		DistancePlayer(P2);
		CntFrame = 0;
	}

	MoveVectorNonePlayer(RouteData[ai->routeIndex], 1.0f);

	// 思考するか
	//if (!StopRoutineNonePlayer())
	//{
	//	CntFrame++;
	//	// 30フレーム思考停止
	//	if (CntFrame % 30 == 0)
	//	{
	//		for (int i = 0; i < DECISION_MEMORY_MAX; i++)
	//		{
	//			ai->deciMemory[i] = 0;	//初期化
	//		}

	//		CntFrame = 0;
	//	}

	//	return;
	//}

	//ai->cntMemory < DECISION_MEMORY_MAX ? ai->cntMemory++ : ai->cntMemory = 0;	// 配列の添え字を更新

	//// 相手との距離による判定
	//D3DXVECTOR3 pvpVec = player[P1].pos - player[P2].pos;
	//float vecLength = D3DXVec3Length(&pvpVec);
	//ai->chase[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_CHASE_FUZZY_X1, MOVE_DISTANCE_CHASE_FUZZY_X2,
	//												MOVE_DISTANCE_CHASE_FUZZY_X3, MOVE_DISTANCE_CHASE_FUZZY_X4);
	//ai->escape[PATTERN1] = FuzzyRightDown(vecLength, MOVE_DISTANCE_ESCAPE_FUZZY_X1, MOVE_DISTANCE_ESCAPE_FUZZY_X2);
	//ai->patrol[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_PATROL_FUZZY_X1, MOVE_DISTANCE_PATROL_FUZZY_X2,
	//											     MOVE_DISTANCE_PATROL_FUZZY_X3, MOVE_DISTANCE_PATROL_FUZZY_X4);
	////ai->wait[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2, 
	////												   MOVE_DISTANCE_WAIT_FUZZY_X3, MOVE_DISTANCE_WAIT_FUZZY_X4);

	//// 自ライフによる判定
	//if (player[P2].life < PLAYER_LIFE_MAX)
	//{
	//	ai->chase[PATTERN2] = FuzzyRightDown(player[P2].life, MOVE_NPCLIFE_CHASE_FUZZY_X1, MOVE_NPCLIFE_CHASE_FUZZY_X2);
	//	ai->escape[PATTERN2] = FuzzyRightUp(player[P2].life, MOVE_NPCLIFE_ESCAPE_FUZZY_X1, MOVE_NPCLIFE_ESCAPE_FUZZY_X2);
	//	
	//	ai->cmpPattern[CHASE] = ai->chase[PATTERN1] * ai->chase[PATTERN2];
	//	ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1] * ai->escape[PATTERN2];
	//	ai->cmpPattern[PATROL] = ai->patrol[PATTERN1];
	//	//ai->cmpPattern[WAIT] = ai->wait[PATTERN1];
	//}
	//// 相手ライフが最高値の時は考慮しない
	//else if (player[P2].life == 0.0f)
	//{
	//	ai->cmpPattern[CHASE] = ai->chase[PATTERN1];
	//	ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1];
	//	ai->cmpPattern[PATROL] = ai->patrol[PATTERN1];
	//	//i->cmpPattern[WAIT] = ai->wait[PATTERN1];
	//}

	//// 相手ライフによる判定
	//if (player[P1].life < PLAYER_LIFE_MAX)
	//{
	//	ai->chase[PATTERN3] = FuzzyRightDown(player[P1].life, MOVE_PLAYERLIFE_CHASE_FUZZY_X1, MOVE_PLAYERLIFE_CHASE_FUZZY_X2);
	//	ai->escape[PATTERN3] = FuzzyRightUp(player[P1].life, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2);

	//	ai->cmpPattern[CHASE] = ai->chase[PATTERN1] * ai->chase[PATTERN2] * ai->chase[PATTERN3];
	//	ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1] * ai->escape[PATTERN2] * ai->escape[PATTERN3];
	//	ai->cmpPattern[PATROL] = ai->patrol[PATTERN1];
	//	ai->cmpPattern[WAIT] = ai->wait[PATTERN1];
	//}

	//// 比較
	//ai->decision = Or(ai->cmpPattern[CHASE], ai->cmpPattern[ESCAPE]);
	//ai->decision = Or(ai->cmpPattern[PATROL], ai->decision);
	//ai->decision = Or(ai->cmpPattern[WAIT], ai->decision);

	//// 結果反映
	//// 追跡
	//if (ai->decision == ai->cmpPattern[CHASE])
	//{
	//	MoveVectorNonePlayer(player[P1].pos, ai->decision);
	//	ai->deciMemory[ai->cntMemory] = CHASE;
	//}
	//// 逃走
	//else if (ai->decision == ai->cmpPattern[ESCAPE])
	//{
	//	MoveVectorNonePlayer(player[P2].pos, ai->decision);
	//	ai->deciMemory[ai->cntMemory] = ESCAPE;
	//}
	//// 巡回
	//else if (ai->decision == ai->cmpPattern[PATROL])
	//{
	//	NonePlayerPatrol(P2);
	//	ai->deciMemory[ai->cntMemory] = PATROL;
	//}
	//// 待機
	//else if (ai->decision == ai->cmpPattern[WAIT])
	//{
	//	return;
	//}
}

//===========================================================================
// プレイヤーと経路データとの距離計算処理
// 引　数：int index(プレイヤーのアドレス番号)
// 戻り値：D3DXVECTOR3型　最も近い経路のベクトルを返す
//==========================================================================
void DistancePlayer(int index)
{
	AI *ai = &aiWk[0];
	PLAYER *player = GetPlayer(index);
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float vestRoute = 0.0;

	for (int i = 0; i < ROUTEDATA_MAX; i++)
	{
		ai->lapingTime[i]++;

		// ベクトルの大きさを計算
		vec = player->pos - RouteData[i];
		ai->lenVec[i] = D3DXVec3Length(&vec);

		// メンバーシップの作成
		ai->routeRot[i][ROUTE_LENGTH] = FuzzyRightDown(ai->lenVec[i], ROUTE_DISTANCE_FUZZY_X1, ROUTE_DISTANCE_FUZZY_X2);
	
		ai->routeRot[i][ROUTE_TIME] = FuzzyRightUp(ai->lapingTime[i], ROUTE_TIME_FUZZY_X1, ROUTE_TIME_FUZZY_X2);
		ai->routeRot[i][ROUTE_DECISION] = ai->routeRot[i][ROUTE_LENGTH] * ai->routeRot[i][ROUTE_TIME];

		vestRoute = max(ai->routeRot[i][ROUTE_DECISION], vestRoute);
		
		if (ai->routeIndex == i) continue;

		if (vestRoute == ai->routeRot[i][ROUTE_DECISION])
		{
			ai->routeIndex = i;
		}
	}

	// ファジー値の最も高いルートへ
	ai->lapingTime[ai->routeIndex] = 0;
}

//==============================================================================
// NPCの思考切替
// 引　数：な　し
// 戻り値：な　し
//==============================================================================
bool StopRoutineNonePlayer(void)
{
	AI *ai = &aiWk[0];
	int cnt = 0;

	// 過去4回の結果と比較
	for (int i = 0; i < DECISION_MEMORY_MAX; i++)
	{
		// 異なる処理ならカウントインクリメント
		if (ai->deciMemory[i] != ai->deciMemory[i + 1])
		{
			cnt++;
		}
	}

	// 異なる結果を２回繰り返しているなら思考しない
	if (cnt > STOP_ROUTINE_CNT) return false;

	return true;
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
	D3DXVECTOR3 vec = vecDest - player->pos;	// 進行方向ベクトル
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
// 進行方向の決定処理
// 引　数：D3DXVECTOR3 vec(進む方向)
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

//==============================================================================
// NPCのフィールド巡回
// 引　数：な　し
// 戻り値：な　し
//==============================================================================
void NonePlayerPatrol(int index)
{
	PLAYER *player = GetPlayer(index);
	BLOCK *block = GetBlock(0);
	AI *ai = &aiWk[index];
	D3DXVECTOR3 rayPos = player->frontVec + player->pos;	// 前方ベクトルの終点
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 前方ベクトルのとブロックの判定
	if (!HitCheckBlock(rayPos, player->pos, BLOCK_NUM_WALL))
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
// NPCの巡回行動遷移処理
// 引　数：int pattern(巡回パターン番号)
// 戻り値：な　し
//==========================================================================
int SwitchPatrolPattern(int pattern)
{
	int out = 0;

	switch (pattern)
	{
		case PATROL01 : out = PATROL03;
			break;

		case PATROL03 : out = PATROL05;
			break;

		case PATROL05 : out = PATROL07;
			break;

		case PATROL07 : out = PATROL01;
			break;

		//case PATROL05: out = PATROL06;
		//	break;

		//case PATROL06: out = PATROL07;
		//	break;

		//case PATROL07: out = PATROL08;
		//	break;

		//case PATROL08: out = PATROL01;
		//	break;
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
	AI *ai = &aiWk[0];
	PLAYER *player = GetPlayer(0);
	BULLET *bullet = GetBullet(P2);
	BLOCK *block = GetBlock(0);
	
	// バレット使用中チャージ不可
	if (!bullet->use[P2])
	{
		// 最大値になった場合
		if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
		{
			SetChargeEffect(player[P2].pos, player[P2].rot, 0, P2);
			bullet->speedIncrease = BULLET_CHARGE_MAX;
		}
		// 10フレーム
		else if (player->cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
		{
			SetChargeEffect(player[P2].pos, player[P2].rot, 0, P2);
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

	// バレット後１秒間処理を開ける
	if (CntFrame % RELOAD_ATTCK_FRAME == 0)
	{
		// 攻撃開始判定
		if (CheckHitRay(player[P1].pos, player[P2].pos, player[P2].frontVec, 15.0f))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			CntFrame = 0;
		}
		// 反射を考慮した攻撃
		else if (CheckHitRay(player[P1].pos, player[P2].pos, reflectVec, 15.0f))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			CntFrame = 0;
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
