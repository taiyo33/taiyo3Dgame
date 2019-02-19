//=============================================================================
//
// AI���� [ai.cpp]
// Author : GP11A_341_22_�c�����z 
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
#include "child.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// NPC�̈ړ��}�N��
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


#define MOVE_NPCLIFE_CHASE_FUZZY_X1			(20.0f)
#define MOVE_NPCLIFE_CHASE_FUZZY_X2			(80.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X2		(80.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X1		(10.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X2		(30.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X3		(70.0f)
#define MOVE_NPCLIFE_ROUTINE_FUZZY_X4		(90.0f)

#define MOVE_PLAYERLIFE_CHASE_FUZZY_X1		(10.0f)
#define MOVE_PLAYERLIFE_CHASE_FUZZY_X2		(70.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2		(50.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X1	(10.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X2	(30.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X3	(80.0f)
#define MOVE_PLAYERLIFE_ROUTINE_FUZZY_X4	(100.0f)

#define MOVE_NPCCHILD_ROUTINE_FUZZY_X1		(30.0f)
#define MOVE_NPCCHILD_ROUTINE_FUZZY_X2		(50.0f)
#define MOVE_NPCCHILD_ROUTINE_FUZZY_X3		(70.0f)
#define MOVE_NPCCHILD_ROUTINE_FUZZY_X4		(100.0f)
#define MOVE_NPCCHILD_CHASE_FUZZY_X1		(50.0f)
#define MOVE_NPCCHILD_CHASE_FUZZY_X2		(75.0f)
#define MOVE_NPCCHILD_ESCAPE_FUZZY_X1		(50.0f)
#define MOVE_NPCCHILD_ESCAPE_FUZZY_X2		(80.0f)

#define MOVE_PLAYERCHILD_ROUTINE_FUZZY_X1	(0.0f)
#define MOVE_PLAYERCHILD_ROUTINE_FUZZY_X2	(10.0f)
#define MOVE_PLAYERCHILD_ROUTINE_FUZZY_X3	(70.0f)
#define MOVE_PLAYERCHILD_ROUTINE_FUZZY_X4	(100.0f)
#define MOVE_PLAYERCHILD_CHASE_FUZZY_X1		(20.0f)
#define MOVE_PLAYERCHILD_CHASE_FUZZY_X2		(75.0f)
#define MOVE_PLAYERCHILD_ESCAPE_FUZZY_X1	(0.0f)
#define MOVE_PLAYERCHILD_ESCAPE_FUZZY_X2	(100.0f)

#define RELOAD_ATTCK_FRAME					(60)		// �U���̉\�܂ł̊Ԋu

// ���[�g����}�N��
#define STOP_ROUTINE_CNT					(2)
#define ROUTE_DISTANCE_FUZZY_X1				(0.0f)
#define ROUTE_DISTANCE_FUZZY_X2				(500.0f)
#define ROUTE_TIME_FUZZY_X1					(0.0f)
#define ROUTE_TIME_FUZZY_X2					(30.0f)
#define ROUTE_CNT_TIME						(70)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int SwitchPatrolPattern(int pattern);
void NonePlayerDest(D3DXVECTOR3 vecDest, int index);
bool StopRoutineNonePlayer(void);
void DistanceRoutePlayer01(D3DXVECTOR3 vec, int cnt, int cntMax, int raiseCnt);
void DistanceRoutePlayer02(D3DXVECTOR3 vec, int cnt, int cntMax, int raiseCnt);
void SwitchRoutePlayer(int pattern, D3DXVECTOR3 vec);
void DistanceNearVector(int index, D3DXVECTOR3 vec, int cnt, int cntMax, int raiseCnt);

void MoveVectorNonePlayer(D3DXVECTOR3 vec, float fuzzy);
void RouteLapTimeCnt(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

// �t�@�W�[�̂P�v�f��
enum {
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4
};
// ��r���v�̎��
enum {
	CHASE,
	ESCAPE,
	ROUTINE,
};

// ����̎��
enum {
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

enum {
	ROUTE_DECISION,
	ROUTE_LENGTH,
	ROUTE_TIME
};

// ����̃p�^�[��
D3DXVECTOR3			RouteData[ROUTEDATA_MAX]{

	D3DXVECTOR3(0.0f, 0.0f, 0.0f),										// ����
	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 10.0f, FIELD_SIZE_Z - 80.0f),	// ����
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 10.0f, FIELD_SIZE_Z - 80.0f),	// �E��
	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 10.0f, -FIELD_SIZE_Z + 80.0f),	// ����
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 10.0f, -FIELD_SIZE_Z + 80.0f),	// �E��
	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 10.0f, 0.0f),	// ������
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 10.0f, 0.0f),	// �E����
	D3DXVECTOR3(0.0f, 10.0f, FIELD_SIZE_Z - 80.0f),		// �㒆��
	D3DXVECTOR3(0.0f, 10.0f, -FIELD_SIZE_Z + 80.0f),	// ������
};

AI			aiWk[PLAYER_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitAi(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	AI *ai = &aiWk[0];

	ai->cmpPattern[CHASE] = 0.0f;	// ��r�̍��v�l��������
	ai->cmpPattern[ESCAPE] = 0.0f;	//
	ai->cmpPattern[ROUTINE] = 0.0f;	// 
	ai->decision = 0.0f;			// ��r���ʂ�������
	ai->cntMemory = 0;				// ���ʋL���z��̓Y������������
	ai[P1].patrolNum = ROUTE01;		// ����p�^�[���̔ԍ���������
	ai[P2].patrolNum = ROUTE04;		//
	ai->routineCntFrame = 0;		// �v�l�Ԋu
	ai->atcCntFrame = 0;			// �U���̊Ԋu
	ai->routineStart = true;		// ���񂵂Ă��邩

	for (int i = 0; i < CMP_PATTERN_MAX; i++)
	{
		ai->chase[i] = 0.0f;	// �ǐՂ̒l��������
		ai->escape[i] = 0.0f;	// �����̒l��������
		ai->routine[i] = 0.0f;		// �ҋ@�̒l��������
	}

	for (int j = 0; j < DECISION_MEMORY_MAX; j++)
	{
		ai->deciMemory[j] = j;	// ���ʋL����������
	}

	for (int k = 0; k < ROUTEDATA_MAX; k++)
	{
		ai->routeRot[k][ROUTE_DECISION] = 0.0f;	// ����
		ai->routeRot[k][ROUTE_LENGTH] = 0.0f;	// �x�N�g���̃t�@�W�[�l
		ai->routeRot[k][ROUTE_TIME] = 0.0f;		// �o�ߎ��Ԃ̃t�@�W�[�l
		ai->lenVec[k] = 0.0f;					// �x�N�g���̑傫��
		ai->lapingTime[k] = 0;					// �o�ߎ��Ԃ̏�����
	}

	ai->routeIndex = ROUTE08;

	return S_OK;
}

//===========================================================================
// NPC�̈ړ�����
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//==========================================================================
void NonePlayerMove(void)
{
	AI *ai = &aiWk[0];
	PLAYER *player = GetPlayer(0);
	CHILD *child = GetChild(0);
	ai->routineCntFrame++;
	RouteLapTimeCnt();

	if (ai->routineCntFrame % ROUTE_CNT_TIME == 0)
	{
		ai->cntMemory < DECISION_MEMORY_MAX ? ai->cntMemory++ : ai->cntMemory = 0;	// �z��̓Y�������X�V

		// ���{�[����锻��
		ai->chase[PATTERN1] = FuzzyRightDown((float)(float)child[P2].cnt, MOVE_NPCCHILD_CHASE_FUZZY_X1, MOVE_NPCCHILD_CHASE_FUZZY_X2);
		ai->escape[PATTERN1] = FuzzyRightUp((float)child[P2].cnt, MOVE_NPCCHILD_ESCAPE_FUZZY_X1, MOVE_NPCCHILD_ESCAPE_FUZZY_X2);
		ai->routine[PATTERN1] = FuzzyTrapezoid((float)child[P2].cnt, MOVE_NPCCHILD_ROUTINE_FUZZY_X1, MOVE_NPCCHILD_ROUTINE_FUZZY_X2,
			MOVE_NPCCHILD_ROUTINE_FUZZY_X3, MOVE_NPCCHILD_ROUTINE_FUZZY_X4);
		// ����{�[����锻��
		ai->chase[PATTERN2] = FuzzyRightDown((float)child[P1].cnt, MOVE_PLAYERCHILD_CHASE_FUZZY_X1, MOVE_PLAYERCHILD_CHASE_FUZZY_X2);
		ai->escape[PATTERN2] = FuzzyRightUp((float)child[P1].cnt, MOVE_PLAYERCHILD_ESCAPE_FUZZY_X1, MOVE_PLAYERCHILD_ESCAPE_FUZZY_X2);
		ai->routine[PATTERN2] = FuzzyTrapezoid((float)child[P1].cnt, MOVE_PLAYERCHILD_ROUTINE_FUZZY_X1, MOVE_PLAYERCHILD_ROUTINE_FUZZY_X2,
			MOVE_PLAYERCHILD_ROUTINE_FUZZY_X3, MOVE_PLAYERCHILD_ROUTINE_FUZZY_X4);

		// �����C�t�ɂ�锻��
		if (player[P2].life < PLAYER_LIFE_MAX)
		{
			ai->chase[PATTERN3] = FuzzyRightDown(player[P2].life, MOVE_NPCLIFE_CHASE_FUZZY_X1, MOVE_NPCLIFE_CHASE_FUZZY_X2);
			ai->escape[PATTERN3] = FuzzyRightUp(player[P2].life, MOVE_NPCLIFE_ESCAPE_FUZZY_X1, MOVE_NPCLIFE_ESCAPE_FUZZY_X2);
			ai->routine[PATTERN3] = FuzzyTrapezoid(player[P2].life, MOVE_NPCLIFE_ROUTINE_FUZZY_X1, MOVE_NPCLIFE_ROUTINE_FUZZY_X2,
				MOVE_NPCLIFE_ROUTINE_FUZZY_X3, MOVE_NPCLIFE_ROUTINE_FUZZY_X4);
			// ���ʔ��f
			ai->cmpPattern[CHASE] *= ai->chase[PATTERN3];
			ai->cmpPattern[ESCAPE] *= ai->escape[PATTERN3];
			ai->cmpPattern[ROUTINE] *= ai->routine[PATTERN3];

		}
		// ���胉�C�t���ō��l�̎��͍l�����Ȃ�
		else if (player[P2].life == 0.0f)
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

		// ��r
		ai->decision = Or(ai->cmpPattern[CHASE], ai->cmpPattern[ESCAPE]);
		ai->decision = Or(ai->cmpPattern[ROUTINE], ai->decision);

		// �����������J��Ԃ��Ă���ꍇ
		if (!StopRoutineNonePlayer())
		{
			for (int i = 0; i < DECISION_MEMORY_MAX; i++)
			{
				ai->deciMemory[i] = 0;	//������
			}

			// �ړ��֑J��
			ai->decision = ai->cmpPattern[ROUTINE];
			ai->routineCntFrame = 0;
		}

		// �ǐ�
		if (ai->decision == ai->cmpPattern[CHASE])
		{
			ai->routineStart = false;
			// ���[�g�f�[�^�ƃv���C���[�̋������v�Z
			DistanceNearVector(P1, player[P1].pos, 0, ROUTEDATA_MAX, NULL);
			DistanceNearVector(P2, player[P2].pos, 0, ROUTEDATA_MAX, NULL);
			// P2�ɗאڂ��Ă郋�[�g�f�[�^��P1�ɋ߂����[�g�f�[�^���v�Z
			SwitchRoutePlayer(ai[P2].routeIndex, RouteData[ai[P1].routeIndex]);
			ai->routineCntFrame = 0;
		}
		// ����
		else if (ai->decision == ai->cmpPattern[ESCAPE])
		{
			// ���[�g�f�[�^�ƃv���C���[�̋������v�Z
			DistanceNearVector(P1, player[P1].pos, 0, ROUTEDATA_MAX, NULL);
			DistanceNearVector(P2, player[P2].pos, 0, ROUTEDATA_MAX, NULL);
			// P2�ɗאڂ��Ă郋�[�g�f�[�^��P1�ɋ߂����[�g�f�[�^���v�Z
			SwitchRoutePlayer(ai[P2].routeIndex, RouteData[ai[P1].routeIndex]);
			ai->deciMemory[ai->cntMemory] = ESCAPE;
		}
		else if (ai->decision == ai->cmpPattern[ROUTINE])
		{
			ai->routineStart = true;
			SwitchRoutePlayer(ai->routeIndex, player[P2].pos);
			ai->routineCntFrame = 0;
		}
	}

	MoveVectorNonePlayer(RouteData[ai->routeIndex], 1.0f);

}

//===========================================================================
// �אڂ��Ă�o�H�f�[�^�ւ̈ړ��I������(�f�[�^ROUTE00�`ROUTE04)
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�FD3DXVECTOR3�^�@�ł��߂��o�H�̃x�N�g����Ԃ�
//==========================================================================
void DistanceRoutePlayer01(D3DXVECTOR3 vec, int cnt, int cntMax, int raiseCnt)
{
	AI *ai = &aiWk[0];
	PLAYER *player = GetPlayer(P2);
	D3DXVECTOR3 vec01 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[�g�܂ł̃x�N�g��
	float vestRoute01 = 0.0;								// �ł��D�܂������[�g
	float vestRoute02 = 1000.0;								// �ł��D�܂������[�g

	for (cnt; cnt < cntMax; cnt++)
	{
		// �x�N�g���̑傫�����v�Z
		vec01 = vec - RouteData[cnt];
		ai->lenVec[cnt] = D3DXVec3Length(&vec01);

		if (ai->decision == ai->cmpPattern[ROUTINE])
		{
			// �����o�[�V�b�v�̍쐬
			ai->routeRot[cnt][ROUTE_LENGTH] = FuzzyRightDown(ai->lenVec[cnt], ROUTE_DISTANCE_FUZZY_X1, ROUTE_DISTANCE_FUZZY_X2);

			ai->routeRot[cnt][ROUTE_TIME] = FuzzyRightUp((float)ai->lapingTime[cnt], ROUTE_TIME_FUZZY_X1, ROUTE_TIME_FUZZY_X2);
			ai->routeRot[cnt][ROUTE_DECISION] = ai->routeRot[cnt][ROUTE_LENGTH] * ai->routeRot[cnt][ROUTE_TIME];

			vestRoute01 = max(ai->routeRot[cnt][ROUTE_DECISION], vestRoute01);

			if (vestRoute01 == ai->routeRot[cnt][ROUTE_DECISION])
			{
				ai->routeIndex = cnt;
			}

			cnt = cnt + raiseCnt;
		}
		else if (ai->decision == ai->cmpPattern[CHASE])
		{
			vestRoute02 = min(ai->lenVec[cnt], vestRoute02);

			if (vestRoute02 == ai->lenVec[cnt])
			{
				ai->routeIndex = cnt;
			}

			cnt = cnt + raiseCnt;
		}
		else if (ai->decision == ai->cmpPattern[ESCAPE])
		{
			vestRoute02 = max(ai->lenVec[cnt], vestRoute02);

			if (vestRoute02 == ai->lenVec[cnt])
			{
				ai->routeIndex = cnt;
			}

			cnt = cnt + raiseCnt;
		}

	}

	// �t�@�W�[�l�̍ł��������[�g��
	ai->lapingTime[ai->routeIndex] = 0;
}

//===========================================================================
// �v���C���[�ƌo�H�f�[�^�Ƃ̋����v�Z����(�f�[�^ROUTE05�`ROUTE08)
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�FD3DXVECTOR3�^�@�ł��߂��o�H�̃x�N�g����Ԃ�
//==========================================================================
void DistanceRoutePlayer02(D3DXVECTOR3 vec , int cnt, int cntMax, int raiseCnt)
{
	AI *ai = &aiWk[0];
	PLAYER *player = GetPlayer(P2);
	D3DXVECTOR3 vec01 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[�g�܂ł̃x�N�g��
	float vestRoute01 = 0.0f;								// �ł��D�܂������[�g
	float vestRoute02 = 1000.0f;								// �ł��D�܂������[�g
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

			i == 0 ? arrey = cnt : arrey = raiseCnt;
		}
		else if (ai->decision == ai->cmpPattern[CHASE])
		{
			vestRoute02 = min(ai->lenVec[arrey], vestRoute02);

			if (vestRoute02 == ai->lenVec[arrey])
			{
				ai->routeIndex = arrey;
			}

			i == 0 ? arrey = cnt : arrey = raiseCnt;
		}
		else if (ai->decision == ai->cmpPattern[ESCAPE])
		{
			vestRoute02 = max(ai->lenVec[arrey], vestRoute02);

			if (vestRoute02 == ai->lenVec[arrey])
			{
				ai->routeIndex = arrey;
			}

			i == 0 ? arrey = cnt : arrey = raiseCnt;
		}

	}

	// �t�@�W�[�l�̍ł��������[�g��
	ai->lapingTime[ai->routeIndex] = 0;
}


//===========================================================================
// �v���C���[�ƍł��߂��o�H�f�[�^�̌v�Z����
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void DistanceNearVector(int index, D3DXVECTOR3 vec, int cnt, int cntMax, int raiseCnt)
{
	AI *ai = &aiWk[index];
	D3DXVECTOR3 vec01 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[�g�܂ł̃x�N�g��
	float nearVec = 1000.0f;								// �ł��D�܂������[�g

	for (cnt; cnt < cntMax; cnt++)
	{
		// �x�N�g���̑傫�����v�Z
		vec01 = vec - RouteData[cnt];
		ai->lenVec[cnt] = D3DXVec3Length(&vec01);

		nearVec = min(ai->lenVec[cnt], nearVec);

		if (nearVec == ai->lenVec[cnt])
		{
			ai->routeIndex = cnt;
		}
	}
}

//===========================================================================
// �v���C���[�̋���o�H�Ɨאڂ���o�H�f�[�^�̌v�Z����
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void RouteNearVector(int index, D3DXVECTOR3 vec)
{
	AI *ai = &aiWk[index];
	D3DXVECTOR3 vec01 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[�g�܂ł̃x�N�g��
	float nearVec = 0.0;								// �ł��D�܂������[�g

	for (int i = 0; i < ROUTEDATA_MAX; i++)
	{
		// �x�N�g���̑傫�����v�Z
		vec01 = vec - RouteData[i];
		ai->lenVec[i] = D3DXVec3Length(&vec01);

		nearVec = min(ai->lenVec[i], nearVec);

		if (nearVec == ai->lenVec[i])
		{
			ai->routeIndex = i;
		}
	}
}

//==========================================================================
// NPC�̏���s���J�ڏ���
// ���@���Fint pattern(����p�^�[���ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void SwitchRoutePlayer(int pattern, D3DXVECTOR3 vec)
{
	AI *ai = &aiWk[0];
	switch (pattern)
	{
		case ROUTE00:
			DistanceRoutePlayer01(vec, ROUTE05, ROUTEDATA_MAX, NULL);
			break;

		case ROUTE01: 
			DistanceRoutePlayer01(vec, ROUTE05, ROUTE06, NULL);
			break;

		case ROUTE02: 
			DistanceRoutePlayer01(vec, ROUTE06, ROUTE07, NULL);
			break;

		case ROUTE03:		
			DistanceRoutePlayer01(vec, ROUTE05, ROUTEDATA_MAX, 2);
			break;

		case ROUTE04: 		
			DistanceRoutePlayer01(vec, ROUTE06, ROUTEDATA_MAX, 1);
			break;

		case ROUTE05: 			
			DistanceRoutePlayer02(vec, ROUTE01, ROUTE04, ROUTE03);
			break;

		case ROUTE06:
			DistanceRoutePlayer02(vec, ROUTE01, ROUTE03, ROUTE02);
			break;

		case ROUTE07:
			DistanceRoutePlayer02(vec, ROUTE02, ROUTE05, ROUTE04);
			break;

		case ROUTE08:
			DistanceRoutePlayer02(vec, ROUTE03, ROUTE05, ROUTE04);
			break;

		default:	
			assert("AI�̃��[�g�ؑւŃG���[");
			break;
		}
}

//==============================================================================
// �e���[�g�̌o�ߎ���
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//==============================================================================
void RouteLapTimeCnt(void)
{
	AI *ai = &aiWk[0];

	if (ai->routineCntFrame % 60 == 0)
	{
		for (int i = 0; i < ROUTEDATA_MAX; i++)
		{
			ai->lapingTime[i]++;
		}
	}
}

//==============================================================================
// NPC�̎v�l�ؑ�
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//==============================================================================
bool StopRoutineNonePlayer(void)
{
	AI *ai = &aiWk[0];
	int cnt = 0;

	// �ߋ�4��̌��ʂƔ�r
	for (int i = 0; i < DECISION_MEMORY_MAX; i++)
	{
		// �قȂ鏈���Ȃ�J�E���g�C���N�������g
		if (ai->deciMemory[i] != ai->deciMemory[i + 1])
		{
			cnt++;
		}
	}

	// �قȂ錋�ʂ��Q��J��Ԃ��Ă���Ȃ�v�l���Ȃ�
	if (cnt > STOP_ROUTINE_CNT) return false;

	return true;
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
	D3DXVECTOR3 vec = vecDest - player->pos;	// �i�s�����x�N�g��
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
	AI *ai = &aiWk[0];
	PLAYER *player = GetPlayer(0);
	BULLET *bullet = GetBullet(P2);
	BLOCK *block = GetBlock(0);
	CHILD *child = GetChild(0);

	ai->atcCntFrame++;

	// �o���b�g�g�p���`���[�W�s��
	if (!bullet->use[P2])
	{
		// �ő�l�ɂȂ����ꍇ
		if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
		{
			SetChargeEffect(player[P2].pos, player[P2].rot, 0, P2);
			bullet->speedIncrease = BULLET_CHARGE_MAX;
		}
		// 10�t���[��
		else if (player->cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
		{
			SetChargeEffect(player[P2].pos, player[P2].rot, 0, P2);
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
		if (CheckHitRay(player[P1].pos, player[P2].pos, player[P2].frontVec, 25.0f))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			ai->atcCntFrame = 0;
		}
		// ���˂��l�������U��
		else if (CheckHitRay(player[P1].pos, player[P2].pos, reflectVec, 15.0f))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			ai->atcCntFrame = 0;
		}		
		
		for (int i = 0; i < CHILD_ONESET_MAX; i++)
		{
			if (!child->use) continue;
			// �U���J�n����
			if (CheckHitRay(child->pos[i], player[P2].pos, player[P2].frontVec, 15.0f))
			{
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
	AI *ai = &aiWk[index];
	D3DXVECTOR3 rayPos = player->frontVec + player->pos;	// �O���x�N�g���̏I�_
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �O���x�N�g���̂ƃu���b�N�̔���
	//if (!HitCheckBlock(rayPos, player->pos, BLOCK_NUM_WALL))
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


//*****************************************************************************
// �t�@�W�[�����o�[�V�b�v�E�オ��^
// ���@���Ffloat val(�Q�l�ɂ��鐔�l), float x0(�͈͂̍ŏ��l), float x1(�͈͂̍ő�l)
// �߂�l�Ffloat�^�@0.0�`1.0�̐��l
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
		// ������v�Z
		float denom = x1 - x0;
		return (val / denom) - (x0 / denom);
	}
}

//*****************************************************************************
// �t�@�W�[�����o�[�V�b�v�E������^
// ���@���Ffloat val(�Q�l�ɂ��鐔�l), float x0(�͈͂̍ŏ��l), float x1(�͈͂̍ő�l)
// �߂�l�Ffloat�^�@0.0�`1.0�̐��l
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
// �t�@�W�[�����o�[�V�b�v�O�p�^
// ���@���Ffloat val(�Q�l�ɂ��鐔�l), float x0(�͈͂̍ŏ��l), float x1(�Ԃ�l�̍ő�l)
//		   float x2()
// �߂�l�Ffloat�^�@0.0�`1.0�̐��l
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
// �t�@�W�[�����o�[�V�b�v��`�^
// ���@���Ffloat val(�Q�l�ɂ��鐔�l), float x0(�͈͂̍ŏ��l), float x1(�͈͂̍ő�l)
//		   float x2(), float x3()
// �߂�l�Ffloat�^�@0.0�`1.0�̐��l
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
// �t�@�W�[AND���Z�q
// ���@���Ffloat a(��r�l1), float b(��r�l2)
// �߂�l�Ffloat�^�@���������̒l��Ԃ�
//*****************************************************************************
float And(float a, float b)
{
	return min(a, b);
}

//*****************************************************************************
// �t�@�W�[OR���Z�q
// ���@���Ffloat a(��r�l1), float b(��r�l2)
// �߂�l�Ffloat�^�@�傫�����̒l��Ԃ�
//*****************************************************************************

float Or(float a, float b)
{
	return max(a, b);
}

//*****************************************************************************
// �t�@�W�[Not���Z�q
// ���@���Ffloat a(��r�l1), float b(��r�l2)
// �߂�l�Ffloat�^�@���]�����l��Ԃ�
//*****************************************************************************
float Not(float a, float b)
{
	return 1.0f - a;
}
