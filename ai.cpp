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
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PATROL_PATTERN_MAX					(9) 

// NPCの移動マクロ
#define MOVE_DISTANCE_CHASE_FUZZY_X1		(150.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X2		(250.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X3		(300.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X4		(800.0f)
#define MOVE_DISTANCE_ESCAPE_FUZZY_X1		(200.0f)
#define MOVE_DISTANCE_ESCAPE_FUZZY_X2		(800.0f)
#define MOVE_DISTANCE_PATROL_FUZZY_X1		(200.0f)
#define MOVE_DISTANCE_PATROL_FUZZY_X2		(300.0f)
#define MOVE_DISTANCE_PATROL_FUZZY_X3		(500.0f)
#define MOVE_DISTANCE_PATROL_FUZZY_X4		(800.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X1			(100.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X2			(150.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X3			(300.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X4			(800.0f)


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

#define STOP_ROUTINE_CNT					(2)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int SwitchPatrolPattern(int pattern);
void NonePlayerDest(D3DXVECTOR3 vecDest, int index);
bool StopRoutineNonePlayer(void);

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

// 巡回のパターン
D3DXVECTOR3			MovePattern[PATROL_PATTERN_MAX]{

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

int			cntFrame;
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
	cntFrame = 0;

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
	
	// 思考するか
	if (!StopRoutineNonePlayer())
	{
		cntFrame++;
		// 30フレーム思考停止
		if (cntFrame % 30 == 0)
		{
			for (int i = 0; i < DECISION_MEMORY_MAX; i++)
			{
				ai->deciMemory[i] = 0;	//初期化
			}

			cntFrame = 0;
		}

		return;
	}

	ai->cntMemory < DECISION_MEMORY_MAX ? ai->cntMemory++ : ai->cntMemory = 0;	// 配列の添え字を更新

	// 相手との距離による判定
	D3DXVECTOR3 pvpVec = player[P2].pos - player[P1].pos;
	float vecLength = D3DXVec3Length(&pvpVec);
	ai->chase[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_CHASE_FUZZY_X1, MOVE_DISTANCE_CHASE_FUZZY_X2,
													MOVE_DISTANCE_CHASE_FUZZY_X3, MOVE_DISTANCE_CHASE_FUZZY_X4);
	ai->escape[PATTERN1] = FuzzyRightDown(vecLength, MOVE_DISTANCE_ESCAPE_FUZZY_X1, MOVE_DISTANCE_ESCAPE_FUZZY_X2);
	ai->patrol[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_PATROL_FUZZY_X1, MOVE_DISTANCE_PATROL_FUZZY_X2,
												     MOVE_DISTANCE_PATROL_FUZZY_X3, MOVE_DISTANCE_PATROL_FUZZY_X4);
	ai->wait[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2, 
												   MOVE_DISTANCE_WAIT_FUZZY_X3, MOVE_DISTANCE_WAIT_FUZZY_X4);

	// 自ライフによる判定
	if (player[P2].life > 0.0f)
	{
		ai->chase[PATTERN2] = FuzzyRightDown(player[P2].life, MOVE_NPCLIFE_CHASE_FUZZY_X1, MOVE_NPCLIFE_CHASE_FUZZY_X2);
		ai->escape[PATTERN2] = FuzzyRightUp(player[P2].life, MOVE_NPCLIFE_ESCAPE_FUZZY_X1, MOVE_NPCLIFE_ESCAPE_FUZZY_X2);
		
		ai->cmpPattern[CHASE] = ai->chase[PATTERN1] * ai->chase[PATTERN2];
		ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1] * ai->escape[PATTERN2];
		ai->cmpPattern[PATROL] = ai->patrol[PATTERN1];
		ai->cmpPattern[WAIT] = ai->wait[PATTERN1];
	}
	// 相手ライフが最高値の時は考慮しない
	else if (player[P2].life == 0.0f)
	{
		ai->cmpPattern[CHASE] = ai->chase[PATTERN1];
		ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1];
		ai->cmpPattern[PATROL] = ai->patrol[PATTERN1];
		ai->cmpPattern[WAIT] = ai->wait[PATTERN1];
	}

	// 相手ライフによる判定
	if (player[P1].life < PLAYER_LIFE_MAX)
	{
		ai->chase[PATTERN3] = FuzzyRightDown(player[P1].life, MOVE_PLAYERLIFE_CHASE_FUZZY_X1, MOVE_PLAYERLIFE_CHASE_FUZZY_X2);
		ai->escape[PATTERN3] = FuzzyRightUp(player[P1].life, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2);

		ai->cmpPattern[CHASE] = ai->chase[PATTERN1] * ai->chase[PATTERN2] * ai->chase[PATTERN3];
		ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1] * ai->escape[PATTERN2] * ai->escape[PATTERN3];
		ai->cmpPattern[PATROL] = ai->patrol[PATTERN1];
		ai->cmpPattern[WAIT] = ai->wait[PATTERN1];
	}

	// 比較
	ai->decision = Or(ai->cmpPattern[CHASE], ai->cmpPattern[ESCAPE]);
	ai->decision = Or(ai->cmpPattern[PATROL], ai->decision);
	ai->decision = Or(ai->cmpPattern[WAIT], ai->decision);

	// 結果反映
	// 追跡
	if (ai->decision == ai->cmpPattern[CHASE])
	{
		NonePlayerDest(player[P1].pos, P2);
		D3DXVECTOR3 vec = player[P1].pos - player[P2].pos;
		D3DXVec3Normalize(&vec, &vec);
		D3DXVec3Normalize(&player[P2].frontVec, &player[P2].frontVec);

		player[P2].speed = 5.0f * ai->cmpPattern[CHASE];

		player[P2].move.x = vec.x * player[P2].speed;
		player[P2].move.z = vec.z * player[P2].speed;

		ai->deciMemory[ai->cntMemory] = CHASE;

	}
	// 逃走
	else if (ai->decision == ai->cmpPattern[ESCAPE])
	{
		//D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//for (int i = 0; i < PATROL_PATTERN_MAX; i++)
		//{ 
		//	vec = MovePattern[i] - player[P2].pos;
		//	float vecLen = D3DXVec3Length(&vec);
		//	float moveLen = D3DXVec3Length(&MovePattern[i]);
		//	min(vecLen, moveLen);

		//}

		//D3DXVECTOR3 vec = player[P2].pos - player[P1].pos;
		////NonePlayerDest(vec);
		//D3DXVec3Normalize(&vec, &vec);
		//player[P2].speed = 5.0f * ai->cmpPattern[ESCAPE];
		//player[P2].move.x = vec.x * player[P2].speed;
		//player[P2].move.z = vec.z * player[P2].speed;

		ai->deciMemory[ai->cntMemory] = ESCAPE;
	}
	// 巡回
	else if (ai->decision == ai->cmpPattern[PATROL])
	{
		NonePlayerPatrol(P2);
		ai->deciMemory[ai->cntMemory] = PATROL;
	}
	// 待機
	else if (ai->decision == ai->cmpPattern[WAIT])
	{
		return;
	}
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
	NonePlayerDest(MovePattern[ai->patrolNum], index);

	// 目的地まで移動
	vec = MovePattern[ai->patrolNum] - player->pos;
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
		if (!HitCheckBlock(rayPos, player[P2].pos, BLOCK_VTX_MAX))
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
