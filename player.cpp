//=============================================================================
//
// �v���C���[���� [Player.cpp]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bullet.h"
#include "field.h"
#include "block.h"
#include "checkhit.h"
#include "ai.h"
#include "result.h"
#include "chargeEffect.h"
#include "pause.h"
#include "hitEffect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_BODY			"data/MODEL/model_body.x"	// �ǂݍ��ރ��f����
#define	RATE_MOVE_PLAYER	(0.20f)						// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.02f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.20f)						// ��]�����W��
#define VALUE_MOVE_PLAYER	(0.50f)						// �ړ����x

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MovePlayer(int index);
void InputPlayer1(void);
void InputKeyPlayer2(void);
void InputGamePadPlayer(int index);
void WallShearPlayer(int index);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9			D3DTexture = NULL;		// �e�N�X�`���ǂݍ��ݏꏊ
static LPD3DXMESH					D3DXMesh = NULL;		// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
static LPD3DXBUFFER					D3DXBuffMat = NULL;		// ���b�V���̃}�e���A�������i�[
static DWORD						NumMat = NULL;			// �������̑���
static int							cntFrame[PLAYER_MAX];	// �o���b�g�`���[�W�̃t���[���J�E���g

PLAYER								Player[PLAYER_MAX];		// �v���C���[�\����
//=============================================================================
// ����������
// ���@���Fint type(�ď������Q������l)
// �߂�l�FHRESULT�^
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA *camera = GetCamera();

	Player[P1].pos = D3DXVECTOR3(PLAYER01_INITPOS_X, PLAYER01_INITPOS_Y, PLAYER01_INITPOS_Z);	// �ʒu�̏�����
	Player[P2].pos = D3DXVECTOR3(PLAYER02_INITPOS_X, PLAYER02_INITPOS_Y, PLAYER02_INITPOS_Z);	//
	Player[P1].rot = D3DXVECTOR3(0.0f, 90.0f, 0.0f);	// ��]�̏�����
	Player[P2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�̏�����

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		Player[i].use = true;								// �g�p��Ԃ�������
		Player[i].npc = false;								// �g�p��Ԃ�������
		Player[i].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]�̖ړI�ʒu��������
		Player[i].life = PLAYER_LIFE_MAX;					// �v���C���[�̗̑͂�������
		Player[i].frontVec = D3DXVECTOR3(sinf(Player[i].rot.y) * 100.0f, 0.0f, cosf(Player[i].rot.y) * 100.0f);
		Player[i].speed = VALUE_MOVE_PLAYER;				// �ړ����x�̏�����
		Player[i].cntFrame= 0;
		Player[i].hitSE = LoadSound(SE_HIT);
		Player[i].chargeSE = LoadSound(SE_CHARGE);
	}

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(PLAYER_BODY,
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
		Player[i].prevPos = Player[i].pos;
		Player[i].cntFrame++;
		// �O���x�N�g���̍X�V
		Player[i].frontVec = D3DXVECTOR3(sinf(Player[i].rot.y) * 75.0f, 0.0f, cosf(Player[i].rot.y) * 75.0f);
		
		// �^�C�g�����ł̃v���C���[����
		if (GetStage() == TITLE)
		{
			NonePlayerPatrol(i);
		}
		// �Q�[���J�n��
		else if (GetStage() == START)
		{
			// AI�̍s��
			if (Player[i].npc)
			{
				NonePlayerMove();
				NonePlayerAttack();
			}
			// ����
			else
			{
				InputPlayer1();
				InputGamePadPlayer(i);
				InputKeyPlayer2();
			}
		}

		// �ǂ��菈��
		WallShearPlayer(i);
		// �v���C���[�̑���
		MovePlayer(i);

		PrintDebugProc("�v���C���[�̉�][(%f)]\n", Player[i].rot.y);
		PrintDebugProc("�v���C���[�̈ʒu: [X:(%f),z:(%f)]\n", Player[i].pos.x, Player[i].pos.z);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, *mtxView;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&Player[i].mtxWorld);

		mtxView = GetMtxView();

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Player[i].rot.y, Player[i].rot.x, Player[i].rot.z);
		D3DXMatrixMultiply(&Player[i].mtxWorld, &Player[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, Player[i].pos.x, Player[i].pos.y, Player[i].pos.z);
		D3DXMatrixMultiply(&Player[i].mtxWorld, &Player[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &Player[i].mtxWorld);

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

//=============================================================================
// �v���C���[�̃A�h���X�擾
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�Ȃ�
//=============================================================================
PLAYER *GetPlayer(int index)
{
	return &Player[index];
}

//=============================================================================
// �v���C���[1�̃L�[�{�[�h���쏈��
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//=============================================================================
void InputPlayer1(void)
{
	if (Player[P1].use)
	{
		CAMERA *camera = GetCamera();	// �J�����̃A�h���X���擾
		BULLET *bullet = GetBullet(P1);	// �o���b�g�̃A�h���X���擾

		if (GetKeyboardPress(DIK_RIGHT))
		{
			if (GetKeyboardPress(DIK_UP))
			{// �E�O�ړ�
				Player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// �E��ړ�
				Player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// �E�ړ�
				Player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_LEFT))
		{
			if (GetKeyboardPress(DIK_UP))
			{// ���O�ړ�
				Player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{// ����ړ�
				Player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// ���ړ�
				Player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_UP))
		{
			if(GetKeyboardPress(DIK_RIGHT))
			{// �E�ړ�
				Player[P1].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y + D3DX_PI * 0.50f;
			}
			else
			{
				// �O�ړ�
				Player[P1].move.x -= sinf(D3DX_PI + camera->rot.y) * Player[P1].speed;
				Player[P1].move.z -= cosf(D3DX_PI + camera->rot.y) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{
			if (GetKeyboardPress(DIK_LEFT))
			{// ���ړ�
				Player[P1].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * Player[P1].speed;

				Player[P1].rot.y = camera->rot.y - D3DX_PI * 0.50f;
			}
			else
			{
				// ��ړ�
				Player[P1].move.x -= sinf(camera->rot.y) * Player[P1].speed;
				Player[P1].move.z -= cosf(camera->rot.y) * Player[P1].speed;

				Player[P1].rot.y = D3DX_PI + camera->rot.y;
			}
		}

		// �o���b�g�̃`���[�W
		if (GetKeyboardPress(DIK_SPACE))
		{
			 SetChargeEffect(Player[P1].pos, P1);
			PlaySound(Player[P1].chargeSE, E_DS8_FLAG_NONE);
			// �ő�l�ɂȂ����ꍇ
			if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
			{
                bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10�t���[��
			else if (Player[P1].cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.2f;
			}
		}
		// �o���b�g�̔���
        else if(GetKeyboardRelease(DIK_SPACE))
		{
			StopSound(Player[P1].chargeSE);
          	SetBullet(Player[P1].pos, Player[P1].rot, bullet->speedIncrease, 0, P1);
			cntFrame[P1] = 0;
		}

		if (GetKeyboardTrigger(DIK_M))
		{
			SetPause(true);
		}

#ifdef _DEBUG

		// �ʒu��������
		if (GetKeyboardTrigger(DIK_O))
		{
			Player[P1].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		}
#endif
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

	if (Player[P2].use)
	{
		if (GetKeyboardPress(DIK_D))
		{
			if (GetKeyboardPress(DIK_W))
			{// �E�O�ړ�
				Player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_S))
			{// �E��ړ�
				Player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// �E�ړ�
				Player[P2].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_A))
		{
			if (GetKeyboardPress(DIK_W))
			{// ���O�ړ�
				Player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_S))
			{// ����ړ�
				Player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// ���ړ�
				Player[P2].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * Player[P2].speed;
				Player[P2].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * Player[P2].speed;

				Player[P2].rot.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (GetKeyboardPress(DIK_W))
		{
			// �O�ړ�
			Player[P2].move.x -= sinf(D3DX_PI + camera->rot.y) * Player[P2].speed;
			Player[P2].move.z -= cosf(D3DX_PI + camera->rot.y) * Player[P2].speed;

			Player[P2].rot.y = camera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S))
		{
			// ��ړ�
			Player[P2].move.x -= sinf(camera->rot.y) * Player[P2].speed;
			Player[P2].move.z -= cosf(camera->rot.y) * Player[P2].speed;

			Player[P2].rot.y = D3DX_PI + camera->rot.y;
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
			else if (Player[P2].cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
			{
				PlaySound(Player[P2].chargeSE, E_DS8_FLAG_NONE);
				bullet->speedIncrease += 0.5f;
			}
		}
		else if (GetKeyboardRelease(DIK_Z))
		{			
			StopSound(Player[P2].chargeSE);
			SetBullet(Player[P2].pos, Player[P2].rot, bullet->speedIncrease, 0, P2);
			cntFrame[P2] = 0;
		}
		
		if (GetKeyboardTrigger(DIK_M))
		{
			SetPause(true);
		}

	}
}

//=============================================================================
// �v���C���[�̃Q�[���p�b�h���쏈��
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//=============================================================================
void InputGamePadPlayer(int index)
{
	if (Player[index].use)
	{
		CAMERA *camera = GetCamera();	// �J�����̃A�h���X���擾
		BULLET *bullet = GetBullet(index);	// �o���b�g�̃A�h���X���擾
		if (IsButtonPressed(index,BUTTON_RIGHT))
		{
			if (IsButtonPressed(index,BUTTON_UP))
			{// �E�O�ړ�
				Player[index].move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y + D3DX_PI * 0.25f;
			}
			else if (IsButtonPressed(index,BUTTON_DOWN))
			{// �E��ړ�
				Player[index].move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y + D3DX_PI * 0.75f;
			}
			else
			{// �E�ړ�
				Player[index].move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y + D3DX_PI * 0.50f;
			}
		}
		else if (IsButtonPressed(index,BUTTON_LEFT))
		{
			if (IsButtonPressed(index,BUTTON_UP))
			{// ���O�ړ�
				Player[index].move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y - D3DX_PI * 0.25f;
			}
			else if (IsButtonPressed(index,BUTTON_DOWN))
			{// ����ړ�
				Player[index].move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y - D3DX_PI * 0.75f;
			}
			else
			{// ���ړ�
				Player[index].move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * Player[index].speed;
				Player[index].move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * Player[index].speed;

				Player[index].rot.y = camera->rot.y - D3DX_PI * 0.50f;
			}
		}
		else if (IsButtonPressed(index,BUTTON_UP))
		{
			// �O�ړ�
			Player[index].move.x -= sinf(D3DX_PI + camera->rot.y) * Player[index].speed;
			Player[index].move.z -= cosf(D3DX_PI + camera->rot.y) * Player[index].speed;

			Player[index].rot.y = camera->rot.y;
		}
		else if (IsButtonPressed(index,BUTTON_DOWN))
		{
			// ��ړ�
			Player[index].move.x -= sinf(camera->rot.y) * Player[index].speed;
			Player[index].move.z -= cosf(camera->rot.y) * Player[index].speed;

			Player[index].rot.y = D3DX_PI + camera->rot.y;
		}

		// �o���b�g�̃`���[�W
		if (IsButtonPressed(index, BUTTON_B))
		{
			 SetChargeEffect(Player[index].pos, index);
			PlaySound(Player[index].chargeSE, E_DS8_FLAG_NONE);

			// �ő�l�ɂȂ����ꍇ
			if (bullet->speedIncrease >= BULLET_CHARGE_MAX)
			{
				bullet->speedIncrease = BULLET_CHARGE_MAX;
			}
			// 10�t���[��
			else if (Player[index].cntFrame % BULLET_CHARGE_FRAME_CNT == 0)
			{
				bullet->speedIncrease += 0.2f;
			}
		}
		// �o���b�g�̔���
		else if (IsButtonRelease(index, BUTTON_B))
		{
			StopSound(Player[index].chargeSE);
			SetBullet(Player[index].pos, Player[index].rot, bullet->speedIncrease, 0, index);
			cntFrame[index] = 0;
		}

		if (IsButtonTriggered(index, BUTTON_R))
		{
			SetPause(true);
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
	if (Player[index].rot.y > D3DX_PI)
	{
		Player[index].rot.y -= D3DX_PI * 2.0f;
	}
	if (Player[index].rot.y < -D3DX_PI)
	{
		Player[index].rot.y += D3DX_PI * 2.0f;
	}

	// �ړ��ʂɊ�����������
	Player[index].move.x += (0.0f - Player[index].move.x) * RATE_MOVE_PLAYER;
	Player[index].move.y += (0.0f - Player[index].move.y) * RATE_MOVE_PLAYER;
	Player[index].move.z += (0.0f - Player[index].move.z) * RATE_MOVE_PLAYER;

	// �ʒu�ړ�
	Player[index].pos.x += Player[index].move.x;
	Player[index].pos.y += Player[index].move.y;
	Player[index].pos.z += Player[index].move.z;

}
 
//==========================================================================
// �ǂ��蔻�菈��
// ���@���Fint index(�v���C���[�̃A�h���X�ԍ�)
// �߂�l�F�ȁ@��
//==========================================================================
void WallShearPlayer(int index)
{
	if (!HitCheckBlock(Player[index].prevPos + Player[index].move, Player[index].prevPos, BLOCK_VTX_MAX))
	{
		Player[index].move = WallShear(Player[index].pos + Player[index].move, GetNormal(), index);
		CheckNorPlayer(GetNormal(), index);
	}
}

//=============================================================================
// �v���C���[�̃_���[�W����
// ���@���Fint pno00(�_���[�W���󂯂��v���C���[�A�h���X�ԍ�), 
//		   int pno01(�_���[�W��^�����v���C���[�A�h���X�ԍ�), 
//		   int bno(�o���b�g�P�̂̃A�h���X�ԍ�)
// �߂�l�F�Ȃ�
//=============================================================================
void PlayerDamageManager(int pno00, int pno01, int bno)
{
	BULLET *bullet = GetBullet(0);

	if (CheckHitBC(bullet[pno01].pos[bno], Player[pno00].pos,
		bullet[pno01].size[bno].x, 10.0f))
	{
		// �v���C���[�ɑ΂��鏈��
		PlaySound(Player[pno00].hitSE, E_DS8_FLAG_NONE);
		SetHitEffect(bullet[pno01].pos[bno], D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
		Player[pno00].life -= BULLET_DAMAGE;
		
		// �o���b�g�ɑ΂��鏈��
		bullet[pno01].use[bno] = false;
		bullet[pno01].reflect[bno] = false;
		bullet[pno01].cntReflect[bno] = INIT_REFLECT_CNT;
		bullet[pno01].speed[bno] = INIT_BULLET_SPEED;
	}

	// �_���[�W���󂯂��v���C���[�̃��C�t���Ȃ��Ȃ����Ƃ�
	if (Player[pno00].life < 0)
	{
		SetStage(FINISHCALL);
	}
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
		Player[index].move.x = 0.0f;
		Player[index].pos.x = Player[index].prevPos.x;
		return;
	}
	// �@����Z�������Ȃ�
	if (nor0.z != 0.0f)
	{
		Player[index].move.z = 0.0f;
		Player[index].pos.z = Player[index].prevPos.z;
		return;
	}
}

//=============================================================================
// �v���C���[�̈ʒu����������
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
//=============================================================================
void SetInitPosPlayer(void)
{
	PLAYER *Player = GetPlayer(0);

	Player[P1].pos = D3DXVECTOR3(PLAYER01_INITPOS_X, PLAYER01_INITPOS_Y, PLAYER01_INITPOS_Z);	// �ʒu�̏�����
	Player[P2].pos = D3DXVECTOR3(PLAYER02_INITPOS_X, PLAYER02_INITPOS_Y, PLAYER02_INITPOS_Z);	//
}
