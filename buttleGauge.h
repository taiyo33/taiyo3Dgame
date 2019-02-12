//=============================================================================
//
// �o�g���Q�[�W���� [buttleGauge.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _BUTTLEGAUGE_H_
#define _BUTTLEGAUGE_H_

// �}�N����`
#define TEXTURE_BUTTLEGAUGE_OUTER_SIZE_X	 (500)	 // �e�N�X�`���T�C�Y
#define TEXTURE_BUTTLEGAUGE_OUTER_SIZE_Y	 (300)	 // 
#define TEXTURE_BUTTLEGAUGE_INNER_SIZE_X	 (480)	 // �e�N�X�`���T�C�Y
#define TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y	 (280)	 // 

#define BUTTLEGAUGE_OUTERPOS_X_01				 (380)	 // ����X���W 
#define BUTTLEGAUGE_OUTERPOS_Y_01				 (-100)	 // ����Y���W

#define BUTTLEGAUGE_INNERPOS_X_01				 (390)	 // ����X���W 
#define BUTTLEGAUGE_INNERPOS_Y_01				 (-90)	 // ����Y���W

#define BUTTLEGAUGE_INNERPOS_X_02				 (390)	 // ����X���W 
#define BUTTLEGAUGE_INNERPOS_Y_02				 (-90)	 // ����Y���W


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitButtleGauge(int type);
void UninitButtleGauge(void);
void UpdateButtleGauge(void);
void DrawButtleGauge(void);


#endif