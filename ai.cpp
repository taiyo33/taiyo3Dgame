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
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE_PATTERN_MAX					(4) 

// NPC�̈ړ��}�N��
#define MOVE_DISTANCE_CHASE_FUZZY_X1		(150.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X2		(250.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X3		(400.0f)
#define MOVE_DISTANCE_CHASE_FUZZY_X4		(600.0f)
#define MOVE_DISTANCE_ESCAPE_FUZZY_X1		(150.0f)
#define MOVE_DISTANCE_ESCAPE_FUZZY_X2		(500.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X1			(100.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X2			(200.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X3			(250.0f)
#define MOVE_DISTANCE_WAIT_FUZZY_X4			(300.0f)

#define MOVE_NPCLIFE_CHASE_FUZZY_X1			(0.0f)
#define MOVE_NPCLIFE_CHASE_FUZZY_X2			(80.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X1		(20.0f)
#define MOVE_NPCLIFE_ESCAPE_FUZZY_X2		(80.0f)

#define MOVE_PLAYERLIFE_CHASE_FUZZY_X1		(0.0f)
#define MOVE_PLAYERLIFE_CHASE_FUZZY_X2		(100.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1		(0.0f)
#define MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2		(100.0f)

// NPC�̍U���}�N��
#define ATTACK_DISTANCE_ATC_FUZZY_X1		(150.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X2		(250.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X3		(400.0f)
#define ATTACK_DISTANCE_ATC_FUZZY_X4		(600.0f)
#define ATTACK_PLAYERLIFE_WAIT_FUZZY_X1		(0.0f)
#define ATTACK_PLAYERLIFE_WAIT_FUZZY_X2		(100.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int SwitchPatrolPattern(int pattern);
void NonePlayerDest(D3DXVECTOR3 vecDest);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
enum {
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4
};

D3DXVECTOR3			MovePattern[MOVE_PATTERN_MAX]{

	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 0.0f, -FIELD_SIZE_Z + 100.0f),
	D3DXVECTOR3(-FIELD_SIZE_X + 100.0f, 0.0f, FIELD_SIZE_Z - 100.0f), 
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 0.0f, FIELD_SIZE_Z - 100.0f),
	D3DXVECTOR3(FIELD_SIZE_X - 100.0f, 0.0f, -FIELD_SIZE_Z + 100.0f),

};

AI			aiWk;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitAi(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	AI *ai = &aiWk;

	ai->patrolNum = PATTERN1;			// ����p�^�[���̔ԍ���������
	ai->patternA = 0.0f;				// ��r�̍��v�l��������
	ai->patternB = 0.0f;				//
	ai->patternC = 0.0f;				// 
	ai->decision = 0.0f;				// ��r���ʂ�������

	for (int i = 0; i < MOVE_CMP_MAX; i++)
	{
		ai->atc[i] = 0.0f;		// �U���̒l��������
		ai->chase[i] = 0.0f;	// �ǐՂ̒l��������
		ai->escape[i] = 0.0f;	// �����̒l��������
		ai->wait[i] = 0.0f;		// �ҋ@�̒l��������
	}

	return S_OK;
}


//===========================================================================
// NPC�̈ړ�����
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//==========================================================================
void NonePlayerMove(void)
{
	AI *ai = &aiWk;
	PLAYER *player = GetPlayer(0);

	NonePlayerDest(player[P1].pos);

	// ����Ƃ̋����ɂ�锻��
	D3DXVECTOR3 pvpVec = player[P2].pos - player[P1].pos;
	float vecLength = D3DXVec3Length(&pvpVec);
	ai->chase[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_CHASE_FUZZY_X1, MOVE_DISTANCE_CHASE_FUZZY_X2,
													MOVE_DISTANCE_CHASE_FUZZY_X3, MOVE_DISTANCE_CHASE_FUZZY_X4);
	ai->escape[PATTERN1] = FuzzyRightDown(vecLength, MOVE_DISTANCE_ESCAPE_FUZZY_X1, MOVE_DISTANCE_ESCAPE_FUZZY_X2);
	ai->wait[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2, 
												   MOVE_DISTANCE_WAIT_FUZZY_X3, MOVE_DISTANCE_WAIT_FUZZY_X4);

	// �����C�t�ɂ�锻��
	if (player[P2].life > 0.0f)
	{
		ai->chase[PATTERN2] = FuzzyRightDown(player[P2].life, MOVE_NPCLIFE_CHASE_FUZZY_X1, MOVE_NPCLIFE_CHASE_FUZZY_X2);
		ai->escape[PATTERN2] = FuzzyRightUp(player[P2].life, MOVE_NPCLIFE_ESCAPE_FUZZY_X1, MOVE_NPCLIFE_ESCAPE_FUZZY_X2);
		
		ai->patternA = ai->chase[PATTERN1] * ai->chase[PATTERN2];
		ai->patternB = ai->escape[PATTERN1] * ai->escape[PATTERN2];
		ai->patternC = ai->wait[PATTERN1];
	}
	// ���胉�C�t���ō��l�̎��͍l�����Ȃ�
	else if (player[P2].life == 0.0f)
	{
		ai->patternA = ai->chase[PATTERN1];
		ai->patternB = ai->escape[PATTERN1];
		ai->patternC = ai->wait[PATTERN1];
	}

	// ���胉�C�t�ɂ�锻��
	if (player[P1].life < PLAYER_LIFE_MAX)
	{
		ai->chase[PATTERN3] = FuzzyRightDown(player[P1].life, MOVE_PLAYERLIFE_CHASE_FUZZY_X1, MOVE_PLAYERLIFE_CHASE_FUZZY_X2);
		ai->escape[PATTERN3] = FuzzyRightUp(player[P1].life, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2);

		ai->patternA = ai->chase[PATTERN1] * ai->chase[PATTERN2] * ai->chase[PATTERN3];
		ai->patternB = ai->escape[PATTERN1] * ai->escape[PATTERN2] * ai->escape[PATTERN3];
		ai->patternC = ai->wait[PATTERN1];
	}

	// ��r
	ai->decision = Or(ai->patternA, ai->patternB);
	ai->decision = Or(ai->patternC, ai->decision);

	// ���ʔ��f
	// �ҋ@
	if (ai->decision == ai->patternC)
	{
		return;
	}
	// �ǐ�
	else if (ai->decision == ai->patternA)
	{
		D3DXVECTOR3 vec = player[P1].pos - player[P2].pos;
		D3DXVec3Normalize(&vec, &vec);
		D3DXVec3Normalize(&player[P2].frontVec, &player[P2].frontVec);

		player[P2].speed = 5.0f * ai->patternA;

		player[P2].move.x = vec.x * player[P2].speed;
		player[P2].move.z = vec.z * player[P2].speed;
	}
	// ����
	else if (ai->decision == ai->patternB)
	{
		D3DXVECTOR3 vec = player[P2].pos - player[P1].pos;
		D3DXVec3Normalize(&vec, &vec);
		player[P2].speed = 5.0f * ai->patternB;
		player[P2].move.x = vec.x * player[P2].speed;
		player[P2].move.z = vec.z * player[P2].speed;
	}

}

//==============================================================================
// NPC���v���C���[�̕���������
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//==============================================================================
void NonePlayerDest(D3DXVECTOR3 vecDest)
{
	PLAYER *player = GetPlayer(P2);
	CAMERA *camera = GetCamera();
	D3DXVECTOR3 vec = vecDest - player->pos;
	D3DXVec3Normalize(&vec, &vec);
	D3DXVec3Normalize(&player->frontVec, &player->frontVec);

	float vecAngle = atan2f(vec.z, vec.x);
	float frontAngle = atan2f(player->frontVec.z, player->frontVec.x);
	
	if (frontAngle != vecAngle)
	{
		float difAngle = (frontAngle - vecAngle);
		player->rot.y += difAngle;
	}
}
//==============================================================================
// NPC�̃t�B�[���h����
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//==============================================================================
void NonePlayerPatrol(void)
{
	PLAYER *player = GetPlayer(P2);
	BLOCK *block = GetBlock(0);
	AI *ai = &aiWk;
	D3DXVECTOR3 rayPos = player->frontVec + player->pos;	// �O���x�N�g���̏I�_
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �O���x�N�g���̂ƃu���b�N�̔���
	if (!HitCheckBlock(rayPos, player->pos))
	{
		ai->patrolNum = SwitchPatrolPattern(ai->patrolNum);
	}

	NonePlayerDest(MovePattern[ai->patrolNum]);

	vec = MovePattern[ai->patrolNum] - player->pos;
	
	D3DXVec3Normalize(&vec, &vec);

	player->move.x = vec.x * player->speed;
	player->move.z = vec.z * player->speed;
}

//==========================================================================
// NPC�̏���s���ؑ֏���
// ���@���Fint pattern(����p�^�[���ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
int SwitchPatrolPattern(int pattern)
{
	int out = 0;

	switch (pattern)
	{
		case PATTERN1 : out = PATTERN2;
			break;

		case PATTERN2 : out = PATTERN3;
			break;

		case PATTERN3 : out = PATTERN4;
			break;

		case PATTERN4 : out = PATTERN1;
			break;
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
	AI *ai = &aiWk;
	PLAYER *player = GetPlayer(0);
	BULLET *bullet = GetBullet(P2);
	BLOCK *block = GetBlock(0);
	
	// �o���b�g�g�p���`���[�W�s��
	if (!bullet->use[P2])
	{
		// �ő�l�ɂȂ����ꍇ
		if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
		{
			bullet->speedIncrease = BULLET_CHARGE_MAX;
		}
		// 10�t���[��
		else if (player->cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
		{
			bullet->speedIncrease += 0.2f;
		}
	}

	D3DXVECTOR3 rayPos = player[P2].frontVec + player[P2].pos;	// �O���x�N�g���̏I�_
	D3DXVECTOR3 reflectVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���ˌ�̃x�N�g��

	// �O���x�N�g���̔��ˏ���
	for (int i = 0; i < BLOCK_MAX; i++, block++)
	{
		if (!HitCheckBlock(rayPos, player[P2].pos))
		{
			reflectVec = ReflectVector(rayPos, player[P2].pos, GetNormal());
			break;
		}
	}

	// �U���J�n����
	if (CheckHitRay(player[P1].pos, player[P2].pos, player[P2].frontVec, 15.0f))
	{
		SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
	}
	// ���˂��l�������U��
	else if (CheckHitRay(player[P1].pos, player[P2].pos, reflectVec, 15.0f))
	{
		SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
	}

	//// ����Ƃ̋����ɂ�锻��
	//D3DXVECTOR3 pvpVec = player[P2].pos - player[P1].pos;
	//float vecLength = D3DXVec3Length(&pvpVec);
	//ai->atc[PATTERN1] = FuzzyTrapezoid(vecLength, ATTACK_DISTANCE_ATC_FUZZY_X1, ATTACK_DISTANCE_ATC_FUZZY_X2,
	//											 ATTACK_DISTANCE_ATC_FUZZY_X3, ATTACK_DISTANCE_ATC_FUZZY_X4);
	//ai->wait[PATTERN1] = FuzzyTrapezoid(vecLength, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2,
	//											  MOVE_DISTANCE_WAIT_FUZZY_X3, MOVE_DISTANCE_WAIT_FUZZY_X4);

	////�`���[�W���ɂ�锻��
	//ai->atc[PATTERN2] = FuzzyRightUp(bullet->speedIncrease, ATTACK_DISTANCE_ATC_FUZZY_X1, ATTACK_DISTANCE_ATC_FUZZY_X2);
	//ai->wait[PATTERN2] = FuzzyTrapezoid(bullet->speedIncrease, MOVE_DISTANCE_WAIT_FUZZY_X1, MOVE_DISTANCE_WAIT_FUZZY_X2);

	//ai->patternA = ai->atc[PATTERN1] * ai->atc[PATTERN2];
	//ai->patternB = ai->wait[PATTERN1] * ai->wait[PATTERN2];

	//
	//// ���胉�C�t�ɂ�锻��
	//if (player[P1].life < PLAYER_LIFE_MAX)
	//{
	//	ai->atc[PATTERN3] = FuzzyRightUp(player[P1].life, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X1, MOVE_PLAYERLIFE_ESCAPE_FUZZY_X2);

	//	ai->patternA = ai->atc[PATTERN1] * ai->atc[PATTERN2];
	//	ai->patternB = ai->wait[PATTERN1];
	//}

	//// �U�����ҋ@����r
	//ai->decision = Or(ai->patternA, ai->patternB);
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
