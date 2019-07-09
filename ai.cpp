//=============================================================================
//
// AI���� [ai.cpp]
// Author : GP11A_341_22_�c�����z 
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
// �}�N����`
//*****************************************************************************

// �����ɂ��Ẵ}�N��
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

// NPC�̃��C�t�ɂ��Ẵ}�N��
#define MOVE_NPCLIFE_CHASE_FUZZY_X1			(20.0f)
#define MOVE_NPCLIFE_CHASE_FUZZY_X2			(80.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X2		(80.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X1		(10.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X2		(30.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X3		(70.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X4		(90.0f)

// �v���C���[�̃��C�t�ɂ��Ẵ}�N��
#define MOVE_PLAYERLIFE_CHASE_FUZZY_X1		(10.0f)
#define MOVE_PLAYERLIFE_CHASE_FUZZY_X2		(70.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2		(50.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X1	(10.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X2	(30.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X3	(80.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X4	(100.0f)

// NPC�̃{�[���������ɂ��Ẵ}�N��
#define MOVE_NPCBALL_ROUTINE_FUZZY_X1		(30.0f)
#define MOVE_NPCBALL_ROUTINE_FUZZY_X2		(50.0f)
#define MOVE_NPCBALL_ROUTINE_FUZZY_X3		(70.0f)
#define MOVE_NPCBALL_ROUTINE_FUZZY_X4		(100.0f)
#define MOVE_NPCBALL_CHASE_FUZZY_X1			(30.0f)
#define MOVE_NPCBALL_CHASE_FUZZY_X2			(75.0f)
#define MOVE_NPCBALL_ESCAPE_FUZZY_X1		(50.0f)
#define MOVE_NPCBALL_ESCAPE_FUZZY_X2		(80.0f)

// �v���C���[�̃{�[���������ɂ��Ẵ}�N��
#define MOVE_PLAYERBALL_ROUTINE_FUZZY_X1	(0.0f)
#define MOVE_PLAYERBALL_ROUTINE_FUZZY_X2	(10.0f)
#define MOVE_PLAYERBALL_ROUTINE_FUZZY_X3	(70.0f)
#define MOVE_PLAYERBALL_ROUTINE_FUZZY_X4	(100.0f)
#define MOVE_PLAYERBALL_CHASE_FUZZY_X1		(20.0f)
#define MOVE_PLAYERBALL_CHASE_FUZZY_X2		(75.0f)
#define MOVE_PLAYERBALL_ESCAPE_FUZZY_X1		(0.0f)
#define MOVE_PLAYERBALL_ESCAPE_FUZZY_X2		(100.0f)

// ���[�g����}�N��
#define STOP_ROUTINE_CNT					(2)
#define ROUTE_DISTANCE_FUZZY_X1				(0.0f)
#define ROUTE_DISTANCE_FUZZY_X2				(500.0f)
#define ROUTE_TIME_FUZZY_X1					(0.0f)
#define ROUTE_TIME_FUZZY_X2					(30.0f)
#define ROUTE_CNT_TIME						(70)

#define RELOAD_ATTCK_FRAME					(60.0f)		// �U���̉\�܂ł̊Ԋu

//*****************************************************************************
// �v���g�^�C�v�錾
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
// �O���[�o���ϐ�
//*****************************************************************************

// �t�@�W�[�̗v�f��
enum FUZZY_PATTERN{
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4
};

// ��r���v�̎��
enum MOVE_PATTERN{
	CHASE,
	ESCAPE,
	ROUTINE,
};

// ����̃��[�g�ԍ�
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

// ����Q�ƒl�̎��
enum ROUTE_PATTERN{
	ROUTE_DECISION,
	ROUTE_LENGTH,
	ROUTE_TIME
};

// ����̃p�^�[��
D3DXVECTOR3		RouteData[ROUTEDATA_MAX]{

	D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ����
	D3DXVECTOR3(-400.0f + 100.0f, 10.0f, 300.0f - 50.0f),	// ����
	D3DXVECTOR3(400.0f - 100.0f, 10.0f, 300.0f - 50.0f),	// �E��
	D3DXVECTOR3(-400.0f + 100.0f, 10.0f, -300.0f + 50.0f),	// ����
	D3DXVECTOR3(400.0f - 100.0f, 10.0f, -300.0f + 50.0f),	// �E��
	D3DXVECTOR3(-400.0f + 100.0f, 10.0f, 0.0f),				// ������
	D3DXVECTOR3(400.0f - 100.0f, 10.0f, 0.0f),				// �E����
	D3DXVECTOR3(0.0f, 10.0f, 300.0f - 80.0f),				// �㒆��
	D3DXVECTOR3(0.0f, 10.0f, -300.0f + 80.0f),				// ������
};

AI			AiWk[PLAYER_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitAi(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	AI *ai = &AiWk[0];

	ai->cmpPattern[CHASE] = 0.0f;	// ��r�̍��v�l��������
	ai->cmpPattern[ESCAPE] = 0.0f;	//
	ai->cmpPattern[ROUTINE] = 0.0f;	// 
	ai->decision = 0.0f;			// ��r���ʂ�������
	ai[P1].patrolNum = ROUTE01;		// ����p�^�[���̔ԍ���������
	ai[P2].patrolNum = ROUTE04;		//

	ai->routineCntFrame = 0;		// �v�l�Ԋu
	ai->atcCntFrame = 0;			// �U���̊Ԋu
	ai->routineStart = true;		// ���񂵂Ă��邩

	for (int i = 0; i < CMP_PATTERN_MAX; i++)
	{
		ai->chase[i] = 0.0f;	// �ǐՂ̒l��������
		ai->escape[i] = 0.0f;	// �����̒l��������
		ai->routine[i] = 0.0f;	// �ҋ@�̒l��������
	}

	for (int k = 0; k < ROUTEDATA_MAX; k++)
	{
		ai->routeRot[k][ROUTE_DECISION] = 0.0f;	// ����
		ai->routeRot[k][ROUTE_LENGTH] = 0.0f;	// �x�N�g���̃t�@�W�[�l
		ai->routeRot[k][ROUTE_TIME] = 0.0f;		// �o�ߎ��Ԃ̃t�@�W�[�l
		ai->lenVec[k] = 0.0f;					// �x�N�g���̑傫��
		ai->lapingTime[k] = 0;					// �o�ߎ��Ԃ̏�����
	}

	ai->routeIndex = ROUTE08;	// ���[�g�A�h���X�ԍ��̏�����

	return S_OK;
}

//===========================================================================
// NPC�̈ړ�����
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//==========================================================================
void NonePlayerMove(void)
{
	AI *ai = &AiWk[0];
	PLAYER *player = GetPlayer(0);

	// �v�l���Ԃƌo�H�o�ߎ��Ԃ����Z
	ai->routineCntFrame++;
	RouteLapTimeCnt();

	// 60�t���[���ōčl
	if (ai->routineCntFrame % ROUTE_CNT_TIME == 0)
	{
		// �ړ������̌��ʎZ�o
		NonePlayerFuzzy();
		
		// �ǐ�
		if (ai->decision == ai->cmpPattern[CHASE])
		{
			ai->routineStart = false;
			// ���[�g�f�[�^�ƃv���C���[�̋������v�Z
			DistanceNearVector(P1, player[P1].pos, 0, ROUTEDATA_MAX);
			DistanceNearVector(P2, player[P2].pos, 0, ROUTEDATA_MAX);
			// P2�ɗאڂ��Ă郋�[�g�f�[�^��P1�ɋ߂����[�g�f�[�^���v�Z
			SwitchRoutePlayer(ai[P2].routeIndex, RouteData[ai[P1].routeIndex]);
			ai->routineCntFrame = 0;

#ifdef _DEBUG
			
			SetCollarField(ai->routeIndex, 1.0f, 0.0f, 0.0f);	// �t�B�[���h�F��Ԃ�

#endif
		}
		// ����
		else if (ai->decision == ai->cmpPattern[ESCAPE])
		{
			// ���[�g�f�[�^�ƃv���C���[�̋������v�Z
			DistanceNearVector(P1, player[P1].pos, 0, ROUTEDATA_MAX);
			DistanceNearVector(P2, player[P2].pos, 0, ROUTEDATA_MAX);
			// P2�ɗאڂ��Ă郋�[�g�f�[�^��P1�ɉ������[�g�f�[�^���v�Z
			SwitchRoutePlayer(ai[P2].routeIndex, RouteData[ai[P1].routeIndex]);
			ai->routineCntFrame = 0;
#ifdef _DEBUG

			SetCollarField(ai->routeIndex, 0.0f, 0.0f, 1.0f);	// �t�B�[���h�F���

#endif

		}
		// ����
		else if (ai->decision == ai->cmpPattern[ROUTINE])
		{
			ai->routineStart = true;	// ����̊J�n
			SwitchRoutePlayer(ai->routeIndex, player[P2].pos);	// ���[�g�̐ؑ�
			ai->routineCntFrame = 0;	// ���[�g�čl����
#ifdef _DEBUG

			SetCollarField(ai->routeIndex, 0.0f, 1.0f, 0.0f);	// �t�B�[���h�F��΂�

#endif

		}
	}

	// �ړ�����
	MoveVectorNonePlayer(RouteData[ai->routeIndex], 1.0f);

	PrintDebugProc("���[�g[%d]", ai->routeIndex);

}

//==============================================================================
// �e���[�g�̌o�ߎ���
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
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
// �t�@�W�[�l�ɂ��s�����ʂ̎Z�o����
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//==========================================================================
void NonePlayerFuzzy(void)
{
	AI *ai = &AiWk[0];
	PLAYER *player = GetPlayer(0);
	BALL *ball = GetBall(0);

	// ���{�[����锻��
	ai->chase[PATTERN1] = FuzzyRightDown((float)(float)ball[P2].cnt, MOVE_NPCBALL_CHASE_FUZZY_X1, MOVE_NPCBALL_CHASE_FUZZY_X2);
	ai->escape[PATTERN1] = FuzzyRightUp((float)ball[P2].cnt, MOVE_NPCBALL_ESCAPE_FUZZY_X1, MOVE_NPCBALL_ESCAPE_FUZZY_X2);
	ai->routine[PATTERN1] = FuzzyTrapezoid((float)ball[P2].cnt, MOVE_NPCBALL_ROUTINE_FUZZY_X1, MOVE_NPCBALL_ROUTINE_FUZZY_X2,
																 MOVE_NPCBALL_ROUTINE_FUZZY_X3, MOVE_NPCBALL_ROUTINE_FUZZY_X4);
	
	// ����{�[����锻��
	ai->chase[PATTERN2] = FuzzyRightDown((float)ball[P1].cnt, MOVE_PLAYERBALL_CHASE_FUZZY_X1, MOVE_PLAYERBALL_CHASE_FUZZY_X2);
	ai->escape[PATTERN2] = FuzzyRightUp((float)ball[P1].cnt, MOVE_PLAYERBALL_ESCAPE_FUZZY_X1, MOVE_PLAYERBALL_ESCAPE_FUZZY_X2);
	ai->routine[PATTERN2] = FuzzyTrapezoid((float)ball[P1].cnt, MOVE_PLAYERBALL_ROUTINE_FUZZY_X1, MOVE_PLAYERBALL_ROUTINE_FUZZY_X2,
																MOVE_PLAYERBALL_ROUTINE_FUZZY_X3, MOVE_PLAYERBALL_ROUTINE_FUZZY_X4);
	
	// �����C�t�ɂ�锻��
	if (player[P2].life < PLAYER_LIFE_MAX)
	{
		ai->chase[PATTERN3] = FuzzyRightDown(player[P2].life, MOVE_NPCLIFE_CHASE_FUZZY_X1, MOVE_NPCLIFE_CHASE_FUZZY_X2);
		ai->escape[PATTERN3] = FuzzyRightUp(player[P2].life, MOVE_NPCLIFE_ESCAPE_FUZZY_X1, MOVE_NPCLIFE_ESCAPE_FUZZY_X2);
		ai->routine[PATTERN3] = FuzzyTrapezoid(player[P2].life, MOVE_NPCLIFE_ROUTINE_FUZZY_X1, MOVE_NPCLIFE_ROUTINE_FUZZY_X2,
			MOVE_NPCLIFE_ROUTINE_FUZZY_X3, MOVE_NPCLIFE_ROUTINE_FUZZY_X4);
		// ���ʔ��f
		ai->cmpPattern[CHASE] = ai->chase[PATTERN1] * ai->chase[PATTERN2] * ai->chase[PATTERN3];
		ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1] * ai->escape[PATTERN2] * ai->escape[PATTERN3];
		ai->cmpPattern[ROUTINE] = ai->routine[PATTERN1] * ai->routine[PATTERN2] * ai->routine[PATTERN3];

	}
	// ���胉�C�t���ō��l�̎��͍l�����Ȃ�
	else if (player[P2].life == PLAYER_LIFE_MAX)
	{
		// ���ʔ��f
		ai->cmpPattern[CHASE] = ai->chase[PATTERN1] * ai->chase[PATTERN2];
		ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1] * ai->escape[PATTERN2];
		ai->cmpPattern[ROUTINE] = ai->routine[PATTERN1] * ai->routine[PATTERN2];
	}

	// ���胉�C�t�ɂ�锻��
	if (player[P1].life < PLAYER_LIFE_MAX)
	{
		ai->chase[PATTERN4] = FuzzyRightDown(player[P1].life, MOVE_PLAYERLIFE_CHASE_FUZZY_X1, MOVE_PLAYERLIFE_CHASE_FUZZY_X2);
		ai->escape[PATTERN4] = FuzzyRightUp(player[P1].life, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2);
		ai->routine[PATTERN4] = FuzzyTrapezoid(player[P1].life, MOVE_PLAYERLIFE_ROUTINE_FUZZY_X1, MOVE_PLAYERLIFE_ROUTINE_FUZZY_X2,
			MOVE_PLAYERLIFE_ROUTINE_FUZZY_X3, MOVE_PLAYERLIFE_ROUTINE_FUZZY_X4);

		// ���ʔ��f
		ai->cmpPattern[CHASE] *= ai->chase[PATTERN4];
		ai->cmpPattern[ESCAPE] *= ai->escape[PATTERN4];
		ai->cmpPattern[ROUTINE] *= ai->routine[PATTERN4];
	}

	// �e�s���̃t�@�W�[�l���r
	ai->decision = Or(ai->cmpPattern[CHASE], ai->cmpPattern[ESCAPE]);
	ai->decision = Or(ai->cmpPattern[ROUTINE], ai->decision);
}

//===========================================================================
// �v���C���[�ƍł��߂��o�H�f�[�^�̌v�Z����
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�), D3DXVECTOR3 pos(�v���C���[�̈ʒu)
//		   int cnt(���[�v�J�E���^�[), int cntMax(�J�E���^�[�̍ő�l)
// �߂�l�F�ȁ@��
//==========================================================================
void DistanceNearVector(int index, D3DXVECTOR3 pos, int cnt, int cntMax)
{
	AI *ai = &AiWk[index];
	D3DXVECTOR3 vec01 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[�g�܂ł̃x�N�g��
	float nearVec = 1000.0f;							// �ł��߂����[�g

	for (cnt; cnt < cntMax; cnt++)
	{
		// �x�N�g���̑傫�����v�Z
		vec01 = pos - RouteData[cnt];
		ai->lenVec[cnt] = D3DXVec3Length(&vec01);
		
		// �x�N�g���̑傫�����r
		nearVec = min(ai->lenVec[cnt], nearVec);

		// ��r���ʂ�ۑ�
		if (nearVec == ai->lenVec[cnt])
		{
			ai->routeIndex = cnt;
		}
	}
}

//==========================================================================
// NPC�̏���s���J�ڏ���
// ���@���Fint pattern(����p�^�[���ԍ�), D3DXVECTOR3 vec(�i�s�����x�N�g��)
// �߂�l�F�ȁ@��
//==========================================================================
void SwitchRoutePlayer(int pattern, D3DXVECTOR3 vec)
{
	AI *ai = &AiWk[0];
	switch (pattern)
	{
		case ROUTE00:
			// 5,6,7,8�̃��[�g�f�[�^���r
			DistanceRoutePlayer01(vec, ROUTE05, ROUTEDATA_MAX, NULL);
			break;

		case ROUTE01:
			// 5,7�̃��[�g�f�[�^���r
			DistanceRoutePlayer01(vec, ROUTE05, ROUTE08, 1);
			break;

		case ROUTE02:
			// 6,7�̃��[�g�f�[�^���r
			DistanceRoutePlayer01(vec, ROUTE06, ROUTE08, NULL);
			break;

		case ROUTE03:
			// 5,8�̃��[�g�f�[�^���r
			DistanceRoutePlayer01(vec, ROUTE05, ROUTEDATA_MAX, 2);
			break;

		case ROUTE04:
			// 6,8�̃��[�g�f�[�^���r
			DistanceRoutePlayer01(vec, ROUTE06, ROUTEDATA_MAX, 1);
			break;

		case ROUTE05:
			// 0,1,3�̃��[�g�f�[�^���r
			DistanceRoutePlayer02(vec, ROUTE01, ROUTE04, ROUTE03);
			break;

		case ROUTE06:
			// 0,2,4�̃��[�g�f�[�^���r
			DistanceRoutePlayer02(vec, ROUTE02, ROUTE05, ROUTE04);
			break;

		case ROUTE07:
			// 0,1,2�̃��[�g�f�[�^���r
			DistanceRoutePlayer02(vec, ROUTE01, ROUTE03, ROUTE02);
			break;

		case ROUTE08:
			// 0,3,4�̃��[�g�f�[�^���r
			DistanceRoutePlayer02(vec, ROUTE03, ROUTE05, ROUTE04);
			break;

		default:
			assert("AI�̃��[�g�ؑւŃG���[");
			break;
	}
}

//===========================================================================
// �אڂ��Ă�o�H�f�[�^�ւ̈ړ��I������(�f�[�^ROUTE00�`ROUTE04)
// ���@���FD3DXVECTOR3 vec(����̃��[�g�f�[�^), int cnt(���[�v�J�E���^�[), 
//		   int cntMax(�J�E���^�[�̍ő�l), int raiseCnt(���[�v�J�E���g�̒����l)
// �߂�l�FD3DXVECTOR3�^
//==========================================================================
void DistanceRoutePlayer01(D3DXVECTOR3 vec, int cnt, int cntMax, int raiseCnt)
{
	AI *ai = &AiWk[0];
	PLAYER *player = GetPlayer(P2);
	D3DXVECTOR3 vec01 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[�g�܂ł̃x�N�g��
	float vestRoute01 = 0.0;							// �ł��D�܂������[�g
	float vestRoute02 = 1000.0;							// �ł��D�܂������[�g

	for (cnt; cnt < cntMax; cnt++)
	{
		// �x�N�g���̑傫�����v�Z
		vec01 = vec - RouteData[cnt];
		ai->lenVec[cnt] = D3DXVec3Length(&vec01);

		// ����̏ꍇ
		if (ai->decision == ai->cmpPattern[ROUTINE])
		{
			// �����ƒʉߌo�ߎ���
			ai->routeRot[cnt][ROUTE_LENGTH] = FuzzyRightDown(ai->lenVec[cnt], ROUTE_DISTANCE_FUZZY_X1, ROUTE_DISTANCE_FUZZY_X2);
			ai->routeRot[cnt][ROUTE_TIME] = FuzzyRightUp((float)ai->lapingTime[cnt], ROUTE_TIME_FUZZY_X1, ROUTE_TIME_FUZZY_X2);
			// ����
			ai->routeRot[cnt][ROUTE_DECISION] = ai->routeRot[cnt][ROUTE_LENGTH] * ai->routeRot[cnt][ROUTE_TIME];

			// �D�܂����אڂ������[�g���Z�o
			vestRoute01 = max(ai->routeRot[cnt][ROUTE_DECISION], vestRoute01);
			
			// ���[�g������
			if (vestRoute01 == ai->routeRot[cnt][ROUTE_DECISION])
			{
				ai->routeIndex = cnt;
			}

			// ���[�v�J�E���g�̒���
			cnt = cnt + raiseCnt;
		}
		// �ǐՂ̏ꍇ
		else if (ai->decision == ai->cmpPattern[CHASE])
		{	
			// �D�܂����אڂ������[�g���Z�o
			vestRoute02 = min(ai->lenVec[cnt], vestRoute02);

			// ���[�g������
			if (vestRoute02 == ai->lenVec[cnt])
			{
				ai->routeIndex = cnt;
			}

			// ���[�v�J�E���g�̒���
			cnt = cnt + raiseCnt;
		}
		// �����̏ꍇ
		else if (ai->decision == ai->cmpPattern[ESCAPE])
		{
			// �D�܂����אڂ������[�g���Z�o
			vestRoute01 = max(ai->lenVec[cnt], vestRoute01);

			// ���[�g������
			if (vestRoute01 == ai->lenVec[cnt])
			{
				ai->routeIndex = cnt;
			}

			// ���[�v�J�E���g�̒���
			cnt = cnt + raiseCnt;
		}

	}

	// ���肵�����[�g�̒ʉߌo�ߎ��Ԃ�������
	ai->lapingTime[ai->routeIndex] = 0;
}

//===========================================================================
// �אڂ��Ă�o�H�f�[�^�ւ̈ړ��I������(�f�[�^ROUTE05�`ROUTE08)
// ���@���FD3DXVECTOR3 vec(����̃��[�g�f�[�^), int cnt(���[�v�J�E���^�[), 
//		   int cntMax(�J�E���^�[�̍ő�l), int raiseCnt(���[�v�J�E���g�̒����l)
// �߂�l�FD3DXVECTOR3�^
//==========================================================================
void DistanceRoutePlayer02(D3DXVECTOR3 vec , int cnt, int cntMax, int raiseCnt)
{
	AI *ai = &AiWk[0];
	PLAYER *player = GetPlayer(P2);
	D3DXVECTOR3 vec01 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[�g�܂ł̃x�N�g��
	float vestRoute01 = 0.0f;							// �ł��D�܂������[�g
	float vestRoute02 = 1000.0f;						// �ł��D�܂������[�g
	int arrey = 0;										// �z��Y����

	for (int i = 0; i < cntMax; i++)
	{
		// �x�N�g���̑傫�����v�Z
		vec01 = vec - RouteData[arrey];
		ai->lenVec[arrey] = D3DXVec3Length(&vec01);

		if (ai->decision == ai->cmpPattern[ROUTINE])
		{
			// �����o�[�V�b�v�̍쐬
			ai->routeRot[arrey][ROUTE_LENGTH] = FuzzyRightDown(ai->lenVec[arrey], ROUTE_DISTANCE_FUZZY_X1, ROUTE_DISTANCE_FUZZY_X2);
			ai->routeRot[arrey][ROUTE_TIME] = FuzzyRightUp((float)ai->lapingTime[arrey], ROUTE_TIME_FUZZY_X1, ROUTE_TIME_FUZZY_X2);
			ai->routeRot[arrey][ROUTE_DECISION] = ai->routeRot[arrey][ROUTE_LENGTH] * ai->routeRot[arrey][ROUTE_TIME];

			vestRoute01 = max(ai->routeRot[arrey][ROUTE_DECISION], vestRoute01);

			if (vestRoute01 == ai->routeRot[arrey][ROUTE_DECISION])
			{
				ai->routeIndex = arrey;
			}

			// �z��̒���
			i == 0 ? arrey = cnt : arrey = raiseCnt;
		}
		// ���ʂ��ǐՂ������ꍇ
		else if (ai->decision == ai->cmpPattern[CHASE])
		{
			vestRoute02 = min(ai->lenVec[arrey], vestRoute02);

			if (vestRoute02 == ai->lenVec[arrey])
			{
				ai->routeIndex = arrey;
			}

			// �z��̒���
			i == 0 ? arrey = cnt : arrey = raiseCnt;
		}
		// ���ʂ������������ꍇ
		else if (ai->decision == ai->cmpPattern[ESCAPE])
		{
			vestRoute01 = max(ai->lenVec[arrey], vestRoute01);

			if (vestRoute01 == ai->lenVec[arrey])
			{
				ai->routeIndex = arrey;
			}

			// �z��̒���
			i == 0 ? arrey = cnt : arrey = raiseCnt;
		}

	}

	// �t�@�W�[�l�̍ł��������[�g��
	ai->lapingTime[ai->routeIndex] = 0;	// �o�ߎ��Ԃ�������
}

//==============================================================================
// �i�s�����։�]
// ���@���FD3DXVECTOR3 vecDest(�i�ޕ����̏I�_)
// �߂�l�F�ȁ@��
//==============================================================================
void NonePlayerDest(D3DXVECTOR3 vecDest,int index)
{
	PLAYER *player = GetPlayer(index);
	CAMERA *camera = GetCamera();
	D3DXVECTOR3 vec = vecDest - player->pos;					// �i�s�����x�N�g��
	D3DXVec3Normalize(&vec, &vec);
	D3DXVec3Normalize(&player->frontVec, &player->frontVec);	// ���݂̃��f���̐���

	// �p�x���v�Z
	float vecAngle = atan2f(vec.z, vec.x);
	float frontAngle = atan2f(player->frontVec.z, player->frontVec.x);
	
	if (frontAngle != vecAngle)
	{
		// �p�x�̍������Z�o����]
		float difAngle = (frontAngle - vecAngle);
		player->rot.y += difAngle;
	}
}

//==============================================================================
// �i�s�����ւ̈ړ�����
// ���@���FD3DXVECTOR3 vec(�i�ޕ���), float fuzzy(�t�@�W�[�l)
// �߂�l�F�ȁ@��
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
// NPC�̍U������
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//==========================================================================
void NonePlayerAttack(void)
{
	AI *ai = &AiWk[0];
	PLAYER *player = GetPlayer(0);
	BULLET *bullet = GetBullet(P2);
	BLOCK *block = GetBlock(0);
	BALL *ball = GetBall(0);

	ai->atcCntFrame++;

	// �o���b�g�g�p���`���[�W�s��
	if (!bullet->use[0])
	{
		PlaySound(player[P2].chargeSE, E_DS8_FLAG_LOOP);
		// �ő�l�ɂȂ����ꍇ
		if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
		{
			SetChargeEffect(player[P2].pos, P2);
			bullet->speedIncrease = BULLET_CHARGE_MAX;
		}
		// 10�t���[��
		else if (player->cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
		{
			SetChargeEffect(player[P2].pos, P2);
			bullet->speedIncrease += 0.2f;
		}
	}

	D3DXVECTOR3 rayPos = player[P2].frontVec + player[P2].pos;	// �O���x�N�g���̏I�_
	D3DXVECTOR3 reflectVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���ˌ�̃x�N�g��

	// �O���x�N�g���̔��ˏ���
	for (int i = 0; i < BLOCK_MAX; i++, block++)
	{
		if (!HitCheckBlock(rayPos, player[P2].pos, BLOCK_VTX_MAX))
		{
			reflectVec = ReflectVector(rayPos, player[P2].pos, GetNormal());
			break;
		}
	}

	// 60�t���[���������J����
	if (ai->atcCntFrame > RELOAD_ATTCK_FRAME)
	{
		// �U���J�n����
		if (CheckHitRay(player[P1].pos, player[P2].pos, player[P2].frontVec, 15.0f))
		{
			StopSound(player[P2].chargeSE);
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			ai->atcCntFrame = 0;
		}
		// ���˂��l�������U��
		else if (CheckHitRay(player[P1].pos, player[P2].pos, reflectVec, 10.0f))
		{
			StopSound(player[P2].chargeSE);
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			ai->atcCntFrame = 0;
		}		
		
		for (int i = 0; i < BALL_ONESET_MAX; i++)
		{
			if (!ball->use) continue;
			// �U���J�n����
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
// �^�C�g������NPC�̃t�B�[���h����
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//==============================================================================
void NonePlayerPatrol(int index)
{
	PLAYER *player = GetPlayer(index);
	BLOCK *block = GetBlock(0);
	AI *ai = &AiWk[index];
	D3DXVECTOR3 rayPos = player->frontVec + player->pos;	// �O���x�N�g���̏I�_
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �O���x�N�g���̂ƃu���b�N�̔���
	if (CheckHitBC(player->pos, RouteData[ai->patrolNum], 15.0f, 15.0f))
	{
		ai->patrolNum = SwitchPatrolPattern(ai->patrolNum);
	}

	// ������i�s������
	NonePlayerDest(RouteData[ai->patrolNum], index);

	// �ړI�n�܂ňړ�
	vec = RouteData[ai->patrolNum] - player->pos;
	D3DXVec3Normalize(&vec, &vec);
	player->move.x = vec.x * 5.0f;
	player->move.z = vec.z * 5.0f;
}

//==========================================================================
// �^�C�g������NPC�̏���s���J�ڏ���
// ���@���Fint pattern(����p�^�[���ԍ�)
// �߂�l�F�ȁ@��
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
// AI�̃A�h���X�擾����
// ���@���Fint pno(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�FAI�^�\����
//==========================================================================
AI *GetAi(int pno)
{
	return &AiWk[pno];
}