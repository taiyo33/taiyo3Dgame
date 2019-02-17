//=============================================================================
//
// �^�C�g����ʏ��� [gameCall.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "main.h"
#include "gameCall.h"
#include "input.h"
#include "player.h"
#include "ai.h"
#include "child.h"
#include "time.h"
#include "result.h"

#include "time.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOGOSPACE_ON			(1)		//
#define LOGOSPACE_OFF			(0)		//
#define ON_COUNT				(70)	//
#define OFF_COUNT				(30)	//
#define READY_SHOW_FRAME		(200)	// READY�e���b�v�\���t���[����
#define GO_SHOW_FRAME			(60)	// GO�e�b���v�\���t���[����
#define FINISH_SHOW_FRAME		(120)	// FINISH�e���b�v�\���t���[����
#define STOP_FINISH_FRAME		(60)

#define	TEXTURE_READY			("data/TEXTURE/ready_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_GO				("data/TEXTURE/go_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FINISH			("data/TEXTURE/finish_logo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MAX				(3)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexGameCall(void);
void FinishGameCall(int frame);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureGameCall[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^


VERTEX_2D				vertexWkGameCall[NUM_VERTEX];					// ���_���i�[���[�N
static int				CntFrame;
static int				TextureNum;
LPDIRECTSOUNDBUFFER8	ReadySE = NULL;	// �I�����S��SE
LPDIRECTSOUNDBUFFER8	FinishSE = NULL;	// �I�����S��SE
LPDIRECTSOUNDBUFFER8	GameBGM01 = NULL;	// �Q�[������BGM

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGameCall(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	CntFrame = 0;			// ��ԕω��̃t���[���J�E���g��������
	TextureNum = READY;		// �e�N�X�`���[�ԍ���������

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_READY,					// �t�@�C���̖��O
			&D3DTextureGameCall[READY]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GO,					// �t�@�C���̖��O
			&D3DTextureGameCall[GO]);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FINISH,					// �t�@�C���̖��O
			&D3DTextureGameCall[FINISH]);		// �ǂݍ��ރ������[
	}
	
	// ���_���̍쐬
	MakeVertexGameCall();
	
	// SE���[�h
	GameBGM01 = LoadSound(BGM_GAME01);	
	ReadySE = LoadSound(SE_STARTCALL);
	FinishSE = LoadSound(SE_FINISH);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGameCall(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (D3DTextureGameCall[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureGameCall[i]->Release();
			D3DTextureGameCall[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGameCall(void)
{
	CntFrame++;

	// �\������
	switch (TextureNum)
	{
		case READY:
			PlaySound(ReadySE, E_DS8_FLAG_NONE);
			if (CntFrame % READY_SHOW_FRAME == 0)
			{
				TextureNum = GO;
				CntFrame = 0;
			}
			break;

		case GO:
			if (CntFrame % GO_SHOW_FRAME == 0)
			{
				PlaySound(GameBGM01, E_DS8_FLAG_LOOP);
				SetStage(START);
				TextureNum = FINISH;
				CntFrame = 0;
			}
			break;

		case FINISH:
			FinishGameCall(CntFrame);

			break;

		default :
			assert("�Q�[���e�b���v�̃G���[");
			break;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGameCall(void)
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
		pDevice->SetTexture(0, D3DTextureGameCall[TextureNum]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAMECALL, vertexWkGameCall, sizeof(VERTEX_2D));
	}

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGameCall(void)
{
	// ���_���W�̐ݒ�
	vertexWkGameCall[0].vtx = D3DXVECTOR3(GAMECALL_POS_X, GAMECALL_POS_Y, 0.0f);
	vertexWkGameCall[1].vtx = D3DXVECTOR3(GAMECALL_POS_X + GAMECALL_SIZE_X, GAMECALL_POS_Y, 0.0f);
	vertexWkGameCall[2].vtx = D3DXVECTOR3(GAMECALL_POS_X, GAMECALL_POS_Y + GAMECALL_SIZE_Y, 0.0f);
	vertexWkGameCall[3].vtx = D3DXVECTOR3(GAMECALL_POS_X + GAMECALL_SIZE_X, GAMECALL_POS_Y + GAMECALL_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	vertexWkGameCall[0].rhw =
		vertexWkGameCall[1].rhw =
		vertexWkGameCall[2].rhw =
		vertexWkGameCall[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWkGameCall[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkGameCall[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkGameCall[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkGameCall[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	vertexWkGameCall[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkGameCall[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkGameCall[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkGameCall[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

void FinishGameCall(int frame)
{
	if (frame == 1)
	{
		StopSound(GameBGM01);
		StopSound(GetGameBGM02());
		PlaySound(FinishSE, E_DS8_FLAG_NONE);
	}
	else if (frame % STOP_FINISH_FRAME == 0)
	{
		StopSound(FinishSE);
	}
	if (frame % FINISH_SHOW_FRAME == 0)
	{
		PLAYER *player = GetPlayer(0);
		if (player[P1].life <= 0)
		{
			SetResult(P2);
		}
		else if (player[P2].life <= 0)
		{
			SetResult(P1);
		}

		ComprareChild();

		frame = 0;
	}

}

void SetGameCallTextureNum(int num)
{
	TextureNum = num;
}

LPDIRECTSOUNDBUFFER8 GetGameBGM01(void)
{
	return GameBGM01;
}