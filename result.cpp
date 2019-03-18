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
#define TEXTURE_WINNER01	"data/TEXTURE/winner_logo01.png"	// �ǂݍ��ރe�N�X�`��
#define TEXTURE_WINNER02	"data/TEXTURE/winner_logo02.png"	// �ǂݍ��ރe�N�X�`��
#define TEXTURE_MAX			(5)									// �e�N�X�`���̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

enum RESULTLOGO{
	WINNER,
	ICON,
	RESULTLOGO_MAX
};

LPDIRECT3DTEXTURE9		D3DTextureResult[TEXTURE_MAX];			// �e�N�X�`���ւ̃|���S��
VERTEX_2D				VertexWkResult[RESULT_MAX][NUM_VERTEX];	// ���_���i�[���[�N
int						TextureNumResult;						// ���݂̃e�N�X�`���ԍ�
int						TextureNumIcon;							// ���݂̃A�C�R���ԍ�

//=============================================================================
// ����������
// ���@���Fint type(�ď���������2������ϐ�)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitResult(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_WINNER01,						// �t�@�C���̖��O
			&D3DTextureResult[TEX_NUM001]);			// �ǂݍ��ރ������̃|�C���^

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_WINNER02,						// �t�@�C���̖��O
			&D3DTextureResult[TEX_NUM002]);			// �ǂݍ��ރ������̃|�C���^

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_ICON01,					// �t�@�C���̖��O
			&D3DTextureResult[TEX_NUM003]);		// �ǂݍ��ރ������̃|�C���^
		
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_ICON02,					// �t�@�C���̖��O
			&D3DTextureResult[TEX_NUM004]);		// �ǂݍ��ރ������̃|�C���^
			
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_ICON03,					// �t�@�C���̖��O
			&D3DTextureResult[TEX_NUM005]);		// �ǂݍ��ރ������̃|�C���^
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
	PLAYER *player = GetPlayer(0);
	// �`���[�WSE�̒�~
	StopSound(player[P1].chargeSE);
	StopSound(player[P2].chargeSE);

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

	// WINNER���S�̕`��
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureResult[TextureNumResult]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RESULT, VertexWkResult[WINNER], sizeof(VERTEX_2D));
	}
	
	// ICON�̕`��
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureResult[TextureNumIcon + 2]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RESULT, VertexWkResult[ICON], sizeof(VERTEX_2D));
	}

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
		VertexWkResult[WINNER][0].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X, TEXTURE_WINNER_POS_Y, 0.0f);
		VertexWkResult[WINNER][1].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X + TEXTURE_WINNER_SIZE_X, TEXTURE_WINNER_POS_Y, 0.0f);
		VertexWkResult[WINNER][2].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X, TEXTURE_WINNER_POS_Y + TEXTURE_WINNER_SIZE_Y, 0.0f);
		VertexWkResult[WINNER][3].vtx = D3DXVECTOR3(TEXTURE_WINNER_POS_X + TEXTURE_WINNER_SIZE_X, TEXTURE_WINNER_POS_Y + TEXTURE_WINNER_SIZE_Y, 0.0f);

		// rhw�̐ݒ�
		VertexWkResult[WINNER][0].rhw =
		VertexWkResult[WINNER][1].rhw =
		VertexWkResult[WINNER][2].rhw =
		VertexWkResult[WINNER][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWkResult[WINNER][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkResult[WINNER][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkResult[WINNER][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkResult[WINNER][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWkResult[WINNER][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkResult[WINNER][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkResult[WINNER][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkResult[WINNER][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// �v���C���[�A�C�R��
	{
		// ���_���W�̐ݒ�
		VertexWkResult[ICON][0].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X, TEXTURE_ICON_POS_Y, 0.0f);
		VertexWkResult[ICON][1].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X + TEXTURE_ICON_SIZE_X, TEXTURE_ICON_POS_Y, 0.0f);
		VertexWkResult[ICON][2].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X, TEXTURE_ICON_POS_Y + TEXTURE_ICON_SIZE_Y, 0.0f);
		VertexWkResult[ICON][3].vtx = D3DXVECTOR3(TEXTURE_ICON_POS_X + TEXTURE_ICON_SIZE_X, TEXTURE_ICON_POS_Y + TEXTURE_ICON_SIZE_Y, 0.0f);

		// rhw�̐ݒ�
		VertexWkResult[ICON][0].rhw =
		VertexWkResult[ICON][1].rhw =
		VertexWkResult[ICON][2].rhw =
		VertexWkResult[ICON][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWkResult[ICON][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkResult[ICON][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkResult[ICON][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkResult[ICON][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWkResult[ICON][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkResult[ICON][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkResult[ICON][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkResult[ICON][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//===============================================================================
// ���U���g�ւ̑J�ڏ���
// ���@���Fint pno0(�������v���C���[�ԍ�),int pno1(�v���C���[��CPU��)
// �߂�l�F�ȁ@��
//===============================================================================
void SetResult(int pno0, int pno1)
{
	// �e���U���g�̕\���e�N�X�`���ԍ�
	TextureNumResult = pno0;
	TextureNumIcon = pno1;

	// ���U���g�֑J��
	SetStage(RESULT);
}