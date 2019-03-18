//=============================================================================
//
// �o���b�g�G�t�F�N�g���� [bulletEffect.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "bulletEffect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_01				"data/TEXTURE/bullet001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_02				"data/TEXTURE/bullet002.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLETEFFECT_SIZE_X		(25.0f)							// �r���{�[�h�̕�
#define	BULLETEFFECT_SIZE_Y		(25.0f)							// �r���{�[�h�̍���
#define MAX_BULLETEFFECT		(BULLETEFFECT_SET_MAX * BULLETEFFECT_ONESET_MAX)	// �o���b�g�G�t�F�N�g�̍ő吔
#define DEL_TIME				(10)							// �G�t�F�N�g�̎���
#define TEXTURE_MAX				(2)								// �e�N�X�`������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBulletEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBulletEffect(int index, float sizeX, float sizeY);
void SetDiffuseBulletEffect(int index, float val);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBulletEffect[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffBulletEffect = NULL;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

static int				CntFrame;								// �t���[����
BULLETEFFECT			BulletEffectWk[BULLETEFFECT_SET_MAX];	// BULLETEFFECT�\���̗p�ϐ�

//=============================================================================
// ����������
// ���@���Fint type(�ď������̂Q������ϐ�)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitBulletEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLETEFFECT *bulletEffect = &BulletEffectWk[0];

	// ���_���̍쐬
	MakeVertexBulletEffect(pDevice);

	// �ď��������͓ǂݍ��܂Ȃ�
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_01,					// �t�@�C���̖��O
			&D3DTextureBulletEffect[TEX_NUM001]);			// �ǂݍ��ރ������[

				// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_02,					// �t�@�C���̖��O
			&D3DTextureBulletEffect[TEX_NUM002]);			// �ǂݍ��ރ������[
	}

	for (int i = 0; i < BULLETEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < BULLETEFFECT_ONESET_MAX; j++)
		{
			bulletEffect[i].pos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			bulletEffect[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			bulletEffect[i].time[j] = DEL_TIME;
			bulletEffect[i].dif[j] = INIT_ALPHA;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBulletEffect(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBulletEffect[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureBulletEffect[i]->Release();
			D3DTextureBulletEffect[i] = NULL;
		}
	}

	if (D3DVtxBuffBulletEffect != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffBulletEffect->Release();
		D3DVtxBuffBulletEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBulletEffect(void)
{
	BULLETEFFECT *bulletEffect = &BulletEffectWk[0];

	for (int i = 0; i < BULLETEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < BULLETEFFECT_ONESET_MAX; j++)
		{
			if (bulletEffect[i].use[j])
			{
				bulletEffect[i].scl[j] -= D3DXVECTOR3(0.1f, 0.1f, 0.1f);

				bulletEffect[i].dif[j] -= 0.01f;		// ���߂̒l
				bulletEffect[i].time[j]--;		// �������Ԃ��f�N�������g

				// ���Ŏ��ԂɂȂ��������
				if (bulletEffect[i].time[j] % DEL_TIME == 0)
				{
					bulletEffect[i].dif[j] = INIT_ALPHA;
					bulletEffect[i].use[j] = false;
					bulletEffect[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBulletEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	BULLETEFFECT *bulletEffect = &BulletEffectWk[0];

	// Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < BULLETEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < BULLETEFFECT_ONESET_MAX; j++)
		{
			SetDiffuseBulletEffect(i, bulletEffect[i].dif[j]);

			// ���C���e�B���O�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// �ʏ�u�����h
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			if (bulletEffect[i].use[j])
			{
				// �r���[�}�g���b�N�X���擾
				mtxView = GetMtxView();

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&bulletEffect[i].mtxWorld);

				// �|���S���𐳖ʂɌ�����
				bulletEffect[i].mtxWorld._11 = mtxView._11;
				bulletEffect[i].mtxWorld._12 = mtxView._21;
				bulletEffect[i].mtxWorld._13 = mtxView._31;
				bulletEffect[i].mtxWorld._21 = mtxView._12;
				bulletEffect[i].mtxWorld._22 = mtxView._22;
				bulletEffect[i].mtxWorld._23 = mtxView._32;
				bulletEffect[i].mtxWorld._31 = mtxView._13;
				bulletEffect[i].mtxWorld._32 = mtxView._23;
				bulletEffect[i].mtxWorld._33 = mtxView._33;

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScale, bulletEffect[i].scl[j].x,
					bulletEffect[i].scl[j].y,
					bulletEffect[i].scl[j].z);
				D3DXMatrixMultiply(&bulletEffect[i].mtxWorld,
					&bulletEffect[i].mtxWorld, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, bulletEffect[i].pos[j].x,
					bulletEffect[i].pos[j].y,
					bulletEffect[i].pos[j].z);
				D3DXMatrixMultiply(&bulletEffect[i].mtxWorld,
					&bulletEffect[i].mtxWorld, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &bulletEffect[i].mtxWorld);

				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, D3DVtxBuffBulletEffect, 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureBulletEffect[i]);

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
			}
		}
	}

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);

	// Z��r����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBulletEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BULLETEFFECT,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffBulletEffect,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBulletEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < MAX_BULLETEFFECT; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			SetVertexBulletEffect(i, BULLETEFFECT_SIZE_X, BULLETEFFECT_SIZE_Y);

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		}
		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBulletEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Fint index(�A�h���X�ԍ�), folat sizeX(����), float sizeY(����)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexBulletEffect(int index, float sizeX, float sizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBulletEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (index * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-sizeX / 2, -sizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-sizeX / 2, sizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(sizeX / 2, -sizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(sizeX / 2, sizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBulletEffect->Unlock();
	}
}

//============================================================================
// �o���b�g�G�t�F�N�g�̓��ߏ���
// ���@���Fint index(�A�h���X�ԍ�), float val(���ߗ��̒l)
// �߂�l�F�� ��
//============================================================================
void SetDiffuseBulletEffect(int index, float val)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffBulletEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (index * 4);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffBulletEffect->Unlock();
	}
}

//==========================================================================
// �o���b�g�G�t�F�N�g�̐ݒu
// ���@���FD3DXVECTOR3 pos(�ʒu), int index(�g�o���b�g�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void SetBulletEffect(D3DXVECTOR3 pos, int index)
{
	BULLETEFFECT *bulletEffect = &BulletEffectWk[index];

	for (int i = 0; i < BULLETEFFECT_ONESET_MAX; i++)
	{
		if (!bulletEffect->use[i])
		{
			bulletEffect->use[i] = true;	// �g�p��
			bulletEffect->pos[i] = pos;		// �ʒu��ݒ�

			// ���_��ݒ�
			SetVertexBulletEffect(i, BULLETEFFECT_SIZE_X, BULLETEFFECT_SIZE_Y);

			return;
		}
	}
}
