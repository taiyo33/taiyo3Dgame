//=============================================================================
//
// �{�[���Q�[�W���� [ballGauge.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _BALLGAUGE_H_
#define _BALLGAUGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_BALLGAUGE_OUTER_SIZE_X		(700)	 // �e�N�X�`���T�C�Y
#define TEXTURE_BALLGAUGE_OUTER_SIZE_Y		(350)	 // 
#define TEXTURE_BALLGAUGE_INNER_SIZE_X		(680)	 // �e�N�X�`���T�C�Y
#define TEXTURE_BALLGAUGE_INNER_SIZE_Y		(330)	 // 

#define BALLGAUGE_OUTERPOS_X_01				(580)	 // ����X���W 
#define BALLGAUGE_OUTERPOS_Y_01				(-100)	 // ����Y���W

#define BALLGAUGE_INNERPOS_X_01				(590)	 // ����X���W 
#define BALLGAUGE_INNERPOS_Y_01				(-90)	 // ����Y���W

#define BALLGAUGE_INNERPOS_X_02				(590)	 // ����X���W 
#define BALLGAUGE_INNERPOS_Y_02				(-90)	 // ����Y���W


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBallGauge(int type);
void UninitBallGauge(void);
void UpdateBallGauge(void);
void DrawBallGauge(void);


#endif
