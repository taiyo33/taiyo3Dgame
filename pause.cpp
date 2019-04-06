//=============================================================================
//
// �|�[�Y��ʏ��� [pause.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "player.h"
#include "ai.h"
#include "lifeGauge.h"
#include "ball.h"
#include "gameCall.h"
#include "time.h"
#include "title.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PAUSE		("data/TEXTURE/menu.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_SELECT		("data/TEXTURE/bullet001.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESTART		("data/TEXTURE/restart_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_REOPNE		("data/TEXTURE/reopne_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RETITLE		("data/TEXTURE/retitle_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define PAUSE_LOGO_MAX		(4)									// ���S�̍ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPause(void);
void SetVertexPause(int move);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9				D3DTexturePause = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		D3DTextureLogo[PAUSE_LOGO_MAX];	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		D3DTextureSelect;				// �e�N�X�`���ւ̃|�C���^

VERTEX_2D						VertexWkPause[NUM_VERTEX];					// ���_���i�[���[�N
static VERTEX_2D				VertexWkLogo[PAUSE_LOGO_MAX][NUM_VERTEX];	// ���_���i�[���[�N
static VERTEX_2D				VertexWkSelect[NUM_VERTEX];					// ���_���i�[���[�N
static float					MoveY;						// ���S�̈ʒu����
static int						SelectNum;					// �Z���N�g�̂���Ă��郍�S�ԍ�
bool							PauseUse;					// ���j���[����t���O
static int						CntFrame;					// ����\�܂ł̃t���[���J�E���g
//===============================================================================
// ����������
// ���@���Fint type(�ď�������2������l)
// �߂�l�FHRESULT�^
//===============================================================================
HRESULT InitPause(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �ϐ�������
	MoveY = 0.0;
	SelectNum = 0;
	PauseUse = false;
	CntFrame = 0;

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

	for (int i = 0; i < PAUSE_LOGO_MAX; i++)
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

	// �|�[�Y�֑J�ڂ��Ă���J�E���g
	if (GetPause())
	{
		CntFrame++;
	}

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

	// 30�t���[���ő���\
	if (CntFrame % 30 == 0)
	{
		// �|�[�Y����
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
		else if (IsButtonPressed(P2, BUTTON_M))
		{
			SetPause(false);
		}
	}

	// �`���[�WSE�̒�~
	StopSound(player[P1].chargeSE);
	StopSound(player[P2].chargeSE);

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
		// �Q�[���J�n�O�܂Ŗ߂�
		if ((GetKeyboardTrigger(DIK_RETURN)))
		{
			// �S�Ă�BGM�̒�~
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());

			// NPC���[�h�̏ꍇ
			if (player[P2].npc)
			{
				InitGame();
				StopSound(*GetTitleSound());
				player[P2].npc = true;
				SetIconTextureType(NPCICON);
				SetStage(STARTCALL);
				SetPause(false);
				return;
			}

			// ������Ԃ�
			SetStage(STARTCALL);
			SetPause(false);
			InitGame();
			StopSound(*GetTitleSound());
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{		
			// �S�Ă�BGM�̒�~
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());
			
			// NPC���[�h�̏ꍇ
			if (player[P2].npc)
			{
				InitGame();
				StopSound(*GetTitleSound());
				player[P2].npc = true;
				SetIconTextureType(NPCICON);
				SetStage(STARTCALL);
				SetPause(false);
				return;
			}

			// ������Ԃ�
			InitGame();
			StopSound(*GetTitleSound());
			SetStage(STARTCALL);
			SetPause(false);
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());

			if (player[P2].npc)
			{				
				InitGame();
				StopSound(*GetTitleSound());
				player[P2].npc = true;
				SetIconTextureType(NPCICON);
				SetStage(STARTCALL);
				SetPause(false);
				return;
			}

			InitGame();
			StopSound(*GetTitleSound());
			SetStage(STARTCALL);
			SetPause(false);
		}
	}
	// �^�C�g����
	else if (SelectNum == SELECT_RETITLE)
	{
		// �^�C�g���܂Ŗ߂�
		if ((GetKeyboardTrigger(DIK_RETURN)))
		{
			// BCM�̒�~�Ə�����
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());			
			SetPause(false);
			SetStage(TITLE);
			InitGame();
		}
		else if (IsButtonPressed(P1, BUTTON_A))
		{
			// BCM�̒�~�Ə�����
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());			
			SetPause(false);
			SetStage(TITLE);
			InitGame();
		}
		else if (IsButtonPressed(P2, BUTTON_B))
		{
			// BCM�̒�~�Ə�����
			StopSound(GetGameBGM01());
			StopSound(GetGameBGM02());
			StopSound(GetGameBGM03());			
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
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkPause, sizeof(VERTEX_2D));
	}

	// �Z���N�g�̕`��
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureSelect);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkSelect, sizeof(VERTEX_2D));
	}

	// �Z���N�g���S�̕`��
	for (int i = 0; i < PAUSE_LOGO_MAX; i++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureLogo[i]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkLogo[i], sizeof(VERTEX_2D));
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
	
	// �|�[�Y�g
	{
		// ���_���W�̐ݒ�
		VertexWkPause[0].vtx = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
		VertexWkPause[1].vtx = D3DXVECTOR3(PAUSE_POS_X + PAUSE_SIZE_X, PAUSE_POS_Y, 0.0f);
		VertexWkPause[2].vtx = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y + PAUSE_SIZE_Y, 0.0f);
		VertexWkPause[3].vtx = D3DXVECTOR3(PAUSE_POS_X + PAUSE_SIZE_X, PAUSE_POS_Y + PAUSE_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWkPause[0].rhw =
		VertexWkPause[1].rhw =
		VertexWkPause[2].rhw =
		VertexWkPause[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWkPause[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkPause[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkPause[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkPause[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWkPause[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkPause[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkPause[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkPause[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// �Z���N�g���S
	for (int j =  1; j < PAUSE_LOGO_MAX; j++, set += SET_LOGO)
	{
		// ���_���W�̐ݒ�
		VertexWkLogo[j][0].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X, PAUSE_LOGO_POS_Y + set, 0.0f);
		VertexWkLogo[j][1].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X + PAUSE_LOGO_SIZE_X, PAUSE_LOGO_POS_Y + set, 0.0f);
		VertexWkLogo[j][2].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X, PAUSE_LOGO_POS_Y + PAUSE_LOGO_SIZE_Y + set, 0.0f);
		VertexWkLogo[j][3].vtx = D3DXVECTOR3(PAUSE_LOGO_POS_X + PAUSE_LOGO_SIZE_X, PAUSE_LOGO_POS_Y + PAUSE_LOGO_SIZE_Y + set, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWkLogo[j][0].rhw =
		VertexWkLogo[j][1].rhw =
		VertexWkLogo[j][2].rhw =
		VertexWkLogo[j][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWkLogo[j][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[j][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[j][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[j][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWkLogo[j][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkLogo[j][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkLogo[j][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkLogo[j][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// �Z���N�g
	{
		// ���_���W�̐ݒ�
		VertexWkSelect[0].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y, 0.0f);
		VertexWkSelect[1].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y, 0.0f);
		VertexWkSelect[2].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y, 0.0f);
		VertexWkSelect[3].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWkSelect[0].rhw =
		VertexWkSelect[1].rhw =
		VertexWkSelect[2].rhw =
		VertexWkSelect[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWkSelect[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkSelect[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkSelect[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkSelect[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWkSelect[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkSelect[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkSelect[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkSelect[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//============================================================================
// �Z���N�g�̐ݒu
// ���@���Fint move�i�Z���N�g���S�̎�ށj
// �߂�l�F�ȁ@��
//============================================================================
void SetVertexPause(int move)
{
	// �Z���N�g�e�N�X�`���̈ړ��l
	MoveY = (SELECT_MOVE * (float) move);

	// ���_���W�̐ݒ�
	VertexWkSelect[0].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + MoveY, 0.0f);
	VertexWkSelect[1].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y + MoveY, 0.0f);
	VertexWkSelect[2].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y + MoveY, 0.0f);
	VertexWkSelect[3].vtx = D3DXVECTOR3(PAUSE_SELECT_POS_X + PAUSE_SELECT_SIZE_X, PAUSE_SELECT_POS_Y + PAUSE_SELECT_SIZE_Y + MoveY, 0.0f);
}

