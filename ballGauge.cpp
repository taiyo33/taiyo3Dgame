//=============================================================================
//
// �{�[���Q�[�W���� [ballGauge.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "main.h"
#include "ballGauge.h"
#include "player.h"
#include "ball.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_BALLGAUGE1	("data/TEXTURE/ball_gague.png")		// �ǂݍ��މ摜
#define TEXTURE_BALLGAUGE2	("data/TEXTURE/ball_gugae01.png")	// �ǂݍ��މ摜
#define TEXTURE_BALLGAUGE3	("data/TEXTURE/ball_gugae02.png")	// �ǂݍ��މ摜
#define TEXTURE_MAX			(3)									// �e�N�X�`���[�̍ő吔
#define BALLGAUGE_MAX		(2)									// ���Q�[�W�̍ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBallGauge(void);
void SetVertexBallGauge01(float val);
void SetTextureBallGauge01(float val);
void SetVertexBallGauge02(float val);
void SetTextureBallGauge02(float val);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureBallGauge[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D			VertexWk[TEXTURE_MAX][NUM_VERTEX];	// ���_���i�[���[�N

bool						BallGaugeUse[BALLGAUGE_MAX];		// �g�p��� 
//===============================================================================
// ����������
// ���@���Fint type(�ď�������2������l)
// �߂�l�FHRESULT�^
//===============================================================================
HRESULT InitBallGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BALLGAUGE1,						// �t�@�C���̖��O
			&D3DTextureBallGauge[TEX_NUM001]);		// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BALLGAUGE2,						// �t�@�C���̖��O
			&D3DTextureBallGauge[TEX_NUM002]);		// �ǂݍ��ރ������[
		
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BALLGAUGE3,						// �t�@�C���̖��O
			&D3DTextureBallGauge[TEX_NUM003]);		// �ǂݍ��ރ������[

	}

	// �ϐ��̏�����
	for (int i = 0; i < BALLGAUGE_MAX; i++)
	{
		BallGaugeUse[i] = true;
	}

	// ���_���̍쐬
	MakeVertexBallGauge();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBallGauge(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureBallGauge[i] != NULL)
		{	// �e�N�X�`���̊J��
			D3DTextureBallGauge[i]->Release();
			D3DTextureBallGauge[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBallGauge(void)
{
	float val;
	BALL *ball = GetBall(0);

	for (int i = 0; i < BALLGAUGE_MAX; i++, ball++)
	{
		
		// P�P�̃Q�[�W����
		if (i == 0)
		{
			// �{�[���̏��������Q�[�W�֕ϊ�
			val = (float)ball->cnt / BALL_ONESET_MAX; 
			SetTextureBallGauge01(val);
			SetVertexBallGauge01(val);
		}
		// P2�̃Q�[�W����
		else if (i == 1)
		{
			// �{�[���̏��������Q�[�W�֕ϊ�
			float cnt = BALL_ONESET_MAX - ball->cnt;
			val = cnt / BALL_ONESET_MAX;
			SetTextureBallGauge02(val);
			SetVertexBallGauge02(val);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBallGauge(void)
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
		pDevice->SetTexture(0, D3DTextureBallGauge[TEX_NUM001]);
		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWk[TEX_NUM001], sizeof(VERTEX_2D));
	}

	for (int i = 0; i < BALLGAUGE_MAX; i++)
	{
		{
			// ���g
			{	// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureBallGauge[TEX_NUM002 + i]);

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWk[TEX_NUM002 + i], sizeof(VERTEX_2D));
			}
			
		}
	}

	// ���e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBallGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �O�g
	{
		// ���_���W�̐ݒ�
		VertexWk[TEX_NUM001][0].vtx = D3DXVECTOR3(BALLGAUGE_OUTERPOS_X_01, BALLGAUGE_OUTERPOS_Y_01, 0.0f);
		VertexWk[TEX_NUM001][1].vtx = D3DXVECTOR3(BALLGAUGE_OUTERPOS_X_01 + TEXTURE_BALLGAUGE_OUTER_SIZE_X, BALLGAUGE_OUTERPOS_Y_01, 0.0f);
		VertexWk[TEX_NUM001][2].vtx = D3DXVECTOR3(BALLGAUGE_OUTERPOS_X_01, BALLGAUGE_OUTERPOS_Y_01 + TEXTURE_BALLGAUGE_OUTER_SIZE_Y, 0.0f);
		VertexWk[TEX_NUM001][3].vtx = D3DXVECTOR3(BALLGAUGE_OUTERPOS_X_01 + TEXTURE_BALLGAUGE_OUTER_SIZE_X, BALLGAUGE_OUTERPOS_Y_01 + TEXTURE_BALLGAUGE_OUTER_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWk[TEX_NUM001][0].rhw =
		VertexWk[TEX_NUM001][1].rhw =
		VertexWk[TEX_NUM001][2].rhw =
		VertexWk[TEX_NUM001][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWk[TEX_NUM001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWk[TEX_NUM001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[TEX_NUM001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[TEX_NUM001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[TEX_NUM001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���g
	{
		// ���_���W�̐ݒ�
		VertexWk[TEX_NUM002][0].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01, BALLGAUGE_INNERPOS_Y_01, 0.0f);
		VertexWk[TEX_NUM002][1].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01 + TEXTURE_BALLGAUGE_INNER_SIZE_X, BALLGAUGE_INNERPOS_Y_01, 0.0f);
		VertexWk[TEX_NUM002][2].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01, BALLGAUGE_INNERPOS_Y_01 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
		VertexWk[TEX_NUM002][3].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01 + TEXTURE_BALLGAUGE_INNER_SIZE_X, BALLGAUGE_INNERPOS_Y_01 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWk[TEX_NUM002][0].rhw =
		VertexWk[TEX_NUM002][1].rhw =
		VertexWk[TEX_NUM002][2].rhw =
		VertexWk[TEX_NUM002][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWk[TEX_NUM002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWk[TEX_NUM002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[TEX_NUM002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[TEX_NUM002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[TEX_NUM002][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���g
	{
		// ���_���W�̐ݒ�
		VertexWk[TEX_NUM003][0].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02,BALLGAUGE_INNERPOS_Y_02, 0.0f);
		VertexWk[TEX_NUM003][1].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + TEXTURE_BALLGAUGE_INNER_SIZE_X,BALLGAUGE_INNERPOS_Y_02, 0.0f);
		VertexWk[TEX_NUM003][2].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02,BALLGAUGE_INNERPOS_Y_02 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
		VertexWk[TEX_NUM003][3].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + TEXTURE_BALLGAUGE_INNER_SIZE_X,BALLGAUGE_INNERPOS_Y_02 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWk[TEX_NUM003][0].rhw =
		VertexWk[TEX_NUM003][1].rhw =
		VertexWk[TEX_NUM003][2].rhw =
		VertexWk[TEX_NUM003][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWk[TEX_NUM003][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM003][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM003][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[TEX_NUM003][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWk[TEX_NUM003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[TEX_NUM003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[TEX_NUM003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[TEX_NUM003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}


	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// ���@���Ffloat val(�e�N�X�`����X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetTextureBallGauge01(float val)
{
	VertexWk[TEX_NUM002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWk[TEX_NUM002][1].tex = D3DXVECTOR2(1.0f * (float)(val), 0.0f);
	VertexWk[TEX_NUM002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWk[TEX_NUM002][3].tex = D3DXVECTOR2(1.0f * (float)(val), 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Ffaloat val(���_��X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexBallGauge01(float val)
{
	// ���_���W�̐ݒ�
	VertexWk[TEX_NUM002][0].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01, BALLGAUGE_INNERPOS_Y_01, 0.0f);
	VertexWk[TEX_NUM002][1].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01 + (TEXTURE_BALLGAUGE_INNER_SIZE_X * val), BALLGAUGE_INNERPOS_Y_01, 0.0f);
	VertexWk[TEX_NUM002][2].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01, BALLGAUGE_INNERPOS_Y_01 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
	VertexWk[TEX_NUM002][3].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_01 + (TEXTURE_BALLGAUGE_INNER_SIZE_X * val), BALLGAUGE_INNERPOS_Y_01 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// ���@���Ffloat val(�e�N�X�`����X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetTextureBallGauge02(float val)
{
	VertexWk[TEX_NUM003][0].tex = D3DXVECTOR2((float)(val), 0.0f);
	VertexWk[TEX_NUM003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWk[TEX_NUM003][2].tex = D3DXVECTOR2((float)(val), 1.0f);
	VertexWk[TEX_NUM003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Ffaloat val(���_��X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexBallGauge02(float val)
{
	// ���_���W�̐ݒ�
	VertexWk[TEX_NUM003][0].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + (TEXTURE_BALLGAUGE_INNER_SIZE_X * val), BALLGAUGE_INNERPOS_Y_02, 0.0f);
	VertexWk[TEX_NUM003][1].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + TEXTURE_BALLGAUGE_INNER_SIZE_X, BALLGAUGE_INNERPOS_Y_02, 0.0f);
	VertexWk[TEX_NUM003][2].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + (TEXTURE_BALLGAUGE_INNER_SIZE_X * val), BALLGAUGE_INNERPOS_Y_02 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
	VertexWk[TEX_NUM003][3].vtx = D3DXVECTOR3(BALLGAUGE_INNERPOS_X_02 + TEXTURE_BALLGAUGE_INNER_SIZE_X, BALLGAUGE_INNERPOS_Y_02 + TEXTURE_BALLGAUGE_INNER_SIZE_Y, 0.0f);
}

