//=============================================================================
//
// �n�ʏ��� [field.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "field.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/field003.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureField = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffField = NULL;	// ���_�o�b�t�@�ւ̃|�C���^


D3DXMATRIX				MtxWorldField;			// ���[���h�}�g���b�N�X
D3DXVECTOR3				PosField;				// ���݂̈ʒu
//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	HRESULT hr;

	PosField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FIELD,			// �t�@�C���̖��O
								&D3DTextureField);	// �ǂݍ��ރ������[
	}

	// ���_���̍쐬
	hr = MakeVertexField(pDevice);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
	if(D3DTextureField != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureField->Release();
		D3DTextureField = NULL;
	}

	if(D3DVtxBuffField != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffField->Release();
		D3DVtxBuffField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateField(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&MtxWorldField);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, PosField.x, PosField.y, PosField.z);
	D3DXMatrixMultiply(&MtxWorldField, &MtxWorldField, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &MtxWorldField);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, D3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, D3DTextureField);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffField,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{	
		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D			   *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-FIELD_SIZE_X, FIELD_SIZE_Y, FIELD_SIZE_Z);
		pVtx[1].vtx = D3DXVECTOR3(FIELD_SIZE_X, FIELD_SIZE_Y, FIELD_SIZE_Z);
		pVtx[2].vtx = D3DXVECTOR3(-FIELD_SIZE_X, FIELD_SIZE_Y, -FIELD_SIZE_Z);
		pVtx[3].vtx = D3DXVECTOR3(FIELD_SIZE_X, FIELD_SIZE_Y, -FIELD_SIZE_Z);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffField->Unlock();
	}

	return S_OK;
}
