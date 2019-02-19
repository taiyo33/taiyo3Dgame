//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "player.h"
#include "gameCall.h"
#include "lifeGauge.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WINNER01		"data/TEXTURE/winner_logo01.png"
#define TEXTURE_WINNER02		"data/TEXTURE/winner_logo02.png"
#define TEXTURE_MAX			(5)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

enum {
	WINNER,
	ICON
};

// �e�N�X�`���[�ԍ�
enum {
	TEX_WINNER01,
	TEX_WINNER02,
	TEX_PLAYERICON01,
	TEX_PLAYERICON02,
	TEX_NPCICON
};

LPDIRECT3DTEXTURE9		D3DTextureResult[TEXTURE_MAX];				// �e�N�X�`���ւ̃|���S��
VERTEX_2D				vertexWk_RESULT[RESULT_MAX][NUM_VERTEX];	// ���_���i�[���[�N
int						TextureNumResult;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_WINNER01,						// �t�@�C���̖��O
			&D3DTextureResult[TEX_WINNER01]);			// �ǂݍ��ރ������̃|�C���^

				// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_WINNER02,						// �t�@�C���̖��O
			&D3DTextureResult[TEX_WINNER02]);			// �ǂݍ��ރ������̃|�C���^

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_ICON01,					// �t�@�C���̖��O
			&D3DTextureResult[TEX_PLAYERICON01]);		// �ǂݍ��ރ������̃|�C���^
		
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_ICON02,					// �t�@�C���̖��O
			&D3DTextureResult[TEX_PLAYERICON02]);		// �ǂݍ��ރ������̃|�C���^
				// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_ICON03,					// �t�@�C���̖��O
			&D3DTextureResult[TEX_NPCICON]);		// �ǂݍ��ރ������̃|�C���^
	}


	// ���_���̍쐬
	MakeVertexResult();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureResult[i] != NULL)
		{	// �e�N�X�`���̊J��
			D3DTextureResult[i]->Release();
			D3DTextureResult[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	// �J��
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		StopSound(GetGameBGM03());
		SetStage(TITLE);
		InitGame();
	}
	else if (IsButtonTriggered(P1, BUTTON_A))
	{
		StopSound(GetGameBGM03());
		SetStage(TITLE);
		InitGame();
	}
	else if (IsButtonTriggered(P2, BUTTON_B))
	{
		StopSound(GetGameBGM03());
		SetStage(TITLE);
		InitGame();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, D3DTextureResult[TextureNumResult] );

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RESULT, vertexWk_RESULT[WINNER], sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, D3DTextureResult[TextureNumResult + 2]);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RESULT, vertexWk_RESULT[ICON], sizeof(VERTEX_2D));

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���҃e�N�X�`���[
	{
		// ���_���W�̐ݒ�
		vertexWk_RESULT[WINNER][0].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X, TEXTURE_WINNER_POS_Y, 0.0f);
		vertexWk_RESULT[WINNER][1].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X + TEXTURE_WINNER_SIZE_X, TEXTURE_WINNER_POS_Y, 0.0f);
		vertexWk_RESULT[WINNER][2].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X, TEXTURE_WINNER_POS_Y + TEXTURE_WINNER_SIZE_Y, 0.0f);
		vertexWk_RESULT[WINNER][3].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X + TEXTURE_WINNER_SIZE_X, TEXTURE_WINNER_POS_Y + TEXTURE_WINNER_SIZE_Y, 0.0f);

		// rhw�̐ݒ�
		vertexWk_RESULT[WINNER][0].rhw =
			vertexWk_RESULT[WINNER][1].rhw =
			vertexWk_RESULT[WINNER][2].rhw =
			vertexWk_RESULT[WINNER][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk_RESULT[WINNER][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[WINNER][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[WINNER][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[WINNER][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk_RESULT[WINNER][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk_RESULT[WINNER][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk_RESULT[WINNER][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk_RESULT[WINNER][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// �v���C���[�A�C�R��
	{
		// ���_���W�̐ݒ�
		vertexWk_RESULT[ICON][0].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X, TEXTURE_ICON_POS_Y, 0.0f);
		vertexWk_RESULT[ICON][1].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X + TEXTURE_ICON_SIZE_X, TEXTURE_ICON_POS_Y, 0.0f);
		vertexWk_RESULT[ICON][2].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X, TEXTURE_ICON_POS_Y + TEXTURE_ICON_SIZE_Y, 0.0f);
		vertexWk_RESULT[ICON][3].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X + TEXTURE_ICON_SIZE_X, TEXTURE_ICON_POS_Y + TEXTURE_ICON_SIZE_Y, 0.0f);

		// rhw�̐ݒ�
		vertexWk_RESULT[ICON][0].rhw =
			vertexWk_RESULT[ICON][1].rhw =
			vertexWk_RESULT[ICON][2].rhw =
			vertexWk_RESULT[ICON][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk_RESULT[ICON][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[ICON][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[ICON][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk_RESULT[ICON][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk_RESULT[ICON][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk_RESULT[ICON][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk_RESULT[ICON][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk_RESULT[ICON][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//===============================================================================
// ���U���g�ւ̑J�ڏ���
// ���@���Fint pno(�������v���C���[�ԍ�)
// �߂�l�F�ȁ@��
//===============================================================================
void SetResult(int pno)
{
	TextureNumResult = pno;

	SetStage(RESULT);
}