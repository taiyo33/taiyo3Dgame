//=============================================================================
//
// �`���[�W�G�t�F�N�g���� [chargeEffect.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "chargeEffect.h"
#include "ball.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_01				"data/TEXTURE/bullet001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_02				"data/TEXTURE/bullet002.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	CHARGEEFFECT_SIZE_X		(50.0f)							// �r���{�[�h�̕�
#define	CHARGEEFFECT_SIZE_Y		(50.0f)							// �r���{�[�h�̍���
#define TEXTURE_MAX				(2)								// �e�N�X�`���[�̍ő吔					

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexChargeEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexChargeEffect(int index, float sizeX, float sizeY);
void SetDiffuseChargeEffect(int index, float val);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureChargeEffect[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DTVtxBuffChargeEffect = NULL;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^


CHARGEEFFECT			ChargeEffectWk[CHARGEEFFECT_SET_MAX];
//=============================================================================
// ����������
// ���@���Fint type(�ď���������2������ϐ�)
// �߂�l�F
//=============================================================================
HRESULT InitChargeEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CHARGEEFFECT *chargeEffect = &ChargeEffectWk[0];

	// ���_���̍쐬
	MakeVertexChargeEffect(pDevice);

	// �ď��������͓ǂݍ��܂Ȃ�
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_01,					// �t�@�C���̖��O
			&D3DTextureChargeEffect[TEX_NUM001]);			// �ǂݍ��ރ������[
				// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_02,					// �t�@�C���̖��O
			&D3DTextureChargeEffect[TEX_NUM002]);			// �ǂݍ��ރ������[
	}

	// �e�ϐ��̏���������
	for (int i = 0; i < CHARGEEFFECT_SET_MAX ; i++)
	{
		for (int j = 0; j < CHARGEEFFECT_ONESET_MAX; j++)
		{
			chargeEffect[i].use[j] = false;
			chargeEffect[i].pos[j] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			chargeEffect[i].scl[j] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			chargeEffect[i].dif[j] = INIT_ALPHA;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitChargeEffect(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureChargeEffect[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureChargeEffect[i]->Release();
			D3DTextureChargeEffect[i] = NULL;
		}
	}

	if(D3DTVtxBuffChargeEffect != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DTVtxBuffChargeEffect->Release();
		D3DTVtxBuffChargeEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateChargeEffect(void)
{
	CHARGEEFFECT *chargeEffect = &ChargeEffectWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < CHARGEEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < CHARGEEFFECT_ONESET_MAX; j++)
		{
			if (chargeEffect[i].use[j])
			{
				// �ʒu���v���C���[��
				chargeEffect[i].pos[j] = player[i].pos;	

				// �����̏ꍇ�G�t�F�N�g���g��
				if (j % 2 == 0)
				{
					// �g�債�ē���
					chargeEffect[i].scl[j] += D3DXVECTOR3(0.01f, 0.01f, 0.01f);
				}

				// ���ŏ���
				if (chargeEffect[i].dif[j] < 0.0f)
				{
					chargeEffect[i].use[j] = false;
				}

				// ���ߒl�����炷
				chargeEffect[i].dif[j] -= 0.01f;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawChargeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	CHARGEEFFECT *chargeEffect = &ChargeEffectWk[0];
	
	// Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < CHARGEEFFECT_SET_MAX; i++)
	{
		for (int j = 0; j < CHARGEEFFECT_ONESET_MAX; j++)
		{
			// ���ߏ���
			SetDiffuseChargeEffect(i, chargeEffect[i].dif[j]);

			if (chargeEffect[i].use[j])
			{
				// �r���[�}�g���b�N�X���擾
				mtxView = GetMtxView();

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&chargeEffect[i].mtxWorld);

				// �|���S���𐳖ʂɌ�����
				chargeEffect[i].mtxWorld._11 = mtxView._11;
				chargeEffect[i].mtxWorld._12 = mtxView._21;
				chargeEffect[i].mtxWorld._13 = mtxView._31;
				chargeEffect[i].mtxWorld._21 = mtxView._12;
				chargeEffect[i].mtxWorld._22 = mtxView._22;
				chargeEffect[i].mtxWorld._23 = mtxView._32;
				chargeEffect[i].mtxWorld._31 = mtxView._13;
				chargeEffect[i].mtxWorld._32 = mtxView._23;
				chargeEffect[i].mtxWorld._33 = mtxView._33;

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScale, chargeEffect[i].scl[j].x, chargeEffect[i].scl[j].y, chargeEffect[i].scl[j].z);
				D3DXMatrixMultiply(&chargeEffect[i].mtxWorld, &chargeEffect[i].mtxWorld, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, chargeEffect[i].pos[j].x,
					chargeEffect[i].pos[j].y,
					chargeEffect[i].pos[j].z);
				D3DXMatrixMultiply(&chargeEffect[i].mtxWorld,
					&chargeEffect[i].mtxWorld, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &chargeEffect[i].mtxWorld);

				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, D3DTVtxBuffChargeEffect, 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureChargeEffect[i]);

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
HRESULT MakeVertexChargeEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_CHARGEEFFECT,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DTVtxBuffChargeEffect,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DTVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < MAX_CHARGEEFFECT; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-CHARGEEFFECT_SIZE_X / 2, -CHARGEEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-CHARGEEFFECT_SIZE_X / 2, CHARGEEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(CHARGEEFFECT_SIZE_X / 2, -CHARGEEFFECT_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(CHARGEEFFECT_SIZE_X / 2, CHARGEEFFECT_SIZE_Y / 2, 0.0f);

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
		D3DTVtxBuffChargeEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Fint index(�A�h���X�ԍ�), float sizeX(����), float sizeY(����)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexChargeEffect(int index, float sizeX, float sizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DTVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (index * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-sizeX / 2, -sizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-sizeX / 2, sizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(sizeX / 2, -sizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(sizeX / 2, sizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DTVtxBuffChargeEffect->Unlock();
	}
}

//============================================================================
// ���_�J���[�̓��ߏ���
// ���@���Fint index(�A�h���X�ԍ�), float val(���߂̒l)
// �߂�l�F�ȁ@��
//============================================================================
void SetDiffuseChargeEffect(int index, float val)
{	
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DTVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (index * 4);
		
		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = 
		pVtx[1].diffuse = 
		pVtx[2].diffuse = 
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, val);

		// ���_�f�[�^���A�����b�N����
		D3DTVtxBuffChargeEffect->Unlock();
	}
}

//==========================================================================
// ��e�`���[�W�G�t�F�N�g�̐ݒu���� 
// ���@���FD3DXVECTOR3 pos(�����ʒu), int index(�g�A�h���X�ԍ�)
// �߂�l�G�ȁ@��
//==========================================================================
void SetChargeEffect(D3DXVECTOR3 pos, int index)
{
	CHARGEEFFECT *chargeEffect = &ChargeEffectWk[index];

	
	for (int i = 0; i < CHARGEEFFECT_ONESET_MAX; i++)
	{
		if (!chargeEffect->use[i])
		{
			// �e�ϐ���������
			chargeEffect->use[i] = true;
			chargeEffect->pos[i] = pos;
			chargeEffect->scl[i] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			chargeEffect->dif[i] = INIT_ALPHA;
			SetVertexChargeEffect(i, CHARGEEFFECT_SIZE_X, CHARGEEFFECT_SIZE_Y);

			return;
		}
	}
}
