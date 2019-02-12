//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "debugproc.h"
#include "bullet.h"
#include "field.h"
#include "block.h"
#include "checkhit.h"
#include "ai.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_AIRPLANE		"data/MODEL/model_body.x"	// �ǂݍ��ރ��f����
#define HANDGUM_TEXTURE		"data/TEXTURE/handgun.jpg"	// �ǂݍ��ރe�N�X�`���[
#define	RATE_MOVE_PLAYER		(0.20f)					// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.02f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.20f)						// ��]�����W��
#define VALUE_MOVE_PLAYER	(0.50f)						// �ړ����x
#define PLAYER_POS_Y_LIMIT	(-200.0f)					// Y���̏���l
#define AI_UPDATE_TIME		(10)						// �`�h�̏��X�V����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MovePlayer(int index);
void InputPlayer1(void);
void InputKeyPlayer2(void);
void InputGamePadPlayer1(void);
void InputGamePadPlayer2(void);
D3DXVECTOR3 WallShear(D3DXVECTOR3 pos, D3DXVECTOR3 normal, int index);
void WallShearPlayer(int index);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9			D3DTexture;				// �e�N�X�`���ǂݍ��ݏꏊ
static LPD3DXMESH					D3DXMesh;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
static LPD3DXBUFFER					D3DXBuffMat;			// ���b�V���̃}�e���A�������i�[
static DWORD						NumMat;					// �������̑���
static int							cntFrame[PLAYER_MAX];	// �t���[���J�E���g
PLAYER								player[PLAYER_MAX];		// �v���C���[�\����
//=============================================================================
// ����������
// ���@���Fint type(�ď������Q������l)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA *camera = GetCamera();
	D3DTexture = NULL;	// �e�N�X�`���[�̏�����
	D3DXMesh = NULL;		// �C���^�t�F�[�X�̏�����
	D3DXBuffMat = NULL;	// �}�e���A���̏�����
	player[P1].pos = D3DXVECTOR3(PLAYER01_INITPOS_X, PLAYER01_INITPOS_Y, PLAYER01_INITPOS_Z);	// �ʒu�̏�����
	player[P2].pos = D3DXVECTOR3(PLAYER02_INITPOS_X, PLAYER02_INITPOS_Y, PLAYER02_INITPOS_Z);	//
	player[P1].use = true;								// �g�p��Ԃ�������
	player[P2].use = true;								//
	player[P1].life = PLAYER_LIFE_MAX;					// �v���C���[�̗̑͂�������
	player[P2].life = 0;								// 

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		player[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�̏�����
		player[i].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]�̖ړI�ʒu��������
		player[i].speed = VALUE_MOVE_PLAYER;				// �ړ����x�̏�����
		player[i].cntFrame= 0;
		player[i].frontVec = D3DXVECTOR3(sinf(player[i].rot.y) * 100.0f, 0.0f, cosf(player[i].rot.y) * 100.0f );
	}

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(PLAYER_AIRPLANE,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&D3DXBuffMat,
		NULL,
		&NumMat,
		&D3DXMesh)))
	{
		return E_FAIL;
	}

	// �@�����K���̐ݒ�
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if(D3DTexture != NULL)
	{// �e�N�X�`���̊J��
		D3DTexture->Release();
		D3DTexture = NULL;
	}

	if(D3DXMesh != NULL)
	{// ���b�V���̊J��
		D3DXMesh->Release();
		D3DXMesh = NULL;
	}

	if(D3DXBuffMat != NULL)
	{// �}�e���A���̊J��
		D3DXBuffMat->Release();
		D3DXBuffMat = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *camera = GetCamera();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// ���݈ʒu��ۑ�
		player[i].prevPos = player[i].pos;
		player[i].cntFrame++;
		//player[i].frontVec = D3DXVECTOR3(sinf(player[i].rot.y), 0.0f, cosf(player[i].rot.y));
		player[i].frontVec = D3DXVECTOR3(sinf(player[i].rot.y) * 75.0f, 0.0f, cosf(player[i].rot.y) * 75.0f);

		// ����̏���
		InputPlayer1();
		InputKeyPlayer2();
		InputGamePadPlayer1();
		InputGamePadPlayer2();
		
		if (i == 1)
		{
			NonePlayerAttack();
			//NonePlayerMove();
			NonePlayerPatrol();
		}

		// �ǂ��菈��
		WallShearPlayer(i);
		// �v���C���[�̑���
		MovePlayer(i);

		PlayerDamageManager(i);

		PrintDebugProc("�v���C���[�̉�][(%f)]\n", player[i].rot.y);
		PrintDebugProc("�v���C���[�̈ʒu: [X:(%f),z:(%f)]\n", player[i].pos.x, player[i].pos.z);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxView;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (player[i].use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&player[i].mtxWorld);

			mtxView = GetMtxView();

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, player[i].rot.y, player[i].rot.x, player[i].rot.z);
			D3DXMatrixMultiply(&player[i].mtxWorld, &player[i].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, player[i].pos.x, player[i].pos.y, player[i].pos.z);
			D3DXMatrixMultiply(&player[i].mtxWorld, &player[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &player[i].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)D3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)NumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, D3DTexture);

				// �`��
				D3DXMesh->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �v���C���[�̃A�h���X�擾
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�Ȃ�
//=============================================================================
PLAYER *GetPlayer(int index)
{
	return &player[index];
}

//=============================================================================
// �v���C���[1�̃L�[�{�[�h���쏈��
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//=============================================================================
void InputPlayer1(void)
{
	if (player[P1].use)
	{
		CAMERA *camera = GetCamera();	// �J�����̃A�h���X���擾
		BULLET *bullet = GetBullet(P1);	// �o���b�g�̃A�h���X���擾

		if (GetKeyboardPress(DIK_RIGHT))
		{
			if (GetKeyboardPress(DIK_UP))
			{// �E�O�ړ�
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * player[P1].speed;

				player[P1].rot.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// �E��ړ�
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * player[P1].speed;

				player[P1].rot.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// �E�ړ�
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * player[P1].speed;

				player[P1].rot.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_LEFT))
		{
			if (GetKeyboardPress(DIK_UP))
			{// ���O�ړ�
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * player[P1].speed;

				player[P1].rot.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ����ړ�
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * player[P1].speed;

				player[P1].rot.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// ���ړ�
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * player[P1].speed;

				player[P1].rot.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{
			if(GetKeyboardPress(DIK_RIGHT))
			{// �E�ړ�
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * player[P1].speed;

				player[P1].rot.y = camera->rot.y + D3DX_PI * 0.50f;
			}
			else
			{
				// �O�ړ�
				player[P1].move.x -= sinf(D3DX_PI + camera->rot.y) * player[P1].speed;
				player[P1].move.z -= cosf(D3DX_PI + camera->rot.y) * player[P1].speed;

				player[P1].rot.y = camera->rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{
			if (GetKeyboardPress(DIK_LEFT))
			{// ���ړ�
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * player[P1].speed;

				player[P1].rot.y = camera->rot.y - D3DX_PI * 0.50f;
			}
			else
			{
				// ��ړ�
				player[P1].move.x -= sinf(camera->rot.y) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y) * player[P1].speed;

				player[P1].rot.y = D3DX_PI + camera->rot.y;
			}
		}

		// �o���b�g�̃`���[�W
		if (GetKeyboardPress(DIK_SPACE))
		{
			// �o���b�g���g�p���̓`���[�W�s��
			if (bullet->use[P1]) return;

			// �ő�l�ɂȂ����ꍇ
			if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
			{
                bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10�t���[��
			else if (player[P1].cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.2f;
			}
		}
		// �o���b�g�̔���
        else if(GetKeyboardRelease(DIK_SPACE))
		{
          	SetBullet(player[P1].pos, player[P1].rot, bullet->speedIncrease, 0, P1);
			cntFrame[P1] = 0;
		}

		// �ʒu��������
		if (GetKeyboardTrigger(DIK_O))
		{
			player[P1].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		}
	}
}

//=============================================================================
// �v���C���[2�̃L�[�{�[�h���쏈��
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//=============================================================================
void InputKeyPlayer2(void)
{
	// �J�����̌����擾
	CAMERA *camera = GetCamera();
	BULLET *bullet = GetBullet(P2);

	if (player[P2].use)
	{
		if (GetKeyboardPress(DIK_D))
		{
			if (GetKeyboardPress(DIK_W))
			{// �E�O�ړ�
				player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_S))
			{// �E��ړ�
				player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// �E�ړ�
				player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_A))
		{
			if (GetKeyboardPress(DIK_W))
			{// ���O�ړ�
				player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_S))
			{// ����ړ�
				player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// ���ړ�
				player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_W))
		{
			// �O�ړ�
			player[P2].move.x -= sinf(D3DX_PI + camera->rot.y) * player[P2].speed;
			player[P2].move.z -= cosf(D3DX_PI + camera->rot.y) * player[P2].speed;

			player[P2].rotDest.y = camera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S))
		{
			// ��ړ�
			player[P2].move.x -= sinf(camera->rot.y) * player[P2].speed;
			player[P2].move.z -= cosf(camera->rot.y) * player[P2].speed;

			player[P2].rotDest.y = D3DX_PI + camera->rot.y;
		}

		if (GetKeyboardPress(DIK_Z))
		{
			// �o���b�g���g�p���̓`���[�W�s��
			if (bullet->use[P2]) return;

			// �ő�l�ɂȂ����ꍇ
			if (bullet->speedIncrease > BULLET_CHARGE_MAX)
			{
				bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10�t���[��
			else if (player[P2].cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.5f;
			}
		}
		else if (GetKeyboardRelease(DIK_Z))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			cntFrame[P2] = 0;
		}
		
	}
}

//=============================================================================
// �v���C���[1�̃Q�[���p�b�h���쏈��
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//=============================================================================
void InputGamePadPlayer1(void)
{
	if (player[P1].use)
	{
		CAMERA *camera = GetCamera();	// �J�����̃A�h���X���擾
		BULLET *bullet = GetBullet(P1);	// �o���b�g�̃A�h���X���擾
		if (IsButtonPressed(P1,BUTTON_RIGHT))
		{
			if (IsButtonPressed(P1,BUTTON_UP))
			{// �E�O�ړ�
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (IsButtonPressed(P1,BUTTON_DOWN))
			{// �E��ړ�
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// �E�ړ�
				player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (IsButtonPressed(P1,BUTTON_LEFT))
		{
			if (IsButtonPressed(P1,BUTTON_UP))
			{// ���O�ړ�
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (IsButtonPressed(P1,BUTTON_DOWN))
			{// ����ړ�
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// ���ړ�
				player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * player[P1].speed;
				player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * player[P1].speed;

				player[P1].rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (IsButtonPressed(P1,BUTTON_UP))
		{
			// �O�ړ�
			player[P1].move.x -= sinf(D3DX_PI + camera->rot.y) * player[P1].speed;
			player[P1].move.z -= cosf(D3DX_PI + camera->rot.y) * player[P1].speed;

			player[P1].rotDest.y = camera->rot.y;
		}
		else if (IsButtonPressed(P1,BUTTON_DOWN))
		{
			// ��ړ�
			player[P1].move.x -= sinf(camera->rot.y) * player[P1].speed;
			player[P1].move.z -= cosf(camera->rot.y) * player[P1].speed;

			player[P1].rotDest.y = D3DX_PI + camera->rot.y;
		}

		// �o���b�g�̃`���[�W
		if (IsButtonPressed(P1, BUTTON_B))
		{
			// �o���b�g���g�p���̓`���[�W�s��
			if (bullet->use[P1]) return;

			// �ő�l�ɂȂ����ꍇ
			if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
			{
				bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10�t���[��
			else if (player[P1].cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.2f;
			}
		}
		// �o���b�g�̔���
		else if (IsButtonRelease(P1, BUTTON_B))
		{
			SetBullet(player[P1].pos, player[P1].rot, bullet->speedIncrease, 0, P1);
			cntFrame[P1] = 0;
		}
	}
}

//=============================================================================
// �v���C���[2�̃Q�[���p�b�h���쏈��
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//=============================================================================
void InputGamePadPlayer2(void)
{
	if (player[P2].use)
	{
		CAMERA *camera = GetCamera();	// �J�����̃A�h���X���擾
		BULLET *bullet = GetBullet(P2);	// �o���b�g�̃A�h���X���擾
		if (IsButtonPressed(P2, BUTTON_RIGHT))
		{
			if (IsButtonPressed(P2, BUTTON_UP))
			{// �E�O�ړ�
				player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (IsButtonPressed(P2, BUTTON_DOWN))
			{// �E��ړ�
				player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// �E�ړ�
				player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (IsButtonPressed(P2, BUTTON_LEFT))
		{
			if (IsButtonPressed(P2, BUTTON_UP))
			{// ���O�ړ�
				player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (IsButtonPressed(P2, BUTTON_DOWN))
			{// ����ړ�
				player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// ���ړ�
				player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * player[P2].speed;
				player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * player[P2].speed;

				player[P2].rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (IsButtonPressed(P2, BUTTON_UP))
		{
			// �O�ړ�
			player[P2].move.x -= sinf(D3DX_PI + camera->rot.y) * player[P2].speed;
			player[P2].move.z -= cosf(D3DX_PI + camera->rot.y) * player[P2].speed;

			player[P2].rotDest.y = camera->rot.y;
		}
		else if (IsButtonPressed(P2, BUTTON_DOWN))
		{
			// ��ړ�
			player[P2].move.x -= sinf(camera->rot.y) * player[P2].speed;
			player[P2].move.z -= cosf(camera->rot.y) * player[P2].speed;

			player[P2].rotDest.y = D3DX_PI + camera->rot.y;
		}

		// �o���b�g�̃`���[�W
		if (IsButtonPressed(P2, BUTTON_C))
		{
			// �o���b�g���g�p���̓`���[�W�s��
			if (bullet->use[P2]) return;

			// �ő�l�ɂȂ����ꍇ
			if (bullet->speedIncrease > BULLET_CHARGE_MAX)
			{
				bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10�t���[��
			else if (player[P2].cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.2f;
			}
		}
		// �o���b�g�̔���
		else if (IsButtonRelease(P2, BUTTON_C))
		{
			SetBullet(player[P2].pos, player[P2].rot, bullet->speedIncrease, 0, P2);
			cntFrame[P2] = 0;
		}
	}
}

//=============================================================================
// �v���C���[�̈ړ�����
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�Ȃ�
//=============================================================================
void MovePlayer(int index)
{
	if (player[index].rot.y > D3DX_PI)
	{
		player[index].rot.y -= D3DX_PI * 2.0f;
	}
	if (player[index].rot.y < -D3DX_PI)
	{
		player[index].rot.y += D3DX_PI * 2.0f;
	}

	// �ړ��ʂɊ�����������
	player[index].move.x += (0.0f - player[index].move.x) * RATE_MOVE_PLAYER;
	player[index].move.y += (0.0f - player[index].move.y) * RATE_MOVE_PLAYER;
	player[index].move.z += (0.0f - player[index].move.z) * RATE_MOVE_PLAYER;

	// �ʒu�ړ�
	player[index].pos.x += player[index].move.x;
	player[index].pos.y += player[index].move.y;
	player[index].pos.z += player[index].move.z;

}
 
//==========================================================================
// �ǂ��蔻�菈��
// ���@���FD3DXVECTOR3 nor0(�|���S���̖@��), int index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void WallShearPlayer(int index)
{
	if (!HitCheckBlock(player[index].prevPos + player[index].move, player[index].prevPos, BLOCK_VTX_MAX))
	{
		player[index].move = WallShear(player[index].pos + player[index].move, GetNormal(), index);
		CheckNorPlayer(GetNormal(), index);
	}
}

//=============================================================================
// �v���C���[�̃_���[�W����
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//=============================================================================
bool PlayerDamageManager(int index)
{
	if (player[index].pos.y < PLAYER_POS_Y_LIMIT)
	{
		SetStage(RESULT);
		return false;
	}

	return true;
}

//=============================================================================
// �v���C���[�̃t�B�[���h���O���菈��
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�Fbool�^�@true���ƃt�B�[���h���Afalse���ƃt�B�[���h�O��Ԃ�
//=============================================================================
bool CheckBlockInPlayer(int index)
{
	if (player[index].pos.x > FIELD_SIZE_X) return false;
	if (player[index].pos.x < -FIELD_SIZE_X) return false;
	if (player[index].pos.z > FIELD_SIZE_Z) return false;
	if (player[index].pos.z < -FIELD_SIZE_Z) return false;

	return true;
}

//==========================================================================
// �ǂ���̂��蔲���\�h����(�u���b�N�g�p���Ƀ}�b�v�O�֏o�Ȃ��悤��)
// ���@���FD3DXVECTOR3 nor0(�|���S���̖@��), int index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void CheckNorPlayer(D3DXVECTOR3 nor0, int index)
{
	// �@����X�������Ȃ�
	if (nor0.x != 0.0f)
	{
		player[index].move.x = 0.0f;
		player[index].pos.x = player[index].prevPos.x;
		return;
	}
	// �@����Z�������Ȃ�
	if (nor0.z != 0.0f)
	{
		player[index].move.z = 0.0f;
		player[index].pos.z = player[index].prevPos.z;
		return;
	}
}
