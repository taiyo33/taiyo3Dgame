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


#define MOVE_NPCLIFE_CHASE_FUZZY_X1			(0.0f)
#define MOVE_NPCLIFE_CHASE_FUZZY_X2			(100.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X2		(80.0f)

#define MOVE_PLAYERLIFE_CHASE_FUZZY_X1		(10.0f)
#define MOVE_PLAYERLIFE_CHASE_FUZZY_X2		(70.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2		(50.0f)

// NPC�̍U���}�N��
#define ATTACK_DISTANCE_ATC_FUZZY_X1		(150.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X2		(250.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X3		(400.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X4		(600.0f)
#define ATTACK_PLAYERLIFE_WAIT_FUZZY_X1		(0.0f)
#define ATTACK_PLAYERLIFE_WAIT_FUZZY_X2		(100.0f)
#define RELOAD_ATTCK_FRAME					(60)		// �U���̉\�܂ł̊Ԋu

// ���[�g����}�N��
#define STOP_ROUTINE_CNT					(2)
#define ROUTE_DISTANCE_FUZZY_X1				(0.0f)
#define ROUTE_DISTANCE_FUZZY_X2				(600.0f)
#define ROUTE_TIME_FUZZY_X1					(0.0f)
#define ROUTE_TIME_FUZZY_X2					(60.0f)
#define ROUTE_CNT_TIME						(90)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int SwitchPatrolPattern(int pattern);
void NonePlayerDest(D3DXVECTOR3 vecDest, int index);
bool StopRoutineNonePlayer(void);
void DistancePlayer(int index);
void MoveVectorNonePlayer(D3DXVECTOR3 vec, float fuzzy);


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
	PATROL,
	WAIT
};

// ����̎��
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

// ����̃p�^�[��
D3DXVECTOR3			RouteData[ROUTEDATA_MAX]{

	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 0.0f, -FIELD_SIZE_Z + 100.0f),	// ����
	D3DXVECTOR3(-FIELD_SIZE_X / 2, 0.0f, -FIELD_SIZE_Z + 50.0f),		// ������
	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 0.0f, FIELD_SIZE_Z - 100.0f),	// ����
	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 0.0f, FIELD_SIZE_Z / 2),		// �㒆��
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 0.0f, FIELD_SIZE_Z - 100.0f),	// �E��
	D3DXVECTOR3(FIELD_SIZE_X / 2, 0.0f, FIELD_SIZE_Z - 100.0f),			// �E����
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 0.0f, -FIELD_SIZE_Z + 100.0f),	// �E��
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 0.0f, -FIELD_SIZE_Z / 2),		// ������
	D3DXVECTOR3(0.0f, 0.0f, 0.0f)										// ����

};

int			CntFrame;
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
	ai->cmpPattern[PATROL] = 0.0f;	// 
	ai->decision = 0.0f;			// ��r���ʂ�������
	ai->cntMemory = 0;				// ���ʋL���z��̓Y������������
	ai[P1].patrolNum = PATROL05;	// ����p�^�[���̔ԍ���������
	ai[P2].patrolNum = PATROL01;	//
	CntFrame = 0;

	for (int i = 0; i < CMP_PATTERN_MAX; i++)
	{
		ai->atc[i] = 0.0f;		// �U���̒l��������
		ai->chase[i] = 0.0f;	// �ǐՂ̒l��������
		ai->escape[i] = 0.0f;	// �����̒l��������
		ai->wait[i] = 0.0f;		// �ҋ@�̒l��������
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

	ai->routeIndex = 0;

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
	
	CntFrame++;
	if (CntFrame % ROUTE_CNT_TIME == 0)
	{
		DistancePlayer(P2);
		CntFrame = 0;
	}

	MoveVectorNonePlayer(RouteData[ai->routeIndex], 1.0f);

	// �v�l���邩
	//if (!StopRoutineNonePlayer())
	//{
	//	CntFrame++;
	//	// 30�t���[���v�l��~
	//	if (CntFrame % 30 == 0)
	//	{
	//		for (int i = 0; i < DECISION_MEMORY_MAX; i++)
	//		{
	//			ai->deciMemory[i] = 0;	//������
	//		}

	//		CntFrame = 0;
	//	}

	//	return;
	//}

	//ai->cntMemory < DECISION_MEMORY_MAX ? ai->cntMemory++ : ai->cntMemory = 0;	// �z��̓Y�������X�V

	//// ����Ƃ̋����ɂ�锻��
	//D3DXVECTOR3 pvpVec = player[P1].pos - player[P2].pos;
	//float vecLength = D3DXVec3Length(&pvpVec);
	//ai->chase[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_CHASE_FUZZY_X1, MOVE_DISTANCE_CHASE_FUZZY_X2,
	//												MOVE_DISTANCE_CHASE_FUZZY_X3, MOVE_DISTANCE_CHASE_FUZZY_X4);
	//ai->escape[PATTERN1] = FuzzyRightDown(vecLength, MOVE_DISTANCE_ESCAPE_FUZZY_X1, MOVE_DISTANCE_ESCAPE_FUZZY_X2);
	//ai->patrol[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_PATROL_FUZZY_X1, MOVE_DISTANCE_PATROL_FUZZY_X2,
	//											     MOVE_DISTANCE_PATROL_FUZZY_X3, MOVE_DISTANCE_PATROL_FUZZY_X4);
	////ai->wait[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2, 
	////												   MOVE_DISTANCE_WAIT_FUZZY_X3, MOVE_DISTANCE_WAIT_FUZZY_X4);

	//// �����C�t�ɂ�锻��
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
	//// ���胉�C�t���ō��l�̎��͍l�����Ȃ�
	//else if (player[P2].life == 0.0f)
	//{
	//	ai->cmpPattern[CHASE] = ai->chase[PATTERN1];
	//	ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1];
	//	ai->cmpPattern[PATROL] = ai->patrol[PATTERN1];
	//	//i->cmpPattern[WAIT] = ai->wait[PATTERN1];
	//}

	//// ���胉�C�t�ɂ�锻��
	//if (player[P1].life < PLAYER_LIFE_MAX)
	//{
	//	ai->chase[PATTERN3] = FuzzyRightDown(player[P1].life, MOVE_PLAYERLIFE_CHASE_FUZZY_X1, MOVE_PLAYERLIFE_CHASE_FUZZY_X2);
	//	ai->escape[PATTERN3] = FuzzyRightUp(player[P1].life, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2);

	//	ai->cmpPattern[CHASE] = ai->chase[PATTERN1] * ai->chase[PATTERN2] * ai->chase[PATTERN3];
	//	ai->cmpPattern[ESCAPE] = ai->escape[PATTERN1] * ai->escape[PATTERN2] * ai->escape[PATTERN3];
	//	ai->cmpPattern[PATROL] = ai->patrol[PATTERN1];
	//	ai->cmpPattern[WAIT] = ai->wait[PATTERN1];
	//}

	//// ��r
	//ai->decision = Or(ai->cmpPattern[CHASE], ai->cmpPattern[ESCAPE]);
	//ai->decision = Or(ai->cmpPattern[PATROL], ai->decision);
	//ai->decision = Or(ai->cmpPattern[WAIT], ai->decision);

	//// ���ʔ��f
	//// �ǐ�
	//if (ai->decision == ai->cmpPattern[CHASE])
	//{
	//	MoveVectorNonePlayer(player[P1].pos, ai->decision);
	//	ai->deciMemory[ai->cntMemory] = CHASE;
	//}
	//// ����
	//else if (ai->decision == ai->cmpPattern[ESCAPE])
	//{
	//	MoveVectorNonePlayer(player[P2].pos, ai->decision);
	//	ai->deciMemory[ai->cntMemory] = ESCAPE;
	//}
	//// ����
	//else if (ai->decision == ai->cmpPattern[PATROL])
	//{
	//	NonePlayerPatrol(P2);
	//	ai->deciMemory[ai->cntMemory] = PATROL;
	//}
	//// �ҋ@
	//else if (ai->decision == ai->cmpPattern[WAIT])
	//{
	//	return;
	//}
}

//===========================================================================
// �v���C���[�ƌo�H�f�[�^�Ƃ̋����v�Z����
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�FD3DXVECTOR3�^�@�ł��߂��o�H�̃x�N�g����Ԃ�
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

		// �x�N�g���̑傫�����v�Z
		vec = player->pos - RouteData[i];
		ai->lenVec[i] = D3DXVec3Length(&vec);

		// �����o�[�V�b�v�̍쐬
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

	// �t�@�W�[�l�̍ł��������[�g��
	ai->lapingTime[ai->routeIndex] = 0;
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
// �i�s�����̌��菈��
// ���@���FD3DXVECTOR3 vec(�i�ޕ���)
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

//==============================================================================
// NPC�̃t�B�[���h����
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
	if (!HitCheckBlock(rayPos, player->pos, BLOCK_NUM_WALL))
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
// NPC�̏���s���J�ڏ���
// ���@���Fint pattern(����p�^�[���ԍ�)
// �߂�l�F�ȁ@��
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

	// �o���b�g��P�b�ԏ������J����
	if (CntFrame % RELOAD_ATTCK_FRAME == 0)
	{
		// �U���J�n����
		if (CheckHitRay(player[P1].pos, player[P2].pos, player[P2].frontVec, 15.0f))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			CntFrame = 0;
		}
		// ���˂��l�������U��
		else if (CheckHitRay(player[P1].pos, player[P2].pos, reflectVec, 15.0f))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			CntFrame = 0;
		}
	}
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
