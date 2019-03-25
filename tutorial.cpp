//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "field.h"
#include "player.h"
#include "title.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_TUTORIAL1	("data/TEXTURE/tutorial.png")		// �ǂݍ��މ摜
#define TEXTURE_SIZE_X01	(1090)  // �e�N�X�`���T�C�Y
#define TEXTURE_SIZE_Y01	(800)	// 
#define POS_X01				(400)	// ����X���W 
#define POS_Y01				(150)	// ����Y���W

#define TEXTURE_SIZE_X02	(200)	// �e�N�X�`���T�C�Y
#define TEXTURE_SIZE_Y02	(100)	// 
#define POS_X02				(790)	// ����X���W 
#define POS_Y02				(960)	// ����Y���W

#define TEXTURE_SIZE_X03	(75)	// �e�N�X�`���T�C�Y
#define TEXTURE_SIZE_Y03	(75)	// 
#define POS_X03				(1010)	// ����X���W 
#define POS_Y03				(970)	// ����Y���W

#define TEXTURE_MAX			(3)		// �e�N�X�`���̍ő吔 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTutorial(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureTutorial[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];	// ���_���i�[���[�N

static int					TextureNum;							// �e�N�X�`���ԍ�

//=============================================================================
// ����������
// ���@���Fint type(�ď���������2������ϐ�)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitTutorial(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TUTORIAL1,						// �t�@�C���̖��O
			&D3DTextureTutorial[TEX_NUM001]);		// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ENTHER,							// �t�@�C���̖��O
			&D3DTextureTutorial[TEX_NUM002]);		// �ǂݍ��ރ������[
		
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BUTTON,						// �t�@�C���̖��O
			&D3DTextureTutorial[TEX_NUM003]);		// �ǂݍ��ރ������[
	}

	// �e�N�X�`���ԍ��̏�����
	TextureNum = TEX_NUM001;

	// ���_���̍쐬
	MakeVertexTutorial();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTutorial(void)
{
	if (D3DTextureTutorial[TextureNum] != NULL)
	{	// �e�N�X�`���̊J��
		D3DTextureTutorial[TextureNum]->Release();
		D3DTextureTutorial[TextureNum] = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTutorial(void)
{
	// �X�e�[�W�J��
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		StopSound(*GetTitleSound());
		SetStage(STARTCALL);
	}
	else if (IsButtonTriggered(P1, BUTTON_A))
	{
		StopSound(*GetTitleSound());
		SetStage(STARTCALL);
	}
	else if (IsButtonPressed(P2, BUTTON_B))
	{
		StopSound(*GetTitleSound());
		SetStage(STARTCALL);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureTutorial[TEX_NUM001]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk[TEX_NUM001], sizeof(VERTEX_2D));
	}
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureTutorial[TEX_NUM002]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk[TEX_NUM002], sizeof(VERTEX_2D));
	}
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureTutorial[TEX_NUM003]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk[TEX_NUM003], sizeof(VERTEX_2D));
	}

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	{
		// ���_���W�̐ݒ�
		vertexWk[TEX_NUM001][0].vtx = D3DXVECTOR3(POS_X01, POS_Y01, 0.0f);
		vertexWk[TEX_NUM001][1].vtx = D3DXVECTOR3(POS_X01 + TEXTURE_SIZE_X01, POS_Y01, 0.0f);
		vertexWk[TEX_NUM001][2].vtx = D3DXVECTOR3(POS_X01, POS_Y01 + TEXTURE_SIZE_Y01, 0.0f);
		vertexWk[TEX_NUM001][3].vtx = D3DXVECTOR3(POS_X01 + TEXTURE_SIZE_X01, POS_Y01 + TEXTURE_SIZE_Y01, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[TEX_NUM001][TEX_NUM001].rhw =
		vertexWk[TEX_NUM001][1].rhw =
		vertexWk[TEX_NUM001][2].rhw =
		vertexWk[TEX_NUM001][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[TEX_NUM001][TEX_NUM001].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TEX_NUM001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[TEX_NUM001][TEX_NUM001].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TEX_NUM001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TEX_NUM001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TEX_NUM001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	{
		// ���_���W�̐ݒ�
		vertexWk[TEX_NUM002][0].vtx = D3DXVECTOR3(POS_X02, POS_Y02, 0.0f);
		vertexWk[TEX_NUM002][1].vtx = D3DXVECTOR3(POS_X02 + TEXTURE_SIZE_X02, POS_Y02, 0.0f);
		vertexWk[TEX_NUM002][2].vtx = D3DXVECTOR3(POS_X02, POS_Y02 + TEXTURE_SIZE_Y02, 0.0f);
		vertexWk[TEX_NUM002][3].vtx = D3DXVECTOR3(POS_X02 + TEXTURE_SIZE_X02, POS_Y02 + TEXTURE_SIZE_Y02, 0.0f);

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
	{
		// ���_���W�̐ݒ�
		vertexWk[TEX_NUM003][0].vtx = D3DXVECTOR3(POS_X03, POS_Y03, 0.0f);
		vertexWk[TEX_NUM003][1].vtx = D3DXVECTOR3(POS_X03 + TEXTURE_SIZE_X03, POS_Y03, 0.0f);
		vertexWk[TEX_NUM003][2].vtx = D3DXVECTOR3(POS_X03, POS_Y03 + TEXTURE_SIZE_Y03, 0.0f);
		vertexWk[TEX_NUM003][3].vtx = D3DXVECTOR3(POS_X03 + TEXTURE_SIZE_X03, POS_Y03 + TEXTURE_SIZE_Y03, 0.0f);

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

	return S_OK;
}
