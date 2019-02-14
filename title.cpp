//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "player.h"
#include "ai.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOGOSPACE_ON				(1)		//
#define LOGOSPACE_OFF				(0)		//
#define ON_COUNT					(70)	//
#define OFF_COUNT					(30)	//
#define	TEXTURE_TITLE		("data/TEXTURE/title_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_ENTHER		("data/TEXTURE/push_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_GAMESTART	("data/TEXTURE/start_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_OPTION		("data/TEXTURE/option_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_EXIT		("data/TEXTURE/exit_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_SELECT		("data/TEXTURE/yajirusi.png")	// �ǂݍ��ރe�N�X�`���t�@�C����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(void);
void SetVertexSelect(int move);
void SetLogoTextureDiffuse(int ver);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureTitle = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTextureLogo[LOGO_MAX];	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		D3DTextureSelect;			// �e�N�X�`���ւ̃|�C���^


VERTEX_2D				vertexWkTitle[NUM_VERTEX];					// ���_���i�[���[�N
VERTEX_2D				vertexWkLogo[LOGO_MAX][NUM_VERTEX];		// ���_���i�[���[�N
VERTEX_2D				vertexWkSelect[NUM_VERTEX];				// ���_���i�[���[�N
static float			y_move;
static int				SelectNum;
static int				cnt_frame;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �Z���N�g�e�N�X�`���̈ړ���
	y_move = 0.0;
	cnt_frame = 0;

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE,					// �t�@�C���̖��O
			&D3DTextureTitle);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ENTHER,					// �t�@�C���̖��O
			&D3DTextureLogo[ENTHER]);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BUTTON,					// �t�@�C���̖��O
			&D3DTextureLogo[BUTTON]);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAMESTART,				// �t�@�C���̖��O
			&D3DTextureLogo[GAMESTART]);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_OPTION,					// �t�@�C���̖��O
			&D3DTextureLogo[OPTION]);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EXIT,					// �t�@�C���̖��O
			&D3DTextureLogo[EXIT]);			// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SELECT,					// �t�@�C���̖��O
			&D3DTextureSelect);				// �ǂݍ��ރ������[
	}
	
	// ���_���̍쐬
	MakeVertexTitle();
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if(D3DTextureTitle != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureTitle->Release();
		D3DTextureTitle = NULL;
	}

	for (int i = 0; i < LOGO_MAX; i++)
	{
		if (D3DTextureLogo[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureLogo[i]->Release();
			D3DTextureLogo[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{

	cnt_frame++;

	if (cnt_frame % OFF_COUNT == 0)
	{
		SetLogoTextureDiffuse(LOGOSPACE_OFF);
	}
	if (cnt_frame % ON_COUNT == 0)
	{
		SetLogoTextureDiffuse(LOGOSPACE_ON);
		cnt_frame = 0;
	}

	// �������փZ���N�g���ړ�
	if (GetKeyboardTrigger(DIK_DOWN)) 
	{
		SelectNum = (SelectNum + 1) % 3;
		SetVertexSelect(SelectNum);
	}
	else if (IsButtonTriggered(P1, BUTTON_DOWN))
	{
		SelectNum = (SelectNum + 1) % 3;
		SetVertexSelect(SelectNum);
	}
	else if (IsButtonTriggered(P2, BUTTON_DOWN))
	{
		SelectNum = (SelectNum + 1) % 3;
		SetVertexSelect(SelectNum);
	}

	// ������̃Z���N�g���ړ�
	if (GetKeyboardTrigger(DIK_UP))
	{
		SelectNum = (SelectNum + 2) % 3;
		SetVertexSelect(SelectNum);
	}
	else if (IsButtonTriggered(P1, BUTTON_UP))
	{
		SelectNum = (SelectNum + 2) % 3;
		SetVertexSelect(SelectNum);
	}
	else if (IsButtonTriggered(P2, BUTTON_UP))
	{
		SelectNum = (SelectNum + 2) % 3;
		SetVertexSelect(SelectNum);
	}


	// �Q�[���֑J��
	if (SelectNum == SELECT_START)
	{
		if ((GetKeyboardTrigger(DIK_RETURN)))
		{
			SetStage(TUTORIAL);
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{
			SetStage(TUTORIAL);
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			SetStage(TUTORIAL);
		}
	}
	// �R���t�B���O�֑J��
	else if ((GetKeyboardTrigger(DIK_RETURN)) && (SelectNum == SELECT_OPTION))
	{
			// �R���t�B���O
	}
	// �����I��
	else if ((GetKeyboardTrigger(DIK_RETURN)) && (SelectNum == SELECT_EXIT))
	{
		exit(0);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// �^�C�g���̔w�i�`��
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureTitle);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, vertexWkTitle, sizeof(VERTEX_2D));
	}

	// �Z���N�g�̕`��
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureSelect);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, vertexWkSelect, sizeof(VERTEX_2D));
	}

	// �Z���N�g���S�̕`��
	for (int i = 0; i < LOGO_MAX; i++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureLogo[i]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, vertexWkLogo[i], sizeof(VERTEX_2D));
	}

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	float set = 0.0;
	// �^�C�g���w�i
	{
		// ���_���W�̐ݒ�
		vertexWkTitle[0].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
		vertexWkTitle[1].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_SIZE_X, TITLE_POS_Y, 0.0f);
		vertexWkTitle[2].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_SIZE_Y, 0.0f);
		vertexWkTitle[3].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_SIZE_X, TITLE_POS_Y + TITLE_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWkTitle[0].rhw =
		vertexWkTitle[1].rhw =
		vertexWkTitle[2].rhw =
		vertexWkTitle[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWkTitle[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkTitle[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkTitle[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkTitle[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWkTitle[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkTitle[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkTitle[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkTitle[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// �v�b�V���L�[�̕\��
	{
		// ���_���W�̐ݒ�
		vertexWkLogo[ENTHER][0].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X01 , TITLE_LOGO_PUSHKEY_POS_Y01, 0.0f);
		vertexWkLogo[ENTHER][1].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X01 + TITLE_LOGO_PUSHKEY_SIZE_X01 , TITLE_LOGO_PUSHKEY_POS_Y01, 0.0f);
		vertexWkLogo[ENTHER][2].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X01 , TITLE_LOGO_PUSHKEY_POS_Y01 + TITLE_LOGO_PUSHKEY_SIZE_Y01, 0.0f);
		vertexWkLogo[ENTHER][3].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X01 + TITLE_LOGO_PUSHKEY_SIZE_X01 , TITLE_LOGO_PUSHKEY_POS_Y01 + TITLE_LOGO_PUSHKEY_SIZE_Y01, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWkLogo[ENTHER][0].rhw =
			vertexWkLogo[ENTHER][1].rhw =
			vertexWkLogo[ENTHER][2].rhw =
			vertexWkLogo[ENTHER][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWkLogo[ENTHER][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[ENTHER][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[ENTHER][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[ENTHER][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWkLogo[ENTHER][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkLogo[ENTHER][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkLogo[ENTHER][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkLogo[ENTHER][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_���W�̐ݒ�
	vertexWkLogo[BUTTON][0].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X02 , TITLE_LOGO_PUSHKEY_POS_Y02, 0.0f);
	vertexWkLogo[BUTTON][1].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X02 + TITLE_LOGO_PUSHKEY_SIZE_X02 , TITLE_LOGO_PUSHKEY_POS_Y02, 0.0f);
	vertexWkLogo[BUTTON][2].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X02 , TITLE_LOGO_PUSHKEY_POS_Y02 + TITLE_LOGO_PUSHKEY_SIZE_Y02, 0.0f);
	vertexWkLogo[BUTTON][3].vtx = D3DXVECTOR3(TITLE_LOGO_PUSHKEY_POS_X02 + TITLE_LOGO_PUSHKEY_SIZE_X02 , TITLE_LOGO_PUSHKEY_POS_Y02 + TITLE_LOGO_PUSHKEY_SIZE_Y02, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	vertexWkLogo[BUTTON][0].rhw =
		vertexWkLogo[BUTTON][1].rhw =
		vertexWkLogo[BUTTON][2].rhw =
		vertexWkLogo[BUTTON][3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWkLogo[BUTTON][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkLogo[BUTTON][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkLogo[BUTTON][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkLogo[BUTTON][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	vertexWkLogo[BUTTON][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkLogo[BUTTON][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkLogo[BUTTON][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkLogo[BUTTON][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �Z���N�g���S
	for (int j = PUSHKEY_MAX; j < LOGO_MAX; j++, set += SET_LOGO)
	{
		// ���_���W�̐ݒ�
		vertexWkLogo[j][0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + set, 0.0f);
		vertexWkLogo[j][1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_SIZE_X, TITLE_LOGO_POS_Y + set, 0.0f);
		vertexWkLogo[j][2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + TITLE_LOGO_SIZE_Y + set, 0.0f);
		vertexWkLogo[j][3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_SIZE_X, TITLE_LOGO_POS_Y + TITLE_LOGO_SIZE_Y + set, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWkLogo[j][0].rhw =
		vertexWkLogo[j][1].rhw =
		vertexWkLogo[j][2].rhw =
		vertexWkLogo[j][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWkLogo[j][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[j][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[j][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkLogo[j][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWkLogo[j][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkLogo[j][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkLogo[j][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkLogo[j][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// �Z���N�g
	{
		// ���_���W�̐ݒ�
		vertexWkSelect[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y, 0.0f);
		vertexWkSelect[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y, 0.0f);
		vertexWkSelect[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y, 0.0f);
		vertexWkSelect[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWkSelect[0].rhw =
		vertexWkSelect[1].rhw =
		vertexWkSelect[2].rhw =
		vertexWkSelect[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWkSelect[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkSelect[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkSelect[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkSelect[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWkSelect[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkSelect[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkSelect[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkSelect[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//============================================================================
// �Z���N�g�̐ݒu
// �����Fmove�i�Z���N�g���S�̎�ށj
//============================================================================
void SetVertexSelect(int move)
{
	// �Z���N�g�e�N�X�`���̈ړ��l
	y_move = (SELECT_MOVE * (float) move);

	// ���_���W�̐ݒ�
	vertexWkSelect[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y + y_move, 0.0f);
	vertexWkSelect[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y + y_move, 0.0f);
	vertexWkSelect[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y + y_move, 0.0f);
	vertexWkSelect[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_SIZE_X, TITLE_SELECT_POS_Y + TITLE_SELECT_SIZE_Y + y_move, 0.0f);
}

void SetLogoTextureDiffuse(int ver) 
{
	// ���ˌ��̐ݒ�
	vertexWkLogo[ENTHER][0].diffuse = 
	vertexWkLogo[ENTHER][1].diffuse = 
	vertexWkLogo[ENTHER][2].diffuse = 
	vertexWkLogo[ENTHER][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * ver);

	// ���ˌ��̐ݒ�
	vertexWkLogo[BUTTON][0].diffuse =
		vertexWkLogo[BUTTON][1].diffuse =
		vertexWkLogo[BUTTON][2].diffuse =
		vertexWkLogo[BUTTON][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * ver);

}