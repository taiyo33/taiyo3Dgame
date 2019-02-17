//=============================================================================
//
// �G�t�F�N�g���� [hitEffect.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "hitEffect.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "debugproc.h"
#include "child.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_HITEFFECT01		"data/TEXTURE/bullet001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	HITEFFECT_SIZE_X		(25.0f)							// �r���{�[�h�̕�
#define	HITEFFECT_SIZE_Y		(50.0f)							// �r���{�[�h�̍���
#define INIT_SPEED				(2.0f)
#define TEXTURE_MAX				(2)								// �e�N�X�`���[�̍ő吔					

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexHitEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexHitEffect(int Index, float fSizeX, float fSizeY);
void SetDiffuseHitEffect(int Index, float val);
void MoveHitEffect(int index, int hno);
void TyouseiHitEffect(int index, int hno);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureHitEffect[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DTVtxBuffHitEffect = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

HITEFFECT				hitEffectWk[HITEFFECT_SET_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitHitEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HITEFFECT *hitEffect = &hitEffectWk[0];

	// ���_���̍쐬
	MakeVertexHitEffect(pDevice);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_HITEFFECT01,					// �t�@�C���̖��O
			&D3DTextureHitEffect[TEX_NUM001]);			// �ǂݍ��ރ������[

	}

	for (int i = 0; i < HITEFFECT_SET_MAX ; i++)
	{
		hitEffect[i].use = false;

		for (int j = 0; j < HITEFFECT_ONESET_MAX; j++)
		{
			hitEffect[i].pos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			hitEffect[i].move[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			hitEffect[i].speed[j] = INIT_SPEED;
			hitEffect[i].dif_mi[j] = INIT_ALPHA;
			hitEffect[i].angle[j] = 0.0f;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHitEffect(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureHitEffect[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureHitEffect[i]->Release();
			D3DTextureHitEffect[i] = NULL;
		}
	}
	if(D3DTVtxBuffHitEffect != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DTVtxBuffHitEffect->Release();
		D3DTVtxBuffHitEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHitEffect(void)
{
	HITEFFECT *hitEffect = &hitEffectWk[0];

	for (int i = 0; i < HITEFFECT_SET_MAX; i++)
	{
		if (hitEffect[i].use)
		{
			for (int j = 0; j < HITEFFECT_ONESET_MAX; j++)
			{
				hitEffect[i].move[j].x = cosf(hitEffect[i].angle[j]) * hitEffect[i].speed[j];
				hitEffect[i].move[j].y = sinf(hitEffect[i].angle[j]) * hitEffect[i].speed[j];

				MoveHitEffect(i, j);
				
				TyouseiHitEffect(i, j);

				// ���Ŏ��ԂɂȂ��������
				if (hitEffect[i].speed[j] < 0.0f)
				{
					hitEffect[i].use = false;
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	HITEFFECT *hitEffect = &hitEffectWk[0];
	
	// Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < HITEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < HITEFFECT_ONESET_MAX; j++)
		{
			// ���ߏ���
			SetDiffuseHitEffect(i, hitEffect[i].dif_mi[j]);

			if (hitEffect[i].use)
			{
				// �r���[�}�g���b�N�X���擾
				mtxView = GetMtxView();

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&hitEffect[i].mtxWorld);

				// �|���S���𐳖ʂɌ�����
				hitEffect[i].mtxWorld._11 = mtxView._11;
				hitEffect[i].mtxWorld._12 = mtxView._21;
				hitEffect[i].mtxWorld._13 = mtxView._31;
				hitEffect[i].mtxWorld._21 = mtxView._12;
				hitEffect[i].mtxWorld._22 = mtxView._22;
				hitEffect[i].mtxWorld._23 = mtxView._32;
				hitEffect[i].mtxWorld._31 = mtxView._13;
				hitEffect[i].mtxWorld._32 = mtxView._23;
				hitEffect[i].mtxWorld._33 = mtxView._33;

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, hitEffect[i].pos[j].x,
					hitEffect[i].pos[j].y,
					hitEffect[i].pos[j].z);
				D3DXMatrixMultiply(&hitEffect[i].mtxWorld,
					&hitEffect[i].mtxWorld, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &hitEffect[i].mtxWorld);

				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, D3DTVtxBuffHitEffect, 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureHitEffect[TEX_NUM001]);

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
HRESULT MakeVertexHitEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * HITEFFECT_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DTVtxBuffHitEffect,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DTVtxBuffHitEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < HITEFFECT_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HITEFFECT_SIZE_X / 2, -HITEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-HITEFFECT_SIZE_X / 2, HITEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(HITEFFECT_SIZE_X / 2, -HITEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HITEFFECT_SIZE_X / 2, HITEFFECT_SIZE_Y / 2, 0.0f);

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
		D3DTVtxBuffHitEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexHitEffect(int Index, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DTVtxBuffHitEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DTVtxBuffHitEffect->Unlock();
	}
}

//============================================================================
//
//============================================================================
void SetDiffuseHitEffect(int Index, float val)
{	
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DTVtxBuffHitEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);
		
		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = 
		pVtx[1].diffuse = 
		pVtx[2].diffuse = 
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

		// ���_�f�[�^���A�����b�N����
		D3DTVtxBuffHitEffect->Unlock();
	}
}

//==========================================================================
// ��e�G�t�F�N�g�̐ݒu���� 
// ���@���FD3DXVECTOR3 pos(�����ʒu)�AD3DXVECTOR3 rot(��]��)�A
//		   float Dest(����)�Aint index(�g�A�h���X�ԍ�)
// �߂�l�G�ȁ@��
//==========================================================================
void SetHitEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float Dest)
{
	HITEFFECT *hitEffect = &hitEffectWk[0];

	for (int i = 0; i < HITEFFECT_SET_MAX; i++)
	{
		if (!hitEffect[i].use)
		{
			for (int j = 0; j < HITEFFECT_ONESET_MAX; j++)
			{
				hitEffect[i].use = true;	// �g�p��
				hitEffect[i].pos[j] = pos;
				hitEffect[i].dif_mi[j] = INIT_ALPHA;
				hitEffect[i].speed[j] = INIT_SPEED;
				SetVertexHitEffect(i, HITEFFECT_SIZE_X, HITEFFECT_SIZE_Y);
				hitEffect[i].angle[j] = atan2f(pos.y, pos.x);			// �p�x�v�Z
				hitEffect[i].angle[j] = hitEffect[i].angle[j] + (2.0f * D3DX_PI) * ((float)j / HITEFFECT_ONESET_MAX);	// 12���� 30�x�Ԋu
			}
		}

		return;
	}
}

//==========================================================================
// ��e�G�t�F�N�g�̈ړ����� 
// ���@���Fint index(��e�G�t�F�N�g�̃A�h���X�ԍ�)�Aint index(�g�̃A�h���X�ԍ�)
// �߂�l�G�ȁ@��
//==========================================================================
void MoveHitEffect(int index, int hno)
{
	HITEFFECT *hitEffect = &hitEffectWk[index];
	// �ʒu�ړ�
	hitEffect->pos[hno].x += hitEffect->move[hno].x;
	hitEffect->pos[hno].y += hitEffect->move[hno].y;
	hitEffect->pos[hno].z += hitEffect->move[hno].z;
}

//==========================================================================
// ��e�G�t�F�N�g�̈ړ����� 
// ���@���Fint index(��e�G�t�F�N�g�̃A�h���X�ԍ�)�Aint index(�g�̃A�h���X�ԍ�)
// �߂�l�G�ȁ@��
//==========================================================================
void TyouseiHitEffect(int index, int hno)
{
	HITEFFECT *hitEffect = &hitEffectWk[index];
	hitEffect->dif_mi[hno] -= 0.01f;	// ���߂̒l
	hitEffect->speed[hno] -= 0.1f;		// �ړ����x�̌���
}