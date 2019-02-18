//=============================================================================
//
// �`���[�g���A������ [LifeGauge.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _LIFEGAUGE_H_
#define _LIFEGAUGE_H_

// �}�N����`
#define TEXTURE_ICON01	("data/TEXTURE/player01.png")	// ���s�p�摜
#define TEXTURE_ICON02	("data/TEXTURE/player02.png")	// ���s�p�摜
#define TEXTURE_ICON03	("data/TEXTURE/cpu_icon.png")	// ���s�p�摜

#define TEXTURE_LIFEGAUGE_OUTER_SIZE_X	 (700)	 // �e�N�X�`���T�C�Y
#define TEXTURE_LIFEGAUGE_OUTER_SIZE_Y	 (150)	 // 
#define TEXTURE_LIFEGAUGE_INNER_SIZE_X	 (680)	 // �e�N�X�`���T�C�Y
#define TEXTURE_LIFEGAUGE_INNER_SIZE_Y	 (130)	 // 
#define TEXTURE_LIFEICON_SIZE_X			 (150)	 // �e�N�X�`���T�C�Y
#define TEXTURE_LIFEICON_SIZE_Y			 (150)	 // 

#define LIFEGAUGE_OUTERPOS_X_01				 (150)	 // ����X���W 
#define LIFEGAUGE_OUTERPOS_Y_01				 (90)	 // ����Y���W
#define LIFEGAUGE_INNERPOS_X_01				 (160)	 // ����X���W 
#define LIFEGAUGE_INNERPOS_Y_01				 (100)	 // ����Y���W

#define LIFEGAUGE_OUTERPOS_X_02				 (1080)	 // ����X���W 
#define LIFEGAUGE_OUTERPOS_Y_02				 (90)	 // ����Y���W
#define LIFEGAUGE_INNERPOS_X_02				 (1090)	 // ����X���W 
#define LIFEGAUGE_INNERPOS_Y_02				 (100)	 // ����Y���W

#define LIFEICON_POS_X_01				 (40)	 // P1�A�C�R������X���W 
#define LIFEICON_POS_Y_01				 (90)	 // P1�A�C�R������Y���W
#define LIFEICON_POS_X_02				 (1740)	 // P2�A�C�R������X���W 
#define LIFEICON_POS_Y_02				 (90)	 // P2�A�C�R������Y���W

// �e�N�X�`���[�ԍ��ƒ��_���̃A�h���X�ԍ�
enum {
	LIFEGAUGE001,
	LIFEGAUGE002,
	LIFEGAUGE003,
	LIFEGAUGE004,
	PLAYERICON01,
	PLAYERICON02,
	NPCICON
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitLifeGauge(int type);
void UninitLifeGauge(void);
void UpdateLifeGauge(void);
void DrawLifeGauge(void);

void SetIconTextureType(int index);

#endif
