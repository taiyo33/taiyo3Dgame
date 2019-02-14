//=============================================================================
//
// �v���C���[���� [player.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================

#define PLAYER_MAX				(2)
#define PLAYER_LIFE_MAX			(100.0f)
#define PLAYER_FALL_SPEED		(5.0f)

#define PLAYER01_INITPOS_X		(-300.0f)
#define PLAYER01_INITPOS_Y		(10.0f)
#define PLAYER01_INITPOS_Z		(200.0f)

#define PLAYER02_INITPOS_X		(300.0f)
#define PLAYER02_INITPOS_Y		(10.0f)
#define PLAYER02_INITPOS_Z		(-200.0f)


typedef struct {
	
	D3DXMATRIX					mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3					pos;				// ���݂̈ʒu
	D3DXVECTOR3					prevPos;			// �ߋ��̈ʒu
	D3DXVECTOR3					move;				// �ړ���
	D3DXVECTOR3					rot;				// ��]��
	D3DXVECTOR3					rotDest;			// ���݂̌���
	D3DXVECTOR3					frontVec;			// �O���x�N�g��
	bool						use;				// �g�p���
	float						speed;				// �ړ��̑���
	float						life;				// �̗�
	int							IdxShadow;			// �eID
	float						SizeShadow;			// �e�̃T�C�Y
	D3DXCOLOR					colShadow;			// �e�̐F
	int							cntFrame;


}PLAYER;

enum {
	P1,
	P2
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(int index);
void PlayerDamageManager(void);
bool CheckBlockInPlayer(int index);
void CheckNorPlayer(D3DXVECTOR3 nor0, int index);

#endif
