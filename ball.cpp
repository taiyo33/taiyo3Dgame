//=============================================================================
//
// �{�[�����f������ [ball.cpp]
// Author : GP11A341_22_�c�����z
//
//=============================================================================
#include "ball.h"
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

#define RATE_MOVE_BALL		(0.20f)
#define BALL_SIZE			(17.0f)					// ���f���T�C�Y
#define CENTER_PULL_BALL	(25.0f)


enum BALLMODEL{
	MODEL_TYPE001,
	MODEL_TYPE002
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MoveBall(int index, int cno);
void ChaseBall(int index, int cno);
void InitPosBall(void);
void WallShearBall(int index, int cno);
void CheckNorBall(D3DXVECTOR3 nor0, int index, int cno);

void AlignmentBall(int index, int cno);
void ChesionBall(int index, int cno);
void SeparationBall(int index, int cno);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9	D3DTexture;		// �e�N�X�`���ւ̃|�C���^
static LPD3DXMESH			D3DMesh[BALL_SET_MAX];		// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER			D3DXBuffMat[BALL_SET_MAX];	// �}�e���A�����ւ̃|�C���^
static DWORD				NumMat[BALL_SET_MAX];			// �}�e���A�����̐�


static D3DXMATRIX			MtxWorld;				// ���[���h�}�g���b�N�X
BALL						ballWk[BALL_SET_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BALL *ball = &ballWk[0];
	PLAYER *player = GetPlayer(0);

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


	D3DTexture = NULL;

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
	BALL *ball = &ballWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		for (int j = 0; j < BALL_ONESET_MAX; j++)
		{
			if (ball[i].use[j])
			{
				ball[i].prevPos[j] = ball[i].pos[j];

				if (ball[i].pos[j].y > 10.0f)
				{
					ball[i].pos[j].y -= PLAYER_FALL_SPEED;
				}

				if (ball[i].pos[j].y > 0.0f)
				{
					// �ǐ�
					ChaseBall(i, j);
					// �ړ�
					MoveBall(i, j);
				}

				//// �ǂ���
				//WallShearBall(i, j);

				//AlignmentBall(i, j);
				//ChesionBall(i, j);
				//SeparationBall(i, j);
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
	BALL *ball = &ballWk[0];

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
// �q�����f���̔��ˉ񐔔��菈��
// ���@���Fint index(�g�q�����f���̃A�h���X), int blockNo(�u���b�N�̃A�h���X)
//		   D3DXVECTOR3 pos(�Ώێq�����f���̈ʒu)
// �߂�l�Fbool�^�@true�ł���΁Afalse�Ȃ�Ώ���
//========================================================================
BALL *GetBall(int index)
{
	return &ballWk[index];
}

//========================================================================
// �q�����f�����̂ɂ�郊�U���g�ύX���菈��
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//========================================================================
void ComprareBall(void)
{
	BALL *ball = &ballWk[0]; 
	PLAYER *player = GetPlayer(0);
	int max = 0;
	int index = 0;

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		max = max(ball[i].cnt, max);
		if (ball[i].cnt == max)
		{
			index = i;
		}
	}

	if (player[index].npc)
	{
		max == ball[index].cnt ? SetResult(index, index) : SetResult(index, NPC);
	}
	else
	{
		max == ball[index].cnt ? SetResult(index, index) : SetResult(index, index);
	}
}

//========================================================================
// �q�����f���̃_���[�W���菈��
// ���@���Fint index(�g�q�����f���̃A�h���X), int blockNo(�u���b�N�̃A�h���X)
//		   D3DXVECTOR3 pos(�Ώێq�����f���̈ʒu)
// �߂�l�Fbool�^�@true�ł���΁Afalse�Ȃ�Ώ���
//========================================================================
void InitPosBall(void)
{
	BALL *ball = &ballWk[0];

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		ball[0].pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ball[1].pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//========================================================================
// �q�����f���̈ړ�����
// ���@���Fint index(�g�q�����f���̃A�h���X), int blockNo(�u���b�N�̃A�h���X)
// �߂�l�Fbool�^�@true�ł���΁Afalse�Ȃ�Ώ���
//========================================================================
void MoveBall(int index, int cno)
{
	BALL *ball = &ballWk[index];

	//ball->move[cno].x = ball->move[cno].x + (ball->v1[cno].x * 0.1f) + (ball->v2[cno].x * 1.0f) + (ball->v3[cno].x * 0.8f);
	//ball->move[cno].z = ball->move[cno].z + (ball->v1[cno].z * 0.1f) + (ball->v2[cno].z * 1.0f) + (ball->v3[cno].z * 0.8f);

	// �ړ��ʂɊ�����������
	ball->move[cno].x += (0.0f - ball->move[cno].x) * RATE_MOVE_BALL;
	ball->move[cno].y += (0.0f - ball->move[cno].y) * RATE_MOVE_BALL;
	ball->move[cno].z += (0.0f - ball->move[cno].z) * RATE_MOVE_BALL;

	// �ʒu�ړ�
	ball->pos[cno].x += ball->move[cno].x;
	ball->pos[cno].y += ball->move[cno].y;
	ball->pos[cno].z += ball->move[cno].z;

	//ball->v1[cno] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//ball->v2[cno] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//ball->v3[cno] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//============================================================================
// �q�����f���̒Ǐ]���菈��
// ���@���Fint index(�g�q�����f���̃A�h���X), int cno(�q�����f���̃A�h���X)
// �߂�l�F�Ȃ�
//============================================================================
void ChaseBall(int index, int cno)
{
	BALL *ball = &ballWk[index];
	PLAYER *player = GetPlayer(index);
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	 // �v���C���[�Ǝq���̐擪�A�h���X�ԍ��̒Ǐ]
	//if (cno % 4 == 0)
	{
		// �Ǐ]�x�N�g���̌v�Z
		vec = player->pos - ball->pos[cno];
		D3DXVec3Normalize(&vec, &vec);
		// �v���C���[���͈͊O�Ɉړ������ꍇ�Ǐ]
		if (!CheckHitBC(player->pos, ball->pos[cno], BALL_SIZE, BALL_SIZE))
		{
			ball->move[cno].x = vec.x * 5.0f;
			ball->move[cno].z = vec.z * 5.0f;
		}
	}
	// �q�����m�̒Ǐ]
	//else
	//{
	//	// �Ǐ]�x�N�g���̌v�Z
	//	vec = ball->pos[cno - 1] - ball->pos[cno];
	//	D3DXVec3Normalize(&vec, &vec);
	//	// �O�A�h���X�̎q�����͈͊O�Ɉړ������ꍇ�Ǐ]
	//	if (!CheckHitBC(ball->pos[cno], ball->pos[cno - 1], BALL_SIZE, BALL_SIZE))
	//	{
	//		ball->move[cno].x = vec.x * 5.0f;
	//		ball->move[cno].z = vec.z * 5.0f;
	//	}
	//}

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (!ball->use[i]) continue;
		if (CheckHitBC(ball->pos[cno], ball->pos[i], BALL_SIZE, BALL_SIZE))
		{
			ball->vec[cno] = ball->pos[cno] - ball->pos[i];
			D3DXVec3Normalize(&ball->vec[cno], &ball->vec[cno]);
			ball->move[cno].x = (ball->move[cno].x + ball->vec[cno].x);
			ball->move[cno].z = (ball->move[cno].z + ball->vec[cno].z);
		}
	}
}

//==========================================================================
// �ǂ���̂��蔲���\�h����(�u���b�N�g�p���Ƀ}�b�v�O�֏o�Ȃ��悤��)
// ���@���FD3DXVECTOR3 nor0(�|���S���̖@��), int index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void WallShearBall(int index, int cno)
{
	BALL *ball = &ballWk[index];
	PLAYER *player = GetPlayer(index);
	if (!HitCheckBlock(ball->prevPos[cno] + ball->move[cno], ball->prevPos[cno], BLOCK_VTX_MAX))
	{
		ball->move[cno] = WallShear(ball->pos[cno] + ball->move[cno], GetNormal(), index);
		CheckNorBall(GetNormal(), index, cno);
	}
}

//==========================================================================
// �ǂ���̂��蔲���\�h����(�u���b�N�g�p���Ƀ}�b�v�O�֏o�Ȃ��悤��)
// ���@���FD3DXVECTOR3 nor0(�|���S���̖@��), int index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void CheckNorBall(D3DXVECTOR3 nor0, int index, int cno)
{
	BALL *ball = &ballWk[index];

	// �@����X�������Ȃ�
	if (nor0.x != 0.0f)
	{
		ball->move[cno].x = 0.0f;
		ball->pos[cno].x = ball->prevPos[cno].x;
		return;
	}
	// �@����Z�������Ȃ�
	if (nor0.z != 0.0f)
	{
		ball->move[cno].z = 0.0f;
		ball->pos[cno].z = ball->prevPos[cno].z;
		return;
	}
}

//=========================================================================
// �q�����f���̑���
// ���@���FD3DXVECTOR3 pos(�ʒu)�Aint index(�q�����f���̃A�h���X�ԍ�)
// �߂�l�Fbool�^�@���g�p�̏ꍇ true�A�g�p���̏ꍇ false
//=========================================================================
void SetBall(int index)
{
	BALL *ball = &ballWk[index];

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
// �q�����f���̑���
// ���@���FD3DXVECTOR3 pos(�ʒu)�Aint index(�q�����f���̃A�h���X�ԍ�)
// �߂�l�Fbool�^�@���g�p�̏ꍇ true�A�g�p���̏ꍇ false
//=========================================================================
void SetInitPosBall(void)
{
	BALL *ball = &ballWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < BALL_SET_MAX; i++)
	{
		for (int j = 0; j < BALL_ONESET_MAX / 2; j++)
		{
			// �ʒu�E��]�E�X�P�[���̏����ݒ�
			ball[i].pos[j].x = player[i].pos.x + rand() % 50;
			ball[i].pos[j].y = player[i].pos.y;
			ball[i].pos[j].z = player[i].pos.z + rand() % 50;
		}
	}
}


// ����
void AlignmentBall(int index, int cno)
{
	BALL *ball = &ballWk[index];
	PLAYER *player = GetPlayer(index);

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (cno == i) continue;
		ball->v1[cno].x += ball->move[i].x;
		ball->v1[cno].z += ball->move[i].z;
	}
	ball->v1[cno].x += player->move.x;
	ball->v1[cno].z += player->move.z;

	ball->v1[cno].x /= (BALL_ONESET_MAX);
	ball->v1[cno].z /= (BALL_ONESET_MAX);


	ball->v1[cno].x = (ball->v1[cno].x - ball->move[cno].x) / 2;
	ball->v1[cno].z = (ball->v1[cno].z - ball->move[cno].z) / 2;

}

// ����
void ChesionBall(int index, int cno)
{
	BALL *ball = &ballWk[index];
	PLAYER *player = GetPlayer(index);

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (cno == i) continue;
		ball->v2[cno].x += ball->pos[i].x;
		ball->v2[cno].z += ball->pos[i].z;
	}

	ball->v2[cno].x += player->pos.x;
	ball->v2[cno].z += player->pos.z;

	ball->v2[cno].x /= (BALL_ONESET_MAX);
	ball->v2[cno].z /= (BALL_ONESET_MAX);

	ball->v2[cno].x = (ball->v2[cno].x - ball->pos[cno].x) / CENTER_PULL_BALL;
	ball->v2[cno].z = (ball->v2[cno].z - ball->pos[cno].z) / CENTER_PULL_BALL;
}


// ����
void SeparationBall(int index, int cno)
{
	BALL *ball = &ballWk[index];
	PLAYER *player = GetPlayer(index);

	for (int i = 0; i < BALL_ONESET_MAX; i++)
	{
		if (cno == i) continue;
		if (CheckHitBC(ball->pos[cno], ball->pos[i], 15.0f, 15.0f))
		{
			ball->v3[cno].x -= ball->pos[cno].x - ball->pos[i].x;
			ball->v3[cno].z -= ball->pos[cno].z - ball->pos[i].z;
		}
	}
}
