//=============================================================================
//
// �A�C�R������ [icon.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "icon.h"
#include "player.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_ICON01			"data/TEXTURE/player_icon01.png"	// �ǂݍ��ރe�N�X�`���[
#define TEXTURE_ICON02			"data/TEXTURE/player_icon02.png"	// �ǂݍ��ރe�N�X�`���[
#define ICON_TEXTURE_SIZE		(75)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexIcon(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9			D3DModelTexture;		// �e�N�X�`���ǂݍ��ݏꏊ
static LPDIRECT3DTEXTURE9			D3DTexture[ICON_MAX];	// �e�N�X�`���ǂݍ��ݏꏊ
static LPDIRECT3DVERTEXBUFFER9		D3DVtxBuff;				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

ICON								iconWk[ICON_MAX];
//=============================================================================
// ����������
// ���@���Fint type(�ď������Q������l)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitIcon(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ICON *icon = &iconWk[0];

	// ���_���̍쐬
	MakeVertexIcon(pDevice);

	for (int i = 0; i < ICON_MAX; i++)
	{
		icon[i].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
		icon[i].use = true;
	}


	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ICON01,		// �t�@�C���̖��O
			&D3DTexture[P1]);	// �ǂݍ��ރ������[

			// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ICON02,		// �t�@�C���̖��O
			&D3DTexture[P2]);	// �ǂݍ��ރ������[
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitIcon(void)
{
	for (int i = 0; i < ICON_MAX; i++)
	{
		if (D3DTexture[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTexture[i]->Release();
			D3DTexture[i] = NULL;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateIcon(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawIcon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxView;
	ICON *icon = &iconWk[0];
	PLAYER *player = GetPlayer(0);

	// Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < ICON_MAX; i++, player++)
	{
		if (icon[i].use)
		{
			mtxView = GetMtxView();

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&icon[i].mtxWorld);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, icon[i].pos.x, icon[i].pos.y, icon[i].pos.z);
			D3DXMatrixMultiply(&icon[i].mtxWorld, &icon[i].mtxWorld, &mtxTranslate);

			D3DXMatrixMultiply(&icon[i].mtxWorld, &icon[i].mtxWorld, &player->mtxWorld);

			// �|���S���𐳖ʂɌ�����
			icon[i].mtxWorld._11 = mtxView._11;
			icon[i].mtxWorld._12 = mtxView._21;
			icon[i].mtxWorld._13 = mtxView._31;
			icon[i].mtxWorld._21 = mtxView._12;
			icon[i].mtxWorld._22 = mtxView._22;
			icon[i].mtxWorld._23 = mtxView._32;
			icon[i].mtxWorld._31 = mtxView._13;
			icon[i].mtxWorld._32 = mtxView._23;
			icon[i].mtxWorld._33 = mtxView._33;

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &icon[i].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, D3DVtxBuff, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTexture[i]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * ICON_MAX), NUM_POLYGON);
		}
	}

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z��r����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//===============================================================================
// ���_���̍쐬
//===============================================================================
HRESULT MakeVertexIcon(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * ICON_MAX ,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuff,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < ICON_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-ICON_TEXTURE_SIZE / 2, ICON_TEXTURE_SIZE / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(ICON_TEXTURE_SIZE / 2, ICON_TEXTURE_SIZE / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-ICON_TEXTURE_SIZE / 2, -ICON_TEXTURE_SIZE / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(ICON_TEXTURE_SIZE / 2, -ICON_TEXTURE_SIZE / 2, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}
		// ���_�f�[�^���A�����b�N����
		D3DVtxBuff->Unlock();
	}

	return S_OK;
}
