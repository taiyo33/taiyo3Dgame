//=============================================================================
//
// �����G�t�F�N�g���� [explosion.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _GAUGEEFFECT_H_
#define _GAUGEEFFECT_H_

#include "main.h"

#define TEXTURE_GAUGEEFFECT_SIZE_X	 (100)	 // �e�N�X�`���T�C�Y
#define TEXTURE_GAUGEEFFECT_SIZE_Y	 (120)	 // 

#define GAUGEEFFECT_POS_X_01				 (550)	 // ����X���W 
#define GAUGEEFFECT_POS_Y_01				 (-10)	 // ����Y���W

#define GAUGEEFFECT_POS_X_02				 (610)	 // ����X���W 
#define GAUGEEFFECT_POS_Y_02				 (-10)	 // ����Y���W


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGaugeEffect(int type);
void UninitGaugeEffect(void);
void UpdateGaugeEffect(void);
void DrawGaugeEffect(void);

#endif

