//=============================================================================
//
// ���C�t�Q�[�W���� [lifeGauge.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "main.h"
#include "lifeGauge.h"
#include "player.h"
#include "ball.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_LIFEGAUGE1	("data/TEXTURE/gauge00.png")	// �O�g�摜
#define TEXTURE_LIFEGAUGE2	("data/TEXTURE/gauge_01.png")	// �΃Q�[�W�摜
#define TEXTURE_LIFEGAUGE3	("data/TEXTURE/gauge_02.png")	// ���F�Q�[�W�摜
#define TEXTURE_LIFEGAUGE4	("data/TEXTURE/gauge_03.png")	// �ԃQ�[�W�摜
#define LIFEGAUGE_RED_P1	(20.0f)							// P1�̃Q�[�W�F�ύX�l(��)
#define LIFEGAUGE_YELLOW_P1	(50.0f)							// P1�̃Q�[�W�F�ύX�l(���F)
#define LIFEGAUGE_RED_P2	(20.0f)							// P2�̃Q�[�W�F�ύX�l(��)
#define LIFEGAUGE_YELLOW_P2	(50.0f)							// P2�̃Q�[�W�F�ύX�l(���F)
#define TEXTURE_MAX			(7)								// �e�N�X�`���[�̍ő吔
#define LIFEGAUGE_MAX		(2)								// ���C�t�Q�[�W�̍ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexLifeGauge(void);
void SetVertexLifeGauge01(float val);
void SetTextureLifeGauge01(float val);
void SetLifeGaugeTextureType01(int index, float life);
void SetVertexLifeGauge02(float val);
void SetTextureLifeGauge02(float val);
void SetLifeGaugeTextureType02(int index, float life);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureLifeGauge[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D	VertexWk[TEXTURE_MAX][NUM_VERTEX];	// ���_���i�[���[�N

int					TextureNumLifeGauge[LIFEGAUGE_MAX];	// ���C�t�Q�[�W�̃e�N�X�`���ԍ�
int					TextureNumIcon[LIFEGAUGE_MAX];		// �A�C�R���̃e�N�X�`���ԍ�
bool				LifeGaugeUse[LIFEGAUGE_MAX];		// �g�p���
//===============================================================================
// ����������
// ���@���Fint type(�ď�������2������l)
// �߂�l�FHRESULT�^
//===============================================================================
HRESULT InitLifeGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_LIFEGAUGE1,						// �t�@�C���̖��O
			&D3DTextureLifeGauge[LIFEGAUGE001]);	// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_LIFEGAUGE2,						// �t�@�C���̖��O
			&D3DTextureLifeGauge[LIFEGAUGE002]);	// �ǂݍ��ރ������[
		
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_LIFEGAUGE3,						// �t�@�C���̖��O
			&D3DTextureLifeGauge[LIFEGAUGE003]);	// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_LIFEGAUGE4,						// �t�@�C���̖��O
			&D3DTextureLifeGauge[LIFEGAUGE004]);	// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ICON01,							// �t�@�C���̖��O
			&D3DTextureLifeGauge[PLAYERICON01]);	// �ǂݍ��ރ������[
			
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ICON02,							// �t�@�C���̖��O
			&D3DTextureLifeGauge[PLAYERICON02]);	// �ǂݍ��ރ������[

				// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ICON03,							// �t�@�C���̖��O
			&D3DTextureLifeGauge[NPCICON]);			// �ǂݍ��ރ������[

	}

	for (int i = 0; i < LIFEGAUGE_MAX; i++)
	{
		TextureNumLifeGauge[i] = LIFEGAUGE002;
		LifeGaugeUse[i] = true;
	}

	TextureNumIcon[P1] = PLAYERICON01;
	TextureNumIcon[P2] = PLAYERICON02;

	// ���_���̍쐬
	MakeVertexLifeGauge();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLifeGauge(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureLifeGauge[i] != NULL)
		{	// �e�N�X�`���̊J��
			D3DTextureLifeGauge[i]->Release();
			D3DTextureLifeGauge[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLifeGauge(void)
{
	float val;
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < LIFEGAUGE_MAX; i++, player++)
	{
		if (i == 0)
		{
			val = player->life / PLAYER_LIFE_MAX;
			SetLifeGaugeTextureType01(i, player->life);
			SetTextureLifeGauge01(val);
			SetVertexLifeGauge01(val);
		}
		else if (i == 1)
		{
			float life = PLAYER_LIFE_MAX - player->life;
			val = life / PLAYER_LIFE_MAX;
			SetLifeGaugeTextureType02(i, player->life);
			SetTextureLifeGauge02(val);
			SetVertexLifeGauge02(val);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLifeGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < LIFEGAUGE_MAX; i++)
	{
		if (LifeGaugeUse[i])
		{
			// �O�g
			{	// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureLifeGauge[LIFEGAUGE001]);

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWk[LIFEGAUGE001 + i], sizeof(VERTEX_2D));
			}

			// ���g
			{	// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureLifeGauge[TextureNumLifeGauge[i]]);

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWk[LIFEGAUGE003 + i], sizeof(VERTEX_2D));
			}
			
			// �A�C�R��
			{	// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTextureLifeGauge[TextureNumIcon[i]]);

				// �|���S���̕`��
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWk[PLAYERICON01 + i], sizeof(VERTEX_2D));
			}
		}
	}

	// ���e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexLifeGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �O�g
	{
		// ���_���W�̐ݒ�
		VertexWk[LIFEGAUGE001][0].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_01, LIFEGAUGE_OUTERPOS_Y_01, 0.0f);
		VertexWk[LIFEGAUGE001][1].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_01 + TEXTURE_LIFEGAUGE_OUTER_SIZE_X, LIFEGAUGE_OUTERPOS_Y_01, 0.0f);
		VertexWk[LIFEGAUGE001][2].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_01, LIFEGAUGE_OUTERPOS_Y_01 + TEXTURE_LIFEGAUGE_OUTER_SIZE_Y, 0.0f);
		VertexWk[LIFEGAUGE001][3].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_01 + TEXTURE_LIFEGAUGE_OUTER_SIZE_X, LIFEGAUGE_OUTERPOS_Y_01 + TEXTURE_LIFEGAUGE_OUTER_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWk[LIFEGAUGE001][0].rhw =
		VertexWk[LIFEGAUGE001][1].rhw =
		VertexWk[LIFEGAUGE001][2].rhw =
		VertexWk[LIFEGAUGE001][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWk[LIFEGAUGE001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWk[LIFEGAUGE001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[LIFEGAUGE001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[LIFEGAUGE001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[LIFEGAUGE001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// �O�g
	{
		// ���_���W�̐ݒ�
		VertexWk[LIFEGAUGE002][0].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_02,LIFEGAUGE_OUTERPOS_Y_02, 0.0f);
		VertexWk[LIFEGAUGE002][1].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_02 + TEXTURE_LIFEGAUGE_OUTER_SIZE_X,LIFEGAUGE_OUTERPOS_Y_02, 0.0f);
		VertexWk[LIFEGAUGE002][2].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_02,LIFEGAUGE_OUTERPOS_Y_02 + TEXTURE_LIFEGAUGE_OUTER_SIZE_Y, 0.0f);
		VertexWk[LIFEGAUGE002][3].vtx = D3DXVECTOR3(LIFEGAUGE_OUTERPOS_X_02 + TEXTURE_LIFEGAUGE_OUTER_SIZE_X,LIFEGAUGE_OUTERPOS_Y_02 + TEXTURE_LIFEGAUGE_OUTER_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWk[LIFEGAUGE002][0].rhw =
		VertexWk[LIFEGAUGE002][1].rhw =
		VertexWk[LIFEGAUGE002][2].rhw =
		VertexWk[LIFEGAUGE002][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWk[LIFEGAUGE002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWk[LIFEGAUGE002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[LIFEGAUGE002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[LIFEGAUGE002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[LIFEGAUGE002][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	
	// ���g
	{
		// ���_���W�̐ݒ�
		VertexWk[LIFEGAUGE003][0].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01, LIFEGAUGE_INNERPOS_Y_01, 0.0f);
		VertexWk[LIFEGAUGE003][1].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_X, LIFEGAUGE_INNERPOS_Y_01, 0.0f);
		VertexWk[LIFEGAUGE003][2].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01, LIFEGAUGE_INNERPOS_Y_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
		VertexWk[LIFEGAUGE003][3].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_X, LIFEGAUGE_INNERPOS_Y_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWk[LIFEGAUGE003][0].rhw =
		VertexWk[LIFEGAUGE003][1].rhw =
		VertexWk[LIFEGAUGE003][2].rhw =
		VertexWk[LIFEGAUGE003][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWk[LIFEGAUGE003][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE003][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE003][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE003][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWk[LIFEGAUGE003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[LIFEGAUGE003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[LIFEGAUGE003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[LIFEGAUGE003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���g
	{
		// ���_���W�̐ݒ�
		VertexWk[LIFEGAUGE004][0].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02,LIFEGAUGE_INNERPOS_Y_02, 0.0f);
		VertexWk[LIFEGAUGE004][1].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_X,LIFEGAUGE_INNERPOS_Y_02, 0.0f);
		VertexWk[LIFEGAUGE004][2].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02,LIFEGAUGE_INNERPOS_Y_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
		VertexWk[LIFEGAUGE004][3].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_X,LIFEGAUGE_INNERPOS_Y_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWk[LIFEGAUGE004][0].rhw =
		VertexWk[LIFEGAUGE004][1].rhw =
		VertexWk[LIFEGAUGE004][2].rhw =
		VertexWk[LIFEGAUGE004][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWk[LIFEGAUGE004][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE004][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE004][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[LIFEGAUGE004][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWk[LIFEGAUGE004][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[LIFEGAUGE004][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[LIFEGAUGE004][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[LIFEGAUGE004][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// �A�C�R��
	{
		// ���_���W�̐ݒ�
		VertexWk[PLAYERICON01][0].vtx = D3DXVECTOR3(LIFEICON_POS_X_01, LIFEICON_POS_Y_01, 0.0f);
		VertexWk[PLAYERICON01][1].vtx = D3DXVECTOR3(LIFEICON_POS_X_01 + TEXTURE_LIFEICON_SIZE_X, LIFEICON_POS_Y_01, 0.0f);
		VertexWk[PLAYERICON01][2].vtx = D3DXVECTOR3(LIFEICON_POS_X_01, LIFEICON_POS_Y_01 + TEXTURE_LIFEICON_SIZE_Y, 0.0f);
		VertexWk[PLAYERICON01][3].vtx = D3DXVECTOR3(LIFEICON_POS_X_01 + TEXTURE_LIFEICON_SIZE_X, LIFEICON_POS_Y_01 + TEXTURE_LIFEICON_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWk[PLAYERICON01][0].rhw =
		VertexWk[PLAYERICON01][1].rhw =
		VertexWk[PLAYERICON01][2].rhw =
		VertexWk[PLAYERICON01][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWk[PLAYERICON01][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[PLAYERICON01][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[PLAYERICON01][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[PLAYERICON01][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWk[PLAYERICON01][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[PLAYERICON01][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[PLAYERICON01][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[PLAYERICON01][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// �A�C�R��
	{
		// ���_���W�̐ݒ�
		VertexWk[PLAYERICON02][0].vtx = D3DXVECTOR3(LIFEICON_POS_X_02, LIFEICON_POS_Y_02, 0.0f);
		VertexWk[PLAYERICON02][1].vtx = D3DXVECTOR3(LIFEICON_POS_X_02 + TEXTURE_LIFEICON_SIZE_X, LIFEICON_POS_Y_02, 0.0f);
		VertexWk[PLAYERICON02][2].vtx = D3DXVECTOR3(LIFEICON_POS_X_02, LIFEICON_POS_Y_02 + TEXTURE_LIFEICON_SIZE_Y, 0.0f);
		VertexWk[PLAYERICON02][3].vtx = D3DXVECTOR3(LIFEICON_POS_X_02 + TEXTURE_LIFEICON_SIZE_X, LIFEICON_POS_Y_02 + TEXTURE_LIFEICON_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWk[PLAYERICON02][0].rhw =
		VertexWk[PLAYERICON02][1].rhw =
		VertexWk[PLAYERICON02][2].rhw =
		VertexWk[PLAYERICON02][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWk[PLAYERICON02][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[PLAYERICON02][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[PLAYERICON02][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWk[PLAYERICON02][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWk[PLAYERICON02][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWk[PLAYERICON02][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWk[PLAYERICON02][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWk[PLAYERICON02][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// ���@���Ffloat val(�e�N�X�`����X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetTextureLifeGauge01(float val)
{
	VertexWk[LIFEGAUGE003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWk[LIFEGAUGE003][1].tex = D3DXVECTOR2(1.0f * (float)(val), 0.0f);
	VertexWk[LIFEGAUGE003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWk[LIFEGAUGE003][3].tex = D3DXVECTOR2(1.0f * (float)(val), 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Ffaloat val(���_��X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexLifeGauge01(float val)
{
	// ���_���W�̐ݒ�
	VertexWk[LIFEGAUGE003][0].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01, LIFEGAUGE_INNERPOS_Y_01, 0.0f);
	VertexWk[LIFEGAUGE003][1].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01 + (TEXTURE_LIFEGAUGE_INNER_SIZE_X * val), LIFEGAUGE_INNERPOS_Y_01, 0.0f);
	VertexWk[LIFEGAUGE003][2].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01, LIFEGAUGE_INNERPOS_Y_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
	VertexWk[LIFEGAUGE003][3].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_01 + (TEXTURE_LIFEGAUGE_INNER_SIZE_X * val), LIFEGAUGE_INNERPOS_Y_01 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// ���@���Ffloat val(�e�N�X�`����X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetTextureLifeGauge02(float val)
{
	VertexWk[LIFEGAUGE004][0].tex = D3DXVECTOR2((float)(val), 0.0f);
	VertexWk[LIFEGAUGE004][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWk[LIFEGAUGE004][2].tex = D3DXVECTOR2((float)(val), 1.0f);
	VertexWk[LIFEGAUGE004][3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Ffaloat val(���_��X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexLifeGauge02(float val)
{
	// ���_���W�̐ݒ�
	VertexWk[LIFEGAUGE004][0].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + (TEXTURE_LIFEGAUGE_INNER_SIZE_X * val), LIFEGAUGE_INNERPOS_Y_02, 0.0f);
	VertexWk[LIFEGAUGE004][1].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_X, LIFEGAUGE_INNERPOS_Y_02, 0.0f);
	VertexWk[LIFEGAUGE004][2].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + (TEXTURE_LIFEGAUGE_INNER_SIZE_X * val), LIFEGAUGE_INNERPOS_Y_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
	VertexWk[LIFEGAUGE004][3].vtx = D3DXVECTOR3(LIFEGAUGE_INNERPOS_X_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_X, LIFEGAUGE_INNERPOS_Y_02 + TEXTURE_LIFEGAUGE_INNER_SIZE_Y, 0.0f);
}

//=============================================================================
// P1�̃e�N�X�`���^�C�v�̐ݒ�
// ���@���Fint index(�e�N�X�`���^�C�v�̃A�h���X�ԍ�), faloat life(�v���C���[�̗̑�)
// �߂�l�F�ȁ@��
//=============================================================================
void SetLifeGaugeTextureType01(int index, float life)
{
	// �ԃQ�[�W�֕ω�
	if (life < LIFEGAUGE_RED_P1)
	{
		TextureNumLifeGauge[index] = LIFEGAUGE004;
	}
	// ���F�Q�[�W�֕ω�
	else if (life < LIFEGAUGE_YELLOW_P1)
	{
		TextureNumLifeGauge[index] = LIFEGAUGE003;
	}
	// �΃Q�[�W
	else
	{
		TextureNumLifeGauge[index] = LIFEGAUGE002;
	}
}

//=============================================================================
// P2�̃e�N�X�`���^�C�v�̐ݒ�
// ���@���Fint index(�e�N�X�`���^�C�v�̃A�h���X�ԍ�), faloat life(�v���C���[�̗̑�)
// �߂�l�F�ȁ@��
//=============================================================================
void SetLifeGaugeTextureType02(int index, float life)
{
	// �ԃQ�[�W�֕ω�
	if (life < LIFEGAUGE_RED_P2)
	{
		TextureNumLifeGauge[index] = LIFEGAUGE004;
	}
	// ���F�Q�[�W�֕ω�
	else if (life < LIFEGAUGE_YELLOW_P2)
	{
		TextureNumLifeGauge[index] = LIFEGAUGE003;
	}
	// �΃Q�[�W
	else
	{
		TextureNumLifeGauge[index] = LIFEGAUGE002;
	}
}

//=============================================================================
// P2�̃A�C�R���̃e�N�X�`���^�C�v�̐ݒ�
// ���@���Fint index(�e�N�X�`���ԍ�)
// �߂�l�F�ȁ@��
//=============================================================================
void SetIconTextureType(int index)
{
	TextureNumIcon[P2] = index;
}
