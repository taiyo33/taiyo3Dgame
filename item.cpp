//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "item.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "hitEffect.h"
#include "checkhit.h"
#include "explosion.h"
#include "bullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	ITEM_TEXTURE001			"data/TEXTURE/hp_up.png"	// �ǂݍ��ރ��f����
#define	ITEM_TEXTURE002			"data/TEXTURE/charge_up.png"	// �ǂݍ��ރ��f����
#define	ITEM_TEXTURE003			"data/TEXTURE/speed_up.png"	// �ǂݍ��ރ��f����
#define TEXTURE_MAX				(5)
#define ITEM_VERTEX				(4)			// 
#define ITEM_SIZE				(15.0f)		// �A�C�e����1�ӂ̃T�C�Y
#define INIT_LIFE				(5)			// �����̏����l	

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);
void SetVertexItem(int Index, float fSizeX, float fSizeY);

enum {
	TEX_TYPE001,
	TEX_TYPE002,
	TEX_TYPE003
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffItem;						// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTextureItem[TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^

ITEM					itemWk[ITEM_MAX];		// �A�C�e���\����
//=============================================================================
// ����������
// ���@���Fint type(�ď������Q������l)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitItem(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ITEM *item = &itemWk[0];

	// ���_���̍쐬
	MakeVertexItem(pDevice);
	
	for (int i = 0; i < ITEM_MAX; i++)
	{
		item[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		item[i].use = false;		// �g�p�֏�����
		item[i].textureNum = TEX_TYPE002;
	}
	
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
							ITEM_TEXTURE001,	// �t�@�C���̖��O
						&D3DTextureItem[TEX_TYPE001]);	// �ǂݍ��ރ������[
		
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			ITEM_TEXTURE002,	// �t�@�C���̖��O
			&D3DTextureItem[TEX_TYPE002]);	// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
			ITEM_TEXTURE003,	// �t�@�C���̖��O
			&D3DTextureItem[TEX_TYPE003]);	// �ǂݍ��ރ������[

	}	
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureItem[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureItem[i]->Release();
			D3DTextureItem[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxView;
	ITEM *item = &itemWk[0];

	// Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for (int i = 0; i < ITEM_MAX; i++)
	{
		// ���C���e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (item[i].use)
		{
			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&item[i].mtxWorld);
		
			// �|���S���𐳖ʂɌ�����
			item[i].mtxWorld._11 = mtxView._11;
			item[i].mtxWorld._12 = mtxView._21;
			item[i].mtxWorld._13 = mtxView._31;
			item[i].mtxWorld._21 = mtxView._12;
			item[i].mtxWorld._22 = mtxView._22;
			item[i].mtxWorld._23 = mtxView._32;
			item[i].mtxWorld._31 = mtxView._13;
			item[i].mtxWorld._32 = mtxView._23;
			item[i].mtxWorld._33 = mtxView._33;

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, item[i].pos.x, item[i].pos.y, item[i].pos.z);
			D3DXMatrixMultiply(&item[i].mtxWorld, &item[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &item[i].mtxWorld);
			
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, D3DVtxBuffItem, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTextureItem[item[i].textureNum]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
		}
	}

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z��r����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);


}

//=============================================================================
// �A�C�e���̃A�h���X�擾
//=============================================================================
ITEM *GetItem(int bno)
{
	return &itemWk[bno];
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * ITEM_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffItem,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < ITEM_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-ITEM_SIZE, -ITEM_SIZE, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-ITEM_SIZE, ITEM_SIZE, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(ITEM_SIZE, -ITEM_SIZE, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(ITEM_SIZE, ITEM_SIZE, 0.0f);

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
		D3DVtxBuffItem->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexItem(int Index, float fSizeX, float fSizeY)
{
	ITEM *item = &itemWk[0];

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffItem->Unlock();
	}
}


//=============================================================================
// �A�C�e���̓����|���S���Ƃ̓����蔻��
// pos0:�n�_�i�ړ��O�j
// pos1:�I�_�i�ړ���j
//=============================================================================
void SetItem(D3DXVECTOR3 pos)
{
	ITEM *item = &itemWk[0];

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (!item[i].use)
		{
			if (rand() % 10 == 0)
			{
				item[i].use = true;
				item[i].pos = pos;
				item[i].textureNum = TEX_TYPE002;
				SetVertexItem(i, ITEM_SIZE, ITEM_SIZE);


				return;
			}
		}
	}
}


////=============================================================================
//// �A�C�e���̓����|���S���Ƃ̓����蔻��
//// pos0:�n�_�i�ړ��O�j
//// pos1:�I�_�i�ړ���j
////=============================================================================
//bool HitCheckItem(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	D3DXVECTOR3		pos[NUM_VERTEX]; // ���_���W�̕ۑ�
//	bool			ans;
//	VERTEX_3D	    *pVtx;
//
//	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//	D3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int i = 0; i < ITEM_MAX; i++, pVtx += 4)
//	{
//		if (item[i].use)
//		{
//			// ���_���W�̐ݒ�
//			pos[0] = pVtx[0].vtx;
//			pos[1] = pVtx[1].vtx;
//			pos[2] = pVtx[2].vtx;
//			pos[3] = pVtx[3].vtx;
//
//			// �������|���S���Ɛ����̓����蔻��
//			ans = CheckHitPolygon(pos[0], pos[2], pos[3], pos0, pos1);
//			if (!ans)
//			{
//				break;
//			}
//			// �E�㑤�|���S���Ɛ����̓����蔻��
//			ans = CheckHitPolygon(pos[0], pos[3], pos[1], pos0, pos1);
//			if (!ans)
//			{
//				break;
//			}
//		}
//	}
//
//	// ���_�f�[�^���A�����b�N����
//	D3DVtxBuffItem->Unlock();
//
//	return ans;
//}
