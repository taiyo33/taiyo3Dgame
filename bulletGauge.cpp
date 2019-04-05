//=============================================================================
//
// �o���b�g�Q�[�W���� [BulletGauge.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "main.h"
#include "bulletGauge.h"
#include "bullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_BULLETGAUGE1	("data/TEXTURE/bullet_gauge.png")		// �Q�[�W�̊O�g
#define TEXTURE_BULLETGAUGE2	("data/TEXTURE/bullet_gauge_01.png")	// �Q�[�W�ΐF
#define BULLETGAUGE_MAX			(2)										// �Q�[�W�̐�
#define TEXTURE_MAX				(2)										// �e�N�X�`���̍ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBulletGauge(void);
void SetTextureBulletGauge(int index, float val);
void SetVertexBulletGauge(int index, float val);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureBulletGauge[TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];		// ���_���i�[���[�N

int							TexNumBulletGauge[BULLETGAUGE_MAX];		// �e�N�X�`���[�̎��
bool						BulletGaugeUse[BULLETGAUGE_MAX];		// �g�p���
//===============================================================================
// ����������
// ���@���Fint type(�ď�������2������l)
// �߂�l�FHRESULT�^
//===============================================================================
HRESULT InitBulletGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BULLETGAUGE1,						// �t�@�C���̖��O
			&D3DTextureBulletGauge[TEX_NUM001]);		// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BULLETGAUGE2,						// �t�@�C���̖��O
			&D3DTextureBulletGauge[TEX_NUM002]);		// �ǂݍ��ރ������[

	}

	// �e�ϐ��̏�����
	for (int i = 0; i < BULLETGAUGE_MAX; i++)
	{
		TexNumBulletGauge[i] = TEX_NUM004;
		BulletGaugeUse[i] = true;
	}

	// ���_���̍쐬
	MakeVertexBulletGauge();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBulletGauge(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBulletGauge[i] != NULL)
		{	// �e�N�X�`���̊J��
			D3DTextureBulletGauge[i]->Release();
			D3DTextureBulletGauge[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBulletGauge(void)
{
	BULLET *bullet = GetBullet(0);
	for (int i = 0; i < BULLETGAUGE_MAX; i++)
	{		
		// �`���[�W�����Q�[�W�֕ϊ�
       	float val = bullet[i].speedIncrease / BULLET_CHARGE_MAX;
		SetTextureBulletGauge(i, val);
		SetVertexBulletGauge(i, val);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBulletGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < BULLETGAUGE_MAX; i++)
	{
		if (BulletGaugeUse[i])
		{
			// �O�g
			{	// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureBulletGauge[TEX_NUM001]);

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[TEX_NUM001 + i], sizeof(VERTEX_2D));
			}

			// ���g
			{	// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureBulletGauge[TexNumBulletGauge[i]]);

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[TEX_NUM003 + i], sizeof(VERTEX_2D));
			}
		}
	}

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBulletGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �O�g
	{
		// ���_���W�̐ݒ�
		vertexWk[TEX_NUM001][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[TEX_NUM001][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[TEX_NUM001][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM001][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[TEX_NUM001][0].rhw =
			vertexWk[TEX_NUM001][1].rhw =
			vertexWk[TEX_NUM001][2].rhw =
			vertexWk[TEX_NUM001][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[TEX_NUM001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[TEX_NUM001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TEX_NUM001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TEX_NUM001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TEX_NUM001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// �O�g
	{
		// ���_���W�̐ݒ�
		vertexWk[TEX_NUM002][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[TEX_NUM002][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[TEX_NUM002][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM002][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[TEX_NUM002][0].rhw =
			vertexWk[TEX_NUM002][1].rhw =
			vertexWk[TEX_NUM002][2].rhw =
			vertexWk[TEX_NUM002][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[TEX_NUM002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[TEX_NUM002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TEX_NUM002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TEX_NUM002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TEX_NUM002][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���g
	{
		// ���_���W�̐ݒ�
		vertexWk[TEX_NUM003][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[TEX_NUM003][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01, 0.0f);
		vertexWk[TEX_NUM003][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM003][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[TEX_NUM003][0].rhw =
			vertexWk[TEX_NUM003][1].rhw =
			vertexWk[TEX_NUM003][2].rhw =
			vertexWk[TEX_NUM003][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[TEX_NUM003][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM003][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM003][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM003][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[TEX_NUM003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TEX_NUM003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TEX_NUM003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TEX_NUM003][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// ���g
	{
		// ���_���W�̐ݒ�
		vertexWk[TEX_NUM004][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[TEX_NUM004][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02, 0.0f);
		vertexWk[TEX_NUM004][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM004][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[TEX_NUM004][0].rhw =
			vertexWk[TEX_NUM004][1].rhw =
			vertexWk[TEX_NUM004][2].rhw =
			vertexWk[TEX_NUM004][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[TEX_NUM004][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM004][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM004][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM004][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[TEX_NUM004][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TEX_NUM004][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TEX_NUM004][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TEX_NUM004][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// ���@���Fint index(�Q�[�W�̃A�h���X), float val(�Q�[�W�̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetTextureBulletGauge(int index, float val)
{
	vertexWk[TEX_NUM003 + index][0].tex = D3DXVECTOR2(0.0f, 1.0f - (1.0f * val));
	vertexWk[TEX_NUM003 + index][1].tex = D3DXVECTOR2(1.0f, 1.0f - (1.0f * val));
	vertexWk[TEX_NUM003 + index][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[TEX_NUM003 + index][3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// ���_���̐ݒ�
// ���@���Fint index(�Q�[�W�̃A�h���X), float val(�Q�[�W�̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexBulletGauge(int index, float val)
{
	if (index == 0)
	{
		vertexWk[TEX_NUM003][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, (BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[TEX_NUM003][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, (BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[TEX_NUM003][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM003][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_01 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_01 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
	}
	else if (index == 1)
	{
		vertexWk[TEX_NUM004][0].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, (BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[TEX_NUM004][1].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, (BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y - (TEXTURE_BULLETGAUGE_SIZE_Y * val)), 0.0f);
		vertexWk[TEX_NUM004][2].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
		vertexWk[TEX_NUM004][3].vtx = D3DXVECTOR3(BULLETGAUGE_POS_X_02 + TEXTURE_BULLETGAUGE_SIZE_X, BULLETGAUGE_POS_Y_02 + TEXTURE_BULLETGAUGE_SIZE_Y, 0.0f);
	}
}
