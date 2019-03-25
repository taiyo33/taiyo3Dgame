//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "player.h"
#include "ai.h"
#include "lifeGauge.h"
#include "ball.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	POS_X						(550)			// �^�C�g���̕\���ʒu
#define	POS_Y						(380)			// �^�C�g���̕\���ʒu
#define	SIZE_X						(750)			// �^�C�g���̕�
#define	SIZE_Y						(310)			// �^�C�g���̍���
#define	LOGO_POS_X					(840)			// �^�C�g�����S�̕\���ʒu
#define	LOGO_POS_Y					(700)			// �^�C�g�����S�̕\���ʒu
#define	LOGO_SIZE_X					(200)			// �^�C�g�����S�̕�
#define	LOGO_SIZE_Y					(150)			// �^�C�g�����S�̍���
#define LOGO_PUSHKEY_POS_X01		(710)			// �J�ڃL�[�̕\���ʒu
#define LOGO_PUSHKEY_POS_Y01		(625)			//
#define LOGO_PUSHKEY_SIZE_X01		(300)			// �傫��
#define LOGO_PUSHKEY_SIZE_Y01		(100)			//
#define LOGO_PUSHKEY_POS_X02		(1055)			// �J�ڃL�[�̕\���ʒu
#define LOGO_PUSHKEY_POS_Y02		(645)			//
#define LOGO_PUSHKEY_SIZE_X02		(70)			// �傫��
#define LOGO_PUSHKEY_SIZE_Y02		(70)			//
#define LOGO_MAX					(5)				// ���S�̍ő吔
#define	SELECT_POS_X				(770)			// �^�C�g�����S�̕\���ʒu
#define	SELECT_POS_Y				(745)			// �^�C�g�����S�̕\���ʒu
#define	SELECT_SIZE_X				(60)			// �^�C�g�����S�̕�
#define	SELECT_SIZE_Y				(60)			// �^�C�g�����S�̍���
#define SELECT_MOVE					(75)			// �Z���N�g�̈ړ��l
#define PUSHKEY_MAX					(2)				// �J�ڃL�[�̍ő�\����
#define SELECT_NPC					(0)				// vs NPC���[�h
#define SELECT_PVP					(1)				// vs P2���[�h
#define SELECT_EXIT					(2)				// �I��
#define LOGOSPACE_ON				(1)				// �J�ڃ{�^���̃��S�_�ŏ��
#define LOGOSPACE_OFF				(0)				// 
#define ON_COUNT					(70)			// �_���̎���
#define OFF_COUNT					(30)			// �����̎���
#define MOVE_MENU_CNT_FREAM			(60)			// �J�ڂ܂ł̎���
#define BUTTON_OFF_CNT				(30)			// ����s�\����

#define	TEXTURE_TITLE				("data/TEXTURE/title_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PVP					("data/TEXTURE/pvp_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_NPC					("data/TEXTURE/pvc_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_EXIT				("data/TEXTURE/exit_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_SELECT				("data/TEXTURE/yajirusi.png")	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(void);
void SetVertexSelect(int selectNum);
void SetLogoTextureDiffuse(int ver);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9				D3DTextureTitle = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9				D3DTextureLogo[LOGO_MAX];	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9				D3DTextureSelect;			// �e�N�X�`���ւ̃|�C���^
LPDIRECTSOUNDBUFFER8			TitleBGM = NULL;			// �^�C�g����BGM
LPDIRECTSOUNDBUFFER8			DecisionSE = NULL;			// �^�C�g���̃��j���[����SE
LPDIRECTSOUNDBUFFER8			SelectSE = NULL;			// �I�����S��SE
VERTEX_2D						VertexWkTitle[NUM_VERTEX];				// ���_���i�[���[�N
VERTEX_2D						VertexWkLogo[LOGO_MAX][NUM_VERTEX];		// ���_���i�[���[�N
VERTEX_2D						VertexWkSelect[NUM_VERTEX];				// ���_���i�[���[�N
static float					MoveY;						// ���S�̈ʒu����
static int						SelectNum;					// �Z���N�g�̂���Ă��郍�S�ԍ�
static int						CntFrame;					// �_�ŏ����̃t���[���J�E���g
static int						ButtonCntFrame;				// ����\�܂ł̃t���[���J�E���g
bool							SelectFlag;					// ���j���[����t���O

//=============================================================================
// ����������
// ���@���Fint type(�ď���������2������ϐ�)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �ϐ�������
	MoveY = 0.0;
	CntFrame = 0;
	SelectFlag = false;
	SelectNum = 0;
	ButtonCntFrame = 0;

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE,					// �t�@�C���̖��O
			&D3DTextureTitle);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ENTHER,					// �t�@�C���̖��O
			&D3DTextureLogo[TEX_NUM001]);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BUTTON,					// �t�@�C���̖��O
			&D3DTextureLogo[TEX_NUM002]);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_NPC,					// �t�@�C���̖��O
			&D3DTextureLogo[TEX_NUM003]);			// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PVP,					// �t�@�C���̖��O
			&D3DTextureLogo[TEX_NUM004]);			// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EXIT,					// �t�@�C���̖��O
			&D3DTextureLogo[TEX_NUM005]);			// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SELECT,					// �t�@�C���̖��O
			&D3DTextureSelect);				// �ǂݍ��ރ������[
	}
	
	// ���_���̍쐬
	MakeVertexTitle();
	
	// BGM�̃��[�h�ƍĐ�
	TitleBGM = LoadSound(BGM_00);
	PlaySound(TitleBGM, E_DS8_FLAG_LOOP);
	
	// SE�̃��[�h
	DecisionSE = LoadSound(SE_SPACE);
	SelectSE = LoadSound(SE_SELECT);

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
	PLAYER *player = GetPlayer(0);

	// �J��
	if (SelectFlag)
	{
		if (CntFrame % MOVE_MENU_CNT_FREAM == 0)
		{
			SetInitPosPlayer();
			SetInitPosBall();
			SetStage(TUTORIAL);
		}
	}

	// �e�t���[���̍X�V
	CntFrame++;		 
	ButtonCntFrame++;

	if (CntFrame % OFF_COUNT == 0)
	{
		SetLogoTextureDiffuse(LOGOSPACE_OFF);
	}
	if (CntFrame % ON_COUNT == 0)
	{
		SetLogoTextureDiffuse(LOGOSPACE_ON);
		CntFrame = 0;
	}

	// ����\�ɂȂ�܂�
	if (ButtonCntFrame > BUTTON_OFF_CNT)
	{
		// �������փZ���N�g���ړ�
		if (GetKeyboardTrigger(DIK_DOWN))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 1) % 3;
			SetVertexSelect(SelectNum);
		}
		else if (IsButtonTriggered(P1, BUTTON_DOWN))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 1) % 3;
			SetVertexSelect(SelectNum);
		}
		else if (IsButtonTriggered(P2, BUTTON_DOWN))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 1) % 3;
			SetVertexSelect(SelectNum);
		}

		// ������̃Z���N�g���ړ�
		if (GetKeyboardTrigger(DIK_UP))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 2) % 3;
			SetVertexSelect(SelectNum);
		}
		else if (IsButtonTriggered(P1, BUTTON_UP))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 2) % 3;
			SetVertexSelect(SelectNum);
		}
		else if (IsButtonTriggered(P2, BUTTON_UP))
		{
			PlaySound(SelectSE, E_DS8_FLAG_NONE);
			SelectNum = (SelectNum + 2) % 3;
			SetVertexSelect(SelectNum);
		}

		// NPC��֑J��
		if (SelectNum == SELECT_NPC)
		{
			if ((GetKeyboardTrigger(DIK_RETURN)))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				player[P2].npc = true;
				//player[P3].npc = true;
				//player[P4].npc = true;
				SetIconTextureType(NPCICON);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
			else if (IsButtonPressed(P1, BUTTON_A))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				player[P2].npc = true;
				//player[P3].npc = true;
				//player[P4].npc = true;
				SetIconTextureType(NPCICON);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
			else if (IsButtonPressed(P2, BUTTON_B))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				player[P2].npc = true;
				//player[P3].npc = true;
				//player[P4].npc = true;
				SetIconTextureType(NPCICON);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
		}
		// PVP�֑J��
		else if (SelectNum == SELECT_PVP)
		{
			if ((GetKeyboardTrigger(DIK_RETURN)))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				SetIconTextureType(PLAYERICON02);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
			else if (IsButtonPressed(P1, BUTTON_A))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				SetIconTextureType(PLAYERICON02);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
			else if (IsButtonPressed(P2, BUTTON_B))
			{
				PlaySound(DecisionSE, E_DS8_FLAG_NONE);
				SetIconTextureType(PLAYERICON02);
				SelectFlag = true;
				ButtonCntFrame = 0;
			}
		}
		// �����I��
		else if (SelectNum == SELECT_EXIT)
		{
			if ((GetKeyboardTrigger(DIK_RETURN)))
			{
				exit(NULL);
			}
			else if (IsButtonPressed(P1, BUTTON_A))
			{
				exit(NULL);
			}
			else if (IsButtonPressed(P2, BUTTON_B))
			{
				exit(NULL);
			}
		}
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
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkTitle, sizeof(VERTEX_2D));
	}

	// �Z���N�g�̕`��
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureSelect);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkSelect, sizeof(VERTEX_2D));
	}

	// �Z���N�g���S�̕`��
	for (int i = 0; i < LOGO_MAX; i++)
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
HRESULT MakeVertexTitle(void)
{
	float set = 0.0;
	// �^�C�g���w�i
	{
		// ���_���W�̐ݒ�
		VertexWkTitle[0].vtx = D3DXVECTOR3(POS_X, POS_Y, 0.0f);
		VertexWkTitle[1].vtx = D3DXVECTOR3(POS_X + SIZE_X, POS_Y, 0.0f);
		VertexWkTitle[2].vtx = D3DXVECTOR3(POS_X, POS_Y + SIZE_Y, 0.0f);
		VertexWkTitle[3].vtx = D3DXVECTOR3(POS_X + SIZE_X, POS_Y + SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWkTitle[0].rhw =
		VertexWkTitle[1].rhw =
		VertexWkTitle[2].rhw =
		VertexWkTitle[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWkTitle[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTitle[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTitle[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTitle[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWkTitle[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkTitle[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkTitle[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkTitle[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// �v�b�V���L�[�̕\��
	{
		// ���_���W�̐ݒ�
		VertexWkLogo[TEX_NUM001][0].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X01 , LOGO_PUSHKEY_POS_Y01, 0.0f);
		VertexWkLogo[TEX_NUM001][1].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X01 + LOGO_PUSHKEY_SIZE_X01 , LOGO_PUSHKEY_POS_Y01, 0.0f);
		VertexWkLogo[TEX_NUM001][2].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X01 , LOGO_PUSHKEY_POS_Y01 + LOGO_PUSHKEY_SIZE_Y01, 0.0f);
		VertexWkLogo[TEX_NUM001][3].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X01 + LOGO_PUSHKEY_SIZE_X01 , LOGO_PUSHKEY_POS_Y01 + LOGO_PUSHKEY_SIZE_Y01, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		VertexWkLogo[TEX_NUM001][0].rhw =
		VertexWkLogo[TEX_NUM001][1].rhw =
		VertexWkLogo[TEX_NUM001][2].rhw =
		VertexWkLogo[TEX_NUM001][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWkLogo[TEX_NUM001][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[TEX_NUM001][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[TEX_NUM001][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkLogo[TEX_NUM001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWkLogo[TEX_NUM001][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		VertexWkLogo[TEX_NUM001][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		VertexWkLogo[TEX_NUM001][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		VertexWkLogo[TEX_NUM001][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_���W�̐ݒ�
	VertexWkLogo[TEX_NUM002][0].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X02 , LOGO_PUSHKEY_POS_Y02, 0.0f);
	VertexWkLogo[TEX_NUM002][1].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X02 + LOGO_PUSHKEY_SIZE_X02 , LOGO_PUSHKEY_POS_Y02, 0.0f);
	VertexWkLogo[TEX_NUM002][2].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X02 , LOGO_PUSHKEY_POS_Y02 + LOGO_PUSHKEY_SIZE_Y02, 0.0f);
	VertexWkLogo[TEX_NUM002][3].vtx = D3DXVECTOR3(LOGO_PUSHKEY_POS_X02 + LOGO_PUSHKEY_SIZE_X02 , LOGO_PUSHKEY_POS_Y02 + LOGO_PUSHKEY_SIZE_Y02, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	VertexWkLogo[TEX_NUM002][0].rhw =
	VertexWkLogo[TEX_NUM002][1].rhw =
	VertexWkLogo[TEX_NUM002][2].rhw =
	VertexWkLogo[TEX_NUM002][3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	VertexWkLogo[TEX_NUM002][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkLogo[TEX_NUM002][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkLogo[TEX_NUM002][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	VertexWkLogo[TEX_NUM002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	VertexWkLogo[TEX_NUM002][0].tex = D3DXVECTOR2(0.0f, 0.0f);
	VertexWkLogo[TEX_NUM002][1].tex = D3DXVECTOR2(1.0f, 0.0f);
	VertexWkLogo[TEX_NUM002][2].tex = D3DXVECTOR2(0.0f, 1.0f);
	VertexWkLogo[TEX_NUM002][3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �Z���N�g���S
	for (int j = PUSHKEY_MAX; j < LOGO_MAX; j++, set += SELECT_MOVE)
	{
		// ���_���W�̐ݒ�
		VertexWkLogo[j][0].vtx = D3DXVECTOR3(LOGO_POS_X, LOGO_POS_Y + set, 0.0f);
		VertexWkLogo[j][1].vtx = D3DXVECTOR3(LOGO_POS_X + LOGO_SIZE_X, LOGO_POS_Y + set, 0.0f);
		VertexWkLogo[j][2].vtx = D3DXVECTOR3(LOGO_POS_X, LOGO_POS_Y + LOGO_SIZE_Y + set, 0.0f);
		VertexWkLogo[j][3].vtx = D3DXVECTOR3(LOGO_POS_X + LOGO_SIZE_X, LOGO_POS_Y + LOGO_SIZE_Y + set, 0.0f);

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
		VertexWkSelect[0].vtx = D3DXVECTOR3(SELECT_POS_X, SELECT_POS_Y, 0.0f);
		VertexWkSelect[1].vtx = D3DXVECTOR3(SELECT_POS_X + SELECT_SIZE_X, SELECT_POS_Y, 0.0f);
		VertexWkSelect[2].vtx = D3DXVECTOR3(SELECT_POS_X, SELECT_POS_Y + SELECT_SIZE_Y, 0.0f);
		VertexWkSelect[3].vtx = D3DXVECTOR3(SELECT_POS_X + SELECT_SIZE_X, SELECT_POS_Y + SELECT_SIZE_Y, 0.0f);

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
// ���@���Fint selectNum�i�Z���N�g���S�̎�ށj
// �߂�l�F�ȁ@��
//============================================================================
void SetVertexSelect(int selectNum)
{
	// �Z���N�g�e�N�X�`���̈ړ��l
	MoveY = (SELECT_MOVE * (float) selectNum);

	// ���_���W�̐ݒ�
	VertexWkSelect[0].vtx = D3DXVECTOR3(SELECT_POS_X, SELECT_POS_Y + MoveY, 0.0f);
	VertexWkSelect[1].vtx = D3DXVECTOR3(SELECT_POS_X + SELECT_SIZE_X, SELECT_POS_Y + MoveY, 0.0f);
	VertexWkSelect[2].vtx = D3DXVECTOR3(SELECT_POS_X, SELECT_POS_Y + SELECT_SIZE_Y + MoveY, 0.0f);
	VertexWkSelect[3].vtx = D3DXVECTOR3(SELECT_POS_X + SELECT_SIZE_X, SELECT_POS_Y + SELECT_SIZE_Y + MoveY, 0.0f);
}

//============================================================================
// �J�ڃ{�^���̓_�ŏ���
// ���@���Fint ver�i���߂̒l�j
// �߂�l�F�ȁ@��
//============================================================================
void SetLogoTextureDiffuse(int ver) 
{
	// ���ˌ��̐ݒ�
	VertexWkLogo[TEX_NUM001][0].diffuse = 
	VertexWkLogo[TEX_NUM001][1].diffuse = 
	VertexWkLogo[TEX_NUM001][2].diffuse = 
	VertexWkLogo[TEX_NUM001][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * ver);

	// ���ˌ��̐ݒ�
	VertexWkLogo[TEX_NUM002][0].diffuse =
	VertexWkLogo[TEX_NUM002][1].diffuse =
	VertexWkLogo[TEX_NUM002][2].diffuse =
	VertexWkLogo[TEX_NUM002][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255 * ver);

}

//============================================================================
// �^�C�g��BGM�̎擾
// ���@���F�ȁ@��
// �߂�l�FLPDIRECTSOUNDBUFFER8�^
//============================================================================
LPDIRECTSOUNDBUFFER8 *GetTitleSound(void)
{
	return &TitleBGM;
}

