//=============================================================================
//
// �Q�[�W�G�t�F�N�g���� [gaugeEffect.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "gaugeEffect.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "debugproc.h"
#include "player.h"
#include "child.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_GAUGEEFFECT01		"data/TEXTURE/gague_effect01.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_GAUGEEFFECT02		"data/TEXTURE/gague_effect02.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define GAUGEEFFECT_MAX			(2)									// �ő吔
#define TEXTURE_PATTERN_X		(5)									// �e�N�X�`���[������X����
#define TEXTURE_PATTERN_Y		(1)									// �e�N�X�`���[������Y����
#define ANIM_TIME				(8)									// �A�j���[�V�����̊Ԋu
#define ANIM_PATTERN_NUM		(TEXTURE_PATTERN_X * TEXTURE_PATTERN_Y)	// 
#define TEXTURE_MAX				(2)									// �e�N�X�`���[�̍ő吔	

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexGaugeEffect(void);
void SetTextureGaugeEffect(int index, int cntPattern);
void SetVertexGaugeEffect01(float val);
void SetVertexGaugeEffect02(float val);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
enum {
	GAUGE_EFFECT01,
	GAUGE_EFFECT02
};

LPDIRECT3DTEXTURE9			D3DTextureGaugeEffect[TEXTURE_MAX];			// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];	// ���_���i�[���[�N
static int			PatternAnim[GAUGEEFFECT_MAX];
static int			CntAnim[GAUGEEFFECT_MAX];
static bool			UseAnim[GAUGEEFFECT_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitGaugeEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAUGEEFFECT01,					// �t�@�C���̖��O
			&D3DTextureGaugeEffect[GAUGE_EFFECT01]);			// �ǂݍ��ރ������[
		
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAUGEEFFECT02,					// �t�@�C���̖��O
			&D3DTextureGaugeEffect[GAUGE_EFFECT02]);			// �ǂݍ��ރ������[

	}

	// ���_���̍쐬
	MakeVertexGaugeEffect();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGaugeEffect(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureGaugeEffect != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureGaugeEffect[i]->Release();
			D3DTextureGaugeEffect[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGaugeEffect(void)
{
	float val = 0.0f;
	CHILD *child = GetChild(0);

	for (int i = 0; i < GAUGEEFFECT_MAX; i++, child++)
	{
		CntAnim[i]++;		// �A�j���[�V�����J�E���g�̍X�V 

		// �A�j���[�V����Wait�`�F�b�N
		if ((CntAnim[i] % ANIM_TIME) == 0)
		{
			// �p�^�[���̐؂�ւ�
			PatternAnim[i] = (PatternAnim[i] + 1) % ANIM_PATTERN_NUM;
			CntAnim[i] = 0;
		}

		if (i == 0)
		{
			val = (float)child->cnt / CHILD_ONESET_MAX;

			SetTextureGaugeEffect(i, PatternAnim[i]);	// �e�N�X�`���[���W�̌v�Z
			SetVertexGaugeEffect01(val);
		}
		else if(i == 1)
		{
			float cnt = CHILD_ONESET_MAX - child->cnt;
			val = cnt / CHILD_ONESET_MAX;

			SetTextureGaugeEffect(i, PatternAnim[i]);	// �e�N�X�`���[���W�̌v�Z
			SetVertexGaugeEffect02(val);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGaugeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	for (int i = 0; i < GAUGEEFFECT_MAX; i++)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureGaugeEffect[GAUGE_EFFECT01 + i]);
		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexWk[GAUGE_EFFECT01 + i], sizeof(VERTEX_2D));
	}

	// ���e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGaugeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	vertexWk[GAUGE_EFFECT01][0].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01, GAUGEEFFECT_POS_Y_01, 0.0f);
	vertexWk[GAUGE_EFFECT01][1].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_01, 0.0f);
	vertexWk[GAUGE_EFFECT01][2].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01, GAUGEEFFECT_POS_Y_01 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
	vertexWk[GAUGE_EFFECT01][3].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_01 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	vertexWk[GAUGE_EFFECT01][0].rhw =
		vertexWk[GAUGE_EFFECT01][1].rhw =
		vertexWk[GAUGE_EFFECT01][2].rhw =
		vertexWk[GAUGE_EFFECT01][3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[GAUGE_EFFECT01][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT01][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT01][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT01][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[GAUGE_EFFECT01][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[GAUGE_EFFECT01][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[GAUGE_EFFECT01][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[GAUGE_EFFECT01][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	vertexWk[GAUGE_EFFECT02][0].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02, GAUGEEFFECT_POS_Y_02, 0.0f);
	vertexWk[GAUGE_EFFECT02][1].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_02, 0.0f);
	vertexWk[GAUGE_EFFECT02][2].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02, GAUGEEFFECT_POS_Y_02 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
	vertexWk[GAUGE_EFFECT02][3].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 + TEXTURE_GAUGEEFFECT_SIZE_X, GAUGEEFFECT_POS_Y_02 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	vertexWk[GAUGE_EFFECT02][0].rhw =
		vertexWk[GAUGE_EFFECT02][1].rhw =
		vertexWk[GAUGE_EFFECT02][2].rhw =
		vertexWk[GAUGE_EFFECT02][3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[GAUGE_EFFECT02][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT02][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT02][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[GAUGE_EFFECT02][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[GAUGE_EFFECT02][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[GAUGE_EFFECT02][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[GAUGE_EFFECT02][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[GAUGE_EFFECT02][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// �����F�A�j���[�V�����̃p�^�[���J�E���g
//=============================================================================
void SetTextureGaugeEffect(int index, int cntPattern)
{
	// �A�j���[�V��������e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_X;
	int y = cntPattern / TEXTURE_PATTERN_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_Y;

	vertexWk[index][0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[index][1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[index][2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[index][3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Ffaloat val(���_��X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexGaugeEffect01(float val)
{
	// ���_���W�̐ݒ�
	vertexWk[GAUGE_EFFECT01][0].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + (680.0f * val), GAUGEEFFECT_POS_Y_01, 0.0f);
	vertexWk[GAUGE_EFFECT01][1].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + TEXTURE_GAUGEEFFECT_SIZE_X + (680.0f * val), GAUGEEFFECT_POS_Y_01, 0.0f);
	vertexWk[GAUGE_EFFECT01][2].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + (680.0f * val), GAUGEEFFECT_POS_Y_01 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
	vertexWk[GAUGE_EFFECT01][3].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_01 + TEXTURE_GAUGEEFFECT_SIZE_X + (680.0f * val), GAUGEEFFECT_POS_Y_01 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���Ffaloat val(���_��X���̕ϓ���)
// �߂�l�F�ȁ@��
//=============================================================================
void SetVertexGaugeEffect02(float val)
{
	// ���_���W�̐ݒ�
	vertexWk[GAUGE_EFFECT02][0].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 - TEXTURE_GAUGEEFFECT_SIZE_X + (680.0f * val), GAUGEEFFECT_POS_Y_02, 0.0f);
	vertexWk[GAUGE_EFFECT02][1].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 + (680.0f * val), GAUGEEFFECT_POS_Y_02, 0.0f);
	vertexWk[GAUGE_EFFECT02][2].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 - TEXTURE_GAUGEEFFECT_SIZE_X + (680.0f * val), GAUGEEFFECT_POS_Y_02 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
	vertexWk[GAUGE_EFFECT02][3].vtx = D3DXVECTOR3(GAUGEEFFECT_POS_X_02 + (680.0f * val), GAUGEEFFECT_POS_Y_02 + TEXTURE_GAUGEEFFECT_SIZE_Y, 0.0f);
}
