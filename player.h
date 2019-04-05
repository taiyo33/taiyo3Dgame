//=============================================================================
//
// �v���C���[���� [player.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
//�}�N����`
//=============================================================================
#define PLAYER_MAX				(2)			// �v���C���[�̍ő吔
#define PLAYER_LIFE_MAX			(100.0f)	// �v���C���[�̍ő�̗�

#define PLAYER01_INITPOS_X		(-300.0f)	// P1�̏����ʒu
#define PLAYER01_INITPOS_Y		(10.0f)		//
#define PLAYER01_INITPOS_Z		(200.0f)	//

#define PLAYER02_INITPOS_X		(300.0f)	// P2�̏����ʒu
#define PLAYER02_INITPOS_Y		(10.0f)		//
#define PLAYER02_INITPOS_Z		(-200.0f)	//

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
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
	int							cntFrame;
	bool						npc;				// NPC�̎g�p���
	LPDIRECTSOUNDBUFFER8		hitSE;				// ��e��SE
	LPDIRECTSOUNDBUFFER8		chargeSE;			// �`���[�W��SE

}PLAYER;

enum PLAYER_TYPE{
	P1,
	P2,
	NPC
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(int index);
void PlayerDamageManager(int pno00, int pno01, int bno);
void CheckNorPlayer(D3DXVECTOR3 nor0, int index);
void SetInitPosPlayer(void);

#endif
