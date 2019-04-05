//=============================================================================
//
// �o�g���Q�[�W���� [buttleGauge.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "main.h"
#include "buttleGauge.h"
#include "player.h"
#include "ball.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_BUTTLEGAUGE1	("data/TEXTURE/buttle_gague.png")	// ���s�p�摜
#define TEXTURE_BUTTLEGAUGE2	("data/TEXTURE/buttle_gugae01.png")	// ���s�p�摜
#define TEXTURE_BUTTLEGAUGE3	("data/TEXTURE/buttle_gugae02.png")	// ���s�p�摜
#define BUTTLEGAUGE_VTX_MAX	(6)
#define TEXTURE_MAX			(3)
#define BUTTLEGAUGE_MAX		(2)

enum {
	BUTTLEGAUGE001,
	BUTTLEGAUGE002,
	BUTTLEGAUGE003,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexButtleGauge(void);
void SetVertexButtleGauge01(float val);
void SetTextureButtleGauge01(float val);
void SetVertexButtleGauge02(float val);
void SetTextureButtleGauge02(float val);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureButtleGauge[TEXTURE_MAX];		// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];	// ���_���i�[���[�N

bool						ButtleGaugeUse[BUTTLEGAUGE_MAX];		// 
//=============================================================================
// ����������
//=============================================================================
HRESULT InitButtleGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BUTTLEGAUGE1,						// �t�@�C���̖��O
			&D3DTextureButtleGauge[BUTTLEGAUGE001]);		// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BUTTLEGAUGE2,						// �t�@�C���̖��O
			&D3DTextureButtleGauge[BUTTLEGAUGE002]);		// �ǂݍ��ރ������[
		
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BUTTLEGAUGE3,						// �t�@�C���̖��O
			&D3DTextureButtleGauge[BUTTLEGAUGE003]);		// �ǂݍ��ރ������[

	}

	for (int i = 0; i < BUTTLEGAUGE_MAX; i++)
	{
		ButtleGaugeUse[i] = true;
	}

	// ���_���̍쐬
	MakeVertexButtleGauge();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitButtleGauge(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureButtleGauge[i] != NULL)
		{	// �e�N�X�`���̊J��
			D3DTextureButtleGauge[i]->Release();
			D3DTextureButtleGauge[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateButtleGauge(void)
{
	float val;
	BALL *ball = GetBall(0);

	for (int i = 0; i < BUTTLEGAUGE_MAX; i++, ball++)
	{
		val = (float)ball->cnt / BALL_ONESET_MAX; 
		
		if (i == 0)
		{
			SetTextureButtleGauge01(val);
			SetVertexButtleGauge01(val);
		}
		else if (i == 1)
		{
			float cnt = BALL_ONESET_MAX - ball->cnt;
			val = cnt / BALL_ONESET_MAX;

			SetTextureButtleGauge02(val);
			SetVertexButtleGauge02(val);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawButtleGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// �O�g
	{	// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureButtleGauge[BUTTLEGAUGE001]);
		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[BUTTLEGAUGE001], sizeof(VERTEX_2D));
	}

	for (int i = 0; i < BUTTLEGAUGE_MAX; i++)
	{
		{
			// ���g
			{	// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureButtleGauge[BUTTLEGAUGE002 + i]);

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[BUTTLEGAUGE002 + i], sizeof(VERTEX_2D));
			}
			
		}
	}

	// ���e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexButtleGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �O�g
	{
		// ���_���W�̐ݒ�
		vertexWk[BUTTLEGAUGE001][0].vtx = D3DXVECTOR3(BUTTLEGAUGE_OUTERPOS_X_01, BUTTLEGAUGE_OUTERPOS_Y_01, 0.0f);
		vertexWk[BUTTLEGAUGE001][1].vtx = D3DXVECTOR3(BUTTLEGAUGE_OUTERPOS_X_01 + TEXTURE_BUTTLEGAUGE_OUTER_SIZE_X, BUTTLEGAUGE_OUTERPOS_Y_01, 0.0f);
		vertexWk[BUTTLEGAUGE001][2].vtx = D3DXVECTOR3(BUTTLEGAUGE_OUTERPOS_X_01, BUTTLEGAUGE_OUTERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_OUTER_SIZE_Y, 0.0f);
		vertexWk[BUTTLEGAUGE001][3].vtx = D3DXVECTOR3(BUTTLEGAUGE_OUTERPOS_X_01 + TEXTURE_BUTTLEGAUGE_OUTER_SIZE_X, BUTTLEGAUGE_OUTERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_OUTER_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[BUTTLEGAUGE001][0].rhw =
			vertexWk[BUTTLEGAUGE001][1].rhw =
			vertexWk[BUTTLEGAUGE001][2].rhw =
			vertexWk[BUTTLEGAUGE001][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[BUTTLEGAUGE001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[BUTTLEGAUGE001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[BUTTLEGAUGE001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[BUTTLEGAUGE001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[BUTTLEGAUGE001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	
	// ���g
	{
		// ���_���W�̐ݒ�
		vertexWk[BUTTLEGAUGE002][0].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01, BUTTLEGAUGE_INNERPOS_Y_01, 0.0f);
		vertexWk[BUTTLEGAUGE002][1].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X, BUTTLEGAUGE_INNERPOS_Y_01, 0.0f);
		vertexWk[BUTTLEGAUGE002][2].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01, BUTTLEGAUGE_INNERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
		vertexWk[BUTTLEGAUGE002][3].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X, BUTTLEGAUGE_INNERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[BUTTLEGAUGE002][0].rhw =
			vertexWk[BUTTLEGAUGE002][1].rhw =
			vertexWk[BUTTLEGAUGE002][2].rhw =
			vertexWk[BUTTLEGAUGE002][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[BUTTLEGAUGE002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[BUTTLEGAUGE002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[BUTTLEGAUGE002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[BUTTLEGAUGE002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[BUTTLEGAUGE002][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���g
	{
		// ���_���W�̐ݒ�
		vertexWk[BUTTLEGAUGE003][0].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02,BUTTLEGAUGE_INNERPOS_Y_02, 0.0f);
		vertexWk[BUTTLEGAUGE003][1].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X,BUTTLEGAUGE_INNERPOS_Y_02, 0.0f);
		vertexWk[BUTTLEGAUGE003][2].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02,BUTTLEGAUGE_INNERPOS_Y_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
		vertexWk[BUTTLEGAUGE003][3].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X,BUTTLEGAUGE_INNERPOS_Y_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[BUTTLEGAUGE003][0].rhw =
			vertexWk[BUTTLEGAUGE003][1].rhw =
			vertexWk[BUTTLEGAUGE003][2].rhw =
			vertexWk[BUTTLEGAUGE003][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[BUTTLEGAUGE003][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE003][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE003][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[BUTTLEGAUGE003][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[BUTTLEGAUGE003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[BUTTLEGAUGE003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[BUTTLEGAUGE003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[BUTTLEGAUGE003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}


	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// ���@���Ffloat val(�e�N�X�`����X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetTextureButtleGauge01(float val)
{
	vertexWk[BUTTLEGAUGE002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[BUTTLEGAUGE002][1].tex = D3DXVECTOR2(1.0f * (float)(val), 0.0f);
	vertexWk[BUTTLEGAUGE002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[BUTTLEGAUGE002][3].tex = D3DXVECTOR2(1.0f * (float)(val), 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Ffaloat val(���_��X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexButtleGauge01(float val)
{
	// ���_���W�̐ݒ�
	vertexWk[BUTTLEGAUGE002][0].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01, BUTTLEGAUGE_INNERPOS_Y_01, 0.0f);
	vertexWk[BUTTLEGAUGE002][1].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01 + (TEXTURE_BUTTLEGAUGE_INNER_SIZE_X * val), BUTTLEGAUGE_INNERPOS_Y_01, 0.0f);
	vertexWk[BUTTLEGAUGE002][2].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01, BUTTLEGAUGE_INNERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
	vertexWk[BUTTLEGAUGE002][3].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_01 + (TEXTURE_BUTTLEGAUGE_INNER_SIZE_X * val), BUTTLEGAUGE_INNERPOS_Y_01 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// ���@���Ffloat val(�e�N�X�`����X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetTextureButtleGauge02(float val)
{
	vertexWk[BUTTLEGAUGE003][0].tex = D3DXVECTOR2((float)(val), 0.0f);
	vertexWk[BUTTLEGAUGE003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[BUTTLEGAUGE003][2].tex = D3DXVECTOR2((float)(val), 1.0f);
	vertexWk[BUTTLEGAUGE003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Ffaloat val(���_��X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexButtleGauge02(float val)
{
	// ���_���W�̐ݒ�
	vertexWk[BUTTLEGAUGE003][0].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + (TEXTURE_BUTTLEGAUGE_INNER_SIZE_X * val), BUTTLEGAUGE_INNERPOS_Y_02, 0.0f);
	vertexWk[BUTTLEGAUGE003][1].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X, BUTTLEGAUGE_INNERPOS_Y_02, 0.0f);
	vertexWk[BUTTLEGAUGE003][2].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + (TEXTURE_BUTTLEGAUGE_INNER_SIZE_X * val), BUTTLEGAUGE_INNERPOS_Y_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
	vertexWk[BUTTLEGAUGE003][3].vtx = D3DXVECTOR3(BUTTLEGAUGE_INNERPOS_X_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_X, BUTTLEGAUGE_INNERPOS_Y_02 + TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y, 0.0f);
}

