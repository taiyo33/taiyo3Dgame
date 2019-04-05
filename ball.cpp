//=============================================================================
//
// �{�[�����f������ [ball.cpp]
// Author : GP11A341_22_�c�����z
//
//=============================================================================
#include "main.h"
#include "ball.h"
#include "bullet.h"
#include "player.h"
#include "checkhit.h"
#include "block.h"
#include "explosion.h"
#include "result.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	BALL_MODEL01		"data/MODEL/ball01.x"				// �ǂݍ��ރ��f����
#define	BALL_MODEL02		"data/MODEL/ball02.x"				// �ǂݍ��ރ��f����

#define RATE_MOVE_BALL		(0.20f)				// �ړ����x
#define BALL_SIZE			(17.0f)				// ���f���T�C�Y
#define BALL_FALL_SPEED	(5.0f)					// �������x

enum BALLMODEL{
	MODEL_TYPE001,
	MODEL_TYPE002
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MoveBall(int index, int cno);
void ChaseBall(int index, int cno);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9	D3DTexture = NULL;					// �e�N�X�`���ւ̃|�C���^
static LPD3DXMESH			D3DMesh[BALL_SET_MAX];		// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER			D3DXBuffMat[BALL_SET_MAX];	// �}�e���A�����ւ̃|�C���^
static DWORD				NumMat[BALL_SET_MAX];		// �}�e���A�����̐�

static D3DXMATRIX			MtxWorld;					// ���[���h�}�g���b�N�X
BALL						BallWk[BALL_SET_MAX];		// �{�[���\����
//===============================================================================
// ����������
// ���@���Fint type(�ď�������2������l)
// �߂�l�FHRESULT�^
//===============================================================================
HRESULT InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BALL *ball = &BallWk[0];
	PLAYER *player = GetPlayer(0);

	// �e�ϐ��̏�����
	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		D3DMesh[i] = NULL;
		D3DXBuffMat[i] = NULL;
		NumMat[i] = 0;
		ball[i].cnt = 50;
		ball[i].damageSE = LoadSound(SE_BALLDAMAGE);

		for (int j = 0; j < BALL_ONESET_MAX; j++)
		{
			ball[i].pos[j].x = player[i].pos.x + rand() % 50;
			ball[i].pos[j].y = player[i].pos.y;
			ball[i].pos[j].z = player[i].pos.z + rand() % 50;
			ball[i].rot[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ball[i].scl[j] = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
			ball[i].prevPos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ball[i].vec[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		for (int k = 0; k < BALL_ONESET_MAX / 2; k++)
		{
			ball[i].use[k] = true;
		}
	}

	// X�t�@�C���̓ǂݍ���
	{
		if(FAILED(D3DXLoadMeshFromX(BALL_MODEL01,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
								D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
								pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
								NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
								&D3DXBuffMat[MODEL_TYPE001],	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^�@�e�N�X�`����J���[���
								NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
								&NumMat[MODEL_TYPE001],		// D3DXMATERIAL�\���̂̐�	�o�^����Ă�}�e���A�����
								&D3DMesh[MODEL_TYPE001])))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X�@���_���̃f�[�^
	{
		return E_FAIL;
	}

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(BALL_MODEL02,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
									D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
									pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
									NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
									&D3DXBuffMat[MODEL_TYPE002],	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^�@�e�N�X�`����J���[���
									NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
									&NumMat[MODEL_TYPE002],		// D3DXMATERIAL�\���̂̐�	�o�^����Ă�}�e���A�����
									&D3DMesh[MODEL_TYPE002])))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X�@���_���̃f�[�^
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBall(void)
{
	if (D3DTexture != NULL)
	{// �e�N�X�`���̊J��
		D3DTexture->Release();
		D3DTexture = NULL;
	}

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		if (D3DMesh != NULL)
		{// ���b�V���̊J��
			D3DMesh[i]->Release();
			D3DMesh[i] = NULL;
		}

		if (D3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			D3DXBuffMat[i]->Release();
			D3DXBuffMat[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBall(void)
{
	BALL *ball = &BallWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		for (int j = 0; j < BALL_ONESET_MAX; j++)
		{
			if (ball[i].use[j])
			{
				// �ߋ��̈ʒu��ۑ�
				ball[i].prevPos[j] = ball[i].pos[j];
				
				// �t�B�[���h�֗���
				if (ball[i].pos[j].y > 10.0f)
				{
					ball[i].pos[j].y -= BALL_FALL_SPEED;
				}

				// �t�B�[���h��ł̓���
				if (ball[i].pos[j].y > 10.0f)
				{
					// �ǐ�
					ChaseBall(i, j);
					// �ړ�
					MoveBall(i, j);
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	BALL *ball = &BallWk[0];

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		for (int j = 0; j < BALL_ONESET_MAX; j++)
		{
			if (ball[i].use[j])
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&MtxWorld);

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScl, ball[i].scl[j].x, ball[i].scl[j].y, ball[i].scl[j].z);
				D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxScl);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, ball[i].rot[j].y, ball[i].rot[j].x, ball[i].rot[j].z);
				D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxRot);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, ball[i].pos[j].x, ball[i].pos[j].y, ball[i].pos[j].z);
				D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxTranslate);

				//D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &player->mtxWorld);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);

				// ���݂̃}�e���A�����擾�@�i���ɖ߂����߂̃o�b�N�A�b�v�j
				pDevice->GetMaterial(&matDef);

				// �}�e���A�����ɑ΂���|�C���^���擾
				pD3DXMat = (D3DXMATERIAL*)D3DXBuffMat[i]->GetBufferPointer();

				// �}�e���A���̐����̕\�����K�v�Ȃ���FOR�����g�p
				for (int nCntMat = 0; nCntMat < (int)NumMat[i]; nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, D3DTexture);

					// �`��
					D3DMesh[i]->DrawSubset(nCntMat);
				}

				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//========================================================================
// �{�[���̃A�h���X�擾
// ���@���Fint index(�g�{�[���̃A�h���X)
// �߂�l�FBALL�^
//========================================================================
BALL *GetBall(int index)
{
	return &BallWk[index];
}

//========================================================================
// �{�[�����̂ɂ�郊�U���g�ύX���菈��
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//========================================================================
void ComprareBall(void)
{
	BALL *ball = &BallWk[0]; 
	PLAYER *player = GetPlayer(0);
	int max = 0;
	int index = 0;

	// �������̔�r
	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		max = max(ball[i].cnt, max);
		if (ball[i].cnt == max)
		{
			index = i;
		}
	}

	// �Q�[�����[�h�ɂ��A�C�R���ύX
	// NPC���[�h
	if (player[index].npc)
	{
		max == ball[index].cnt ? SetResult(index, index) : SetResult(index, NPC);
	}
	// P2���[�h
	else
	{
		max == ball[index].cnt ? SetResult(index, index) : SetResult(index, index);
	}
}

//========================================================================
// �{�[���̈ړ�����
// ���@���Fint index(�g�{�[���̃A�h���X), int cno(�{�[���P�̂̃A�h���X)
// �߂�l�F�ȁ@��
//========================================================================
void MoveBall(int index, int cno)
{
	BALL *ball = &BallWk[index];

	// �ړ��ʂɊ�����������
	ball->move[cno].x += (0.0f - ball->move[cno].x) * RATE_MOVE_BALL;
	ball->move[cno].y += (0.0f - ball->move[cno].y) * RATE_MOVE_BALL;
	ball->move[cno].z += (0.0f - ball->move[cno].z) * RATE_MOVE_BALL;

	// �ʒu�ړ�
	ball->pos[cno].x += ball->move[cno].x;
	ball->pos[cno].y += ball->move[cno].y;
	ball->pos[cno].z += ball->move[cno].z;

}

//============================================================================
// �{�[���̒Ǐ]���菈��
// ���@���Fint index(�g�{�[���̃A�h���X), int cno(�{�[���̃A�h���X)
// �߂�l�F�Ȃ�
//============================================================================
void ChaseBall(int index, int cno)
{
	BALL *ball = &BallWk[index];
	PLAYER *player = GetPlayer(index);
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �Ǐ]�x�N�g���̌v�Z
	vec = player->pos - ball->pos[cno];
	D3DXVec3Normalize(&vec, &vec);
	// �v���C���[���͈͊O�Ɉړ������ꍇ�Ǐ]
	if (!CheckHitBC(player->pos, ball->pos[cno], BALL_SIZE, BALL_SIZE))
	{
		ball->move[cno].x = vec.x * 5.0f;
		ball->move[cno].z = vec.z * 5.0f;
	}

	// �{�[�����m�̗��U����
	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (!ball->use[i]) continue;
		if (CheckHitBC(ball->pos[cno], ball->pos[i], BALL_SIZE, BALL_SIZE))
		{
			// �d�Ȃ荇���{�[���Ƌt�����x�N�g����
			ball->vec[cno] = ball->pos[cno] - ball->pos[i];
			D3DXVec3Normalize(&ball->vec[cno], &ball->vec[cno]);
			ball->move[cno].x = (ball->move[cno].x + ball->vec[cno].x);
			ball->move[cno].z = (ball->move[cno].z + ball->vec[cno].z);
		}
	}
}

//=========================================================================
// �{�[���̔�������
// ���@���Fint index(�g�{�[���̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//=========================================================================
void SetBall(int index)
{
	BALL *ball = &BallWk[index];

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (!ball->use[i])
		{
			// �����ݒ�
			ball->use[i] = true;
			ball->pos[i] = D3DXVECTOR3(0.0f, 200.0f, 0.0f);
			ball->rot[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ball->scl[i] = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
			ball->prevPos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ball->vec[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			
			SetExplosion(ball->pos[i], ball->rot[i], 0);
			return;
		}
	}

	return;
}

//=========================================================================
// �{�[���̃_���[�W���菈��
// ���@���Fint index00(�_���[�W���󂯂��{�[���̃A�h���X)�A
//		   int index01(�_���[�W��^��������̃A�h���X)�Aint bno(�g�o���b�g�̃A�h���X)
// �߂�l�Fbool�^
//=========================================================================
void DamageBall(int index00, int index01, int bno)
{
	BALL *ball = GetBall(0);
	BULLET *bullet = GetBullet(0);

	for (int j = 0; j < BALL_ONESET_MAX; j++)
	{
		if (!ball[index00].use[j]) continue;

		if (CheckHitBC(bullet[index01].pos[bno], ball[index00].pos[j], 15.0f, 15.0f))
		{
			PlaySound(ball[index00].damageSE, E_DS8_FLAG_NONE);				// ���ŉ��̍Đ�
			ball[index00].use[j] = false;	// �{�[��������
			ball[index00].cnt--;			// �{�[���̏����������炷
			ball[index01].cnt++;			// �{�[���̏������𑝂₷
			SetBall(index01);				// �{�[���̐ݒu
			SetExplosion(ball[index00].pos[j], ball[index00].rot[j], 0);	// ���ŃG�t�F�N�g
		}
	}
}

//=========================================================================
// �{�[���̈ʒu����������
// ���@���Fint index(�{�[���̃A�h���X�ԍ�)
// �߂�l�Fbool�^�@���g�p�̏ꍇ true�A�g�p���̏ꍇ false
//=========================================================================
void SetInitPosBall(void)
{
	BALL *ball = &BallWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		for (int j = 0; j < BALL_ONESET_MAX / 2; j++)
		{
			ball[i].pos[j].x = player[i].pos.x + rand() % 50;
			ball[i].pos[j].y = player[i].pos.y;
			ball[i].pos[j].z = player[i].pos.z + rand() % 50;
		}
	}
}
