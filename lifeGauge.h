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

#define TEXTURE_LIFEGAUGE_OUTER_SIZE_X	 (500)	 // �e�N�X�`���T�C�Y
#define TEXTURE_LIFEGAUGE_OUTER_SIZE_Y	 (100)	 // 
#define TEXTURE_LIFEGAUGE_INNER_SIZE_X	 (480)	 // �e�N�X�`���T�C�Y
#define TEXTURE_LIFEGAUGE_INNER_SIZE_Y	 (80)	 // 
#define TEXTURE_LIFEICON_SIZE_X			 (100)	 // �e�N�X�`���T�C�Y
#define TEXTURE_LIFEICON_SIZE_Y			 (100)	 // 

#define LIFEGAUGE_OUTERPOS_X_01				 (100)	 // ����X���W 
#define LIFEGAUGE_OUTERPOS_Y_01				 (70)	 // ����Y���W
#define LIFEGAUGE_INNERPOS_X_01				 (110)	 // ����X���W 
#define LIFEGAUGE_INNERPOS_Y_01				 (80)	 // ����Y���W

#define LIFEGAUGE_OUTERPOS_X_02				 (650)	 // ����X���W 
#define LIFEGAUGE_OUTERPOS_Y_02				 (70)	 // ����Y���W
#define LIFEGAUGE_INNERPOS_X_02				 (660)	 // ����X���W 
#define LIFEGAUGE_INNERPOS_Y_02				 (80)	 // ����Y���W

#define LIFEICON_POS_X_01				 (0)	 // P1�A�C�R������X���W 
#define LIFEICON_POS_Y_01				 (70)	 // P1�A�C�R������Y���W
#define LIFEICON_POS_X_02				 (1170)	 // P2�A�C�R������X���W 
#define LIFEICON_POS_Y_02				 (70)	 // P2�A�C�R������Y���W


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitLifeGauge(int type);
void UninitLifeGauge(void);
void UpdateLifeGauge(void);
void DrawLifeGauge(void);


#endif
