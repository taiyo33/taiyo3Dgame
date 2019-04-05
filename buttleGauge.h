//=============================================================================
//
// �o�g���Q�[�W���� [buttleGauge.h]
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _BUTTLEGAUGE_H_
#define _BUTTLEGAUGE_H_

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#define TEXTURE_BUTTLEGAUGE_OUTER_SIZE_X	 (700)	 // �e�N�X�`���T�C�Y
#define TEXTURE_BUTTLEGAUGE_OUTER_SIZE_Y	 (350)	 // 
#define TEXTURE_BUTTLEGAUGE_INNER_SIZE_X	 (680)	 // �e�N�X�`���T�C�Y
#define TEXTURE_BUTTLEGAUGE_INNER_SIZE_Y	 (330)	 // 

#define BUTTLEGAUGE_OUTERPOS_X_01				 (580)	 // ����X���W 
#define BUTTLEGAUGE_OUTERPOS_Y_01				 (-100)	 // ����Y���W

#define BUTTLEGAUGE_INNERPOS_X_01				 (590)	 // ����X���W 
#define BUTTLEGAUGE_INNERPOS_Y_01				 (-90)	 // ����Y���W

#define BUTTLEGAUGE_INNERPOS_X_02				 (590)	 // ����X���W 
#define BUTTLEGAUGE_INNERPOS_Y_02				 (-90)	 // ����Y���W


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitButtleGauge(int type);
void UninitButtleGauge(void);
void UpdateButtleGauge(void);
void DrawButtleGauge(void);


#endif
