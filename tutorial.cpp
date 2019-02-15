//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "field.h"
#include "player.h"
#include "title.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_TUTORIAL1	("data/TEXTURE/tutorial02.png")		// ���s�p�摜
#define TEXTURE_TUTORIAL2	("data/TEXTURE/push_logo.png")		// ���s�p�摜
#define TEXTURE_TUTORIAL3	("data/TEXTURE/tutorial3.png")		// ���s�p�摜

enum {
	TUTORIAL001,
	TUTORIAL002,
	TUTORIAL003
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTutorial(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			D3DTextureTutorial[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D			vertexWk[TEXTURE_MAX][NUM_VERTEX];				// ���_���i�[���[�N

static int					TextureNum;							// 

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTutorial(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TUTORIAL1,						// �t�@�C���̖��O
			&D3DTextureTutorial[TUTORIAL001]);		// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TUTORIAL2,						// �t�@�C���̖��O
			&D3DTextureTutorial[TUTORIAL002]);		// �ǂݍ��ރ������[
		
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BUTTON,						// �t�@�C���̖��O
			&D3DTextureTutorial[TUTORIAL003]);		// �ǂݍ��ރ������[

	}

	TextureNum = TUTORIAL001;

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
		pDevice->SetTexture(0, D3DTextureTutorial[TUTORIAL001]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TUTORIAL, vertexWk[TUTORIAL001], sizeof(VERTEX_2D));
	}
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureTutorial[TUTORIAL002]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TUTORIAL, vertexWk[TUTORIAL002], sizeof(VERTEX_2D));
	}
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureTutorial[TUTORIAL003]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TUTORIAL, vertexWk[TUTORIAL003], sizeof(VERTEX_2D));
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
		vertexWk[TUTORIAL001][0].vtx = D3DXVECTOR3(TUTORIAL_POS_X01, TUTORIAL_POS_Y01, 0.0f);
		vertexWk[TUTORIAL001][1].vtx = D3DXVECTOR3(TUTORIAL_POS_X01 + TEXTURE_TUTORIAL_SIZE_X01, TUTORIAL_POS_Y01, 0.0f);
		vertexWk[TUTORIAL001][2].vtx = D3DXVECTOR3(TUTORIAL_POS_X01, TUTORIAL_POS_Y01 + TEXTURE_TUTORIAL_SIZE_Y01, 0.0f);
		vertexWk[TUTORIAL001][3].vtx = D3DXVECTOR3(TUTORIAL_POS_X01 + TEXTURE_TUTORIAL_SIZE_X01, TUTORIAL_POS_Y01 + TEXTURE_TUTORIAL_SIZE_Y01, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[TUTORIAL001][TUTORIAL001].rhw =
			vertexWk[TUTORIAL001][1].rhw =
			vertexWk[TUTORIAL001][2].rhw =
			vertexWk[TUTORIAL001][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[TUTORIAL001][TUTORIAL001].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TUTORIAL001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TUTORIAL001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TUTORIAL001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[TUTORIAL001][TUTORIAL001].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TUTORIAL001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TUTORIAL001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TUTORIAL001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	{
		// ���_���W�̐ݒ�
		vertexWk[TUTORIAL002][0].vtx = D3DXVECTOR3(TUTORIAL_POS_X02, TUTORIAL_POS_Y02, 0.0f);
		vertexWk[TUTORIAL002][1].vtx = D3DXVECTOR3(TUTORIAL_POS_X02 + TEXTURE_TUTORIAL_SIZE_X02, TUTORIAL_POS_Y02, 0.0f);
		vertexWk[TUTORIAL002][2].vtx = D3DXVECTOR3(TUTORIAL_POS_X02, TUTORIAL_POS_Y02 + TEXTURE_TUTORIAL_SIZE_Y02, 0.0f);
		vertexWk[TUTORIAL002][3].vtx = D3DXVECTOR3(TUTORIAL_POS_X02 + TEXTURE_TUTORIAL_SIZE_X02, TUTORIAL_POS_Y02 + TEXTURE_TUTORIAL_SIZE_Y02, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[TUTORIAL002][0].rhw =
			vertexWk[TUTORIAL002][1].rhw =
			vertexWk[TUTORIAL002][2].rhw =
			vertexWk[TUTORIAL002][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[TUTORIAL002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TUTORIAL002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TUTORIAL002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TUTORIAL002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[TUTORIAL002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TUTORIAL002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TUTORIAL002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TUTORIAL002][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	{
		// ���_���W�̐ݒ�
		vertexWk[TUTORIAL003][0].vtx = D3DXVECTOR3(TUTORIAL_POS_X03, TUTORIAL_POS_Y03, 0.0f);
		vertexWk[TUTORIAL003][1].vtx = D3DXVECTOR3(TUTORIAL_POS_X03 + TEXTURE_TUTORIAL_SIZE_X03, TUTORIAL_POS_Y03, 0.0f);
		vertexWk[TUTORIAL003][2].vtx = D3DXVECTOR3(TUTORIAL_POS_X03, TUTORIAL_POS_Y03 + TEXTURE_TUTORIAL_SIZE_Y03, 0.0f);
		vertexWk[TUTORIAL003][3].vtx = D3DXVECTOR3(TUTORIAL_POS_X03 + TEXTURE_TUTORIAL_SIZE_X03, TUTORIAL_POS_Y03 + TEXTURE_TUTORIAL_SIZE_Y03, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[TUTORIAL003][0].rhw =
			vertexWk[TUTORIAL003][1].rhw =
			vertexWk[TUTORIAL003][2].rhw =
			vertexWk[TUTORIAL003][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[TUTORIAL003][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TUTORIAL003][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TUTORIAL003][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWk[TUTORIAL003][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[TUTORIAL003][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[TUTORIAL003][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[TUTORIAL003][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[TUTORIAL003][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTutorial1(void)
{
	// �A�j���[�V�������Ȃ��e�N�X�`�����W�̐ݒ�
	int x = NULL;
	int y = NULL;
	float sizeX = 1.0f;
	float sizeY = 1.0f;

	vertexWk[0][0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[0][1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[0][2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[0][3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}
