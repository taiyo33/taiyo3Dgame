//=============================================================================
//
// �������ԏ��� [time.cpp]
// Author�FGP11A341_22_�c�����z 
//=============================================================================
#include "main.h"
#include "time.h"
#include "input.h"
#include "ball.h"
#include "gameCall.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAME_TIME		("data/TEXTURE/number16x32.png")	// �^�C�}�[�摜
#define TEXTURE_TIME00_SIZE_X	(70)		// �e�N�X�`���T�C�Y
#define TEXTURE_TIME00_SIZE_Y	(100)		// ����
#define TIME_POS_X				(930)		// �|���S���̏����ʒuX
#define TIME_POS_Y				(120)		// ����
#define TIME_MAX				(60)		// �X�R�A�̍ő�l
#define TIME_DIGIT				(3)			// ����
#define TIME_COUNT_FRAME		(60)		// ���Ԍo�߂̃t���[����
#define CHANGE_BGM				(10)		// BGM��炷�b��
#define DEBUG_TIME				(999)		// �f�o�b�N���̐�������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTime(void);
void SetTextureTime(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTime = NULL;				// �e�N�X�`���ւ̃|���S��
VERTEX_2D				VertexWkTime[TIME_DIGIT][NUM_VERTEX];	// ���_���i�[���[�N

D3DXVECTOR3				PosTime;					// �|���S���̈ړ���
static int				CntFrame;
int						Time;						// ��������
LPDIRECTSOUNDBUFFER8	GameBGM002 = NULL;			// �Q�[��BGM
//=============================================================================
// ����������
//=============================================================================
HRESULT InitTime(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TIME,				// �t�@�C���̖��O
			&g_pD3DTextureTime);				// �ǂݍ��ރ������̃|�C���^
	}

	// �e�ϐ��̏�����
	PosTime = D3DXVECTOR3((float)TIME_POS_X - 16.0f, (float)TIME_POS_Y, 0.0f);
	CntFrame = 0;
	Time = TIME_MAX;

	
#if _DEBUG // �f�o�b�N���̕ϐ�������

	Time = DEBUG_TIME;	// 999�b��

#endif

	// �T�E���h�̃��[�h
	GameBGM002 = LoadSound(BGM_GAME02);

	// ���_���̍쐬
	MakeVertexTime();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTime(void)
{
	if( g_pD3DTextureTime != NULL )
	{	// �e�N�X�`���̊J��
		g_pD3DTextureTime->Release();
		g_pD3DTextureTime = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTime(void)
{
	// ���t���[�����s����鏈�����L�q����
	SetTextureTime();
	CntFrame++;

	// 0�b�őJ��
	if (Time == NULL) 
	{
		SetStage(FINISHCALL);
	}
	// �c��10�b��BGM�ύX
	else if (Time < CHANGE_BGM)
	{
		StopSound(GetGameBGM01());
		PlaySound(GameBGM002, E_DS8_FLAG_NONE);
	}

	// �b���J�E���g����
	if(CntFrame % TIME_COUNT_FRAME == 0)
	{
		Time--;
		CntFrame = 0;
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_T))
	{
		Time = 5;
	}
#endif 

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i, DigitMax = 1;


	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pD3DTextureTime );
	
	// ��������
	for (int j = 1; j < TIME_DIGIT; j++, DigitMax++)
	{
		if (Time < pow(10, j))
		{
			break;
		}
	}

	// �b�������\��
	for( i = 0; i < DigitMax; i++ )
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkTime[i], sizeof(VERTEX_2D));
	}

	// ���e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTime(void)
{
	int i;
	float haba_x = 40.0f;	// �����̉���
	
	// ��������������
	for( i = 0; i < TIME_DIGIT; i++ )
	{
		// ���_���W�̐ݒ�
		VertexWkTime[i][0].vtx.x = -haba_x * i + PosTime.x;
		VertexWkTime[i][0].vtx.y = PosTime.y;
		VertexWkTime[i][0].vtx.z = 0.0f;
		VertexWkTime[i][1].vtx.x = -haba_x * i + PosTime.x + TEXTURE_TIME00_SIZE_X;
		VertexWkTime[i][1].vtx.y = PosTime.y;
		VertexWkTime[i][1].vtx.z = 0.0f;
		VertexWkTime[i][2].vtx.x = -haba_x * i + PosTime.x;
		VertexWkTime[i][2].vtx.y = PosTime.y + TEXTURE_TIME00_SIZE_Y;
		VertexWkTime[i][2].vtx.z = 0.0f;
		VertexWkTime[i][3].vtx.x = -haba_x * i + PosTime.x + TEXTURE_TIME00_SIZE_X;
		VertexWkTime[i][3].vtx.y = PosTime.y + TEXTURE_TIME00_SIZE_Y;
		VertexWkTime[i][3].vtx.z = 0.0f;

		// rhw�̐ݒ�
		VertexWkTime[i][0].rhw =
		VertexWkTime[i][1].rhw =
		VertexWkTime[i][2].rhw =
		VertexWkTime[i][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		VertexWkTime[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTime[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTime[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		VertexWkTime[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		VertexWkTime[i][0].tex = D3DXVECTOR2( 0.0f, 0.0f );
		VertexWkTime[i][1].tex = D3DXVECTOR2( 1.0f, 0.0f );
		VertexWkTime[i][2].tex = D3DXVECTOR2( 0.0f, 1.0f );
		VertexWkTime[i][3].tex = D3DXVECTOR2( 1.0f, 1.0f );
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
// ���@���F�ȁ@��
// �߂�l�F�ȁ@��
//=============================================================================
void SetTextureTime(void)
{
	int i;
	int number = Time;
	
	for( i = 0; i < TIME_DIGIT; i++ )
	{
		// �e�N�X�`�����W�̐ݒ�
		float x = (float)(number % 10);
		VertexWkTime[i][0].tex = D3DXVECTOR2( 0.1f * x, 0.0f );
		VertexWkTime[i][1].tex = D3DXVECTOR2( 0.1f * ( x + 1 ),	 0.0f );
		VertexWkTime[i][2].tex = D3DXVECTOR2( 0.1f * x, 1.0f );
		VertexWkTime[i][3].tex = D3DXVECTOR2( 0.1f * ( x + 1 ),	 1.0f );
		number /= 10;
	}

}
//=============================================================================
// �������ԃf�[�^���Z�b�g����
// ���@��:int add(�o�ߎ���)
// �߂�l�F�ȁ@��
//=============================================================================
void AddTime( int add )
{	
	// �o�ߎ��Ԃ����Z
	Time += add;
	if( Time > TIME_MAX )
	{
		Time = TIME_MAX;
	}
	else if( Time < 0 )
	{
		Time = 0;
	}
}

//==============================================================================
// �c�莞�Ԃ��擾
// ���@���F�ȁ@��
// �߂�l�Fint �^�@
//==============================================================================
int GetTime(void)
{
	return Time;
}

//==============================================================================
// �Q�[��BGM�Q���擾
// ���@���F�ȁ@��
// �߂�l�FLPDIRECTSOUNDBUFFER8 �^�@
//==============================================================================
LPDIRECTSOUNDBUFFER8 GetGameBGM02(void)
{
	return GameBGM002;
}
