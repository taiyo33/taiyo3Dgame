//=============================================================================
//
// �|�[�Y��ʏ��� [pause.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "input.h"
#include "player.h"
#include "ai.h"
#include "lifeGauge.h"
#include "child.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PAUSE		("data/TEXTURE/menu.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_SELECT		("data/TEXTURE/bullet001.png")
#define	TEXTURE_RESTART		("data/TEXTURE/restart_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_REOPNE		("data/TEXTURE/reopne_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RETITLE		("data/TEXTURE/retitle_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define LOGO_MAX			(4)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPause(void);
void SetVertexPause(int move);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9				D3DTexturePause = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9				D3DTextureLogo[LOGO_MAX];	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9				D3DTextureSelect;			// �e�N�X�`���ւ̃|�C���^

VERTEX_2D						vertexWkPause[NUM_VERTEX];				// ���_���i�[���[�N
static VERTEX_2D						vertexWkLogo[LOGO_MAX][NUM_VERTEX];		// ���_���i�[���[�N
static VERTEX_2D						vertexWkSelect[NUM_VERTEX];				// ���_���i�[���[�N
static float					yMove;				// ���S�̈ʒu����
static int						SelectNum;			// �Z���N�g�̂���Ă��郍�S�ԍ�
bool							PauseUse;			// ���j���[����t���O
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPause(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �ϐ�������
	yMove = 0.0;
	SelectNum = 0;
	PauseUse = false;

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PAUSE,					// �t�@�C���̖��O
			&D3DTexturePause);				// �ǂݍ��ރ������[

				// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SELECT,					// �t�@�C���̖��O
			&D3DTextureSelect);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_RESTART,					// �t�@�C���̖��O
			&D3DTextureLogo[RESTART]);			// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_REOPNE,					// �t�@�C���̖��O
			&D3DTextureLogo[REOPNE]);			// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_RETITLE,					// �t�@�C���̖��O
			&D3DTextureLogo[RETITLE]);			// �ǂݍ��ރ������[
	}
	
	// ���_���̍쐬
	MakeVertexPause();
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{
	if(D3DTexturePause != NULL)
	{// �e�N�X�`���̊J��
		D3DTexturePause->Release();
		D3DTexturePause = NULL;
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
void UpdatePause(void)
{
	PLAYER *player = GetPlayer(0);

	// �������փZ���N�g���ړ�
	if (GetKeyboardTrigger(DIK_DOWN)) 
	{
		SelectNum = (SelectNum + 1) % 3;
		SetVertexPause(SelectNum);
	}
	else if (IsButtonTriggered(P1, BUTTON_DOWN))
	{

		SelectNum = (SelectNum + 1) % 3;
		SetVertexPause(SelectNum);
	}
	else if (IsButtonTriggered(P2, BUTTON_DOWN))
	{

		SelectNum = (SelectNum + 1) % 3;
		SetVertexPause(SelectNum);
	}

	// ������̃Z���N�g���ړ�
	if (GetKeyboardTrigger(DIK_UP))
	{

		SelectNum = (SelectNum + 2) % 3;
		SetVertexPause(SelectNum);
	}
	else if (IsButtonTriggered(P1, BUTTON_UP))
	{

		SelectNum = (SelectNum + 2) % 3;
		SetVertexPause(SelectNum);
	}
	else if (IsButtonTriggered(P2, BUTTON_UP))
	{

		SelectNum = (SelectNum + 2) % 3;
		SetVertexPause(SelectNum);
	}

	if ((GetKeyboardTrigger(DIK_M)))
	{
		SetPause(false);
	}
	else if ((GetKeyboardTrigger(DIK_V)))
	{
		SetPause(false);
	}
	else if (IsButtonPressed(P1, BUTTON_R))
	{
		SetPause(false);
	}
	else if (IsButtonPressed(P2, BUTTON_R))
	{
		SetPause(false);
	}


	// �ĊJ��
	if (SelectNum == SELECT_RESTART)
	{
		if ((GetKeyboardTrigger(DIK_RETURN)))
		{
			SetPause(false);
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{
			SetPause(false);
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			SetPause(false);
		}
	}
	// ��蒼����
	else if (SelectNum == SELECT_REOPNE)
	{
		if ((GetKeyboardTrigger(DIK_RETURN)))
		{
			SetStage(STARTCALL);
			SetPause(false);
			if (player[P2].npc)
			{
				InitGame();
				player[P2].npc = true;
				SetIconTextureType(NPCICON);
			}
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{
			SetStage(STARTCALL);
			SetPause(false);
			if (player[P2].npc)
			{
				InitGame();
				player[P2].npc = true;
				SetIconTextureType(NPCICON);
			}
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			SetStage(STARTCALL);
			SetPause(false);
			if (player[P2].npc)
			{
				InitGame();
				player[P2].npc = true;
				SetIconTextureType(NPCICON);
			}
		}
	}
	// �^�C�g����
	else if (SelectNum == SELECT_RETITLE)
	{
		if ((GetKeyboardTrigger(DIK_RETURN)))
		{
			SetPause(false);
			SetStage(TITLE);
			InitGame();
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{
			SetPause(false);
			SetStage(TITLE);
			InitGame();
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			SetPause(false);
			SetStage(TITLE);
			InitGame();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// �^�C�g���̔w�i�`��
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTexturePause);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_PAUSE, vertexWkPause, sizeof(VERTEX_2D));
	}

	// �Z���N�g�̕`��
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureSelect);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_PAUSE, vertexWkSelect, sizeof(VERTEX_2D));
	}

	// �Z���N�g���S�̕`��
	for (int i = 0; i < LOGO_MAX; i++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureLogo[i]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_PAUSE, vertexWkLogo[i], sizeof(VERTEX_2D));
	}

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPause(void)
{
	float set = 0.0;
	// �^�C�g���w�i
	{
		// ���_���W�̐ݒ�
		vertexWkPause[0].vtx = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
		vertexWkPause[1].vtx = D3DXVECTOR3(PAUSE_POS_X + PAUSE_SIZE_X, PAUSE_POS_Y, 0.0f);
		vertexWkPause[2].vtx = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y + PAUSE_SIZE_Y, 0.0f);
		vertexWkPause[3].vtx = D3DXVECTOR3(PAUSE_POS_X + PAUSE_SIZE_X, PAUSE_POS_Y + PAUSE_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWkPause[0].rhw =
		vertexWkPause[1].rhw =
		vertexWkPause[2].rhw =
		vertexWkPause[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWkPause[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkPause[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkPause[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vertexWkPause[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vertexWkPause[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWkPause[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWkPause[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWkPause[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// �Z���N�g���S
	for (int j =  1; j < LOGO_MAX; j++, set += SET_LOGO)
	{
		// ���_���W�̐ݒ�
		vertexWkLogo[j][0].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X, PAUSE_LOGO_POS_Y + set, 0.0f);
		vertexWkLogo[j][1].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X + PAUSE_LOGO_SIZE_X, PAUSE_LOGO_POS_Y + set, 0.0f);
		vertexWkLogo[j][2].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X, PAUSE_LOGO_POS_Y + PAUSE_LOGO_SIZE_Y + set, 0.0f);
		vertexWkLogo[j][3].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X + PAUSE_LOGO_SIZE_X, PAUSE_LOGO_POS_Y + PAUSE_LOGO_SIZE_Y + set, 0.0f);

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
		vertexWkSelect[0].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y, 0.0f);
		vertexWkSelect[1].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y, 0.0f);
		vertexWkSelect[2].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y, 0.0f);
		vertexWkSelect[3].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y, 0.0f);

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
void SetVertexPause(int move)
{
	// �Z���N�g�e�N�X�`���̈ړ��l
	yMove = (SELECT_MOVE * (float) move);

	// ���_���W�̐ݒ�
	vertexWkSelect[0].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + yMove, 0.0f);
	vertexWkSelect[1].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y + yMove, 0.0f);
	vertexWkSelect[2].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y + yMove, 0.0f);
	vertexWkSelect[3].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y + yMove, 0.0f);
}

