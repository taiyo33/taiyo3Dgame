//=============================================================================
//
// �q�����f������ [child.cpp]
// Author : GP11A341_22_�c�����z
//
//=============================================================================
#include "child.h"
#include "input.h"
#include "shadow.h"
#include "player.h"
#include "checkhit.h"
#include "debugproc.h"
#include "block.h"
#include "explosion.h"
#include "result.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CHILD_MODEL01		"data/MODEL/child01.x"				// �ǂݍ��ރ��f����
#define	CHILD_MODEL02		"data/MODEL/child02.x"				// �ǂݍ��ރ��f����

#define RATE_MOVE_CHILD		(0.20f)
#define CHILD_SIZE			(17.0f)					// ���f���T�C�Y
#define CENTER_PULL_CHILD	(25.0f)


enum {
	MODEL_TYPE001,
	MODEL_TYPE002
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MoveChild(int index, int cno);
void ChaseChild(int index, int cno);
void InitPosChild(void);
void WallShearChild(int index, int cno);
void CheckNorChild(D3DXVECTOR3 nor0, int index, int cno);

void AlignmentChild(int index, int cno);
void ChesionChild(int index, int cno);
void SeparationChild(int index, int cno);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9	D3DTexture;		// �e�N�X�`���ւ̃|�C���^
static LPD3DXMESH			D3DMesh[CHILD_SET_MAX];		// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER			D3DXBuffMat[CHILD_SET_MAX];	// �}�e���A�����ւ̃|�C���^
static DWORD				NumMat[CHILD_SET_MAX];			// �}�e���A�����̐�


static int					g_shadownum;
static D3DXCOLOR			g_shadowcolor;			
static D3DXMATRIX			MtxWorld;				// ���[���h�}�g���b�N�X
CHILD						childWk[CHILD_SET_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitChild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CHILD *child = &childWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < CHILD_SET_MAX; i++)
	{
		D3DMesh[i] = NULL;
		D3DXBuffMat[i] = NULL;
		NumMat[i] = 0;
		child[i].cnt = 50;

		for (int j = 0; j < CHILD_ONESET_MAX / 2; j++)
		{
			// �ʒu�E��]�E�X�P�[���̏����ݒ�
			child[i].pos[j].x = player[i].pos.x + rand() % 50;
			child[i].pos[j].y = player[i].pos.y;
			child[i].pos[j].z = player[i].pos.z + rand() % 50;
			child[i].rot[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			child[i].scl[j] = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
			child[i].use[j] = true;
			child[i].use[j + 49] = false;
			child[i].prevPos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			child[i].vec[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}


	D3DTexture = NULL;

	// X�t�@�C���̓ǂݍ���
	{
		if(FAILED(D3DXLoadMeshFromX(CHILD_MODEL01,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
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
		if (FAILED(D3DXLoadMeshFromX(CHILD_MODEL02,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
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
void UninitChild(void)
{
	if (D3DTexture != NULL)
	{// �e�N�X�`���̊J��
		D3DTexture->Release();
		D3DTexture = NULL;
	}

	for (int i = 0; i < CHILD_SET_MAX; i++)
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
void UpdateChild(void)
{
	CHILD *child = &childWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < CHILD_SET_MAX; i++)
	{
		for (int j = 0; j < CHILD_ONESET_MAX; j++)
		{
			if (child[i].use[j])
			{
				child[i].prevPos[j] = child[i].pos[j];

				if (child[i].pos[j].y > 10.0f)
				{
					child[i].pos[j].y -= PLAYER_FALL_SPEED;
				}

				// �ǐ�
				ChaseChild(i, j);

				// �ǂ���
				//WallShearChild(i, j);

				// �ړ�
				MoveChild(i, j);

				//AlignmentChild(i, j);
				//ChesionChild(i, j);
				//SeparationChild(i, j);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawChild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	CHILD *child = &childWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < CHILD_SET_MAX; i++)
	{
		for (int j = 0; j < CHILD_ONESET_MAX; j++)
		{
			if (child[i].use[j])
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&MtxWorld);

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScl, child[i].scl[j].x, child[i].scl[j].y, child[i].scl[j].z);
				D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxScl);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, child[i].rot[j].y, child[i].rot[j].x, child[i].rot[j].z);
				D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxRot);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, child[i].pos[j].x, child[i].pos[j].y, child[i].pos[j].z);
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
CHILD *GetChild(int index)
{
	return &childWk[index];
}

//========================================================================
// �q�����f�����̂ɂ�郊�U���g�ύX���菈��
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//========================================================================
void ComprareChild(void)
{
	CHILD *child = &childWk[0];
	int max;

	max = max(child[P1].cnt, child[P2].cnt);
	
	max == child[P1].cnt ? SetResult(P1) : SetResult(P2);
}

//========================================================================
// �q�����f���̃_���[�W���菈��
// ���@���Fint index(�g�q�����f���̃A�h���X), int blockNo(�u���b�N�̃A�h���X)
//		   D3DXVECTOR3 pos(�Ώێq�����f���̈ʒu)
// �߂�l�Fbool�^�@true�ł���΁Afalse�Ȃ�Ώ���
//========================================================================
void InitPosChild(void)
{
	CHILD *child = &childWk[0];

	for (int i = 0; i < CHILD_ONESET_MAX; i++)
	{
		child[0].pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		child[1].pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//========================================================================
// �q�����f���̈ړ�����
// ���@���Fint index(�g�q�����f���̃A�h���X), int blockNo(�u���b�N�̃A�h���X)
// �߂�l�Fbool�^�@true�ł���΁Afalse�Ȃ�Ώ���
//========================================================================
void MoveChild(int index, int cno)
{
	CHILD *child = &childWk[index];

	//child->move[cno].x = child->move[cno].x + (child->v1[cno].x * 0.1f) + (child->v2[cno].x * 1.0f) + (child->v3[cno].x * 0.8f);
	//child->move[cno].z = child->move[cno].z + (child->v1[cno].z * 0.1f) + (child->v2[cno].z * 1.0f) + (child->v3[cno].z * 0.8f);

	// �ړ��ʂɊ�����������
	child->move[cno].x += (0.0f - child->move[cno].x) * RATE_MOVE_CHILD;
	child->move[cno].y += (0.0f - child->move[cno].y) * RATE_MOVE_CHILD;
	child->move[cno].z += (0.0f - child->move[cno].z) * RATE_MOVE_CHILD;

	// �ʒu�ړ�
	child->pos[cno].x += child->move[cno].x;
	child->pos[cno].y += child->move[cno].y;
	child->pos[cno].z += child->move[cno].z;

	//child->v1[cno] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//child->v2[cno] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//child->v3[cno] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//============================================================================
// �q�����f���̒Ǐ]���菈��
// ���@���Fint index(�g�q�����f���̃A�h���X), int cno(�q�����f���̃A�h���X)
// �߂�l�F�Ȃ�
//============================================================================
void ChaseChild(int index, int cno)
{
	CHILD *child = &childWk[index];
	PLAYER *player = GetPlayer(index);
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	 // �v���C���[�Ǝq���̐擪�A�h���X�ԍ��̒Ǐ]
	//if (cno % 4 == 0)
	{
		// �Ǐ]�x�N�g���̌v�Z
		vec = player->pos - child->pos[cno];
		D3DXVec3Normalize(&vec, &vec);
		// �v���C���[���͈͊O�Ɉړ������ꍇ�Ǐ]
		if (!CheckHitBC(player->pos, child->pos[cno], CHILD_SIZE, CHILD_SIZE))
		{
			child->move[cno].x = vec.x * 5.0f;
			child->move[cno].z = vec.z * 5.0f;
		}
	}
	// �q�����m�̒Ǐ]
	//else
	//{
	//	// �Ǐ]�x�N�g���̌v�Z
	//	vec = child->pos[cno - 1] - child->pos[cno];
	//	D3DXVec3Normalize(&vec, &vec);
	//	// �O�A�h���X�̎q�����͈͊O�Ɉړ������ꍇ�Ǐ]
	//	if (!CheckHitBC(child->pos[cno], child->pos[cno - 1], CHILD_SIZE, CHILD_SIZE))
	//	{
	//		child->move[cno].x = vec.x * 5.0f;
	//		child->move[cno].z = vec.z * 5.0f;
	//	}
	//}

	for (int i = 0; i < CHILD_ONESET_MAX; i++)
	{
		if (!child->use[i]) continue;
		if (CheckHitBC(child->pos[cno], child->pos[i], CHILD_SIZE, CHILD_SIZE))
		{
			child->vec[cno] = child->pos[cno] - child->pos[i];
			D3DXVec3Normalize(&child->vec[cno], &child->vec[cno]);
			child->move[cno].x = (child->move[cno].x + child->vec[cno].x);
			child->move[cno].z = (child->move[cno].z + child->vec[cno].z);
		}
	}
}

//==========================================================================
// �ǂ���̂��蔲���\�h����(�u���b�N�g�p���Ƀ}�b�v�O�֏o�Ȃ��悤��)
// ���@���FD3DXVECTOR3 nor0(�|���S���̖@��), int index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void WallShearChild(int index, int cno)
{
	CHILD *child = &childWk[index];
	PLAYER *player = GetPlayer(index);
	if (!HitCheckBlock(child->prevPos[cno] + child->move[cno], child->prevPos[cno], BLOCK_VTX_MAX))
	{
		child->move[cno] = WallShear(child->pos[cno] + child->move[cno], GetNormal(), index);
		CheckNorChild(GetNormal(), index, cno);
	}
}

//==========================================================================
// �ǂ���̂��蔲���\�h����(�u���b�N�g�p���Ƀ}�b�v�O�֏o�Ȃ��悤��)
// ���@���FD3DXVECTOR3 nor0(�|���S���̖@��), int index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void CheckNorChild(D3DXVECTOR3 nor0, int index, int cno)
{
	CHILD *child = &childWk[index];

	// �@����X�������Ȃ�
	if (nor0.x != 0.0f)
	{
		child->move[cno].x = 0.0f;
		child->pos[cno].x = child->prevPos[cno].x;
		return;
	}
	// �@����Z�������Ȃ�
	if (nor0.z != 0.0f)
	{
		child->move[cno].z = 0.0f;
		child->pos[cno].z = child->prevPos[cno].z;
		return;
	}
}

//=========================================================================
// �q�����f���̑���
// ���@���FD3DXVECTOR3 pos(�ʒu)�Aint index(�q�����f���̃A�h���X�ԍ�)
// �߂�l�Fbool�^�@���g�p�̏ꍇ true�A�g�p���̏ꍇ false
//=========================================================================
void SetChild(D3DXVECTOR3 pos, int index)
{
	CHILD *child = &childWk[index];
	PLAYER *player = GetPlayer(index);

	for (int i = 0; i < CHILD_ONESET_MAX; i++)
	{
		if (!child->use[i])
		{
			// �����ݒ�
			child->use[i] = true;
			child->pos[i] = player->pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f);
			child->rot[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			child->scl[i] = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
			child->prevPos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			child->vec[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			
			SetExplosion(child->pos[i], child->rot[i], 0);
			return;
		}
	}

	return;
}


// ����
void AlignmentChild(int index, int cno)
{
	CHILD *child = &childWk[index];
	PLAYER *player = GetPlayer(index);

	for (int i = 0; i < CHILD_ONESET_MAX; i++)
	{
		if (cno == i) continue;
		child->v1[cno].x += child->move[i].x;
		child->v1[cno].z += child->move[i].z;
	}
	child->v1[cno].x += player->move.x;
	child->v1[cno].z += player->move.z;

	child->v1[cno].x /= (CHILD_ONESET_MAX);
	child->v1[cno].z /= (CHILD_ONESET_MAX);


	child->v1[cno].x = (child->v1[cno].x - child->move[cno].x) / 2;
	child->v1[cno].z = (child->v1[cno].z - child->move[cno].z) / 2;

}
// ����
void ChesionChild(int index, int cno)
{
	CHILD *child = &childWk[index];
	PLAYER *player = GetPlayer(index);

	for (int i = 0; i < CHILD_ONESET_MAX; i++)
	{
		if (cno == i) continue;
		child->v2[cno].x += child->pos[i].x;
		child->v2[cno].z += child->pos[i].z;
	}
	child->v2[cno].x += player->pos.x;
	child->v2[cno].z += player->pos.z;

	child->v2[cno].x /= (CHILD_ONESET_MAX);
	child->v2[cno].z /= (CHILD_ONESET_MAX);

	child->v2[cno].x = (child->v2[cno].x - child->pos[cno].x) / CENTER_PULL_CHILD;
	child->v2[cno].z = (child->v2[cno].z - child->pos[cno].z) / CENTER_PULL_CHILD;
}


// ����
void SeparationChild(int index, int cno)
{
	CHILD *child = &childWk[index];
	PLAYER *player = GetPlayer(index);

	for (int i = 0; i < CHILD_ONESET_MAX; i++)
	{
		if (cno == i) continue;
		if (CheckHitBC(child->pos[cno], child->pos[i], 15.0f, 15.0f))
		{
			child->v3[cno].x -= child->pos[cno].x - child->pos[i].x;
			child->v3[cno].z -= child->pos[cno].z - child->pos[i].z;
		}
	}
}
