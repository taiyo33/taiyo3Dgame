//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "block.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "effect.h"
#include "checkhit.h"
#include "explosion.h"
#include "bullet.h"
#include "item.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	BLOCK_MODEL				"data/MODEL/block001.x"	// �ǂݍ��ރ��f����
#define TEXTURE_MAX				(5)
#define INIT_POS_X_RIGHTSIDE	(375.0f)	// �E���̃u���b�N�̏���X���W
#define INIT_POS_X_LEFTSIDE		(-375.0f)	// �����̃u���b�N�̏���X���W
#define INIT_POS_Y				(0.0f)		// �u���b�N�̏���Y���W
#define INIT_POS_Z_NEARSIDE		(-287.5f)	// ��O�̃u���b�N�̏���Z���W
#define INIT_POS_Z_FEARSIDE		(287.5f)	// ���̃u���b�N�̏���Z���W
#define BLOCK_VERTEX			(4)			// 
#define VTX_SIZE_X				(355.0f)	// ���_��X���T�C�Y
#define VTX_SIZE_Y				(25.0f)		// �@�@�@Y���T�C�Y
#define VTX_SIZE_Z				(273.5f)	// �@�@�@Z���T�C�Y
#define BLOCK_DAMAGE			(1)			// �u���b�N�ւ̃_���[�W
#define BLOCK_SIZE				(25.0f)		// �u���b�N��1�ӂ̃T�C�Y
#define INIT_LIFE				(5)			// �����̏����l	

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetPosBlock(void);
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice);
void InitVertexBlock(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBlock[TEXTURE_MAX] = { 
									NULL,NULL,NULL,NULL,NULL }; // �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH				D3DXMeshBlock;		// ID3DXMeshBlock�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER			D3DXBuffMatBlock;	// ���b�V���̃}�e���A�������i�[
DWORD					NumMat;				// �������̑���
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBlock;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				MtxWorld;			// ���[���h�}�g���b�N�X
D3DXMATRIX				MtxWorldVtx;		// ���[���h�}�g���b�N�X
static int				TextureNum;			// �e�N�X�`���[�̔ԍ�
BLOCK					block[BLOCK_MAX];	// �u���b�N�\����

//=============================================================================
// ����������
// ���@���Fint type(�ď������Q������l)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitBlock(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// ���_���̍쐬
	hr = MakeVertexBlock(pDevice);
	
	// �u���b�N�̏����ʒu��ݒ�
	SetPosBlock();

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		block[i].use = true;		// �g�p�֏�����
		block[i].life = INIT_LIFE;	// ���C�t�̏�����
	}

	D3DXMeshBlock = NULL;			// �C���^�[�t�F�[�X�̏�����
	D3DXBuffMatBlock = NULL;		// �}�e���A���̏�����
	TextureNum = NULL;				// �e�N�X�`���[�ԍ���������

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(BLOCK_MODEL,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&D3DXBuffMatBlock,
		NULL,
		&NumMat,
		&D3DXMeshBlock)))
	{
		return E_FAIL;
	}
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBlock(void)
{
	if(D3DXMeshBlock != NULL)
	{// ���b�V���̊J��
		D3DXMeshBlock->Release();
		D3DXMeshBlock = NULL;
	}

	if(D3DXBuffMatBlock != NULL)
	{// �}�e���A���̊J��
		D3DXBuffMatBlock->Release();
		D3DXBuffMatBlock = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBlock[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureBlock[i]->Release();
			D3DTextureBlock[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBlock(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (block[i].use)
		{
			//// ���[���h�}�g���b�N�X�̏�����
			//D3DXMatrixIdentity(&MtxWorld);

			//// �ړ��𔽉f
			//D3DXMatrixTranslation(&mtxTranslate, block[i].pos.x, block[i].pos.y, block[i].pos.z);
			//D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxTranslate);

			//// ���[���h�}�g���b�N�X�̐ݒ�
			//pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);

			//// ���݂̃}�e���A�����擾
			//pDevice->GetMaterial(&matDef);

			//// �}�e���A�����ɑ΂���|�C���^���擾
			//pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatBlock->GetBufferPointer();

			//for (int nCntMat = 0; nCntMat < (int)NumMat; nCntMat++)
			//{
			//	// �}�e���A���̐ݒ�
			//	pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			//	// �e�N�X�`���̐ݒ�
			//	pDevice->SetTexture(0, D3DTextureBlock[TextureNum]);

			//	// �`��
			//	D3DXMeshBlock->DrawSubset(nCntMat);
			//}

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&MtxWorldVtx);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &MtxWorldVtx);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, D3DVtxBuffBlock, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTextureBlock[0]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);

		}
	}
}


//=============================================================================
// �u���b�N�̃A�h���X�擾
//=============================================================================
BLOCK *GetBlock(int bno)
{
	return &block[bno];
}

//=============================================================================
// �u���b�N�̐ݒu
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//=============================================================================
void SetPosBlock(void)
{
	int i = 0;
	float size = 0.0f;

	for (i = 0; i < BLOCK_NUM_LEFTSIDE; i++, size += BLOCK_SIZE)
	{
		block[i].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE, INIT_POS_Y, INIT_POS_Z_FEARSIDE - size);
	}

	size = BLOCK_SIZE;

	for (i = BLOCK_NUM_LEFTSIDE; i < BLOCK_NUM_NEARSIDE; i++, size += BLOCK_SIZE)
	{
		block[i].pos = D3DXVECTOR3(INIT_POS_X_LEFTSIDE + size, INIT_POS_Y, INIT_POS_Z_NEARSIDE);
	}

	size = 0.0f;

	for (i = BLOCK_NUM_NEARSIDE; i < BLOCK_NUM_RIGHTSIDE; i++, size += BLOCK_SIZE)
	{
		block[i].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE, INIT_POS_Y, INIT_POS_Z_NEARSIDE + size);
	}

	size = BLOCK_SIZE;

	for (i = BLOCK_NUM_RIGHTSIDE; i < BLOCK_NUM_FEARSIDE; i++, size += BLOCK_SIZE)
	{
		block[i].pos = D3DXVECTOR3(INIT_POS_X_RIGHTSIDE - size, INIT_POS_Y, INIT_POS_Z_FEARSIDE);
	}
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BLOCK_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffBlock,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	/* ���_���̐ݒ� */
	InitVertexBlock();

	return S_OK;
}

//============================================================================
// �Q�[���t�B�[���h�̍쐬
//============================================================================
void InitVertexBlock(void)
{
	int i = 0;
	float size = 0.0f;

	// BOX�̒��_���̐ݒ�
	{	//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D			   *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

		for (i = 0; i < BLOCK_NUM_LEFTSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X, VTX_SIZE_Y, VTX_SIZE_Z - size);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X, VTX_SIZE_Y, VTX_SIZE_Z + BLOCK_SIZE - size);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X, 0.0f, VTX_SIZE_Z - size);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X, 0.0f, VTX_SIZE_Z + BLOCK_SIZE - size);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

			// ���ˌ��̐ݒ�
				pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		size = BLOCK_SIZE;

		for (i = BLOCK_NUM_LEFTSIDE; i < BLOCK_NUM_NEARSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-VTX_SIZE_X - BLOCK_SIZE + size, VTX_SIZE_Y, -VTX_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(-VTX_SIZE_X + size, VTX_SIZE_Y, -VTX_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(-VTX_SIZE_X - BLOCK_SIZE + size, 0.0f, -VTX_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(-VTX_SIZE_X + size, 0.0f, -VTX_SIZE_Z);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}

		size = 0.0f;

		for (i = BLOCK_NUM_NEARSIDE; i < BLOCK_NUM_RIGHTSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X, VTX_SIZE_Y, -VTX_SIZE_Z + size);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X, VTX_SIZE_Y, -VTX_SIZE_Z - BLOCK_SIZE + size);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X, 0.0f, -VTX_SIZE_Z + size);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X, 0.0f, -VTX_SIZE_Z - BLOCK_SIZE + size);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		size = BLOCK_SIZE;

		for (i = BLOCK_NUM_RIGHTSIDE; i < BLOCK_NUM_FEARSIDE; i++, pVtx += 4, size += BLOCK_SIZE)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(VTX_SIZE_X - size, VTX_SIZE_Y, VTX_SIZE_Z);
			pVtx[1].vtx = D3DXVECTOR3(VTX_SIZE_X + BLOCK_SIZE - size, VTX_SIZE_Y, VTX_SIZE_Z);
			pVtx[2].vtx = D3DXVECTOR3(VTX_SIZE_X - size, 0.0f, VTX_SIZE_Z);
			pVtx[3].vtx = D3DXVECTOR3(VTX_SIZE_X + BLOCK_SIZE - size, 0.0f, VTX_SIZE_Z);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBlock->Unlock();
	}
}

//=============================================================================
// �u���b�N�̓����|���S���Ƃ̓����蔻��
// pos0:�n�_�i�ړ��O�j
// pos1:�I�_�i�ړ���j
//=============================================================================
bool HitCheckBlock(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		pos[NUM_VERTEX]; // ���_���W�̕ۑ�
	bool			ans;
	VERTEX_3D	    *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < BLOCK_NUM_FEARSIDE; i++, pVtx += 4)
	{
		if (block[i].use)
		{
			// ���_���W�̐ݒ�
			pos[0] = pVtx[0].vtx;
			pos[1] = pVtx[1].vtx;
			pos[2] = pVtx[2].vtx;
			pos[3] = pVtx[3].vtx;

			// �������|���S���Ɛ����̓����蔻��
			ans = CheckHitPolygon(pos[0], pos[2], pos[1], pos0, pos1);
			if (!ans)
			{
				break;
			}
			// �E�㑤�|���S���Ɛ����̓����蔻��
			ans = CheckHitPolygon(pos[3], pos[1], pos[2], pos0, pos1);
			if (!ans)
			{
				break;
			}
		}
	}

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffBlock->Unlock();

	return ans;
}

//=============================================================================
// 4�p�Ƃ̓����蔻��
// ���@���FD3DXVECTOR3 pos0(����Ώۂ̈ʒu�j
// �߂�l�Fbool�^�@true���Ɠ������Ă���Afalse���Ɠ������Ă��Ȃ�
//=============================================================================
bool HitCheckCornerBlock(D3DXVECTOR3 pos0)
{
	if (CheckHitBB(pos0, block[0].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(20.0f, 25.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[11].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(20.0f, 25.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[42].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(20.0f, 25.0f, 20.0f))) return true;

	if (CheckHitBB(pos0, block[53].pos,
		D3DXVECTOR3(25.0f, 100.0f, 25.0f), D3DXVECTOR3(20.0f, 25.0f, 20.0f))) return true;

	return false;
}

//================================================================================
// �u���b�N�ւ̃_���[�W����
// ���@���Fint bno(�u���b�N�̃A�h���X�ԍ�)
// �߂�l�F�Ȃ�
//================================================================================
void BlockDamageManager(int bno)
{
	block[bno].life -= BLOCK_DAMAGE;

	if (block[bno].life == 3)
	{
		//TextureNum = TEX_TYPE02;
	}
	else if (block[bno].life == 0)
	{
		block[bno].use = false;
		SetExplosion(block[bno].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
		SetItem(block[bno].pos);
	}
}
